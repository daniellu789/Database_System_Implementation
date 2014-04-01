#include "BigQ.h"

RecComparison :: RecComparison(OrderMaker *order){
        this->order=order;
}

bool RecComparison::operator ()( Record* left, Record* right) const{
        ComparisonEngine engine;

        if (engine.Compare (left, right, order) < 0) 
                return true;
        else
                return false;
}

PQComparison :: PQComparison(OrderMaker *order){
        this->order=order;
}

bool PQComparison::operator ()( RunRecord* left, RunRecord* right) const{
        ComparisonEngine engine;
        if (engine.Compare (left->record, right->record, order) >= 0) 
                return true;
        else
                return false;
}

void *TPMMS(void* arg) {
  //define struct&value
  static int bufferCount;
  char tmpFile[100];
  sprintf(tmpFile,"buffer%d.tmp",bufferCount++);
  BigQStruct *bq = (BigQStruct *) arg;
  RecComparison recCompare(bq->order);
  vector<Record*> recStr;
  RecordPQ runPQ(PQComparison(bq->order));
  File myFile;
  Page myPage;
  off_t pageIndex = 0;
  myPage.EmptyItOut();
  Record temp, *rec;
  int counter = 0, popped = 0, pushed = 0;
  myFile.Open(0, tmpFile);
  int max_size = bq->runlen * PAGE_SIZE;
  int curSizeInBytes = sizeof(int);
  int recSize = 0;
  vector<int> pageIndices;
  int runCount = 0;
  //read from pipe and put into priority queue
  while (bq->inPipe->Remove(&temp) == 1) {
    recSize = (&temp)->GetSize(); 
    Record *newrec = new Record;
    newrec->Consume(&temp);
    if (curSizeInBytes + recSize <= max_size) {
        recStr.push_back(newrec);
        pushed++;
        curSizeInBytes += recSize;
       }
    else {
      runCount++;
      pageIndices.push_back(pageIndex);
      sort(recStr.begin(), recStr.end(), recCompare);
      for (int i = 0; i < recStr.size(); i++) {
      rec = recStr.at(i);
      if (myPage.Append(rec) == 0) {
        myFile.AddPage(&myPage, pageIndex++);
        myPage.EmptyItOut();
        myPage.Append(rec);
      }
      delete rec;
      }
      recStr.clear();
      if (myPage.getCurSizeInBytes() > 0) {
      myFile.AddPage(&myPage, pageIndex++);
      myPage.EmptyItOut();
      }
      recStr.push_back(newrec);
      curSizeInBytes = sizeof(int) + recSize;
       }
      }//when add to PQ,sort
        sort(recStr.begin(), recStr.end(), recCompare);

        pageIndices.push_back(pageIndex);

        for (int i = 0; i < recStr.size(); i++) {
       rec = recStr.at(i);

       if (myPage.Append(rec) == 0) {
            myFile.AddPage(&myPage, pageIndex++); 
            myPage.EmptyItOut(); 
            myPage.Append(rec);
       }
       delete rec;
        }
        recStr.clear();

        if (myPage.getCurSizeInBytes() > 0) {
            myFile.AddPage(&myPage, pageIndex++);
            myPage.EmptyItOut();
        }
        //phase2
        pageIndices.push_back(pageIndex);
        int numOfRuns = pageIndices.size() - 1;
        Run *runs[numOfRuns];
        for (int i = 0; i < numOfRuns; i++) {
        Record* tmprec = new Record;
        runs[i] = new Run(&myFile, pageIndices[i], pageIndices[i + 1]);
        runs[i]->GetNext(tmprec);
        RunRecord* runRecord = new RunRecord(tmprec, runs[i]);
        runPQ.push(runRecord);

        }
        
        RunRecord *popPQ;
        Record* popRec;
        Run* popRun;
        while (!runPQ.empty()) {
        popPQ = runPQ.top();
        runPQ.pop();
        popRun = popPQ->run;
        popRec = popPQ->record;
        bq->outPipe->Insert(popRec);
        delete popRec;
        Record* nextRecord = new Record;
        if (popRun->GetNext(nextRecord) == 1) {
          popPQ->record = nextRecord;
          runPQ.push(popPQ);
        } 
       else {
          delete popRun;
          delete popPQ;
          delete nextRecord;
       }

        }
        bq->outPipe->ShutDown();
        myFile.Close();
        remove(tmpFile);
}

BigQ::BigQ(Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen) {
        pthread_t worker;
        BigQStruct *struc = new BigQStruct{&in, &out, &sortorder, runlen};
        pthread_create(&worker, NULL, TPMMS, struc);

}

BigQ::~BigQ() {
}
