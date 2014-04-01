#include "BigQ.h"
BigQutil util;



void *TPMMS(void* arg){
        // read data from in pipe sort them into runlen pages
        char *tmpFile;
    File tempFile;
    vector<int> runIndex;
    int pageNum;

        BigQutil *bq = (BigQutil *) arg;
        cout<<"Sort start..."<<endl;
        tmpFile = "tempfile.bin";
        pageNum = 0;
        //TPPMS Phase-1
        //cout<<"stage0"<<endl;
        Page p;
        priority_queue<Record *, vector<Record *> , myComp> inmemQueue(bq->order);
        Record *extRecord, *inRecord;
        int countp=0;///, count;
        tempFile.Open(0, tmpFile);
        extRecord = new Record;
        //inRecord = new Record;
        //cout<<"stage1"<<endl;
        while (bq->inputPipe->Remove(extRecord)) {
            //cout<<"stage1"<<endl;
                inRecord = new Record;
                inRecord->Copy(extRecord);
                inmemQueue.push(inRecord);
                if (!p.Append(extRecord)) {
                       countp++;
                      // cout<<"countp "<<countp<<endl;
                       //cout<<"runlength "<<runlength;
                        if (countp == bq->runlen) {
                                //cout<<"count error";
                                //write the record vector to the file as a set of pages
                                Page p;
                                runIndex.push_back(tempFile.GetLength());
                                //cout<<"size:"<<runIndex.size()<<endl;
                                while(inmemQueue.size()!=0) {
                                    if (!p.Append(inmemQueue.top())) {
                                    tempFile.AddPage(&p, tempFile.GetLength());
                                    pageNum++;
                                    p.EmptyItOut();
                                    p.Append(inmemQueue.top());
                                    }
                                    inmemQueue.pop();
                                }
                                        //write the last page in the memory
                                tempFile.AddPage(&p, tempFile.GetLength());
                                pageNum++;
                                p.EmptyItOut();
                        inmemQueue.empty();
                                //clear countp
                        countp = 0;
                        }
                        //clear page p and add new records until the length =runlength
                      p.EmptyItOut();
                      p.Append(extRecord);
                }

        }
        //cout<<"stage2"<<endl;
        delete extRecord;
        if (inmemQueue.size() != 0) {
                Page p;
                runIndex.push_back(tempFile.GetLength());
                while(inmemQueue.size()!=0) {
                        if (!p.Append(inmemQueue.top())) {
                        tempFile.AddPage(&p, tempFile.GetLength());
                        pageNum++;
                        p.EmptyItOut();
                        p.Append(inmemQueue.top());
                }
                inmemQueue.pop();
        }
                 //write the last page in the memory
                tempFile.AddPage(&p, tempFile.GetLength());
                pageNum++;
                p.EmptyItOut();
                inmemQueue.empty();
        }

        runIndex.push_back(tempFile.GetLength());
        //cout<<"stage3"<<endl;
        tempFile.Close();
        //TPPMS Phase-2
        tempFile.Open(1, tmpFile);
        struct mergePQRec *pqRec;
        int numOfRuns = runIndex.size() - 1;
        //cout<<"numOfRuns"<<numOfRuns<<endl;
        int numofPages[numOfRuns]; 
        int i;  

        for (int j = 0; j < runIndex.size() - 1; j++){
                numofPages[j] = (runIndex[j + 1] - runIndex[j]) / 2;
                //cout<<numofPages[j];
        }

        Page mergePage[numOfRuns];//the first page for each run
        priority_queue<mergePQRec *, vector<mergePQRec *> , myCompPQ> PQ(bq->order);
        // run length number of head pages from file
        //initializing
        for (i = 0; i < numOfRuns; i++) {
                pqRec = new mergePQRec;
                mergePage[i].EmptyItOut();
                tempFile.GetPage(&mergePage[i], runIndex[i]);
                numofPages[i]--;
                runIndex[i] += 2;

                // Get First record from each page in memory
                if (mergePage[i].GetFirst(&(pqRec->mRec))) {
                        pqRec->noofrun = i;
                        // Insert the first records from each page into PQ
                        PQ.push(pqRec);
                }
        }
        // Pop the record from Priority queue and write it to output pipe
        while (!PQ.empty()) {
                pqRec = PQ.top();
                PQ.pop();
                i = pqRec->noofrun;
                bq->outputPipe->Insert(&(pqRec->mRec));
                pqRec = new mergePQRec;
                //get the next first record from the page 
                 if (mergePage[i].GetFirst(&(pqRec->mRec))) {
                        pqRec->noofrun = i;
                        PQ.push(pqRec);
                }
                // if old pages run out of records, get new page
                else {
                        if (numofPages[i] != 0) {
                                int runNoofPage = runIndex[i];
                                runIndex[i] += 2;
                                mergePage[i].EmptyItOut();
                                tempFile.GetPage(&mergePage[i], runNoofPage);
                                numofPages[i]--;
                                mergePage[i].GetFirst(&(pqRec->mRec));
                                pqRec->noofrun = i;
                                PQ.push(pqRec);
                        }
                } 
        }
        cout<<"Sort over..."<<endl;
        // Check if all runs exhausted, exit
        tempFile.Close();
        bq->outputPipe->ShutDown();
}
BigQ::BigQ(Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen) {

    // read data from in pipe sort them into runlen pages

    if (runlen <= 0) {
        cout << "Invalid Runlength" << endl;
        exit(1);
    }

    pthread_t worker;
    util = {&in, &out, &sortorder, runlen};
    pthread_create(&worker, NULL, TPMMS, &util);


    // construct priority queue over sorted runs and dump sorted data
    // into the out pipe
}

BigQ::~BigQ() {
        remove(tmpFile);
}
