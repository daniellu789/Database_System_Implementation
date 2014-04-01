#include "Statistics.h"
#include <string.h>
using namespace std;

Statistics::Statistics() 
{
  PartitionNum = 0;
  RelationsCopied = false;
}


Statistics::Statistics(Statistics &copyMe)
{
   PartitionNum = copyMe.GetPartitionNumber();
   map <string, RelInfo> * pmRelations = copyMe.GetRelations();

   map <string, unsigned long long int >::iterator mAtts_it;
   map <string, RelInfo>::iterator rel_it = pmRelations->begin();

   // copy the RelInfo structure
   for ( ;rel_it != pmRelations->end(); rel_it++)
   {
      RelInfo relinfo;
      relinfo.TupleNum = rel_it->second.TupleNum;
      relinfo.numPartition = rel_it->second.numPartition;
      for (mAtts_it = rel_it->second.mAtts.begin();
          mAtts_it != rel_it->second.mAtts.end();
          mAtts_it++)
      {
         relinfo.mAtts[mAtts_it->first] = mAtts_it->second;
      }
      mRelations[rel_it->first] = relinfo;
   }
   
   // copy mPartitionInfo
   map <int, vector<string> > * pPartitionInfo = copyMe.GetPartitionInfoMap();
   map <int, vector<string> >::iterator partInfo_it;
   for (partInfo_it = pPartitionInfo->begin();
       partInfo_it != pPartitionInfo->end(); partInfo_it++)
   {
      vector<string> vecRelNames;
      vector<string> * pVec = &partInfo_it->second;
      for (int i = 0; i < pVec->size(); i++)
      {
         vecRelNames.push_back(pVec->at(i));
      }
      mPartitionInfo[partInfo_it->first] = vecRelNames;
   }
}

Statistics::~Statistics()
{ }

void Statistics::AddRel(char *relName, int TupleNum)
{
   
   map <string, RelInfo>::iterator rel_it;
   rel_it = mRelations.find(string(relName));

   if (rel_it == mRelations.end()) //if not found, add it
   {
      RelInfo relinfo;
      relinfo.TupleNum = TupleNum;
      mRelations[string(relName)] = relinfo;
   }
   else //if found, update the TupleNum
   {
      rel_it->second.TupleNum = TupleNum;
   }
}

void Statistics::AddAtt(char *relName, char *attName, int numDistincts)
{
   
    map <string, RelInfo>::iterator rel_it;
    rel_it = mRelations.find(string(relName));


    if (rel_it == mRelations.end())   // if not found the relation name in mRelations, add it
    {
   RelInfo relinfo;
      relinfo.mAtts[string(attName)] = numDistincts;

   mRelations[string(relName)] = relinfo;

      vector<string> vec_RelName;
      vec_RelName.push_back(string(relName));
      mAttToRel[string(attName)] = vec_RelName;
    }
    else //if found the relation name in mRelations, update the att
    {
      map <string, unsigned long long int>::iterator att_it;
      att_it = (rel_it->second).mAtts.find(string(attName));
      if (att_it == (rel_it->second).mAtts.end()) // if not found the att name, add it 
      {
         (rel_it->second).mAtts[string(attName)] = numDistincts;
         vector<string> vec_RelName;
      vec_RelName.push_back(string(relName));
      mAttToRel[string(attName)] = vec_RelName;
      }
      else    // if found the att name, update the it
         att_it->second = numDistincts;
    }
}

void Statistics::CopyRel(char *oldName, char *newName)
{
   map <string, RelInfo>::iterator rel_it;
   map <string, vector <string> >::iterator mAttToRel_it;
   rel_it = mRelations.find(string(oldName)); //find the old rel name

   RelInfo *pOldRelInfo = &(rel_it->second);

   RelInfo relinfo;
   relinfo.TupleNum = pOldRelInfo->TupleNum; //fetch the TupleNum

   map <string, unsigned long long int>::iterator mAtts_it;
   for (mAtts_it = pOldRelInfo->mAtts.begin(); mAtts_it != pOldRelInfo->mAtts.end(); mAtts_it++)
   {
      relinfo.mAtts[mAtts_it->first] = mAtts_it->second; //fetch the distinct num of the att

      mAttToRel_it = mAttToRel.find(mAtts_it->first);

      if (mAttToRel_it == mAttToRel.end())
      {
            cerr << "\nAttumn name " << (mAtts_it->first).c_str()
                  << " not found in mAttToRel! ERROR!\n";
            return;
      }

      (mAttToRel_it->second).push_back(string(newName));
   }

   mRelations[string(newName)] = relinfo;

}

void Statistics::Read(char *fromWhere)
{
   FILE * fptr = fopen(fromWhere, "r");
   if (fptr == NULL)
      return;

    char buffer[200];

   string relName, attName;
   unsigned long long int numValues;
    map <string, vector <string> >::iterator mAttToRel_it;

   while (fscanf(fptr, "%s", buffer) != EOF)  
   {
      if (strcmp(buffer, "----------") == 0)
      {
         RelInfo relinfo;
         fscanf(fptr, "%s", buffer); //get the rel name
         relName = buffer;
         fscanf(fptr, "%llu", &relinfo.TupleNum); // then the tuple num of this rel
         fscanf(fptr, "%s", buffer); //get the att name
         while (strcmp(buffer, "__________") != 0) 
         {
            attName = buffer;
            fscanf(fptr, "%llu", &numValues);  //get distinct num of that att
            relinfo.mAtts[attName] = numValues;  

      mAttToRel_it = mAttToRel.find(string(attName));
      if (mAttToRel_it == mAttToRel.end())  //if not find att num in mAttToRel map, add it
      {
       vector<string> vec_relName;
          vec_relName.push_back(string(relName));
          mAttToRel[string(attName)] = vec_relName;
      }
      else 
          (mAttToRel_it->second).push_back(string(relName));//
           
            fscanf(fptr, "%s", buffer);
         }
         mRelations[relName] = relinfo;
      }
   }      
}

void Statistics::Write(char *toWhere)
{
   FILE * fptr = fopen(toWhere, "w");
   map <string, RelInfo>::iterator rel_it;
   map <string, unsigned long long int>::iterator mAtts_it;

   for (rel_it = mRelations.begin();
       rel_it != mRelations.end(); rel_it++)
   {
      fprintf(fptr, "\n----------");
      fprintf(fptr, "\n%s", rel_it->first.c_str()); //write the rel name
      RelInfo * pTInfo = &(rel_it->second);
      fprintf(fptr, "\n%llu", pTInfo->TupleNum); //write the tuple num of that rel
      for (mAtts_it = pTInfo->mAtts.begin();
          mAtts_it != pTInfo->mAtts.end(); mAtts_it++)
      {
         fprintf(fptr, "\n%s", mAtts_it->first.c_str()); //write the att name
         fprintf(fptr, " %llu", mAtts_it->second);  //write the distinct num of that att
      }
      fprintf(fptr, "\n__________\n");
   }
  
   fclose(fptr);
}

void  Statistics::Apply(struct AndList *parseTree, char *relNames[], int numToJoin)
{
  

   double EstResult = Estimate(parseTree, relNames, numToJoin);
   if (EstResult == -1)   // Error in estimation
   {
      cerr << "\nError during Estimate() in Apply()\n";
      return;
   }
   else
   {

      set <string> join_Rel_set;
      vector<string> vec_joinmAtts;
      if (!ErrorCheck(parseTree, relNames, numToJoin, vec_joinmAtts, join_Rel_set))
      {
         cerr << "\nError during CNF parsing in Apply().\n";
         return;
      }
      else //join_Rel_set contains the rels to be joined
      {

         int nOldPartitionNum = -1;
         string sOldJoinRelName;
         set <string>::iterator set_it = join_Rel_set.begin();
         map <string, RelInfo>::iterator rel_it;
       for (; set_it != join_Rel_set.end(); set_it++)
       {
            rel_it = mRelations.find(*set_it);
            if (rel_it == mRelations.end())
      {
          cerr << "\n Rel " << (*set_it).c_str() << " details not found! error! \n";
          return;
      }      
            if ((rel_it->second).numPartition != -1)
            {
               nOldPartitionNum = (rel_it->second).numPartition;
               break;
            }
         }

         if (nOldPartitionNum == -1)
         {
            PartitionNum++;
            vector <string> vec_RelNames;
          for (set_it = join_Rel_set.begin(); set_it != join_Rel_set.end(); set_it++)
      {
         rel_it = mRelations.find(*set_it);
          (rel_it->second).numPartition = PartitionNum;
         (rel_it->second).TupleNum = (unsigned long long int)EstResult;

          vec_RelNames.push_back(*set_it);
          }
          mPartitionInfo[PartitionNum] = vec_RelNames;
         }

         else  // Old partition need to be modified   
         {

            vector <string> vec_RelNames = mPartitionInfo[nOldPartitionNum];
            for (int i=0; i<vec_RelNames.size(); i++)
               join_Rel_set.insert(vec_RelNames.at(i));

            vec_RelNames.clear();
            for (set_it = join_Rel_set.begin(); set_it != join_Rel_set.end(); set_it++)
            {
               rel_it = mRelations.find(*set_it);
               (rel_it->second).numPartition = nOldPartitionNum;
               (rel_it->second).TupleNum = (unsigned long long int)EstResult;
     

               vec_RelNames.push_back(*set_it);
            }

            mPartitionInfo[nOldPartitionNum] = vec_RelNames;
         }
      }
   }
}

bool Statistics::ErrorCheck(struct AndList *someParseTree, char *relNames[], int numToJoin, vector<string>& joinmAttsInPair, set<string>& Rel_names_set)
{

   int prefixedRelNamePos;
   string sRelName, sAttName;
   map<string, vector<string> >::iterator mAttToRel_it;

    AndList* parseTree = someParseTree;
   
       for (int i = 0; i < numToJoin; i++)
    {
       if(mRelations.find(relNames[i]) == mRelations.end())
       return false;
    }

    while(parseTree != NULL)
    {
   OrList* theOrList = parseTree->left;
   while(theOrList != NULL) 
   {
       ComparisonOp* theComparisonOp = theOrList->left;
       if(theComparisonOp == NULL)
      break;

       int leftCode = theComparisonOp->left->code;
       string leftVal = theComparisonOp->left->value;

       joinmAttsInPair.push_back(TrasferToStr::tras(leftCode));
       joinmAttsInPair.push_back(leftVal);   

       joinmAttsInPair.push_back(TrasferToStr::tras(theComparisonOp->code));

       int rightCode = theComparisonOp->right->code;
       string rightVal = theComparisonOp->right->value;

       joinmAttsInPair.push_back(TrasferToStr::tras(rightCode));
       joinmAttsInPair.push_back(rightVal); 

       if(leftCode == NAME) 
       {

      prefixedRelNamePos = leftVal.find(".");
      if (prefixedRelNamePos != string::npos)  
      {
          sRelName = leftVal.substr(0, prefixedRelNamePos);
          sAttName = leftVal.substr(prefixedRelNamePos + 1);
         mAttToRel_it = mAttToRel.find(sAttName);
         if (mAttToRel_it == mAttToRel.end())     
         return false;
      }
      else
      {
          sAttName = leftVal;
         mAttToRel_it = mAttToRel.find(sAttName);
         if (mAttToRel_it == mAttToRel.end())  
         return false;

          if ((mAttToRel_it->second).size() > 1)
         return false;  
          else
        sRelName = (mAttToRel_it->second).at(0);
      }
      Rel_names_set.insert(sRelName);
       }
       if(rightCode == NAME)  
       {

      prefixedRelNamePos = rightVal.find(".");
      if (prefixedRelNamePos != string::npos)    
      {
          sRelName = rightVal.substr(0, prefixedRelNamePos);
          sAttName = rightVal.substr(prefixedRelNamePos + 1);
         mAttToRel_it = mAttToRel.find(sAttName);
         if (mAttToRel_it == mAttToRel.end()) 
         return false;
      }
      else
      {
          sAttName = rightVal;
         mAttToRel_it = mAttToRel.find(sAttName);
         if (mAttToRel_it == mAttToRel.end())
         return false;

          if ((mAttToRel_it->second).size() > 1)
         return false;  
          else
         sRelName = (mAttToRel_it->second).at(0);
      }
      Rel_names_set.insert(sRelName);
       }

       if(theOrList->rightOr != NULL)
      joinmAttsInPair.push_back("OR");
       theOrList = theOrList->rightOr;
   }

   if(parseTree->rightAnd != NULL)
       joinmAttsInPair.push_back("AND");
   else
       joinmAttsInPair.push_back(".");
   parseTree = parseTree->rightAnd;
    }
    
   for (int i=0; i<numToJoin; i++)
   {
      sRelName = relNames[i];
      int partitionNum = mRelations[sRelName].numPartition;
      if (partitionNum != -1)
      {

         vector<string> vec_tbl_names = mPartitionInfo[partitionNum];
         for (int j = 0; j < vec_tbl_names.size(); j++)
         {
            string t1 = vec_tbl_names.at(j);
            bool found = false;
            for (int k = 0; k < numToJoin; k++)
            {
               string t2 = relNames[k];
               if (t1.compare(t2) == 0)
               {
                  found = true;
                  break;
               }
            }      
            if (found == false)
               return false;      
         }
      }
   }

   set <string>::iterator set_it = Rel_names_set.begin();
    for (; set_it != Rel_names_set.end(); set_it++)
   {
      string t1 = *set_it;
      bool found = false;
   for (int k = 0; k < numToJoin; k++)
      {
      string t2 = relNames[k];
       if (t1.compare(t2) == 0)
       {  
      found = true;
      break;
       }
      }
   if (found == false)
      return false;  
   }

    return true;
}


double Statistics::Estimate(struct AndList *parseTree, char **relNames, int numToJoin)
{

    set <string> dummy;
    vector<string> joinmAttsInPair;

    //if it is not an error tree
    if (!ErrorCheck(parseTree, relNames, numToJoin, joinmAttsInPair, dummy)) 
   {
   cout<< "\nInvalid parseTree" << endl;
   return -1;
    }
   //parseTree->left = copyLeft;
   
    set <string> join_Rel_set;
    vector<long double> estiFactor;
    map<string, AttCountAndEstimate> localOrListestiFactor;
    string last_connector = "";
    int i = 0;
    while(i < joinmAttsInPair.size())
    {
   long double localEstimate = -1;

   int AttType_l = atoi(joinmAttsInPair.at(i++).c_str());
   string Att1Val = joinmAttsInPair.at(i++);

   int operatorCode = atoi(joinmAttsInPair.at(i++).c_str());
   int AttType_r = atoi(joinmAttsInPair.at(i++).c_str());

   string Att2Val = joinmAttsInPair.at(i++);
   string connector = joinmAttsInPair.at(i++);

   string Rel_l;
   int prefixedRelNamePos;
   if(AttType_l == NAME)
   {
       prefixedRelNamePos = Att1Val.find(".");
       if(prefixedRelNamePos != string::npos)
       {
      Rel_l = Att1Val.substr(0, prefixedRelNamePos);
      Att1Val = Att1Val.substr(prefixedRelNamePos + 1);
       }
       else
      Rel_l = mAttToRel[Att1Val].at(0);

       join_Rel_set.insert(Rel_l);
   }

   string Rel_r;
   if(AttType_r == NAME)
   {
       prefixedRelNamePos = Att2Val.find(".");
       if(prefixedRelNamePos != string::npos)
       {
      Rel_r = Att2Val.substr(0, prefixedRelNamePos);
      Att2Val = Att2Val.substr(prefixedRelNamePos + 1);
       }
       else
      Rel_r = mAttToRel[Att2Val].at(0);

       join_Rel_set.insert(Rel_r);
   }

   if(AttType_l == NAME && AttType_r == NAME)  
   {
       RelInfo t1;
       RelInfo t2;
       //find localEstimate
       t1 = mRelations[Rel_l];
       t2 = mRelations[Rel_r];

       localEstimate = 1.0/(max(t1.mAtts[Att1Val], t2.mAtts[Att2Val]));

       estiFactor.push_back(localEstimate);
   }
   else if(AttType_l == NAME || AttType_r == NAME)
   {
       RelInfo t;
       string AttName;
       if(AttType_l == NAME)
       {
      t = mRelations[Rel_l];
      AttName = Att1Val;
       }
       else
       {
      t = mRelations[Rel_r];
      AttName = Att2Val;
       }
       if(operatorCode == EQUALS)
       {
         // if connector or the last_connector is "OR"
      if(connector.compare("OR") == 0 || last_connector.compare("OR") == 0)
      {
             
          if(localOrListestiFactor.find(AttName + "=") == localOrListestiFactor.end())
          //if find a = again, put it into the map
          {
         localEstimate = (1.0- 1.0/t.mAtts[AttName]);
         AttCountAndEstimate *cce = new AttCountAndEstimate();
         cce->repeatCount = 1;
         cce->estimate = localEstimate;
         localOrListestiFactor[AttName + "="] = *cce;
          }
          else   // example a=2 or a=3
          {
         localEstimate = 1.0/t.mAtts[AttName];
         AttCountAndEstimate* cce = &(localOrListestiFactor[AttName + "="]);
         cce->repeatCount += 1;
         cce->estimate = cce->repeatCount*localEstimate; //example: f = (1.0 - (1 - 1/f1)(1-1/f2)..)
          }
          if(connector.compare("OR") != 0) 
          {

         long double tempResult = 1.0;
         map<string, AttCountAndEstimate>::iterator it = localOrListestiFactor.begin();
         for(; it != localOrListestiFactor.end(); it++)
         {
        if(it->second.repeatCount == 1)
            tempResult *= it->second.estimate;
        else
            tempResult *= (1 - it->second.estimate);
         }

         long double totalCurrentEstimate = 1.0 - tempResult;
         estiFactor.push_back(totalCurrentEstimate);

         localOrListestiFactor.clear();         
          }
      }
      else // oprator is "="
      {
          localEstimate = 1.0/t.mAtts[AttName];
          estiFactor.push_back(localEstimate);
      }
       }
       else  // operator is more_then or less_then
       {
      if(connector.compare("OR") == 0 || last_connector.compare("OR") == 0)
      {
          localEstimate = (1.0 - 1.0/3);

          AttCountAndEstimate *cce = new AttCountAndEstimate();
          cce->repeatCount = 1;
          cce->estimate = localEstimate;
          localOrListestiFactor[AttName] = *cce;

          //current orlist is done
          if(connector.compare("OR") != 0)  
          {

                  long double tempResult = 1.0;
         map<string, AttCountAndEstimate>::iterator it = localOrListestiFactor.begin();
         for(; it != localOrListestiFactor.end(); it++)
         {
        if(it->second.repeatCount == 1)
            tempResult *= it->second.estimate;
        else
            tempResult *= (1 - it->second.estimate);
         }

         long double totalCurrentEstimate = 1.0 - tempResult;
         estiFactor.push_back(totalCurrentEstimate);

         localOrListestiFactor.clear();
          }
      }
      else
      {
          localEstimate = (1.0/3);  // frequent of more_then and less_then
          estiFactor.push_back(localEstimate);
      }
       }
   }
   else    
   {}
   last_connector = connector;
    }

    unsigned long long int it_tuples = 1;
    set <string>::iterator set_it = join_Rel_set.begin();
    for (; set_it != join_Rel_set.end(); set_it++)
    it_tuples *= mRelations[*set_it].TupleNum;

    double result = it_tuples;
    for(int i = 0; i < estiFactor.size(); i++)
    {
   result *= estiFactor.at(i); // estimate result = it_tuple * estiFactor
    }
    return result;
}

