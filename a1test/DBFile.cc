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
    File FileFile;
    Page FilePage;
    Record RecordRecord;
    int page_num;
    int pagenum;
DBFile::DBFile () {
    page_num = 0;
    pagenum=1;
}

int DBFile::Create (char *f_path, fType f_type, void *startup) {
	FileFile.Open(0, f_path);
    return 1;
}

void DBFile::Load (Schema &f_schema, char *loadpath) {
	FILE *fp;
    fp = fopen(loadpath, "r"); 
     while (RecordRecord.SuckNextRecord(&f_schema, fp))
           {     
               Add(RecordRecord);
           }
            
        FileFile.AddPage(&FilePage,page_num);
        FilePage.EmptyItOut();
      return;
}

int DBFile::Open (char *f_path) {
	FileFile.Open(1, f_path);
        return 1;
}

void DBFile::MoveFirst () {
	FileFile.GetPage(&FilePage, 0);  
	
}

int DBFile::Close () {
	return FileFile.Close();
}

void DBFile::Add (Record &rec) {

if(!FilePage.Append(&rec)){
   FileFile.AddPage(&FilePage, page_num++);
   FilePage.EmptyItOut();
   FilePage.Append(&rec);  
}
}

int DBFile::GetNext (Record &fetchme) {
	int getreturn = FilePage.GetFirst(&fetchme);    
    if(getreturn == 0)
        {
            if(pagenum + 1 >= FileFile.GetLength() && getreturn==0)
                 return 0;
            FileFile.GetPage(&FilePage,pagenum++);
            FilePage.GetFirst(&fetchme);
         }
         
     return 1;
}


int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
     ComparisonEngine comp;
     while(GetNext(fetchme))
     {
           if(comp.Compare(&fetchme,&literal,&cnf))       //compare the CNF conditions one by one
           {
            return 1;
           }
     }
            return 0;
}
