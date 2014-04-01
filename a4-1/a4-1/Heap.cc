#include "DBFile.h"
Heap::Heap(){

}


int Heap::Create(char * filepath){

        toWrite=true;

        pageIndex=0;
        myPage.EmptyItOut();
        fpath=filepath;
        myFile.Open(0,filepath);                                                                                                                        //create a new bin file
        return 1;
}

int Heap::Open(char * filepath){

        toWrite=false;
        pageIndex=0;
        myPage.EmptyItOut();
        fpath=filepath;
        myFile.Open(1,filepath);
        return 1;
}


void Heap::Load (Schema &f_schema, char *loadpath) {



        FILE *tbl_path = fopen (loadpath, "r");                         
        pageIndex=0;                                
        Record temp;
        int counter=0;
        myPage.EmptyItOut();                               
        while (temp.SuckNextRecord (&f_schema, tbl_path) == 1) {
         counter++;

         if (counter % 10000 == 0) {
          cerr << counter << "\n";
         }

         Add(temp);                                     

        }

        myFile.AddPage(&myPage,pageIndex++);                       
        myPage.EmptyItOut();                              
        fclose(tbl_path);                           

}






void Heap::Add (Record &rec) {


        if(myPage.Append(&rec)==0){                        
          myFile.AddPage(&myPage,pageIndex++);                    

          myPage.EmptyItOut();                            
          myPage.Append(&rec);                            
         }

}

int Heap::GetNext (Record &fetchme){

            if(myPage.GetFirst(&fetchme)==1)                                                                                                     
                return 1;

        else{                                                                                                                                                           

                pageIndex++;
                if(pageIndex<myFile.GetLength()-1)                                                                               
                {
                        myFile.GetPage(&myPage,pageIndex);                                                                        
                        myPage.GetFirst(&fetchme);                                                                                                     
                        return 1;
                }
                else                                                                                                                                                    
                        return 0;
        }

}


int Heap::GetNext (Record &fetchme, CNF &cnf, Record &literal) {

        ComparisonEngine comp;

        while(1)
        {
         if(myPage.GetFirst(&fetchme)==1 )                        
         {
          if (comp.Compare (&fetchme, &literal, &cnf))                  
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

void Heap::MoveFirst () {

        pageIndex=0;                                
        myPage.EmptyItOut();                                
        myFile.GetPage(&myPage,pageIndex);                        

}




int Heap::Close(){


        if(toWrite && myPage.getCurSizeInBytes()>0){

          myFile.AddPage(&myPage,pageIndex++);                       
          myPage.EmptyItOut();                                     
          toWrite=false;
        }

         myFile.Close();                                          
         return 1;

}

Heap::~Heap(){
}

