#include "DBFile.h"



Sorted::Sorted(SortInfo *info){

        sortInfo = info;
        buffsz = 100;
        queryBuilt=false;
        sortOrderExists=true;
        bigq=NULL;
}

int Sorted::Create(char * filepath){
        mode=Reading;
        pageIndex=0;
        myPage.EmptyItOut();
        fpath=filepath;
        myFile.Open(0,filepath);                                                                                                                        //create a new bin file
        return 1;
}

int Sorted::Open(char * filepath){
        mode=Reading;
        pageIndex=0;
        myPage.EmptyItOut();
        fpath=filepath;
        myFile.Open(1,filepath);
        return 1;
}

void Sorted::InitBigQ(){
  input = new Pipe(buffsz);
  output = new Pipe(buffsz);
  bigq=new BigQ(*input, *output,*(sortInfo->myOrder),sortInfo->runLength);
}


void Sorted::Load(Schema &myschema, char *loadpath){

  FILE *table = fopen (loadpath, "r");            
  pageIndex=0;          
  Record temp;
  int counter=0;
  myPage.EmptyItOut();           
  while (temp.SuckNextRecord (&myschema, table) == 1) {
   counter++;
   if (counter % 10000 == 0)
    cerr <<".";
    Add(temp);       
  }
  fclose(table);        
}


 void Sorted::Add (Record &addme){
   if(mode==Reading){
    if(!bigq)
       InitBigQ();
      mode=Writing;
   }

  input->Insert(&addme);
 }
int Sorted ::GetNextQuery(Record &fetchme, CNF &cnf, Record &literal){

   ComparisonEngine engine;
    while(1){
          if(myPage.GetFirst(&fetchme)==1 ){         

            if(engine.Compare (&literal, query, &fetchme,sortInfo->myOrder) ==0){

      if (engine.Compare (&fetchme, &literal, &cnf))    
        return 1;
           }

           else
            return 0;
          }


          else{              

            pageIndex++;         
            if(pageIndex<myFile.GetLength()-1)         
       myFile.GetPage(&myPage,pageIndex);     
            else              
       return 0;
          }
         }
 }

 int Sorted::SequentialGetNext (Record &fetchme, CNF &cnf, Record &literal) {

        ComparisonEngine myComp;

        while(1)
        {
  if(myPage.GetFirst(&fetchme)==1 )          
  {
   if (myComp.Compare (&fetchme, &literal, &cnf))    
    return 1;
  }
  else        
  {
   pageIndex++;         
   if(pageIndex<myFile.GetLength()-1)          
    myFile.GetPage(&myPage,pageIndex);        
   else         
    return 0;
  }
        }

 }

 int Sorted::GetNext (Record &fetchme){
  if(mode==Writing){
    Merge();
  }
           if(myPage.GetFirst(&fetchme)==1)                                                                                                       //record exists in the page
                return 1;

        else{                                                                                                                                                           //records in the page are over

                pageIndex++;
                if(pageIndex<myFile.GetLength()-1)                                                                                 //next page exists
                {
                        myFile.GetPage(&myPage,pageIndex);                                                                        //load the next page from file
                        myPage.GetFirst(&fetchme);                                                                                                     //get the first record from the new page
                        return 1;
                }
                else                                                                                                                                                    // no page exists
                        return 0;
        }
 }



 int Sorted::GetNext (Record &fetchme, CNF &cnf, Record &literal){


  if(mode==Writing){

   Merge();
   queryBuilt=false;
   sortOrderExists=true;
  }

  if(sortOrderExists){
    if(!queryBuilt){
      queryBuilt=true;
     int search;


      query=new OrderMaker;
       if(cnf.GetQuerySortOrders(*query,*(sortInfo->myOrder))>0){  

        search=BinarySearch(fetchme,cnf,literal);

          ComparisonEngine engine;

          if(search){
    if (engine.Compare (&fetchme, &literal, &cnf))          
      return 1;


    else{
           GetNextQuery(fetchme, cnf, literal);
      }
     }

    else{
     return 0;
    }
         }
        else{
         sortOrderExists=false;
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

 int Sorted::BinarySearch(Record& fetchme,CNF &cnf,Record &literal){

  cerr<<"Binary Search"<<endl;
  off_t first=pageIndex;
  off_t last=myFile.GetLength()-2;

  Record *currentRecord = new Record;
  Page *midPage=new Page;
  bool found=false; 
  ComparisonEngine engine;
  off_t mid=first;

  while(first < last){
       mid=(first+last)/2;

      myFile.GetPage(midPage,mid);

         if(midPage->GetFirst(&fetchme)==1 ){      
          if (engine.Compare (&literal, query, &fetchme,sortInfo->myOrder) <= 0){  
    last = mid;
          }
          else{
           first=mid;

           if(first == last -1){
            myFile.GetPage(midPage,last);
      midPage->GetFirst(&fetchme);

            if (engine.Compare (&literal, query, &fetchme,sortInfo->myOrder) > 0)
           mid=last;
            break;
           }
          }
        }
         else
          break;
  }





  if(mid==pageIndex){  
    while(myPage.GetFirst(&fetchme)==1){
    if( engine.Compare (&literal, query, &fetchme,sortInfo->myOrder) == 0 ){
           found=true;
           break;
         }
    }

  }
  else{   
   myFile.GetPage(&myPage,mid);


    while(myPage.GetFirst(&fetchme)==1){
      if( engine.Compare (&literal, query, &fetchme,sortInfo->myOrder) == 0 ){
        found=true;
        pageIndex=mid;
        break;
      }
    }


  }

  
        if(!found && mid < myFile.GetLength()-2){
      myFile.GetPage(&myPage,mid+1);
      if(myPage.GetFirst(&fetchme)==1 && engine.Compare (&literal, query, &fetchme,sortInfo->myOrder) == 0){
       found=true;
       pageIndex=mid+1;
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

  input->ShutDown();

        mode=Reading;
        if(myFile.GetLength()!=0){
   MoveFirst();
  }

        Record *last = NULL, *prev = NULL;
        Record *fromFile=new Record;
        Record *fromPipe=new Record;
        DBFile dbfile;
        dbfile.Create("heap.tmp",heap,NULL);
        int filecount=0,pipecount=0,totalcount=0;

        int fp=output->Remove (fromPipe);
        int ff=GetNext(*fromFile);

        ComparisonEngine engine;

        while (1) {

  if (fp && ff) {
   if (engine.Compare (fromPipe, fromFile, sortInfo->myOrder) >= 1) {  
        
    dbfile.Add(*fromFile);
    ++filecount;
    ff=GetNext(*fromFile);

   }
   else{
    dbfile.Add(*fromPipe);
    ++pipecount;
    fp=output->Remove (fromPipe);

   }
  }
  else{
   break;
  }

        }

        while(ff){
  dbfile.Add(*fromFile);
  ++filecount;
  ff=GetNext(*fromFile);
        }

        while(fp){
   dbfile.Add(*fromPipe);
   ++pipecount;
   fp=output->Remove (fromPipe);

  }

        cout<<"File Count       "<<filecount;
        cout<<"Pipe Count       "<<pipecount;
        delete fromFile;
        delete fromPipe;
        delete input;
        delete output;
        delete bigq;

        dbfile.Close();


        remove (fpath);
        remove("heap.tmp.md");
        rename("heap.tmp",fpath);



 }


 void Sorted::MoveFirst () {

        if(mode==Writing)
        {
  Merge();

        }
        pageIndex=0;           
        myPage.EmptyItOut();          
        if(myFile.GetLength()>0)
        myFile.GetPage(&myPage,pageIndex);          
        queryBuilt=false;
        sortOrderExists=true;
 }


 int Sorted::Close(){

  if(mode==Writing){
   Merge();

  }
  myFile.Close();
  if(input){

  }
 }

 Sorted::~Sorted(){
  delete sortInfo->myOrder;
  delete sortInfo;
  delete query;
}

