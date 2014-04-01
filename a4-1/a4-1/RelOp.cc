#include "RelOp.h"
#include "Record.h"

//SelectFile
void* selFile_Run(void *selectFile_Obj) {
	SelectFile SF_obj = *((SelectFile *) selectFile_Obj);
	SF_obj.selFile_Operation();
	pthread_exit(NULL);
}

void SelectFile::Run (DBFile &inFile, Pipe &PipeOut, CNF &selOp, Record &literal) {
     selFile_inFile = &inFile;
     selFile_out = &PipeOut;
     selFile_Op = &selOp;
     selFile_record = &literal;
     int pthreadcreate = pthread_create(&thread, NULL, selFile_Run, (void*)this);
     if(pthreadcreate){
     	printf("Error while creating a thread\n");
     	exit(-1);
     }
}

void SelectFile::selFile_Operation(){
     Record fetchMe;
     ComparisonEngine comEngine;
     int selFile_count=0;
     selFile_inFile ->MoveFirst();
     while(selFile_inFile->GetNext(fetchMe, *selFile_Op, *selFile_record)){
     	selFile_out->Insert(&fetchMe);
     	selFile_count++;
     }
     selFile_out->ShutDown();
     //cout<<"Select returned "<<selFile_count<<" records"<<endl;
}

void SelectFile::WaitUntilDone () {
	pthread_join(thread, NULL);
}

void SelectFile::Use_n_Pages (int runlen) {
    selFile_runlen = runlen;
}

//SelectPipe
void* PipeSelect_Run(void *PipeSelect_obj){
	SelectPipe SP_obj = *((SelectPipe *) PipeSelect_obj);
	SP_obj.PipeSelect_Operation();
	pthread_exit(NULL);
}

void SelectPipe::Run(Pipe &PipeIn, Pipe &PipeOut, CNF &selOp, Record &literal) {
	PipeSelect_in = &PipeIn;
	PipeSelect_out = &PipeOut;
	PipeSelect_Op = selOp;
	PipeSelect_record = &literal;
	int pthreadcreate = pthread_create(&thread, NULL, PipeSelect_Run, (void*) this);
	if(pthreadcreate) {
		cout<<"Error detect while creating thread~!"<<endl;
		exit(-1);
	}
}

void SelectPipe::PipeSelect_Operation(){
	Record fetchMe;
	ComparisonEngine comEngine;
	int result;
	while(PipeSelect_in->Remove(&fetchMe)){
		result = comEngine.Compare(&fetchMe, PipeSelect_record, &PipeSelect_Op);
		if(result){
			PipeSelect_out->Insert(&fetchMe);
		}
	}
	PipeSelect_out->ShutDown();
}

void SelectPipe::WaitUntilDone(){
	PipeSelect_in->ShutDown();
	pthread_join(thread, NULL);
}

void SelectPipe::Use_n_Pages(int runlen){
	PipeSelect_runlen = runlen;
}

//Project Operation
void* Pro_Run(void *pro_obj){
    Project P_obj = *((Project *) pro_obj);
    P_obj.pro_Operation();
    pthread_exit(NULL);
}

void Project::pro_Operation(){
  Record *record;
	while(true) {
		record = new Record;
		if(!(PipeProject_in->Remove(record))) {
			break;
		}else{
			record->Project(pro_keepme, pro_numAttsOutput, pro_numAttsInput);
			PipeProject_out->Insert(record);
		}
	}
	PipeProject_in->ShutDown();
	PipeProject_out->ShutDown();
}

void Project::Run(Pipe &PipeIn, Pipe &PipeOut, int *keepMe, int numAttsInput, int numAttsOutput){
	PipeProject_in = &PipeIn;
	PipeProject_out = &PipeOut;
	pro_keepme = keepMe;
	pro_numAttsInput = numAttsInput;
	pro_numAttsOutput = numAttsOutput;
	int pthreadcreate = pthread_create(&thread, NULL, Pro_Run, (void*) this);
	if(pthreadcreate){
		cout<<"Error detect while creating thread: "<<endl;
		exit(-1);
	}
}

void Project::WaitUntilDone(){
	pthread_join(thread, NULL);
}

void Project::Use_n_Pages(int runlen) {
	pro_runlen = runlen;
}

//DuplicateRemoveal Operation
void* DR_Run(void *duplicateRemoval_obj){
	DuplicateRemoval dr_obj = *((DuplicateRemoval *) duplicateRemoval_obj);
    dr_obj.dr_Operation();
    pthread_exit(NULL);
}

void DuplicateRemoval::Run(Pipe &PipeIn, Pipe &PipeOut, Schema &mySchema){
    dr_in = &PipeIn;
    dr_out = &PipeOut;
    dr_schema = &mySchema;
    int pthreadcreate = pthread_create(&thread, NULL, DR_Run, (void *)this);
    if(pthreadcreate){
    	cout<<"Error detect while create a thread~!"<<endl;
    	exit(-1);
    }
}

void DuplicateRemoval::dr_Operation(){
	Record temprec;
	Record *currentrec;
	OrderMaker *ordermaker;
	dr_runlen = 5;
	ordermaker = new OrderMaker(dr_schema);
	Pipe *SortedPipe = new Pipe(100);
	Schema *schema;
	BigQ objbq(*dr_in, *SortedPipe, *ordermaker, dr_runlen);
    ComparisonEngine comEngine;
    currentrec = new Record;
    SortedPipe->Remove(currentrec);
    temprec.Copy(currentrec);
    dr_out->Insert(currentrec);
    delete currentrec;
    currentrec = new Record;
    while(SortedPipe->Remove(currentrec)){
    	if(comEngine.Compare(&temprec, currentrec, ordermaker)!=0){
    		temprec.Copy(currentrec);
    		dr_out->Insert(currentrec);
    	}
    	delete currentrec;
    	currentrec = new Record;
    }

    dr_in->ShutDown();
    dr_out->ShutDown();
    delete SortedPipe;
}

void DuplicateRemoval::WaitUntilDone(){
	pthread_join(thread,NULL);
}

void DuplicateRemoval::Use_n_Pages(int runlen){
	dr_runlen = runlen;
}


//Sum Operation
    typedef struct {
     Pipe *PipeIn;
     Pipe *PipeOut;
     Function *computeMe;
     }sUtil;

void * sum_Operation (void *arg){
  Record *sumRecord = new Record;
  Type type;

  sUtil *sutil = (sUtil *) arg;
  if(sutil->computeMe->returnsInt)
    type=Int;
  else
    type=Double;

  int counter=0,inserted=0;
  Record *buffer=new Record;
  int intSum=0;
  double doubleSum=0;
  int argIntSum;
  double argDoubleSum;


  while(sutil->PipeIn->Remove(buffer)){
      counter ++;
      sutil->computeMe->Apply(*buffer,argIntSum,argDoubleSum);
      if(sutil->computeMe->returnsInt)
   intSum+=argIntSum;
      else
   doubleSum+=argDoubleSum;


    }
   sumRecord->CreateRecord(type,intSum,doubleSum);
    sutil->PipeOut->Insert(sumRecord);
    sutil->PipeOut->ShutDown();
    delete buffer;
    delete sutil;
}

void Sum::Run(Pipe &PipeIn, Pipe &PipeOut, Function &computeMe){
  sUtil *sutil=new sUtil{&PipeIn, &PipeOut, &computeMe};
  pthread_create(&thread, NULL, sum_Operation, sutil);
}

void Sum::WaitUntilDone () {
   pthread_join (thread, NULL);
}

void Sum::Use_n_Pages (int runlen) {
}


//Join Operation
void* Join_run (void* arg){
  Join objjoin = *((Join *) arg);
  objjoin.Join_Operaion();
  
}

void Join::Join_Operaion(){

      OrderMaker orderMakerL;
      OrderMaker orderMakerR;
      Record * left=new Record;
      Record * right=new Record;
      Record *buffer = new Record;
      ComparisonEngine ceng;
      int buffsz = 100; 
      Pipe *PipeOutL=new Pipe(buffsz);
      Pipe *PipeOutR=new Pipe(buffsz);

      vector<Record*> blockL;
      vector<Record*> blockR;

      Record *result=new Record;
      int le=0,ri=0,bo=0;
      int numOfAttsL;
      int numOfAttsR;

      if((j_selOp)->GetSortOrders(orderMakerL,orderMakerR)){

      BigQ bqL (*PipeJoinInL, *PipeOutL, orderMakerL, 7);
       
      BigQ bqR (*PipeJoinInR, *PipeOutR, orderMakerR, 7);

      PipeOutL->Remove(left);
      PipeOutR->Remove(right);

      bool leftEnd=false;
      bool rightEnd=false;
      numOfAttsL=((int *) left->bits)[1] / sizeof(int) -1;
      numOfAttsR=((int *) right->bits)[1] / sizeof(int) -1;

      int attsToKeep[numOfAttsL+numOfAttsR];

      for(int i=0;i<numOfAttsL;i++){
      attsToKeep[i]=i;
       }
     for(int i=0;i<numOfAttsR;i++){
     attsToKeep[numOfAttsL+i]=i;
      }

      while (true){
     if(ceng.Compare(left,&orderMakerL,right,&orderMakerR)<0){
      le++;
      if(!PipeOutL->Remove(left))
       break;

     }

     else if(ceng.Compare(left,&orderMakerL,right,&orderMakerR)>0){
      ri++;
      if(!PipeOutR->Remove(right))
       break;
     }

     else{

        leftEnd=false;
        rightEnd=false;
  while(true){
    le++;
    Record *prev=new Record;
    prev->Consume(left);
    blockL.push_back(prev);
    if(!(PipeOutL->Remove (left))){
     leftEnd=true;
     break;
    }
    if(ceng.Compare (prev, left,&orderMakerL) !=0){
     break;
    }
        }

    while(true){
      ri++;
    Record *prev=new Record;
    prev->Consume(right);
    blockR.push_back(prev);
    if(!(PipeOutR->Remove (right))){
     rightEnd=true;
     break;
    }
    if(ceng.Compare (prev,right,&orderMakerR) !=0){
     break;
    }
   }
        for (int i = 0; i < blockL.size(); i++) {
  for (int j = 0; j < blockR.size(); j++) {
   result->MergeRecords(blockL.at(i),blockR.at(j),numOfAttsL,numOfAttsR,attsToKeep,numOfAttsL+numOfAttsR,numOfAttsL);
      bo++;
   (PipeJoinOut)->Insert(result);
  }
  delete blockL.at(i);
        }

        for (int j = 0; j < blockR.size(); j++) {
  delete blockR.at(j);
        }

        blockL.clear();
        blockR.clear();
  if(rightEnd || leftEnd)
      break;
     }
      }
      while(PipeOutL->Remove(left)){
     le++;
      }

      while(PipeOutR->Remove(left)){
     ri++;
       }
   }
  else{

   while((PipeJoinInR)->Remove(right)){
    Record *temp=new Record;
    temp->Consume(right);
    blockR.push_back(temp);
   }
   (PipeJoinInL)->Remove(left);
   numOfAttsL=((int *) left->bits)[1] / sizeof(int) -1;
      numOfAttsR=((int *) blockR.at(0)->bits)[1] / sizeof(int) -1;
      cerr<<"numOfAttsL "<<numOfAttsL<<"numOfAttsR "<<numOfAttsR<<endl;

      int attsToKeep[numOfAttsL+numOfAttsR];

     for(int i=0;i<numOfAttsL;i++){
     attsToKeep[i]=i;
        }
    for(int i=0;i<numOfAttsR;i++){
      attsToKeep[numOfAttsL+i]=i;
     }

    int count=1;
   do{
    for (int j = 0; j < blockR.size(); j++) {
       result->MergeRecords(left,blockR.at(j),numOfAttsL,numOfAttsR,attsToKeep,numOfAttsL+numOfAttsR,numOfAttsL);
       (PipeJoinOut)->Insert(result);
    }
    cerr<<count++<<endl;
   }while((PipeJoinInL)->Remove(left));
  }
        (PipeJoinOut)->ShutDown();
        delete PipeOutL;
        delete PipeOutR;
        delete result;
        }

void Join::Run (Pipe &PipeInL, Pipe &PipeInR, Pipe &PipeOut, CNF &selOp, Record &literal){
        PipeJoinInL = &PipeInL;
        PipeJoinInR = &PipeInR;
        PipeJoinOut = &PipeOut;
        j_selOp = &selOp;
        j_literal = &literal;
        pthread_create(&thread, NULL, Join_run, (void*)this);

}

void Join:: WaitUntilDone () {
  pthread_join (thread, NULL);
}

void Join:: Use_n_Pages (int n){
  runlen=n;
}

//Group By
typedef struct {
        Pipe *PipeIn;
        Pipe *PipeOut;
        OrderMaker *groupAtts;
        Function *computeMe;
        int runlen;
}gbUtil;
void * gb_Operation (void *arg){
    gbUtil *util=(gbUtil*) arg;
    Record *sumRecord = new Record;
    Record *recLeft = new Record;
    Record *recRight = new Record;
    Type type;
    int buffsz = 100; 
    Pipe *PipeOut=new Pipe(buffsz);

    BigQ bq (*(util->PipeIn), *PipeOut, *(util->groupAtts), util->runlen);

    Record *temp=new Record;

    Record rec[2] ;
    Record *last = NULL, *prev = NULL;
    ComparisonEngine ceng;
    int counter=0;
    int inserted=0;
    int intSum=0;
    double doubleSum=0;
    int argIntSum;
    double argDoubleSum;
    if(util->computeMe->returnsInt)
      type=Int;
    else
      type=Double;

    int attsToKeep[util->groupAtts->numAtts+1];
    attsToKeep[0]=0;

     for(int i=1;i<=util->groupAtts->numAtts;i++){
  attsToKeep[i]=util->groupAtts->whichAtts[i-1];
     }

     while (PipeOut->Remove (&rec[counter%2])) {

        prev = last;
        last = &rec[counter%2];

        if(prev && last){
   if (ceng.Compare (prev, last, util->groupAtts) !=0 ) {

     util->computeMe->Apply(*prev,argIntSum,argDoubleSum);
     if(util->computeMe->returnsInt)
  intSum+=argIntSum;
     else
  doubleSum+=argDoubleSum;

     recLeft->CreateRecord(type,intSum,doubleSum);


     sumRecord->MergeRecords(recLeft,prev,1 ,((int *) prev->bits)[1] / sizeof(int) -1, attsToKeep, (util->groupAtts->numAtts)+1,1);
     util->PipeOut->Insert(sumRecord);
     intSum=0;doubleSum=0;

     inserted++;
       }
   else{

       util->computeMe->Apply(*prev,argIntSum,argDoubleSum);
       if(util->computeMe->returnsInt)
      intSum+=argIntSum;
       else
      doubleSum+=argDoubleSum;

       }
        }
   counter++;
        }


      util->computeMe->Apply(*last,argIntSum,argDoubleSum);
   if(util->computeMe->returnsInt)
       intSum+=argIntSum;
   else
      doubleSum+=argDoubleSum;
      
      recLeft->CreateRecord(type,intSum,doubleSum);

      sumRecord->MergeRecords(recLeft,last,1 , ((int *) prev->bits)[1] / sizeof(int) -1, attsToKeep, util->groupAtts->numAtts+1,1);
      util->PipeOut->Insert(sumRecord);
      inserted++;

     util->PipeOut->ShutDown();
    cout << "Group By: Scanned "<<counter<<" records. " << inserted << " unique recs \n";

}
void GroupBy::Run (Pipe &PipeIn, Pipe &PipeOut, OrderMaker &groupAtts, Function &computeMe) {
  gbUtil *gbutil=new gbUtil {&PipeIn, &PipeOut, &groupAtts,&computeMe,runlen};
  pthread_create(&thread, NULL, gb_Operation, gbutil);
}


void GroupBy::WaitUntilDone () {
   pthread_join (thread, NULL);
}

void GroupBy::Use_n_Pages (int n) {
    runlen=n;
}


void* wo_Run(void *wo_currentObj) {
        WriteOut objWriteOut = *((WriteOut *) wo_currentObj);
        objWriteOut.wo_Operation();
        pthread_exit(NULL);
}

void WriteOut::Run(Pipe &PipeIn, FILE *outFile, Schema &mySchema) {
  wo_in = &PipeIn;
  wo_outFile = outFile;
  wo_schema = &mySchema;
  int pthreadcreate = pthread_create(&thread, NULL, wo_Run, (void *) this);
  if(pthreadcreate) {
    cout<<"Error detect while creating a thread~!"<<endl;
    exit(-1);
  }
}


void WriteOut::wo_Operation() {
  Record temp;

  while (wo_in->Remove(&temp)) {
    temp.PrintRecordToFile(wo_outFile, &temp, this->wo_schema);
  }
  fclose(wo_outFile);
}

void WriteOut::WaitUntilDone() {
  wo_in->ShutDown();
  pthread_join(thread, NULL);
}

void WriteOut::Use_n_Pages(int runlen) {
  wo_runlen = runlen;
}
