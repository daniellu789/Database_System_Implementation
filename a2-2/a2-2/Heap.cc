#include "DBFile.h"
Heap::Heap(){

}


int Heap::Create(char * f_path){
cout<<"HEAP CREATE"<<endl;
	toWrite=true;

	MyPageIndex=0;
	fpath=f_path;
	myfile.Open(0,f_path);															//create a new bin file
	return 1;
}

int Heap::Open(char * f_path){
cout<<"HEAP OPEN"<<endl;
	toWrite=false;

	MyPageIndex=0;
	mypage.EmptyItOut();
	fpath=f_path;
	myfile.Open(1,f_path);
	return 1;
}


void Heap::Load (Schema &f_schema, char *loadpath) {



	FILE *tbl_path = fopen (loadpath, "r");												//open the tbl file
	MyPageIndex=0;																		//set the current page index for the bin file to 0
	Record temp;
	int counter=0;
	mypage.EmptyItOut();																//Empty the page before starting to load
	while (temp.SuckNextRecord (&f_schema, tbl_path) == 1) {
		counter++;

		if (counter % 10000 == 0) {
			cerr << counter << "\n";
		}

		Add(temp);																		// add record to page

	}

	myfile.AddPage(&mypage,MyPageIndex++);											//dirty write
	mypage.EmptyItOut();																//empty the last page
	fclose(tbl_path);																	//close the tbl file

}






void Heap::Add (Record &rec) {


	if(mypage.Append(&rec)==0){														// append record to the page.if page is full return 0
			myfile.AddPage(&mypage,MyPageIndex++);									// since page is full , add page to file

			mypage.EmptyItOut();														// empty the page
			mypage.Append(&rec);														// append the record to the new page
		}

}

int Heap::GetNext (Record &fetchme) {
//cout<<"HEAP GETNEXT"<<endl;
	if(mypage.GetFirst(&fetchme)==1)													//record exists in the page
		return 1;

	else{																				//records in the page are over

		MyPageIndex++;
		if(MyPageIndex<myfile.GetLength()-1)											//next page exists
		{
			myfile.GetPage(&mypage,MyPageIndex);									//load the next page from file
			mypage.GetFirst(&fetchme);													//get the first record from the new page
			return 1;
		}
		else																			// no page exists
			return 0;
	}

}
int Heap::GetNext (Record &fetchme, CNF &cnf, Record &literal) {

	ComparisonEngine comp;

	while(1)
	{
		if(mypage.GetFirst(&fetchme)==1 )												//if record exists in the page
		{
			if (comp.Compare (&fetchme, &literal, &cnf))								//if the CNF is matching with the record
				return 1;
		}
		else																			//records in the page are over
		{
			MyPageIndex++;																//increase the page index
			if(MyPageIndex<myfile.GetLength()-1)										//MyPageIndex is less than the number of pages in the file(  MyPageIndex starts with 0, so when we give 0 , we get the 1st page. thus MyPageIndex less than curlength-1)
				myfile.GetPage(&mypage,MyPageIndex);								//load the next page from the file
			else																		// No page exists
				return 0;
		}
	}

}

void Heap::MoveFirst () {

	MyPageIndex=0;																		//set the page index to the first page
	mypage.EmptyItOut();																// empty the mypage first
	myfile.GetPage(&mypage,MyPageIndex);											// Load the first page from the bin file to the current Page

}




int Heap::Close(){

	cout<<"Heap Close\n";
	cout<<"Write mode: "<<toWrite<<endl;
	//cout<<"HeapBd "<<mypage.getCurSizeInBytes()<<endl;
	//<<"wow"<<endl;
	//cout<<"Page index:"<<MyPageIndex<<endl;
	if(toWrite && mypage.getCurSizeInBytes()>0){

			myfile.AddPage(&mypage,MyPageIndex++);											//dirty write
			mypage.EmptyItOut();																					//empty the last page
			toWrite=false;
	}

		myfile.Close();																							//close the bin file
		return 1;

}

Heap::~Heap(){
}


