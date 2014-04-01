#ifndef STATISTICS_
#define STATISTICS_
#include "ParseTree.h"
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

struct RelInfo
{
        unsigned long long int TupleNum;   
        int numPartition;         // partition this relation belongs to -1 signifies that its still singleton
        map <string, unsigned long long int> mAtts;  // <attribute-name, distinct values>

        RelInfo(): TupleNum(0), numPartition(-1)
        {}              
};


class Statistics
{
private:
        int PartitionNum;
        bool RelationsCopied;
        map <string, RelInfo> mRelations;            // can be modified with Apply()
        map <string, RelInfo> mRelationsCopy;        // is written to file
        map <int, vector<string> > mPartitionInfo;
        map <string, vector <string> > mAttToRel;   //att, vector <relations>
       
        struct AttCountAndEstimate
        {
            int repeatCount;  //number of times same Attumn is repeated
            long double estimate;   
        };


    bool ErrorCheck(struct AndList *parseTree, char *relNames[], int numToJoin, vector<string>&, set<string>&);
       
public:
        Statistics();
        Statistics(Statistics &copyMe);  
        ~Statistics();


        void AddRel(char *relName, int TupleNum);
        void AddAtt(char *relName, char *attName, int numDistincts);
        void CopyRel(char *oldName, char *newName);
       
        void Read(char *fromWhere);
        void Write(char *fromWhere);

        void  Apply(struct AndList *parseTree, char *relNames[], int numToJoin);
        double Estimate(struct AndList *parseTree, char **relNames, int numToJoin);



        int GetPartitionNumber()
        {
                return PartitionNum;
        }
       
        map<string, RelInfo> * GetRelations()
        {
                return &mRelations;   
        }
       
        map<int, vector<string> > * GetPartitionInfoMap()
        {
                return &mPartitionInfo; 
        }
};

class TrasferToStr
{
public:
    static string tras(int orginal)
    {
        stringstream s;
        s << orginal;
        return s.str();
    }

};

#endif

