#ifndef BIGQ_H
#define BIGQ_H
#include <pthread.h>
#include <iostream>
#include "Pipe.h"
#include "File.h"
#include "Record.h"
#include "Comparison.h"
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

class BigQ {

private:
    char *tmpFile;
    File tempFile;
    vector<int> runIndex;
    int pageNum;
public:
    BigQ(Pipe &in, Pipe &out, OrderMaker &sortorder, int runlength);
    ~BigQ();
};



struct mergePQRec
{
    Record mRec;
    int noofrun;
};

class myComp
{
    private:
        OrderMaker* sortOrder;
    public:
        myComp(OrderMaker *sortorder)
        {
            sortOrder = sortorder;
        }
        bool operator()(Record* left,Record* right)
        {
            ComparisonEngine comparisonEngine;
                        if(comparisonEngine.Compare(left,right,sortOrder)<0)
                                return false;
                        else
                                return true;
                }
};

class myCompPQ
{
    private:
        OrderMaker* sortOrder;
    public:
        myCompPQ(OrderMaker *sortorder)
        {
            sortOrder = sortorder;
        }
        bool operator()(mergePQRec *leftRecord, mergePQRec *rightRecord)
        {
            ComparisonEngine comparisonEngine;
                        if(comparisonEngine.Compare(&(leftRecord->mRec), &(rightRecord->mRec), sortOrder)<=0)
                                return false;
                        else
                                return true;

                }
};




#endif
