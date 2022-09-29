#include "TChain.h"
#include "TProof.h"
#include <sstream>
#include <string>
#include <iostream> 
using namespace std;

void sorting_eliade(UInt_t AddBAck = 0, int serverID=5, UInt_t first_run=195,  UInt_t last_run=195, UInt_t vol0=1, UInt_t vol1=1){

 
 std::cout<<" sorting_eliade.C is running "<<std::endl;
 std::cout<<" vol0 "<<vol0<<" vol1 "<<vol1<<std::endl;
  
 //UInt_t last_run = first_run;
  
  
 for(UInt_t run=first_run;run<=last_run;++run){     
 	for (UInt_t vol=vol0;vol<=vol1;++vol){
         
        TChain *ch = new TChain("ELIADE_Tree","ELIADE_Tree");
        string szRun, szVol;

        szRun = Form("%i",run); szVol = Form("%i",vol);

        std::stringstream ifile;
        ifile<<Form("/eliadedisks/s%i/root_files/run%s_%s_eliadeS%i.root", serverID, szRun.c_str(),szVol.c_str(),serverID);
        
         if(gSystem->AccessPathName(ifile.str().c_str())){
	        std::cout << "File "<<ifile.str().c_str()<<" does not exist, skipping" << std::endl;
	        continue;
    	} else {
	        std::cout << "File "<<ifile.str().c_str()<<"  exists! " << std::endl;
                ch->Add(Form("/eliadedisks/s%i/root_files/run%s_%s_eliadeS%i.root", serverID,szRun.c_str(),szVol.c_str(),serverID));
	        std::ostringstream options;
        	options<<run<<","<<vol<<","<<AddBAck<<","<< serverID<<","<<"100";
//     	ch->Process("/data/live/IT/dsoft/EliadeSorting/EliadeSorting.C+",options.str().c_str());
        	ch->Process("~/EliadeSorting/DelilaSelectorEliade.C+",options.str().c_str());
        	};
	};   
  };
}

