#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"
#include <iostream>

// stub file .. replace it with your own DBFile.cc

DBFile::DBFile () {
	pageNum=0;
}

int DBFile::Create (char *f_path, fType f_type, void *startup) {
	if (f_type==heap){	
		myFile.Open(0,f_path);	
		return 1;
	}
	else{
		cout<<"Error, cannot create file."<<endl;
		return 0;
	}
}

void DBFile::Load (Schema &f_schema, char *loadpath) {
	pageNum=0;
	FILE *pFile=fopen(loadpath,"r");
	int count=0;
    while (myRecord.SuckNextRecord(&f_schema, pFile))
    {
		if(!myPage.Append(&myRecord))
		    {
		        myFile.AddPage(&myPage, pageNum);
		        pageNum++;
		        myPage.EmptyItOut();
		        myPage.Append(&myRecord);
		    }
		    count++;
    }   
    myFile.AddPage(&myPage,pageNum++);
    myPage.EmptyItOut();
    fclose(pFile);
    cout<<endl<<"There are "<<count<<" records."<<endl;
	return;
}

int DBFile::Open (char *f_path) {
	myFile.Open(1,f_path);
    cout<<"File "<<f_path<<" opened."<<endl;
    return 1;
}

void DBFile::MoveFirst () {
      myPage.EmptyItOut();
      myFile.GetPage(&myPage,0);
}

int DBFile::Close () {
	int curLength;
	pageNum=0;
	curLength=myFile.Close();//see if it works;the length of file
	cout<<"File lenghth is "<<curLength<<endl;
	return 1;
}

void DBFile::Add (Record &rec) {
                     if(!myPage.Append(&rec)){
                     myFile.AddPage(&myPage,pageNum++);
                     myPage.EmptyItOut();
                     myPage.Append(&rec);
                   return;  
                  }   
           else
           myFile.AddPage(&myPage,pageNum);
}
/*void DBFile::Add (Record &rec) {

    Page p;
    p.EmptyItOut();
    int pageNum=myFile.GetLength();
    if (pageNum==0){
        p.Append(&rec);
        cout<<myFile.GetLength();
        myFile.AddPage(&p,0);
        cout<<myFile.GetLength();
        p.EmptyItOut();
    }

    else{
        myFile.GetPage(&p,pageNum); 
        pageNum--;   
        if(!p.Append(&rec))
            {
                p.EmptyItOut();
                p.Append(&rec);
                myFile.AddPage(&p,pageNum);
            }
        else{
            cout<<pageNum;
        p.Append(&rec);
        myFile.AddPage(&p,pageNum);
        }
    }
}*/
/*void DBFile::Add (Record &rec) {
int page_num=myFile.GetLength();
bool swi=myPage.Append(&rec);
if(!swi){
   myFile.AddPage(&myPage, page_num);
   myPage.EmptyItOut();
   myPage.Append(&rec); 
}}*/

int DBFile::GetNext (Record &fetchme) {
	Record tRec;
    if(!myPage.GetFirst(&tRec))
    {
        myPage.EmptyItOut();
        if (pageNum == myFile.GetLength()-2)
            return 0;
        pageNum++;
        myFile.GetPage(&myPage, pageNum);
        if(!myPage.GetFirst(&tRec))
        {
            return 0;
        }
    }
    fetchme.Consume(&tRec);
    return 1;
}

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
    ComparisonEngine comp;
    while (GetNext(fetchme))
    {
        if (comp.Compare(&fetchme, &literal, &cnf))
        {
            return 1;
        }
    }
    return 0;
}

DBFile::~DBFile(){
	myPage.EmptyItOut();
	pageNum=0;
}