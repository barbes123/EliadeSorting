#include "TChain.h"
#include "TProof.h"
#include <sstream>
#include <string>
#include <iostream> 
using namespace std;

void start_me (UInt_t AddBAck = 0, int serverID=5, UInt_t first_run=195, UInt_t vol0=1, int nevents=0)
{
 
 std::cout<<" start_me.C is running "<<std::endl;
 
 UInt_t vol1 = vol0;  
 UInt_t last_run = first_run;
 string data_path = Form("/eliadedisks/s%i/root_files", serverID);  
// string suffix = "ssgant1";
 string suffix = Form("eliadeS%i", serverID);
 string file_name_prefix = "run";
  
 for(UInt_t run=first_run;run<=last_run;++run){     
 	for (UInt_t vol=vol0;vol<=vol1;++vol){
         
        TChain *ch = new TChain("ELIADE_Tree","ELIADE_Tree");
        string szRun, szVol;

        szRun = Form("%i",run); szVol = Form("%i",vol);

        std::stringstream ifile;
        ifile<<Form("%s/%s%s_%s_%s.root", data_path.c_str(), file_name_prefix.c_str(), szRun.c_str(),szVol.c_str(), suffix.c_str());
        
         if(gSystem->AccessPathName(ifile.str().c_str())){
	        std::cout << "File "<<ifile.str().c_str()<<" does not exist, skipping" << std::endl;
	        continue;
    	} else {
	        std::cout << "File "<<ifile.str().c_str()<<"  exists! " << std::endl;
                ch->Add(Form("%s/%s%s_%s_%s.root", data_path.c_str(), file_name_prefix.c_str(), szRun.c_str(),szVol.c_str(), suffix.c_str()));
	        std::ostringstream options;
        	options<<run<<","<<vol<<","<<AddBAck<<","<< serverID<<","<<"100";
		if (nevents == 0){
	        	ch->Process("~/EliadeSorting/DelilaSelectorEliade.C+",options.str().c_str());		
		}else {
	        	ch->Process("~/EliadeSorting/DelilaSelectorEliade.C+",options.str().c_str(),nevents);
		};
	   };
	};   
  };
}

