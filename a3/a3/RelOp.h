#ifndef REL_OP_H
#define REL_OP_H

#include "Pipe.h"
#include "DBFile.h"
#include "Record.h"
#include "Function.h"
#include <malloc.h>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include <sstream>


class RelationalOp {
	public:
	// blocks the caller until the particular relational operator 
	// has run to completion
	virtual void WaitUntilDone () = 0;

	// tell us how much internal memory the operation can use
	virtual void Use_n_Pages (int n) = 0;

};

class SelectFile : public RelationalOp { 

	private:
    DBFile *selFile_inFile;
    Pipe *selFile_out;
    CNF  *selFile_Op;
    Record *selFile_record;
    pthread_t thread;
    int selFile_runlen;

	public:
	SelectFile() {
	};	
	~SelectFile(){
	};

	void Run (DBFile &inFile, Pipe &PipeOut, CNF &selOp, Record &literal);
	void WaitUntilDone ();
	void Use_n_Pages (int n);
	void selFile_Operation();

};

class SelectPipe : public RelationalOp {
    private:
    Pipe *PipeSelect_in;
    Pipe *PipeSelect_out;
    CNF PipeSelect_Op;
    Record *PipeSelect_record;
    pthread_t thread;
    int PipeSelect_runlen;

	public:
	SelectPipe() {	
	};
	~SelectPipe(){
	};
	void Run (Pipe &PipeIn, Pipe &PipeOut, CNF &selOp, Record &literal);
	void WaitUntilDone ();
	void Use_n_Pages (int n);
	void PipeSelect_Operation();
};


class Project : public RelationalOp { 
    private:
    Pipe *PipeProject_in;
    Pipe *PipeProject_out;
    int *pro_keepme;
    int pro_numAttsInput;
    int pro_numAttsOutput;
    pthread_t thread;
    int pro_runlen;

	public:
	Project(){
	}
	~Project(){
	};
	void Run (Pipe &PipeIn, Pipe &PipeOut, int *keepMe, int numAttsInput, int numAttsOutput);
	void WaitUntilDone ();
	void Use_n_Pages (int n);
	void pro_Operation();

};

class Join : public RelationalOp {

//public:
private:
        pthread_t thread;
        Pipe *PipeJoinInL;
        Pipe *PipeJoinInR;
        Pipe *PipeJoinOut;
        CNF *j_selOp;
        Record *j_literal;
        void* join_run(void* arg);
        //void *workerFunction(void* arg);
        int runlen;
public:
        void Run (Pipe &PipeInL, Pipe &PipeInR, Pipe &PipeOut, CNF &selOp, Record &literal);
        void WaitUntilDone () ;
        void Use_n_Pages (int n);
        void Join_Operaion();
};

class DuplicateRemoval : public RelationalOp {
    private:
    Pipe *dr_in;
    Pipe *dr_out;
    Schema *dr_schema;
    pthread_t thread;
    int dr_runlen;

	public:
	DuplicateRemoval(){
	};
	~DuplicateRemoval(){
	};
	void Run (Pipe &PipeIn, Pipe &PipeOut, Schema &mySchema);
	void WaitUntilDone ();
	void Use_n_Pages (int n);
	void dr_Operation();
};


class Sum : public RelationalOp {

private:

        pthread_t thread;
        void *workerFunction(void* arg);

public:

        void Run (Pipe &PipeIn, Pipe &PipeOut, Function &computeMe) ;
        void WaitUntilDone ();
        void Use_n_Pages (int n);
};

class GroupBy : public RelationalOp {


        pthread_t thread;
        int runlen;


public:
        void Run (Pipe &PipeIn, Pipe &PipeOut, OrderMaker &groupAtts, Function &computeMe) ;
        void WaitUntilDone ();
        void Use_n_Pages (int n);
};

class WriteOut : public RelationalOp {
	private:
	Pipe *wo_in;
	FILE *wo_outFile;
	Schema *wo_schema;
	pthread_t thread;
	int wo_runlen;

	public:
	WriteOut(){
	};
	~WriteOut(){
	};
	void Run (Pipe &PipeIn, FILE *outFile, Schema &mySchema);
	void WaitUntilDone ();
	void Use_n_Pages (int n);
	void wo_Operation();
};

#endif
