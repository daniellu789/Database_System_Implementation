#include "DBFile.h"



Sorted::Sorted(SortInfo *info){

	merge=false;
	b_query=false;
	bigq=NULL;
	sortInfo = info;
	mode=Reading;
	buffsz = 100;

}

int Sorted::Create(char * f_path){
	cout<<"SORTED CREATE"<<endl;
	MyPageIndex=0;
	fpath=f_path;
	myfile.Open(0,f_path);
	return 1;

}

int Sorted::Open(char * f_path){
	cout<<"SORTED OPEN"<<endl;
	MyPageIndex=0;
	mypage.EmptyItOut();
	fpath=f_path;
	myfile.Open(1,f_path);
	return 1;	
}

void Sorted::InititateBigQ(){
		input = new Pipe(buffsz);
		output = new Pipe(buffsz);
		bigq=new BigQ(*input, *output,*(sortInfo->myOrder),sortInfo->runLength);
}


void Sorted::Load(Schema &myschema, char *loadpath){
}

void Sorted::Add (Record &addme){  //write something into pipe or sort
	 	 if(mode==Reading){
	 		if(!bigq)
	 			InititateBigQ();
	 		mode=Writing;
	 	 }

	 	 if (mode==Writing){
	 		 input->Insert(&addme);
	 	 }
 }

//when closing file, merge record in pipe
 int Sorted::Close()
 {
	 if(mode==Writing){
	 	 input->ShutDown();
	 	 mode=Reading;
	 	 Merge();
 	 }
	 myfile.Close();
 }

 void Sorted::MoveFirst () {

	if(mode==Writing)
	{
		input->ShutDown();
		mode=Reading;
		Merge();

	}
 	MyPageIndex=0;					
 	mypage.EmptyItOut();			
 	myfile.GetPage(&mypage,MyPageIndex);	
 	b_query=false;
 	sOrderRight=true;
 }

int Sorted ::GetNext (Record &fetchme) {
//cout<<"SORTED GETNEXT"<<endl;
	if(mypage.GetFirst(&fetchme)==1)													
		return 1;

	else{																			

		MyPageIndex++;
		if(MyPageIndex<myfile.GetLength()-1)											
		{
			myfile.GetPage(&mypage,MyPageIndex);									
			mypage.GetFirst(&fetchme);													
			return 1;
		}
		else																			
			return 0;
	}

}
 int Sorted ::GetNextQuery(Record &fetchme, CNF &cnf, Record &literal){

	ComparisonEngine myCompare;
 	while(1){
		if(mypage.GetFirst(&fetchme)==1 ){	
			if(myCompare.Compare (&literal, query, &fetchme,sortInfo->myOrder) ==0){
				if (myCompare.Compare (&fetchme, &literal, &cnf))	
					return 1;
		}	
		else
		return 0;
		}
		else{		
			MyPageIndex++;		
			if(MyPageIndex<myfile.GetLength()-1)	
				myfile.GetPage(&mypage,MyPageIndex);	
			else						
				return 0;
		}
	}
 }


 int Sorted::SequentialGetNext (Record &fetchme, CNF &cnf, Record &literal) {

 	ComparisonEngine cmp;

 	while(1)
 	{
 		if(mypage.GetFirst(&fetchme)==1 )		
 		{
 			if (cmp.Compare (&fetchme, &literal, &cnf))	
 				return 1;
 		}
 		else											//records in the page are over
 		{
 			MyPageIndex++;								//increase the page index
 			if(MyPageIndex<myfile.GetLength()-1)		
 				myfile.GetPage(&mypage,MyPageIndex);		
 			else										
 				return 0;
 		}
 	}

 }


 int Sorted::GetNext (Record &fetchme, CNF &cnf, Record &literal){

 	 OrderMaker *cnfOrder=new OrderMaker;
 	 OrderMaker *copySortOrder=new OrderMaker(sortInfo->myOrder->numAtts,(sortInfo->myOrder->whichAtts),(sortInfo->myOrder->whichTypes));

 	 if(mode==Writing){
 		 input->ShutDown();
 		 mode=Reading;
 		 Merge();
 		 b_query=false;
 		 sOrderRight=true;
 	 }

 	 if(sOrderRight){
			 if(!b_query){
				b_query=true;
				int search;
				cnf.GetSortOrders(*cnfOrder,*copySortOrder);
							 if(ToGetQuery(cnfOrder,(sortInfo->myOrder))){  //query sort order exists
								  search=BinarySearch(fetchme,cnf,literal);
								  ComparisonEngine myCompare;
								  if(search){
										 if (myCompare.Compare (&fetchme, &literal, &cnf))								//if the CNF is matching with the record
											return 1;
										 else{
											GetNextQuery(fetchme, cnf, literal);
												 }
										  }
									  else{
										  return 0;
									  }
								 }
							 else{//Handle sequential search
								 sOrderRight=false;
								 return SequentialGetNext(fetchme, cnf, literal);
							 }
					 }
					 else{
						GetNextQuery(fetchme, cnf, literal);
					 }
			 }
 	 else{
 	 return SequentialGetNext(fetchme, cnf, literal);
 	 }
 }

 int Sorted::ToGetQuery(OrderMaker *cnfOrder, OrderMaker *sortOrder){
 	
 	 int numAtts=0;
 	 int counter;
 	 int whichAtts[cnfOrder->numAtts];
 	 Type whichTypes[cnfOrder->numAtts];
 	 for(int i=0;i<sortOrder->numAtts;i++){

 		 for(counter=0;counter<cnfOrder->numAtts;counter++){

 			 if(cnfOrder->whichAtts[counter]==sortOrder->whichAtts[i]){
 				 whichAtts[numAtts]=numAtts;
 				 whichTypes[numAtts]=sortOrder->whichTypes[i];
 				 numAtts++;
 				 break;
 			 }

 		}

 		if(counter==cnfOrder->numAtts){
 			break;
 		}
 	 }
 	query = new OrderMaker(numAtts, whichAtts, whichTypes);
 	query->Print();
 	if(numAtts>0)
 		return 1;
 	else
 		return 0;

  }
 int Sorted::BinarySearch(Record& fetchme,CNF &cnf,Record &literal){

	off_t first=MyPageIndex;
	off_t last=myfile.GetLength()-2;
	ComparisonEngine myCompare;
	Record *currentRecord = new Record;
	Page *mPage=new Page;
	bool found=false;  
	myfile.GetPage(mPage,last-2);
	off_t midpoint=first;
	while(first < last){
		midpoint=(first+last)/2;
		myfile.GetPage(mPage,midpoint);
		if(mPage->GetFirst(&fetchme)==1 ){	//if record exists in the page
		if (myCompare.Compare (&literal, query, &fetchme,sortInfo->myOrder) <= 0){  //fetchme > literal
			last = midpoint;
			}
		else{
			first=midpoint;
			if(first == last -1){
				myfile.GetPage(mPage,last);
			    cerr<<"First page: "<<first<<endl;
				cerr<<"Last page: "<<last<<endl;
				cerr<<"Page size:"<<mPage->getCurSizeInBytes()<<endl;
				mPage->GetFirst(&fetchme);
			 //fetchme.Print(new Schema (catalog_path, orders));
			if (myCompare.Compare (&literal, query, &fetchme,sortInfo->myOrder) > 0)
			    midpoint=last;
				break;
				}
				}
				}
				else
				break;
	 }
	 //Scanning from page midpoint till end page

	 if(midpoint==MyPageIndex){		
		 	 while(mypage.GetFirst(&fetchme)==1){
		 		if( myCompare.Compare (&literal, query, &fetchme,sortInfo->myOrder) == 0 ){
		 			found=true;
		 			break;
		 	}
		 	}

	 }
	 else{	
		 myfile.GetPage(&mypage,midpoint);
			 while(mypage.GetFirst(&fetchme)==1){
						if( myCompare.Compare (&literal, query, &fetchme,sortInfo->myOrder) == 0 ){
								found=true;
								MyPageIndex=midpoint;
								break;
						}
			 }


	 }
	if(!found && midpoint < myfile.GetLength()-2){
					 myfile.GetPage(&mypage,midpoint+1);
					 if(mypage.GetFirst(&fetchme)==1 && myCompare.Compare (&literal, query, &fetchme,sortInfo->myOrder) == 0){
						 found=true;
						 MyPageIndex=midpoint+1;
					 }
	 }
	 if(!found)
		 return 0;
	 else
		 return 1;

 }

 int Sorted::Merge(){

 	 int err = 0;
 	 int i = 0;

 	 cout<<MyPageIndex<<endl<<endl;
 	 if(myfile.GetLength()!=0){
 		 cout<<"Move first called"<<endl;
 		 cout<<myfile.GetLength()<<endl;
 		 MoveFirst();
 	 }

 	Record *last = NULL, *prev = NULL;
 	Record *fromFile=new Record;
 	Record *fromPipe=new Record;
 	DBFile dbfile;
 	dbfile.Create("heap.tmp",heap,NULL);
 	int filecount=0,pipecount=0,totalcount=0;

 	int fpipe=output->Remove (fromPipe);
 	int ffile=GetNext(*fromFile);

 	ComparisonEngine myCompare;

 	while (1) {

 		if (fpipe && ffile) {
 			if (myCompare.Compare (fromPipe, fromFile, sortInfo->myOrder) >= 1) {  
 														
 				dbfile.Add(*fromFile);
 				++filecount;
 				ffile=GetNext(*fromFile);

 			}
 			else{
 				dbfile.Add(*fromPipe);
 				++pipecount;
 				fpipe=output->Remove (fromPipe);

 			}
 	 	}
 		else{
 			break;
 		}

 	}

 	while(ffile){
 		dbfile.Add(*fromFile);
 		++filecount;
 		ffile=GetNext(*fromFile);
 	}

 	while(fpipe){
 	 		dbfile.Add(*fromPipe);
 	 		++pipecount;
 	 		fpipe=output->Remove (fromPipe);

 	 	}

 	cout<<"Record number in file:"<<filecount<<endl;
 	cout<<"Record number in Pipe:"<<pipecount<<endl;
 	delete input;
 	delete output;
 	delete bigq;
 	dbfile.Close();

 	//cout<<"Path is"<<fpath<<endl;
 	remove (fpath);
 	remove("heap.tmp.meta");
 	//remove("heap.tmp");
 	rename("heap.tmp",fpath);

 	cout<<"CLOSED";

 }

 Sorted::~Sorted(){
}
