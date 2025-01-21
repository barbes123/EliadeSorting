#define DelilaSelectorEliade_cxx
// The class definition in data.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after hTimeDiffCoreCoreBegin(), when on PROOF called only on the
//                    slave servers.selected_trg_run_459_0.rselected_trg_run_459_0.roobunch_resettoot
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("DelilaSelectorEliade.C")#include <list>hTriggerTrigger

// root> T->Process("DelilaSelectorEliade.C","some options")
// root> T->Process("DelilaSelectorEliade.C+")
//


#include "DelilaSelectorEliade.h"
#include <TH2.h>
#include <TStyle.h>
#include <TString.h>
#include <TObjString.h>
#include <unordered_set>
#include <iomanip>      // sTreatDelilaEvent_td::setwsorted
#include <string>     // std::string, std::stof
using namespace std;
//#include "nlohmann/json.hpp"
#include <list>

////////////////////////////////Please, modify if needed////////////////////////////////////////////
// bool blGammaGamma           = true;
bool blOutTree              = false;
// bool blFold                 = false;
// bool blTimeAlignement       = true;
bool blFillAmaxEnergyDom    = false;
bool blFillSingleSpectra    = true;
bool blLong                 = false;//for Oliver
bool blParticleGammaGamma   = false;
//bool blTOF 		    = true;
// bool blDeeSector            = true;
// bool blDeeRing              = false;
// bool blDeeEx                = false;
// bool blParticleCutGate      = false;

////////////////////////////////Please, DO NOT modify ////////////////////////////////////////////
bool blIsTrigger            = false; //the SimpleTrigger is open
bool blIsWindow             = false; //the preTrigger is open
bool blFirstTrigger         = false;    
bool blAddTriggerToQueue    = false;
bool blCheckBunching        = true;//for Oliver;
// bool blCS                   = false;
bool debug                  = false;
bool blDebugElissa          = false;
bool blLUT_ELIADE           = false;
bool blLUT_TA               = false; //if tru read TA from LUT_TA file
bool blExpIsEliade          = false;
bool blExpIsElifant         = false;

ULong64_t trigger_cnt = 0;
// ULong64_t trigger_events = 0;
int external_trigger_det_def = 99;

const int max_domain = 1000;
const int nbr_of_ch = 3000;

//To reconstruct time in bunches
int number_of_bunch = 0;
bool blNewBunch                 = false; //if new bunch
bool blNewBunch_Event_Length    = false;//if event buidder blIsWindow before the end of bunch

// ULong64_t lastTimeStampTrigger = 0;
std::stringstream OutputFile;


void DelilaSelectorEliade::Read_ELIADE_LookUpTable() {
  std::cout << "I am trying to read ELIADE LookUpTable ... ";
//  std::stringstream CUTFile;
//  CUTFile << CUTG_Directory.Data() << "cut_EeEw_galileo.root";
//  TFile *file_EeEw = TFile::Open(CUTFile.str().c_str());

 char* pLUT_Path;
  pLUT_Path = getenv ("ELIADE_LUT");
  if (pLUT_Path!=NULL)
    printf ("The LookUpTable path is: %s \n",pLUT_Path);


  std::stringstream LUTFile;
  LUTFile << pLUT_Path <<"/"<<"LUT_ELIADE.dat";
  std::ifstream lookuptable(LUTFile.str().c_str());

  if (!lookuptable.good()) {
    std::ostringstream os;
    std::cout << "Could not open " << LUTFile.str().c_str() << " now i will try LUT_ELIADE.json \n";
//     os << "Could not open " << LUTFile.str().c_str() << " now i will try LUT_ELIADE.json \n";
//     Abort(os.str().c_str());
  } else {
    while (lookuptable.good()) {
      std::string oneline;
      std::getline(lookuptable, oneline);
      if (!lookuptable.good()) continue;
      if (oneline[0] == '#') continue; // ignore lines stating with #
      if (oneline.empty())   continue; // ignore empty lines
      TDelilaDetector curDet;
      Float_t theta(-1.), phi(-1.);
      int threshold = 1e6;
      std::istringstream is(oneline);
      if (debug) std::cout << is.str().c_str() << std::endl;
//       is >> curDet.ch >> curDet.dom >> curDet.theta >> curDet.phi >> curDet.TimeOffset >> curDet.threshold;
      is >> curDet.ch >> curDet.dom >> curDet.detType >> curDet.serial >> curDet.time_offset>> curDet.theta >> curDet.phi >> curDet.threshold >> curDet.cs_dom >> curDet.ElasticEnergy;
//       std::cout<<" curDfalseet.ch  "<<curDet.ch <<" curDet.TimeOffset " <<curDet.TimeOffset<<" curDet.ElasticEnerg "<<curDet.ElasticEnergy<<std::endl;
      
      if (curDet.ch >= 0) {
//           curDet.theta *= TMath::DegToRad();
//           curDet.phi *= TMath::DegToRad();
	int pol_order = 0;
 	float offset_gate(0.),slope_gate(1.);
//     is >> offset_gate;
	is >> pol_order;
	curDet.pol_order = pol_order;
    if (debug) std::cout << "Cal order " << pol_order << "  ";
// 	std::vector<float> DetCal_sub0;
	for (int k = 0; k < pol_order; k++) {
	  float par = -FLT_MAX;
	  is >> par;
	  if (par > -FLT_MAX) {
	    if (debug) std::cout << par << "  ";
	    curDet.calibE.push_back(par);
	  }
	}
	LUT_ELIADE[curDet.ch] = curDet;
      }
    }
  lookuptable.close();
  blLUT_ELIADE = true;
  std::cout << " done" << std::endl;
  }
//   lookuptable.close();
//   blLUT_ELIADE = true;
//   std::cout << " done" << std::endl;
  //  std::exit(1);
}


void DelilaSelectorEliade::Read_TimeAlignment_LookUpTable() {
  std::cout << "I am Reading TimeAlignment LookUpTable ... ";
 
  char* pLUT_Path;
  pLUT_Path = getenv ("ELIADE_LUT");
  if (pLUT_Path!=NULL)
    printf ("The LookUpTable path is: %s \n",pLUT_Path);


  std::stringstream LUTFile;
  LUTFile << pLUT_Path <<"/"<<"LUT_TA.dat";
// //   const int nbr_of_ch = 200;
  std::ifstream lookuptable(LUTFile.str().c_str());

  if (!lookuptable.good()) {
    std::ostringstream os;
    os << "Could not open " << LUTFile.str().c_str()
    << " no time alignment will be done\n";
//        << " and I need it ;(\n";
//     Abort(os.str().c_str());
  } else {
    while (lookuptable.good()) {
      std::string oneline;
      std::getline(lookuptable, oneline);
      if (!lookuptable.good()) continue;
      if (oneline[0] == '#') continue; // ignore lines stating with #
      if (oneline.empty())   continue; // ignore empty lines

      std::istringstream is(oneline);
 
      int coinc_id = 0; int time_corr = 0;
      is >> coinc_id >> time_corr;
      LUT_TA[coinc_id] = time_corr;
  }
  blLUT_TA = true;
  lookuptable.close();
  }
  std::cout << " done" << std::endl;
  //  std::exit(1);
}

void DelilaSelectorEliade::Read_AddBackTable() {
  std::cout << "I am Reading AddBackTable ... ";
 
  char* pLUT_Path;
  pLUT_Path = getenv ("ELIADE_LUT");
  if (pLUT_Path!=NULL)
    printf ("The AddBackTable path is: %s \n",pLUT_Path);


  std::stringstream LUTFile;
  LUTFile << pLUT_Path <<"/"<<"LUT_AB.dat";
// //   const int nbr_of_ch = 200;
  std::ifstream lookuptable(LUTFile.str().c_str());

  if (!lookuptable.good()) {
    std::ostringstream os;
    os << "Could not open " << LUTFile.str().c_str()
    << " no addback will be done\n";
//        << " and I need it ;(\n";
//     Abort(os.str().c_str());
  } else {
    while (lookuptable.good()) {
      std::string oneline;
      std::getline(lookuptable, oneline);
      if (!lookuptable.good()) continue;
      if (oneline[0] == '#') continue; // ignore lines stating with #
      if (oneline.empty())   continue; // ignore empty lines

      std::istringstream is(oneline);
 
      int dom1 = 0; int dom2 = 0; float dist = 0.;
      is >> dom1 >> dom2 >> dist;
      AddBack_distances[dom1][dom2] = dist;
      AddBack_distances[dom2][dom1] = dist;
      std::cout<<dom1<<" "<<dom2<<" "<<dist<<" "<<AddBack_distances[dom1][dom2]<<" "<<AddBack_distances[dom2][dom1]<<endl;

 
  }
  lookuptable.close();
  }

  std::cout << "I am Reading CrosstalkTable ... ";
 
  char* pLUTC_Path;
  //pLUT_Path = getenv ("ELIADE_LUT");
  pLUTC_Path = ".";
  if (pLUTC_Path!=NULL)
    printf ("The AddBackTable path is: %s \n",pLUTC_Path);


  std::stringstream LUTCFile;
  LUTCFile << pLUTC_Path <<"/"<<"Crosstalk_matrix.dat";
// //   const int nbr_of_ch = 200;
  std::ifstream lookuptableC(LUTCFile.str().c_str());

  if (!lookuptableC.good()) {
    std::ostringstream os;
    os << "Could not open " << LUTCFile.str().c_str()
    << " no addback will be done\n";
//        << " and I need it ;(\n";
//     Abort(os.str().c_str());
  } else {
    while (lookuptableC.good()) {
      std::string oneline;
      std::getline(lookuptableC, oneline);
      if (!lookuptableC.good()) continue;
      if (oneline[0] == '#') continue; // ignore lines stating with #
      if (oneline.empty())   continue; // ignore empty lines

      std::istringstream is(oneline);
 
      float E_C = 0; char* doublepoint; int dom1 = 0; int dom2 = 0; float C_factor = 0.; float C_error = 0.;
      is >> dom1 >> dom2 >> C_factor;
      Crosstalk_matrix[dom1][dom2] = C_factor;
      std::cout<<dom1<<" "<<dom2<<" "<<Crosstalk_matrix[dom1][dom2]<<" "<<Crosstalk_matrix[dom2][dom1]<<endl;

 
  }
  lookuptableC.close();
  }

  std::cout << " done" << std::endl;
  //  std::exit(1);
}

void DelilaSelectorEliade::Read_AcsTable() {
  std::cout << "I am Reading AcsTable ... ";
 
  char* pLUT_Path;
  pLUT_Path = getenv ("ELIADE_LUT");
  if (pLUT_Path!=NULL)
    printf ("The AddBackTable path is: %s \n",pLUT_Path);

  std::stringstream LUTFile;
  LUTFile << pLUT_Path <<"/"<<"LUT_ACS.dat";
  std::ifstream lookuptable(LUTFile.str().c_str());

  if (!lookuptable.good()) {
    std::ostringstream os;
    os << "Could not open " << LUTFile.str().c_str()
    << " no acs might be done or done based only on Time anti-coinc\n";
//        << " and I need it ;(\n";
//     Abort(os.str().c_str());
  } else {
    while (lookuptable.good()) {
      std::string oneline;
      std::getline(lookuptable, oneline);
      if (!lookuptable.good()) continue;
      if (oneline[0] == '#') continue; // ignore lines stating with #
      if (oneline.empty())   continue; // ignore empty lines

      std::istringstream is(oneline);
 
      int id = 0; float dist = 0.;
      is >> id >> dist;
      acs_distances[id] = id;
      acs_distances[id] = dist;
//       std::cout<<"acsCoincID "<<id<<" distance "<<dist<<" \n";
  }
  lookuptable.close();
  }
  std::cout << " done" << std::endl;
  //  std::exit(1);
}


void DelilaSelectorEliade::Read_SeaTable() {
  std::cout << "I am trying to read SEA Table ... ";

 char* pLUT_Path;
  pLUT_Path = getenv ("ELIADE_LUT");
  if (pLUT_Path!=NULL)
    printf ("The LookUpTable path is: %s \n",pLUT_Path);


  std::stringstream LUTFile;
  LUTFile << pLUT_Path <<"/"<<"LUT_SEA.dat";
  std::ifstream lookuptable(LUTFile.str().c_str());

  if (!lookuptable.good()) {
    std::ostringstream os;
    os << "Could not open " << LUTFile.str().c_str() << " neutron multiplicities will not be converted to efficiency;(\n";
  } else {
    while (lookuptable.good()) {
      std::string oneline;
      std::getline(lookuptable, oneline);
      if (!lookuptable.good()) continue;
      if (oneline[0] == '#') continue; // ignore lines stating with #
      if (oneline.empty())   continue; // ignore empty lines
      Float_t efficiency, nn12, nn13, nn23, nn24, nn34;
      std::istringstream is(oneline);
      if (debug) std::cout << is.str().c_str() << std::endl;
      is >> efficiency >> nn12 >> nn13 >> nn23 >> nn24>> nn34;
      
      mapSeaTable[0].push_back(efficiency);
      mapSeaTable[12].push_back(nn12);
      mapSeaTable[13].push_back(nn13);
      mapSeaTable[23].push_back(nn23);
      mapSeaTable[24].push_back(nn24);
      mapSeaTable[34].push_back(nn34);

  }
  lookuptable.close();
  std::cout << " done" << std::endl;
  //  std::exit(1);
    }
};

void DelilaSelectorEliade::Read_CoincCoinc_TimeAlignment_LookUpTable() {
  std::cout << "I am Reading Read_CoincCoinc_TimeAlignment_LookUpTable ... ";
 
  char* pLUT_Path;
  pLUT_Path = getenv ("ELIADE_LUT");
  if (pLUT_Path!=NULL)
    printf ("The LookUpTable path is: %s \n",pLUT_Path);


  std::stringstream LUTFile;
  LUTFile << pLUT_Path <<"/"<<"LUT_COINC.dat";
//   const int nbr_of_ch = 200;
  std::ifstream lookuptable(LUTFile.str().c_str());

  if (!lookuptable.good()) {
    std::ostringstream os;
    os << "Could not open " << LUTFile.str().c_str()
       << " and I need it ;( but i can survive without time alignment \n";
    //Abort(os.str().c_str());
  } else {
    while (lookuptable.good()) {
      std::string oneline;
      std::getline(lookuptable, oneline);
      if (!lookuptable.good()) continue;
      if (oneline[0] == '#') continue; // ignore lines stating with #
      if (oneline.empty())   continue; // ignore empty lines

      std::istringstream is(oneline);
 
      int coinc_id = 0; int time_corr = 0;
      is >> coinc_id >> time_corr;
      LUT_TA_COINC[coinc_id] = time_corr;

 
  }
  lookuptable.close();
  }
  std::cout << " done" << std::endl;
  //  std::exit(1);
}

void DelilaSelectorEliade::Read_Confs() {
  std::cout << "I am Reading Your Configuration parameters ... ";
 
  char* pLUT_Path;
  pLUT_Path = getenv ("ELIADE_LUT");
  if (pLUT_Path!=NULL)
    printf ("The LookUpTable path is: %s \n",pLUT_Path);


  std::stringstream LUTFile;
  LUTFile << pLUT_Path <<"/"<<"LUT_CONF.dat";
  std::ifstream lookuptable(LUTFile.str().c_str());
  
        
//   event_length = 40000;
  pre_event_length = 0;
  bunch_reset = 0;
  det_def_trg = -1;
  channel_trg = -1;
//   ref_dom = 101;
  TriggerTimeFlag = 0;
//   beta = 0;
  addback_distance = 10000;
  addback_tree = 0;
  EVENT_BUILDER          = true;
//   blFineTimeStamp        = true;
//   blFold                 = false;
  rf_time = 0;
  RF_N = 0;
  blExtTrigger           = false;

  my_confs["DeeRing"]              = false;
  my_confs["DeeSector"]            = false;
  my_confs["DeeEx"]                = false;
  my_confs["ParticleCutGate"]      = false;
  my_confs["IsEliade"]             = false;
  my_confs["IsElifant"]            = false; //not implemented
  my_confs["IsTrigger"]            = false; //not implemented for Oliver
  my_confs["Time_Alignement"]      = true; 
  my_confs["UseFineTS"]            = true;
  my_confs["Fold"]                 = false;
  my_confs["TOF"]                  = false;
  
  my_params["distance"]            = 0;  
  my_params["window_length"]       = 1;
  my_params["pre_window"]          = 1;
  my_params["beta"]                = 0; 
  my_params["reference_dom"]       = 101;
  
  
  //Change binning here
  my_hists["mDelila_raw"] = {1000,0, 1000,16384,0,16384};
  my_hists["mDelila"] = {1000,0, 1000,16384,0,16384};
  //my_hists["mDee_Sector"] = {8192, 0, 8192, 8192, 0,8192};
  my_hists["mDee_Sector"] = {8192, 0, 32768, 8192, 0,32768};
  my_hists["mDee_Ring"] = {4096, 0, 32768, 4096, 0,32768};
  my_hists["mTimeCalibInsideEvent"] = {1000, 0, 1000, 4e3,-2e6, 2e6};      


  
//   std::cout<<"!!!!!!!!!!!! " << my_hists["mDelila_raw"][5];

  if (!lookuptable.good()) {
    std::ostringstream os;
    os << "Could not open " << LUTFile.str().c_str()
       << " and I need it ;( but will can continue with default values\n";
//     Abort(os.str().c_str());
  } else {
    while (lookuptable.good()) {
      std::string oneline;
      std::getline(lookuptable, oneline);
      if (!lookuptable.good()) continue;
      if (oneline[0] == '#') continue; // ignore lines stating with #
      if (oneline.empty())   continue; // ignore empty lines

      std::istringstream is(oneline);
      TString conf_name;
      TString expName;
      int coinc_id = 0; Float_t value = 0;

      is >> conf_name>> coinc_id >> value;
      
      
      std::map<TString, float >::iterator it_par_= my_params.begin();
      if (my_params.find(conf_name) != my_params.end()) {
          my_params[conf_name] = value;
          continue;         
      }
      
      
      std::map<TString, bool >::iterator it_conf_= my_confs.begin();
      if (my_confs.find(conf_name) != my_confs.end()) {
          my_confs[conf_name] = (value == 1);
          continue;         
      }
      
      std::map<TString, std::deque<Float_t>> ::iterator it_hist_= my_hists.begin();
      if (my_hists.find(conf_name) != my_hists.end()) {
          
        bool bl_last = true;
        
        std::cout<<" nn "<<conf_name<<" ";
        int i =0;
        while (bl_last){
             my_hists[conf_name][i] = value;
//              std::cout<<value<<" ";
             std::cout<< my_hists[conf_name][i]<<" ";
            
            if (!(is >> value)) bl_last = false;  
            i++;
        };
          continue;         
      };


      
      

      switch (coinc_id){
          case 1001:
              {
              bunch_reset = value;
              std::cout<<"bunch_reset "<<bunch_reset<<" ps \n";
              break;
          }
          case 1002:
              {
              pre_event_length = value;
              std::cout<<"pre_event_length "<<pre_event_length<<" ps \n";
              break;
          }
          case 2000:{
               addback_distance = value;
               std::cout<<"Addback_distance is "<<addback_distance<<" % \n";
              break;
          };
          case 2001:{
               addback_tree = value;
               std::cout<<"Addback_tree is "<<addback_tree<<" % \n";
              break;
          };
          case 9995:{//rf_time 
              rf_time = value;
//               if (rf_time > 0) blExtTrigger = true;
              std::cout<<"rf_time "<<rf_time<<" \n";
              break;
              
          };
          case 9997:{
              int det = value/1;
              bool bl_last_detector = true;
              while (bl_last_detector){
                  if (detector_name.count(det) !=0 )
                  {
                    has_detector[detector_name[det]] = true;
                    std::cout<<" id " << det <<" det " << detector_name[det] << " is "<<has_detector[detector_name[det]]<<"\n";
                    if (!(is >> det)) bl_last_detector = false;  
                  }
                  else{
                    std::cout<<"Detector key "<< det<<" "<<detector_name[det] <<" does not exsist, check LUT_CONF.conf \n";
                    exit(1);   
                  }
                  
              };
              break;
          }
          case 9998:{
              if (value < 0) {EVENT_BUILDER = false;trigger_det_defs.push_back(-1);  break;}
//               if (value == 0) {det_def_trg = 0; break;}
              if (value/1 == 99) {blExtTrigger = true;trigger_det_defs.push_back(99);break;};
              if (value/1 == 999) {det_def_trg = 999 ;break;};

              trigger_det_defs.push_back(value);
              int next_trigger= -1;
              
              while (is >> next_trigger) trigger_det_defs.push_back(next_trigger/1);
              
              std::cout<<"Trigger of Detector Types: ";
              std::vector<int> ::iterator it_trg_ = trigger_det_defs.begin();
              for (;it_trg_!=trigger_det_defs.end(); ++it_trg_)
              {
                 std::cout<<*it_trg_<<" "; 
              }
              std::cout<<"\n";
              break;
          }
          case 9999:{
               int number_of_trigger_channels =  value/1;
               channel_trg = value/1;
               int next_trigger_domain = -1;
                while (is >> next_trigger_domain) trigger_domains.push_back(next_trigger_domain/1);//this line replaces 4 below; to be checked
//                  for (int k = 0; k < number_of_trigger_channels; k++) {
//                   if (is >> next_trigger_domain) trigger_domains.push_back(next_trigger_domain/1);
//                  };
//                if (trigger_domains.empty()) trigger_domains.push_back(number_of_trigger_channels);
//                if (!trigger_domains.empty()) EVENT_BUILDER = true;
               std::cout<<"channel_trg  "<<channel_trg<<" \n";
               std::cout<<"trigger_domains: ";
               std::vector<int> ::iterator it_trg_dom_ = trigger_domains.begin();
               for (;it_trg_dom_!=trigger_domains.end(); ++it_trg_dom_)
               {
                  std::cout<<*it_trg_dom_<<" "; 
               }
               std::cout<<"\n";
              break;
          }
          default:
          {
              //LUT_CONF[coinc_id] = value;
              coinc_gates[coinc_id] = value;
              //std::cout<<coinc_name<<" coin_id " << coinc_id <<" value "<<value <<" ps \n";
//               std::cout<<coinc_name<<" coin_id " << coinc_id <<"  coinc_gates[coinc_id] "<< coinc_gates[coinc_id] <<" ps \n";
              break;
          };
      };
  }
  
  if ((det_def_trg == -1)&&(!trigger_domains.empty())) det_def_trg = 0;//20230113
      
  lookuptable.close();
  }
  
  std::map<std::string, bool>::iterator  it_has_det_ = has_detector.begin();
  bool blAnyDetector = false;
  for (; it_has_det_!=has_detector.end(); ++it_has_det_){
      if (it_has_det_ ->second) blAnyDetector=true;
  };
  if (!blAnyDetector){
      std::cout<<"Terminated. No detectors are defined in LUT_CONF.dat. Add at least one detector type \n"<<
      "1 - Clover (or HPGe); \n"<< "2 - segment; \n" << "3 - LaBr; \n" << "4 - CsI (back) \n"  << "5 - BGO-side; \n" <<  "6 - BGO-front; \n" <<  "7 - Elissa E (or Si); \n" <<  "8 - Neutron; \n"  "9 - pulser; \n" << "11 - Clover (HPGe) core 2 \n"<< "17 -Elissa (dE) \n"<<"99 - External Trigger \n";
      exit(0);
  };

  std::cout << " Read_Confs is done" << std::endl;
  
  
  std::map<TString, bool >::iterator it_conf_= my_confs.begin();
  for (;it_conf_!=my_confs.end();++it_conf_){std::cout<<it_conf_->first<<" "<<it_conf_->second<<"\n";};
  
  std::map<TString, float >::iterator it_par_= my_params.begin();
  for (;it_par_!=my_params.end();++it_par_){std::cout<<it_par_->first<<" "<<it_par_->second<<"\n";};
     
  
}



void DelilaSelectorEliade::Print_ELIADE_LookUpTable()
{
    std::cout<<"Print_ELIADE_LookUpTable \n";		
    std::map<int, TDelilaDetector > ::iterator it__ = LUT_ELIADE.begin();
    for (; it__ != LUT_ELIADE.end(); ++it__) {
     // is >> curDet.ch >> curDet.dom >> theta >> phi >> curDet.TimeOffset >> curDet.threshold;
	std::cout<<" Ch "<<LUT_ELIADE[it__->first].ch<<" Dom "<< LUT_ELIADE[it__->first].dom<<" detType "<< LUT_ELIADE[it__->first].detType<<" " << LUT_ELIADE[it__->first].theta<<" "<< LUT_ELIADE[it__->first].phi <<" offset "<< LUT_ELIADE[it__->first].TimeOffset<<" Thr "<< LUT_ELIADE[it__->first].threshold<<" serial "<<LUT_ELIADE[it__->first].serial<<" theta "<<LUT_ELIADE[it__->first].theta<<" phi "<<LUT_ELIADE[it__->first].phi <<" cs_dom: "<<LUT_ELIADE[it__->first].cs_dom<<" pol_order: " <<LUT_ELIADE[it__->first].pol_order <<std::endl;
    }
};

void DelilaSelectorEliade::Print_TimeAlignment_LookUpTable()
{
    std::cout<<"Print_TimeAlignment_LookUpTable \n";		
    std::map<int, int > ::iterator it__ = LUT_TA.begin();
    for (; it__ != LUT_TA.end(); ++it__) {
     // is >> curDet.ch >> curDet.dom >> theta >> phi >> curDet.TimeOffset >> curDet.threshold;
	std::cout<<" domain "<<it__->first<<" time_corr "<< it__->second<<std::endl;
    }
};

void DelilaSelectorEliade::Print_CoincCoinc_TimeAlignment_LookUpTable()
{
    std::cout<<"Print_TimeAlignment_trigger_LookUpTable \n";		
    std::map<int, double_t > ::iterator it__ = LUT_TA_COINC.begin();
    for (; it__ != LUT_TA_COINC.end(); ++it__) {
     // is >> curDet.ch >> curDet.dom >> theta >> phi >> curDet.TimeOffset >> curDet.threshold;
	std::cout<<" coinc_id "<<it__->first<<" time_corr "<< it__->second<<std::endl;
    }
};



float DelilaSelectorEliade::CalibDet(float val, int ch)
{

 Float_t Randomize = 0.;
 std::vector<float> cal_par;
 cal_par = LUT_ELIADE[ch].calibE;  
 float cal_value = 0.;
 float flou = (float)rand () / RAND_MAX - 0.5; 
  for (UInt_t j = 0; j < cal_par.size(); j++) {
    cal_value += cal_par[j] * TMath::Power(val + flou, (Int_t) j);
  }
  return cal_value;

};

void DelilaSelectorEliade::Begin(TTree * tree)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).
  
  detector_name[1]="HPGe";     has_detector[detector_name[1]] = false;
  detector_name[2]="SEG";      has_detector[detector_name[2]] = false;
  detector_name[3]="LaBr";     has_detector[detector_name[3]] = false;
  detector_name[4]="CsI";      has_detector[detector_name[4]] = false;
  detector_name[5]="BGOs";     has_detector[detector_name[5]] = false; //side
  detector_name[6]="BGOf";     has_detector[detector_name[6]] = false;//front
  detector_name[7]="Elissa";   has_detector[detector_name[7]] = false;
  detector_name[17]="dElissa"; has_detector[detector_name[17]] = false;
  detector_name[8]="neutron";  has_detector[detector_name[8]] = false;//3He gas counters
  detector_name[88]="psdL";     	has_detector[detector_name[88]] = false;//type one - liuqid
  detector_name[87]="psdP";     	has_detector[detector_name[87]] = false;//type one - plastic
  detector_name[9]="pulser";   has_detector[detector_name[9]] = false;
  detector_name[10]="core10";   has_detector[detector_name[10]] = false;
  detector_name[99]="ExtTrigger";   has_detector[detector_name[99]] = false;
//   detector_name[18]="neutronTN"; has_detector[detector_name[18]] = false;
  
  std::map<UShort_t, std::string >::iterator it_det_name_ = detector_name.begin();
  for(;it_det_name_!=detector_name.end();++it_det_name_) dname[it_det_name_->second]=it_det_name_->first;

  Read_Confs();
  
  
  
   particle_name_in_cut[1] = "proton";
   particle_name_in_cut[10] = "deuteron";
   particle_name_in_cut[100] = "alpha";
   particle_name_in_cut[1000] = "6Li";
   particle_name_in_cut[10000] = "neutron";
   
   particle_name_without_cut[110] = "1a1p";
   particle_name_without_cut[11] = "1d1p";
   particle_name_without_cut[101] = "1a1d";
 

  std::map<int, Float_t>::iterator it_c_gates_ =  coinc_gates.begin();
  for(;it_c_gates_!=coinc_gates.end();++it_c_gates_){
       
      switch (it_c_gates_->first){
           case 11: {
               gg_coinc_id[it_c_gates_->first]="mgg_hpge_hpge";
               break;
         };
           case 12: {
               gg_coinc_id[it_c_gates_->first]="mgg_core_seg";
               break;
         };
          case 13: {
               gg_coinc_id[it_c_gates_->first]="mgg_labr_hpge";
               break;
         };
           case 33: {
               gg_coinc_id[it_c_gates_->first]="mgg_labr_labr";
               break;
         };
           case 37: {
               gg_coinc_id[it_c_gates_->first]="mgg_labr_elissa";//g-E
               break;
         };
         case 17: {
               gg_coinc_id[it_c_gates_->first]="mgg_hpge_elissa";//g-E
               break;
         };
           case 177: {
               gg_coinc_id[it_c_gates_->first]="mdee_elissa";
               break;
         };
         case 1773: {
               gg_coinc_id[it_c_gates_->first]="mdeeex_labr";
               break;
         };
         default: {
//               gg_coinc_id[it_c_gates_->first]=Form("mgg_not_defined_%i",it_c_gates_->first);
               break;
         };
       };
    };
    
   Read_ELIADE_LookUpTable();   
   Read_ELIADE_JSONLookUpTable();
   
   if (!blLUT_ELIADE)
   {
        std::cout<<"Now valid LUT; exciting. Check symbolic link LUT_ELIADE.dat / LUT_ELIADE.json";
        std::exit(1);
    };
    
//   Print_ELIADE_LookUpTable();
//   exit(0);
   
   Read_TimeAlignment_LookUpTable();
   Read_AddBackTable();
   Read_AcsTable();
//    if (!my_confs["IsEliade"]) Read_CutFile();
   if (my_confs["ParticleCutGate"]) Read_CutFile();
   if (my_confs["TOF"]) Read_CutFile();
//    Read_CoincCoinc_TimeAlignment_LookUpTable();//for fine coinc-coinc time allignement 
//    Print_CoincCoinc_TimeAlignment_LookUpTable();
   //Print_TimeAlignment_LookUpTable();
};




void DelilaSelectorEliade::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).
   
   TString option = GetOption();
   toks = option.Tokenize(",");
   RunID = ((TObjString*) toks->At(0))->GetString();
   VolID = ((TObjString*) toks->At(1))->GetString();
   addBackMode = atoi(((TObjString*) toks->At(2))->GetString());//not used, placeholder for parameter
   ServerID = ((TObjString*) toks->At(3))->GetString();
   
   nevents = 0;
//  nevents_reset=0;
   reset_counter = 0;
   
   blAddBack = (addBackMode > 0);
//    if (det_def_trg > 0) blTimeAlignement = false;//in this case trigger is detector type//i commented it checking alignment with pulser and Soiciro 17.5.23
   
   if (my_confs["IsEliade"]){
   
        std::cout<<"Making List of Detectors and Cores from LUT_ELIADE.dat \n";
        std::map<int, TDelilaDetector > ::iterator it__ = LUT_ELIADE.begin();
        std::cout<<" Detector ID : ";
        for (; it__ != LUT_ELIADE.end(); ++it__) {
            if (LUT_ELIADE[it__->first].detType == 1){
                int det_id = LUT_ELIADE[it__->first].dom/100;
                
                if (std::find(ListOfCores.begin(), ListOfCores.end(),det_id)!=ListOfCores.end()) continue;
        //            if (ListOfCores.find(det_id) == ListOfCores.end()) {;};// continue;
                std::cout<<det_id<<" ";
                ListOfCores.push_back(det_id);
        //         std::cout<<LUT_ELIADE[it__->first].dom<<" "<<LUT_ELIADE[it__->first].dom/100 * 10 +LUT_ELIADE[it__->first].dom/10%10 <<std::endl;
            };
        };
        std::cout<<" \n";
        
        it__ = LUT_ELIADE.begin();
        std::cout<<" Core ID : ";
        for (; it__ != LUT_ELIADE.end(); ++it__) {
            if (LUT_ELIADE[it__->first].detType == 1){
                int core_id = LUT_ELIADE[it__->first].dom/100 * 10 +LUT_ELIADE[it__->first].dom/10%10;
                if (std::find(ListOfCores.begin(), ListOfCores.end(),core_id)!=ListOfCores.end()) continue;
                std::cout<<core_id<<" ";
                ListOfCores.push_back(core_id);
                
        //         std::cout<<LUT_ELIADE[it__->first].dom<<" "<<LUT_ELIADE[it__->first].dom/100 * 10 +LUT_ELIADE[it__->first].dom/10%10 <<std::endl;
            };
        };
        std::cout<<" \n";
   };
   
  
   
   
   std::cout<<"I start definition of hists \n";

   
   
   hhGammaSi = new TH1F("hhGammaSi", "hhGammaSi", 1e5, -1e8, 99e8);
   hhGammaSi->GetXaxis()->SetTitle("time some units");
   hhGammaSi->GetYaxis()->SetTitle("counts");   
   fOutput->Add(hhGammaSi);
   
   //for oliver
//    hTimeDiff_g_e = new TH1F("hTimeDiff_g_e", "hTimeDiff_g_e", 5e3, -1e5, 4e5);
//    hTimeDiff_g_e->GetXaxis()->SetTitle("TimeDiff E-LaBr, ps"); 
//    //       hTimeDiff_g_e>GetYaxis()->SetTitle("counts");
//    fOutput->Add(hTimeDiff_g_e);
   

   
   hTimeSort = new TH1F("hTimeSort", "time_diff: current-last", 121e2, -1e6,120e6);
//    hTimeSort = new TH1F("hTimeSort", "time_diff: current-last", 1e3, -1e5,1e5);
   hTimeSort->GetXaxis()->SetTitle("ps");
//    if (has_detector["Neutron"]) hTimeSort->GetXaxis()->TAxis::Set(121e2, -1e6, 120e6);
   fOutput->Add(hTimeSort);
   
   hTimeZero = new TH1F("hTimeZero", "Events with zero time", 500, -0.5, 499.5);
   hTimeZero->GetXaxis()->SetTitle("ch");
   hTimeZero->GetYaxis()->SetTitle("counts");   
   fOutput->Add(hTimeZero);
   
   hElasticEnergy = new TH1F("hElasticEnergy", "hElasticEnergy", 1e3, 19000, 20000);
   hElasticEnergy->GetXaxis()->SetTitle("counts");
   hElasticEnergy->GetYaxis()->SetTitle("Energy, 1 keV/bin");   
   fOutput->Add(hElasticEnergy);
   
   hChannelHit = new TH1F("hChannelHit", "hChannelHit",3400,-0.5,3399.5);
   fOutput->Add(hChannelHit);
   
   hChannelHitNotDefined = new TH1F("hChannelHitNotDefined", "hChannelHitNotDefined",3216,0,3216);
   fOutput->Add(hChannelHitNotDefined);
   
   hSegmentHit = new TH1F("hSegmentHit", "hSegmentHit",20,0,20);
   fOutput->Add(hSegmentHit);
   
   hDomainHit = new TH1F("hDomainHit", "hDomainHit",max_domain,0,max_domain);
   fOutput->Add(hDomainHit);
   
   hDetTypeHit = new TH1F("hDetTypeHit", "hDetTypeHit",20,0,20);
   fOutput->Add(hDetTypeHit);
   
   hMult_gg = new TH1F("hMult_gg", "hMult_gg",20,0,20);
   fOutput->Add(hMult_gg);
   
   hMult_gg_CS = new TH1F("hMult_gg_CS", "hMult_gg_CS",20,0,20);
   fOutput->Add(hMult_gg_CS);
   
   hTimeDiffPulser = new TH1F("hTimeDiffPulser", "hTimeDiffPulser", 1000, -99.5, 899.5);
   fOutput->Add(hTimeDiffPulser);
   
 
   int n_bins =  16384; double max_value = 16383.5; int kev_bin = 4;
   if (blExtTrigger){n_bins =  320; max_value = 31999.5; kev_bin = 100;};
   
   std::cout<<" mDelila_raw "<< my_hists["mDelila_raw"][0] <<" "<< my_hists["mDelila_raw"][1]<<" "<< my_hists["mDelila_raw"][2]<<" "<< my_hists["mDelila_raw"][3]<<" "<< my_hists["mDelila_raw"][4]<<" "<< my_hists["mDelila_raw"][5]<<std::endl;
   
   mDelila_raw = new TH2F("mDelila_raw", "mDelila_raw", my_hists["mDelila_raw"][0], my_hists["mDelila_raw"][1]-0.5, my_hists["mDelila_raw"][2]-0.5, my_hists["mDelila_raw"][3],my_hists["mDelila_raw"][4]-0.5,my_hists["mDelila_raw"][5]-0.5);
   mDelila_raw->GetXaxis()->SetTitle("domain");
   mDelila_raw->GetYaxis()->SetTitle("ADC channels");   
   fOutput->Add(mDelila_raw);
   
   mDelila = new TH2F("mDelila", "mDelila", my_hists["mDelila"][0], my_hists["mDelila"][1]-0.5, my_hists["mDelila"][2]-0.5, my_hists["mDelila"][3],my_hists["mDelila"][4]-0.5,my_hists["mDelila"][5]-0.5);
   mDelila->GetXaxis()->SetTitle("domain");
   mDelila->GetYaxis()->SetTitle(Form("%i keV/bin ",int(my_hists["mDelila"][3]/my_hists["mDelila"][5])));
   fOutput->Add(mDelila);
   
   mDelilaDC = new TH2F("mDelilaDC", "mDelilaDC", my_hists["mDelila"][0], my_hists["mDelila"][1]-0.5, my_hists["mDelila"][2]-0.5, my_hists["mDelila"][3],my_hists["mDelila"][4]-0.5,my_hists["mDelila"][5]-0.5);
   mDelilaDC->GetXaxis()->SetTitle("domain");
   mDelilaDC->GetYaxis()->SetTitle(Form("%i keV/bin ",int(my_hists["mDelila"][3]/my_hists["mDelila"][5])));
   fOutput->Add(mDelilaDC); 
   
   mDelilaCS = new TH2F("mDelilaCS", "mDelilaCS",my_hists["mDelila"][0], my_hists["mDelila"][1]-0.5, my_hists["mDelila"][2]-0.5, my_hists["mDelila"][3],my_hists["mDelila"][4]-0.5,my_hists["mDelila"][5]-0.5);
   mDelilaCS->GetYaxis()->SetTitle("1 keV/bin ");
   mDelilaCS->GetYaxis()->SetTitle(Form("%i keV/bin ",int(my_hists["mDelila"][3]/my_hists["mDelila"][5])));
   fOutput->Add(mDelilaCS);
   
   mDelilaCS_DC = new TH2F("mDelilaCS_DC", "mDelilaCS_DC", my_hists["mDelila"][0], my_hists["mDelila"][1]-0.5, my_hists["mDelila"][2]-0.5, my_hists["mDelila"][3],my_hists["mDelila"][4]-0.5,my_hists["mDelila"][5]-0.5);
   mDelilaCS_DC->GetXaxis()->SetTitle("domain");
   mDelilaCS_DC->GetYaxis()->SetTitle(Form("%i keV/bin ",int(my_hists["mDelila"][3]/my_hists["mDelila"][5])));
   fOutput->Add(mDelilaCS_DC);
   
   hdelilaQu_size = new TH1F("hdelilaQu_size", "hdelilaQu_size", 100,0,100);
   hdelilaQu_size->GetXaxis()->SetTitle("size");
   hdelilaQu_size->GetYaxis()->SetTitle("counts");
   fOutput->Add(hdelilaQu_size);
   
   hdelilaPreQu_size = new TH1F("hdelilaPreQu_size", "hdelilaPreQu_size", 100,0,100);
   hdelilaPreQu_size->GetXaxis()->SetTitle("size");
   hdelilaPreQu_size->GetYaxis()->SetTitle("counts");
   fOutput->Add(hdelilaPreQu_size);
   
   
   
   mThetaPhi = new TH2F("mThetaPhi", "mThetaPhi", 90,60,150,360,0,360);
   mThetaPhi->GetXaxis()->SetTitle("theta, degrees");
   mThetaPhi->GetYaxis()->SetTitle("phi, degrees");
   fOutput->Add(mThetaPhi);
      
   mGammaGamma = new TH2F("mGammaGamma", "mGammaGamma", 4096, -0.5, 16383.5, 4096, -0.5, 16383.5);
   mGammaGamma->GetXaxis()->SetTitle("keV");
   mGammaGamma->GetYaxis()->SetTitle("keV");
   fOutput->Add(mGammaGamma);

//    mGammaGammaCS = new TH2F("mGammaGammaCS", "mGammaGammaCS", 4096, -0.5, 16383.5, 4096, -0.5, 16383.5);
//    mGammaGammaCS->GetXaxis()->SetTitle("keV");
//    mGammaGammaCS->GetYaxis()->SetTitle("keV");
//    fOutput->Add(mGammaGammaCS);
   
   mGammaGammaDC = new TH2F("mGammaGammaDC", "mGammaGammaDC", 4096, -0.5, 16383.5, 4096, -0.5, 16383.5);
   mGammaGammaDC->GetXaxis()->SetTitle("keV");
   mGammaGammaDC->GetYaxis()->SetTitle("keV");
   fOutput->Add(mGammaGammaDC);
   
   
   if (has_detector["psdL"] || has_detector["psdP"]){
       
       
       std::map<int, TDelilaDetector> ::iterator it_dom_psd_ = LUT_ELIADE.begin();
        for (; it_dom_psd_ != LUT_ELIADE.end(); ++it_dom_psd_) {
            int dom = LUT_ELIADE[it_dom_psd_->first].dom;           
            mPSD[dom] = new TH2F(Form("mPSD_dom_%i", dom), Form("mPSD_dom_%i", dom), 4096, -0.5, 16383.5, 1000, 0, 1);
            mPSD[dom]->GetXaxis()->SetTitle("a.u.");
            mPSD[dom]->GetYaxis()->SetTitle("a.u.");
            mPSD[dom]->SetTitle("(ChargeLong - ChargeShort)/(ChargeLong + ChargeShort)");
            fOutput->Add(mPSD[dom]);
            
           
           if (my_confs["TOF"]){
           
            mTOF_ch_short[dom] = new TH2F(Form("mTOF_ch_short_dom_%i", dom), Form("mTOF_ch_short_dom_%i", dom), 1000, 0, 1, 14000, -2e5, 5e5);
            mTOF_ch_short[dom]->GetXaxis()->SetTitle("a.u.");
            mTOF_ch_short[dom]->GetYaxis()->SetTitle("ps");
            mTOF_ch_short[dom]->SetTitle("(ChargeLong - ChargeShort)/(ChargeLong + ChargeShort) vs RF");
            fOutput->Add(mTOF_ch_short[dom]);
            
                  
            mTOF_ch_long[dom] = new TH2F(Form("mTOF_ch_long_dom_%i", dom), Form("mTOF_ch_long_dom_%i", dom), 4096, -0.5, 16383.5, 14000, -2e5, 5e5);
            mTOF_ch_long[dom]->GetXaxis()->SetTitle("a.u.");
            mTOF_ch_long[dom]->GetYaxis()->SetTitle("ps");
            //mTOF_ch_long[dom]->SetTitle("(ChargeLong - ChargeShort)/(ChargeLong + ChargeShort)");
            mTOF_ch_long[dom]->SetTitle("ChargeLong vs RF");
            fOutput->Add(mTOF_ch_long[dom]);
            
            
            mTOF_PSD_ch_short[dom] = new TH2F(Form("mTOF_PSD_ch_short_%i", dom), Form("mTOF_PSD_ch_short_%i", dom), 1000, 0, 1, 14000, -2e5, 5e5);
            mTOF_PSD_ch_short[dom]->GetXaxis()->SetTitle("a.u.");
            mTOF_PSD_ch_short[dom]->GetYaxis()->SetTitle("ps");
            mTOF_PSD_ch_short[dom]->SetTitle("(ChargeLong - ChargeShort)/(ChargeLong + ChargeShort) vs RF and PSD");
            fOutput->Add(mTOF_PSD_ch_short[dom]);
            
                  
            mTOF_PSD_ch_long[dom] = new TH2F(Form("mTOF_PSD_ch_long_%i", dom), Form("mTOF_PSD_ch_long_%i", dom), 4096, -0.5, 16383.5, 14000, -2e5, 5e5);
            mTOF_PSD_ch_long[dom]->GetXaxis()->SetTitle("a.u.");
            mTOF_PSD_ch_long[dom]->GetYaxis()->SetTitle("ps");
            //mTOF_ch_long[dom]->SetTitle("(ChargeLong - ChargeShort)/(ChargeLong + ChargeShort)");
            mTOF_PSD_ch_long[dom]->SetTitle("ChargeLong vs RF and PSD");
            fOutput->Add(mTOF_PSD_ch_long[dom]);
            
            mTOF_MEV[dom] = new TH1F(Form("mTOF_MEV_%i", dom), Form("mTOF_PSD_ch_short_%i", dom), 5000,-0.5,49.5);
            mTOF_MEV[dom]->GetXaxis()->SetTitle("MeV");
            mTOF_MEV[dom]->GetYaxis()->SetTitle("count");
            mTOF_MEV[dom]->SetTitle("TOF energy (and PSD)");
            fOutput->Add(mTOF_MEV[dom]);
            
            };
            
            std::cout<<"Created for dom "<<dom<<std::endl;
        }
   };
   
//    mGammaGammaCS_DC = new TH2F("mGammaGammaCS_DC", "mGammaGammaCS_DC", 4096, -0.5, 16383.5, 4096, -0.5, 16383.5);
//    mGammaGammaCS_DC->GetXaxis()->SetTitle("keV");
//    mGammaGammaCS_DC->GetYaxis()->SetTitle("keV");
//    fOutput->Add(mGammaGammaCS_DC);
   
   if (has_detector["HPGe"] || has_detector["SEG"] ) {
        mEliade = new TH2F("mEliade", "mEliade", max_domain, -0.5, max_domain-0.5, 4096, -0.5, 8191.5);
        mEliade->GetXaxis()->SetTitle("domain");
        mEliade->GetYaxis()->SetTitle("keV");
        fOutput->Add(mEliade);
        
        mEliadeCores = new TH2F("mEliadeCores", "Eliade Cores (core 1)", 100, -0.5, 99.5, 4096, -0.5, 8191.5);
        mEliadeCores->GetXaxis()->SetTitle("coreID");
        mEliadeCores->GetYaxis()->SetTitle("keV");
        fOutput->Add(mEliadeCores);
        
        mEliade_raw = new TH2F("mEliade_raw", "mEliade_raw", max_domain, -0.5, max_domain-0.5, 4096, -0.5, 8191.5);
        mEliade_raw->GetXaxis()->SetTitle("domain");
        mEliade_raw->GetYaxis()->SetTitle("a.u.");
        fOutput->Add(mEliade_raw);
        
        
        if (my_confs["IsEliade"]){
            mSingleCore = new TH2F("mSingleCore", "mSingleCore", 20, -0.5, 19.5, 4096, -0.5, 16383.5);
            mSingleCore->GetXaxis()->SetTitle("CORE ID");
            mSingleCore->GetYaxis()->SetTitle("keV");
            fOutput->Add(mSingleCore);
        };
        
        if (ListOfCores.empty()) std::cout<<"ListOfCores is empty \n";
        std::vector<int > ::iterator it2_coreid_ = ListOfCores.begin();
        for (; it2_coreid_ != ListOfCores.end(); ++it2_coreid_) {
            
            mTimeDiffCoreCore[*it2_coreid_] = new TH2F(Form("mTimeDiffCoreCore_%i", *it2_coreid_), Form("mTimeDiffCoreCore_%i", *it2_coreid_), 32, -0.5, 31.5, 4e2, -2e6, 2e6);
            mTimeDiffCoreCore[*it2_coreid_]->GetXaxis()->SetTitle("core");
            mTimeDiffCoreCore[*it2_coreid_]->GetYaxis()->SetTitle("10 ns / bin");
            fOutput->Add(mTimeDiffCoreCore[*it2_coreid_]);
            
        };
    };
   
   
   if ((has_detector["HPGe"] || has_detector["SEG"]) && blAddBack && my_confs["IsEliade"] ){
       
       if (ListOfCores.empty()) std::cout<<"ListOfCores is empty \n";
       
       std::vector<int > ::iterator it1_coreid_ = ListOfCores.begin();
       
       for (; it1_coreid_ != ListOfCores.end(); ++it1_coreid_) {
        hCoreFold[*it1_coreid_] = new TH1F(Form("hCoreFold_%i", *it1_coreid_), Form("hCoreFold_%i", *it1_coreid_), 50,-0.5, 49.5);
        hCoreFold[*it1_coreid_]->GetXaxis()->SetTitle("segment");
        hCoreFold[*it1_coreid_]->GetYaxis()->SetTitle("counts");
        fOutput->Add(hCoreFold[*it1_coreid_]);
        
        mCoreSegments[*it1_coreid_] = new TH2F(Form("mCoreSegments_%i", *it1_coreid_), Form("mCoreSegments_%i", *it1_coreid_), 50, -0.5, 49.5, 4096, -0.5, 16383.5);
        mCoreSegments[*it1_coreid_]->GetXaxis()->SetTitle("segment");
        mCoreSegments[*it1_coreid_]->GetYaxis()->SetTitle("keV");
        fOutput->Add(mCoreSegments[*it1_coreid_]);
        
        mTimeDiffCoreSegments[*it1_coreid_] = new TH2F(Form("mTimeDiffCoreSegments_%i", *it1_coreid_), Form("mTimeDiffCoreSegments_%i", *it1_coreid_), 50, -0.5, 49.5, 4e2, -2e6, 2e6);
        mTimeDiffCoreSegments[*it1_coreid_]->GetXaxis()->SetTitle("segment");
        mTimeDiffCoreSegments[*it1_coreid_]->GetYaxis()->SetTitle("10 ns / bin");
        fOutput->Add(mTimeDiffCoreSegments[*it1_coreid_]);

        mGGCoreSegments[*it1_coreid_] = new TH2F(Form("mGGCoreSegments_%i", *it1_coreid_), Form("mGGCoreSegments_%i", *it1_coreid_),  4096, -0.5, 16383.5, 4096, -0.5, 16383.5);
        mGGCoreSegments[*it1_coreid_]->GetXaxis()->SetTitle("segment");
        mGGCoreSegments[*it1_coreid_]->GetYaxis()->SetTitle("keV");
        fOutput->Add(mGGCoreSegments[*it1_coreid_]);
        
        //mFoldSpec[*it1_coreid_] = new TH2F(Form("mFoldSpec_%i", *it1_coreid_), Form("mFoldSpec_%i", *it1_coreid_),   50, -0.5, 49.5, 4096, -0.5, 16383.5);
        mFoldSpec[*it1_coreid_] = new TH2F(Form("mFoldSpec_%i", *it1_coreid_), Form("mFoldSpec_%i", *it1_coreid_),   50, -0.5, 49.5, 50, -0.5, 49.5);
        mFoldSpec[*it1_coreid_]->GetXaxis()->SetTitle("fold");
        mFoldSpec[*it1_coreid_]->GetYaxis()->SetTitle("keV");
        fOutput->Add(mFoldSpec[*it1_coreid_]);
        
        mFoldSpecSum[*it1_coreid_] = new TH2F(Form("mFoldSpecSum_%i", *it1_coreid_), Form("mFoldSpecSum_%i", *it1_coreid_),   50, -0.5, 49.5, 16384, -0.5, 16383.5);
        mFoldSpecSum[*it1_coreid_]->GetXaxis()->SetTitle("fold");
        mFoldSpecSum[*it1_coreid_]->GetYaxis()->SetTitle("keV");
        fOutput->Add(mFoldSpecSum[*it1_coreid_]);
        std::cout<<" core ID "<<*it1_coreid_<<" AddBack hists Initialized \n"; 
        
        if (blCS){
            mTimeDiffCoreSegmentsCS[*it1_coreid_] = new TH2F(Form("mTimeDiffCoreSegmentsCS_%i", *it1_coreid_), Form("mTimeDiffCoreSegmentsCS_%i", *it1_coreid_), 50, -0.5, 49.5, 4e2, -2e6, 2e6);
            mTimeDiffCoreSegmentsCS[*it1_coreid_]->GetXaxis()->SetTitle("segment");
            mTimeDiffCoreSegmentsCS[*it1_coreid_]->GetYaxis()->SetTitle("10 ns / bin");
            fOutput->Add(mTimeDiffCoreSegmentsCS[*it1_coreid_]);
            
            mFoldSpecCS[*it1_coreid_] = new TH2F(Form("mFoldSpecCS_%i", *it1_coreid_), Form("mFoldSpecCS_%i", *it1_coreid_),   50, -0.5, 49.5, 4096, -0.5, 16383.5);
            mFoldSpecCS[*it1_coreid_]->GetXaxis()->SetTitle("fold");
            mFoldSpecCS[*it1_coreid_]->GetYaxis()->SetTitle("keV");
            fOutput->Add(mFoldSpecCS[*it1_coreid_]);
        };
        
//         mFoldSpecClover[*it1_coreid_] = new TH2F(Form("mFoldSpecClover_%i", *it1_coreid_), Form("mFoldSpecClover_%i", *it1_coreid_),   10, -0.5, 9.5, 4096, -0.5, 16383.5);
//         mFoldSpecClover[*it1_coreid_]->GetXaxis()->SetTitle("fold");
//         mFoldSpecClover[*it1_coreid_]->GetYaxis()->SetTitle("keV");
//         fOutput->Add(mFoldSpecClover[*it1_coreid_]);
//         
        
//         std::cout<<" core ID "<<*it1_coreid_<<" AddBack hists Initialized \n";  
        
       };
   };
   
     if (has_detector["HPGe"] && (has_detector["BGOs"] || has_detector["BGOf"] || has_detector["CsI"])  && blCS && my_confs["IsEliade"]){
         if (ListOfCores.empty()) std::cout<<"ListOfCores is empty \n";
         
         std::vector<int > ::iterator it2_coreid_ = ListOfCores.begin();
       
         for (; it2_coreid_ != ListOfCores.end(); ++it2_coreid_) {
             hACSFold[*it2_coreid_] = new TH1F(Form("hACSFold_%i", *it2_coreid_), Form("hACSFold_%i", *it2_coreid_), 10,-0.5, 9.5);
             hACSFold[*it2_coreid_]->GetXaxis()->SetTitle("acs id");
             hACSFold[*it2_coreid_]->GetYaxis()->SetTitle("counts");
             fOutput->Add(hACSFold[*it2_coreid_]);
             
             
            mCoreACS[*it2_coreid_] = new TH2F(Form("mCoreACS_%i", *it2_coreid_), Form("mCoreACS_%i", *it2_coreid_), 16, -0.5, 15.5, 4096, -0.5, 16383.5);
            mCoreACS[*it2_coreid_]->GetXaxis()->SetTitle("acs id");
            mCoreACS[*it2_coreid_]->GetYaxis()->SetTitle("keV");
            fOutput->Add(mCoreACS[*it2_coreid_]);
            
            mCoreSpecACS[*it2_coreid_] = new TH2F(Form("mCoreSpecACS_%i", *it2_coreid_), Form("mCoreSpecACS_%i", *it2_coreid_), 16, -0.5, 15.5, 4096, -0.5, 16383.5);
            mCoreSpecACS[*it2_coreid_]->GetXaxis()->SetTitle("acs id");
            mCoreSpecACS[*it2_coreid_]->GetYaxis()->SetTitle("keV");
            fOutput->Add(mCoreSpecACS[*it2_coreid_]);
            
            mTimeDiffCoreACS[*it2_coreid_] = new TH2F(Form("mTimeDiffCoreACS_%i", *it2_coreid_), Form("mTimeDiffCoreACS_%i", *it2_coreid_), 10, -0.5, 9.5, 4e2, -2e6, 2e6);
            mTimeDiffCoreACS[*it2_coreid_]->GetXaxis()->SetTitle("acs id");
            mTimeDiffCoreACS[*it2_coreid_]->GetYaxis()->SetTitle("10 ns / bin");
            fOutput->Add(mTimeDiffCoreACS[*it2_coreid_]);
            
            mAcsFold[*it2_coreid_] = new TH2F(Form("mAcsFold_%i", *it2_coreid_), Form("mAcsFold_%i", *it2_coreid_),   10, -0.5, 9.5, 4096, -0.5, 16383.5);
            mAcsFold[*it2_coreid_]->GetXaxis()->SetTitle("fold");
            mAcsFold[*it2_coreid_]->GetYaxis()->SetTitle("keV");
            fOutput->Add(mAcsFold[*it2_coreid_]);
            
            /*mTimeDiffCoreCore[*it2_coreid_] = new TH2F(Form("mTimeDiffCoreCore_%i", *it2_coreid_), Form("mTimeDiffCoreCore_%i", *it2_coreid_), 32, -0.5, 31.5, 4e2, -2e6, 2e6);
            mTimeDiffCoreCore[*it2_coreid_]->GetXaxis()->SetTitle("core");
            mTimeDiffCoreCore[*it2_coreid_]->GetYaxis()->SetTitle("10 ns / bin");
            fOutput->Add(mTimeDiffCoreCore[*it2_coreid_]);*/
            
//             std::cout<<" core ID "<<*it2_coreid_<<" ACS hists Initialized \n";  
             }; 
            mSingleCoreCS = new TH2F("mSingleCoreCS", "mSingleCoreCS", 20, -0.5, 19.5, 4096, -0.5, 16383.5);
            mSingleCoreCS->GetXaxis()->SetTitle("CORE ID");
            mSingleCoreCS->GetYaxis()->SetTitle("keV");
            fOutput->Add(mSingleCoreCS);
            
     
            
               
            std::map<int, TDelilaDetector > ::iterator it_lut1_ = LUT_ELIADE.begin();
        
            for (; it_lut1_ != LUT_ELIADE.end(); ++it_lut1_) {
                if (LUT_ELIADE[it_lut1_->first].detType != 2) continue;
                int dom = LUT_ELIADE[it_lut1_->first].dom;           
                
                mSegACS[dom] = new TH2F(Form("mSegACS_%i", dom), Form("mSegACS_%i", dom), 16, -0.5, 15.5, 4096, -0.5, 16383.5);
                mSegACS[dom]->GetXaxis()->SetTitle("acs id");
                mSegACS[dom]->GetYaxis()->SetTitle("keV");
                fOutput->Add(mSegACS[dom]);
            
                mSegSpecACS[dom] = new TH2F(Form("mSegSpecACS_%i", dom), Form("mSegSpecACS_%i", dom), 16, -0.5, 15.5, 4096, -0.5, 16383.5);
                mSegSpecACS[dom]->GetXaxis()->SetTitle("acs id");
                mSegSpecACS[dom]->GetYaxis()->SetTitle("keV");
                fOutput->Add(mSegSpecACS[dom]);
                
                mTimeDiffSegACS[dom] = new TH2F(Form("mTimeDiffSegACS_%i", dom), Form("mTimeDiffSegACS_%i", dom), 32, -0.5, 31.5, 4e2, -2e6, 2e6);
                mTimeDiffSegACS[dom]->GetXaxis()->SetTitle("segment");
                mTimeDiffSegACS[dom]->GetYaxis()->SetTitle("10 ns / bin");
                fOutput->Add(mTimeDiffSegACS[dom]);
                
                mAcsFoldSeg[dom] = new TH2F(Form("mAcsFoldSeg_%i", dom), Form("mAcsFoldSeg_%i", dom),   10, -0.5, 9.5, 4096, -0.5, 16383.5);
                mAcsFoldSeg[dom]->GetXaxis()->SetTitle("fold");
                mAcsFoldSeg[dom]->GetYaxis()->SetTitle("keV");
                fOutput->Add(mAcsFoldSeg[dom]);
            };
            
            
            
            
     };
   
   
   
   
   
   if (has_detector["Elissa"] || has_detector["dElissa"]){

       std::map<int, TDelilaDetector > ::iterator it_lut_ = LUT_ELIADE.begin();
        
        if (blFillAmaxEnergyDom){            
            for (; it_lut_ != LUT_ELIADE.end(); ++it_lut_) {
                if (LUT_ELIADE[it_lut_->first].detType == 7){
                    int dom = LUT_ELIADE[it_lut_->first].dom;
    //                 std::cout<<"dom "<<dom<<"\n";
                    mAmaxEnergyDom[dom] = new TH2F(Form("mAmaxEnergy_dom%i",dom), Form("mAmaxEnergy_dom%i",dom), 4096,0, 32768, 10e2,0,10e4);
                    mAmaxEnergyDom[dom] ->GetXaxis()->SetTitle("Energy, a.u.");
                    mAmaxEnergyDom[dom] ->GetYaxis()->SetTitle("rise time (Amax)");
                    fOutput->Add(mAmaxEnergyDom[dom]);
                                        
                    if (dom >= 200){
                        mDee[dom] = new TH2F(Form("mDee_%i",dom), Form("mDee_dom%i",dom), 4096,0, 32768, 10e2,0,10e4);
                        mDee[dom] ->GetXaxis()->SetTitle("Energy, a.u.");
                        mDee[dom] ->GetYaxis()->SetTitle("Energy, a.u.");
                        fOutput->Add(mDee[dom]);
                    };
                };
            };
        };
        
        hPID = new TH1F("hPID", "hPID", 2200,-0.5, 2199.5);
        hPID ->GetXaxis()->SetTitle("PID");
        fOutput->Add(hPID); 
        
        //matrices for dEs-Es
        if (my_confs["DeeSector"] || my_confs["DeeRing"] ){
            for (; it_lut_ != LUT_ELIADE.end(); ++it_lut_) {
                    if (LUT_ELIADE[it_lut_->first].detType == 7){
                        int dee_dom = LUT_ELIADE[it_lut_->first].cs_dom;           
//                         mDee_Sector[dee_dom] = new TH2F(Form("mDee_Sector_dom%i",dee_dom), Form("mDee_Sector_dom%i",dee_dom), 8192, 0, 8192, 8192, 0,8192);
                        mDee_Sector[dee_dom] = new TH2F(Form("mDee_Sector_dom%i",dee_dom), Form("mDee_Sector_dom%i",dee_dom), my_hists["mDee_Sector"][0], my_hists["mDee_Sector"][1], my_hists["mDee_Sector"][2],my_hists["mDee_Sector"][3], my_hists["mDee_Sector"][4], my_hists["mDee_Sector"][5]);
                        mDee_Sector[dee_dom] ->GetXaxis()->SetTitle(Form("Energy E, %i keV/bin ", int(my_hists["mDee_Sector"][2]/my_hists["mDee_Sector"][0] )));
                        mDee_Sector[dee_dom] ->GetYaxis()->SetTitle(Form("Energy dE, %i keV/bin ", int(my_hists["mDee_Sector"][2]/my_hists["mDee_Sector"][0] )));
                        fOutput->Add(mDee_Sector[dee_dom]);
                    };
                    if (LUT_ELIADE[it_lut_->first].detType == 17){
                        int dee_dom1 = LUT_ELIADE[it_lut_->first].dom;           
//                         mDee_Ring[dee_dom1] = new TH2F(Form("mDee_Ring_dom%i",dee_dom1), Form("mDee_Ring_dom%i",dee_dom1),  4096, 0, 32768, 4096, 0,32768);
                        mDee_Ring[dee_dom1] = new TH2F(Form("mDee_Ring_dom%i",dee_dom1), Form("mDee_Ring_dom%i",dee_dom1),  my_hists["mDee_Ring"][0], my_hists["mDee_Ring"][1], my_hists["mDee_Ring"][2],my_hists["mDee_Ring"][3], my_hists["mDee_Ring"][4], my_hists["mDee_Ring"][5]);
                        mDee_Ring[dee_dom1] ->GetXaxis()->SetTitle("Energy E, a.u.");
                        mDee_Ring[dee_dom1] ->GetYaxis()->SetTitle("Energy dE, a.u.");
                        fOutput->Add(mDee_Ring[dee_dom1]);
                    };
            };
            mDee_Sector_TimeDiff = new TH2F("mDee_Sector_TimeDiff", "mDee_Sector_TimeDiff", 100,99.5,200.5, 5e3, -1e5, 4e5);
            mDee_Sector_TimeDiff ->GetXaxis()->SetTitle("time, ps");
            mDee_Sector_TimeDiff ->GetYaxis()->SetTitle("counts");
            fOutput->Add(mDee_Sector_TimeDiff); 
            
            mDee_Ring_TimeDiff = new TH2F("mDee_Ring_TimeDiff", "mDee_Ring_TimeDiff", 500, 0.5, 499.5, 5e3, -1e5, 4e5);
            mDee_Ring_TimeDiff ->GetXaxis()->SetTitle("time, ps");
            mDee_Ring_TimeDiff ->GetYaxis()->SetTitle("counts");
            fOutput->Add(mDee_Ring_TimeDiff); 
            
            
            
            mDee_SectorAll = new TH2F("mDee_SectorAll", "mDee_SectorAll", my_hists["mDee_Sector"][0], my_hists["mDee_Sector"][1], my_hists["mDee_Sector"][2],my_hists["mDee_Sector"][3], my_hists["mDee_Sector"][4], my_hists["mDee_Sector"][5]);
            mDee_SectorAll ->GetXaxis()->SetTitle(Form("Energy E, %i keV/bin ", int(my_hists["mDee_Sector"][2]/my_hists["mDee_Sector"][0] )));
            mDee_SectorAll ->GetYaxis()->SetTitle(Form("Energy dE, %i keV/bin ", int(my_hists["mDee_Sector"][2]/my_hists["mDee_Sector"][0] )));
            fOutput->Add(mDee_SectorAll);
            
            mDee_RingAll = new TH2F("mDee_RingAll", "mDee_RingAll",  my_hists["mDee_Ring"][0], my_hists["mDee_Ring"][1], my_hists["mDee_Ring"][2],my_hists["mDee_Ring"][3], my_hists["mDee_Ring"][4], my_hists["mDee_Ring"][5]);
            mDee_RingAll ->GetXaxis()->SetTitle("Energy E, a.u.");
            mDee_RingAll ->GetYaxis()->SetTitle("Energy dE, a.u.");
            fOutput->Add(mDee_RingAll);
            
            hDee_SectorAll_TimeDiff = new TH1F("hDee_SectorAll_TimeDiff", "hDee_SectorAll_TimeDiff", 10e3, -10e6, 10e6);
            hDee_SectorAll_TimeDiff ->GetXaxis()->SetTitle("time, ps");
//             mDee_SectorAll_TimeDiff ->GetYaxis()->SetTitle("counts");
            fOutput->Add(hDee_SectorAll_TimeDiff); 
            
            hDee_RingAll_TimeDiff = new TH1F("hDee_RingAll_TimeDiff", "hDee_RingAll_TimeDiff", 10e3, -10e6, 10e6);
            hDee_RingAll_TimeDiff ->GetXaxis()->SetTitle("time, ps");
//             mDee_SectorAll_TimeDiff ->GetYaxis()->SetTitle("counts");
            fOutput->Add(hDee_RingAll_TimeDiff); 
            
//             hPID = new TH1F("hPID", "hPID", 2200,-0.5, 2199.5);
//             hPID ->GetXaxis()->SetTitle("PID");
//             fOutput->Add(hPID); 
            
            hPID_dee = new TH1F("hPID_dee", "hPID_dee", 2200,-0.5, 2199.5);
            hPID_dee ->GetXaxis()->SetTitle("hPID_dee");
            fOutput->Add(hPID_dee); 
            
            hMult_dee = new TH1F("hMult_dee", "hMult_dee", 20,-0.5, 19.5);
            hMult_dee ->GetXaxis()->SetTitle("hMult_dee");
            fOutput->Add(hMult_dee); 
            
            
            
            std::cout<<"Created dEs-Es matricies \n";        
            
        }
        
        
        if (my_confs["ParticleCutGate"] ){
            std::map<UInt_t, string>::iterator it_id_ = particle_name_in_cut.begin();
            for (;it_id_!=particle_name_in_cut.end();++it_id_){
                mdee_gate_check[it_id_->first] = new TH2F(Form("mdee_gate_check_%s", it_id_->second.c_str()), Form("mdee_gate_check_%s", it_id_->second.c_str()), my_hists["mDee_Sector"][0], my_hists["mDee_Sector"][1], my_hists["mDee_Sector"][2],my_hists["mDee_Sector"][3], my_hists["mDee_Sector"][4], my_hists["mDee_Sector"][5]);
                mdee_gate_check[it_id_->first] ->SetTitle(Form("Energy E, %i keV/bin ", int(my_hists["mDee_Sector"][2]/my_hists["mDee_Sector"][0] )));
                mdee_gate_check[it_id_->first] ->SetTitle(Form("Energy dE, %i keV/bin ", int(my_hists["mDee_Sector"][2]/my_hists["mDee_Sector"][0] )));
                fOutput->Add(mdee_gate_check[it_id_->first]);
            }
        }
        
//         mdee_gate_check[0] = new TH2F("mdee_gate_check_zero", "mdee_gate_check_everything_else",  8192,0, 8192, 8192, 0,8192);
//         mdee_gate_check[0] ->GetXaxis()->SetTitle("Energy E, a.u.");
//         mdee_gate_check[0] ->GetYaxis()->SetTitle("Energy dE, a.u.");
//         fOutput->Add(mdee_gate_check[0]);
        
        
        mDeeTimeDiff = new TH2F("mDeeTimeDiff", "mDeeTimeDiff_dom%i", 4096,0, 32768, 10e3, 0, 10e6);
        mDeeTimeDiff ->GetXaxis()->SetTitle("time, ps");
        mDeeTimeDiff ->GetYaxis()->SetTitle("counts");
        fOutput->Add(mDeeTimeDiff); 
    
        hAmax = new TH1F("hAmax", "hAmax", 10e4,0,10e4);
        hAmax->GetXaxis()->SetTitle("rise time (Amax)");
        hAmax->GetYaxis()->SetTitle("counts");
        fOutput->Add(hAmax);
        
        mAmaxEnergy = new TH2F("mAmaxEnergy", "mAmaxEnergy", 2048, 0, 32768, 10e2,0,10e4);
        mAmaxEnergy->GetXaxis()->SetTitle("Energy, a.u.");
        mAmaxEnergy->GetYaxis()->SetTitle("rise time (Amax)");
        fOutput->Add(mAmaxEnergy);
        
          
        mElissa = new TH2F("mElissa", "mElissa", max_domain, 0, max_domain, 4096, -0.5, 32770);
        mElissa->GetXaxis()->SetTitle("domain");
        mElissa->GetYaxis()->SetTitle("keV");
        fOutput->Add(mElissa);
        
        if (my_confs["ParticleCutGate"] ){
        
//         int nbins = 16384; float bin_max = 16383.5; kev_bin = 1;
        
        int nbins = my_hists["mDelila"][3]; 
        float bin_max = my_hists["mDelila"][5]-0.5; 
        kev_bin = my_hists["mDelila"][5]/my_hists["mDelila"][3];
        
        std::map<UInt_t, string>::iterator it_pid_=particle_name_in_cut.begin();
        for (;it_pid_!=particle_name_in_cut.end();++it_pid_){
            hGG_particle[it_pid_->second] = new TH1F(Form("pid_gamma_%i_%s",it_pid_->first, it_pid_->second.c_str()), Form("pid_%i_g_%s",it_pid_->first, it_pid_->second.c_str()), nbins, -0.5, bin_max);
            hGG_particle[it_pid_->second]->GetXaxis()->SetTitle(Form("keV, %i keV/bin", kev_bin)); 
            hGG_particle[it_pid_->second]->GetYaxis()->SetTitle("counts");
            fOutput->Add(hGG_particle[it_pid_->second]);
            std::cout<<"hGG_particle_"<<it_pid_->second<<" created \n";
            
            
            if (blParticleGammaGamma){
            
                mGG_particle_time_diff[it_pid_->second] = new TH2F(Form("pid_gamma_%i_%s_time_diff",it_pid_->first, it_pid_->second.c_str()), Form("pid_%i_g_%s_time_diff",it_pid_->first, it_pid_->second.c_str()),  500, 0.5, 499.5,1e2,-5e5, 5e5);
                mGG_particle_time_diff[it_pid_->second]->GetXaxis()->SetTitle("domain");
                mGG_particle_time_diff[it_pid_->second]->GetYaxis()->SetTitle(Form("keV, %i keV/bin", kev_bin)); 
                fOutput->Add(mGG_particle_time_diff[it_pid_->second]);
                std::cout<<"mGG_particle_time_diff_"<<it_pid_->second<<" created \n";
                
                mGG_particle[it_pid_->second] = new TH2F(Form("m_pid_gamma_%i_%s",it_pid_->first, it_pid_->second.c_str()), Form("m_pid_%i_gg_%s",it_pid_->first, it_pid_->second.c_str()), nbins, -0.5, bin_max,  nbins, -0.5, bin_max);
                mGG_particle[it_pid_->second]->GetXaxis()->SetTitle(Form("keV, %i keV/bin", kev_bin)); 
                mGG_particle[it_pid_->second]->GetYaxis()->SetTitle(Form("keV, %i keV/bin", kev_bin));
                fOutput->Add(mGG_particle[it_pid_->second]);
                std::cout<<"mGG_particle"<<it_pid_->second<<" created \n";
            };
            
        }
        it_pid_=particle_name_without_cut.begin();
        for (;it_pid_!=particle_name_without_cut.end();++it_pid_){
            hGG_particle[it_pid_->second] = new TH1F(Form("pid_gamma_%i_%s",it_pid_->first, it_pid_->second.c_str()), Form("pid_%i_g_%s",it_pid_->first, it_pid_->second.c_str()), nbins, -0.5, bin_max);
            hGG_particle[it_pid_->second]->GetXaxis()->SetTitle(Form("keV, %i keV/bin", kev_bin)); 
            hGG_particle[it_pid_->second]->GetYaxis()->SetTitle("counts");
            fOutput->Add(hGG_particle[it_pid_->second]);
            std::cout<<"hGG_particle_"<<it_pid_->second<<" created \n";
            if (blParticleGammaGamma){
                mGG_particle[it_pid_->second] = new TH2F(Form("m_pid_gamma_%i_%s",it_pid_->first, it_pid_->second.c_str()), Form("m_pid_%i_gg_%s",it_pid_->first, it_pid_->second.c_str()), nbins, -0.5, bin_max,  nbins, -0.5, bin_max);
                mGG_particle[it_pid_->second]->GetXaxis()->SetTitle(Form("keV, %i keV/bin", kev_bin)); 
                mGG_particle[it_pid_->second]->GetYaxis()->SetTitle(Form("keV, %i keV/bin", kev_bin));
                fOutput->Add(mGG_particle[it_pid_->second]);
                std::cout<<"mGG_particle"<<it_pid_->second<<" created \n";
                
                mGG_particle_time_diff[it_pid_->second] = new TH2F(Form("pid_gamma_%i_%s_time_diff",it_pid_->first, it_pid_->second.c_str()), Form("pid_%i_g_%s_time_diff",it_pid_->first, it_pid_->second.c_str()),  500, 0.5, 499.5,1e2,-5e5, 5e5);
                mGG_particle_time_diff[it_pid_->second]->GetXaxis()->SetTitle("domain");
                mGG_particle_time_diff[it_pid_->second]->GetYaxis()->SetTitle(Form("keV, %i keV/bin", kev_bin)); 
                fOutput->Add(mGG_particle_time_diff[it_pid_->second]);
                std::cout<<"mGG_particle_time_diff_"<<it_pid_->second<<" created \n";
            };
            
        }
        };
        
        
   };
   
   
      

   

   std::map<UInt_t,std::string>::iterator itna =  gg_coinc_id.begin();
   

  

  for(;itna!=gg_coinc_id.end();++itna){
      
       if ((itna->first == 37 || itna->first == 17) && 
       has_detector["Elissa"] && has_detector["LaBr"]){
           
           int nbins = 16384; float bin_max = 16383.5; kev_bin = 1;
           
           mGG[itna->first] = new TH2F(Form("%s",itna->second.c_str()), Form("%s",itna->second.c_str()), nbins, -0.5, bin_max, nbins, -0.5, bin_max);
           mGG[itna->first]->GetXaxis()->SetTitle(Form("Elifant (LaBr), %i keV", kev_bin)); 
           mGG[itna->first]->GetYaxis()->SetTitle(Form("Elissa (Es), %i keV", kev_bin));
           fOutput->Add(mGG[itna->first]);
           
//            mGG_time_diff[itna->first] = new TH2F(Form("%s_time_diff",itna->second.c_str()), Form("%s_time_diff",itna->second.c_str()), max_domain, 0, max_domain, 4e2, -2e6, 2e6);
// 20240413           mGG_time_diff[itna->first] = new TH2F(Form("%s_time_diff",itna->second.c_str()), Form("%s_time_diff",itna->second.c_str()), max_domain, 0, max_domain, 100e2, -1e8, 9e8);
           mGG_time_diff[itna->first] = new TH2F(Form("%s_time_diff",itna->second.c_str()), Form("%s_time_diff",itna->second.c_str()), max_domain, -0.5, max_domain-0.5, 1.2e4, -6e5, 6e5);
           fOutput->Add(mGG_time_diff[itna->first]);
           
           hMult[itna->first] = new TH1F(Form("%s_hMult",itna->second.c_str()), Form("%s_hMult",itna->second.c_str()), 20,0,20);
           hMult[itna->first]->GetXaxis()->SetTitle("Multiplicity");
           hMult[itna->first]->GetYaxis()->SetTitle("Counts");
           fOutput->Add(hMult[itna->first]);
           
           std::cout<<Form("%s",itna->second.c_str())           <<" Initialized \n";
           std::cout<<Form("%s_time_diff",itna->second.c_str()) <<" Initialized \n";

           
           continue;
        };
        
        if ((itna->first == 1773) && has_detector["Elissa"] && has_detector["LaBr"] && my_confs["DeeEx"]){      
                   
              
            mGG[itna->first] = new TH2F(Form("%s",itna->second.c_str()), Form("%s",itna->second.c_str()), 4096, -0.5, 16383.5, 4096, -0.5, 32767.5);
            mGG[itna->first]->GetXaxis()->SetTitle("Elifant (LaBr), keV"); 
            mGG[itna->first]->GetYaxis()->SetTitle("Ex (Eel -(dE+E) , keV");
            fOutput->Add(mGG[itna->first]);
            
            mGG_time_diff[itna->first] = new TH2F(Form("%s_time_diff",itna->second.c_str()), Form("%s_time_diff",itna->second.c_str()), max_domain, -0.5, max_domain-0.5, 5e3, -1e5, 4e5);
            mGG_time_diff[itna->first]->GetXaxis()->SetTitle("TimeDiff dE/E-LaBr, ps"); 
            mGG_time_diff[itna->first]->GetYaxis()->SetTitle("counts");
            fOutput->Add(mGG_time_diff[itna->first]);
            
        };
        
      if ( ((itna->first == 33) && has_detector["LaBr"]) ||
           ((itna->first == 11) && has_detector["HPGe"]) ||
           ((itna->first == 13) && has_detector["HPGe"] && has_detector["LaBr"] )  ) {  
          
          
            mGG[itna->first] = new TH2F(Form("%s",itna->second.c_str()), Form("%s",itna->second.c_str()), 4096, -0.5, 16383.5, 4096, -0.5, 16383.5);
            mGG[itna->first]->GetXaxis()->SetTitle("keV");
            mGG[itna->first]->GetYaxis()->SetTitle("keV");
            fOutput->Add(mGG[itna->first]);
            
            mGG_AddBack[itna->first] = new TH2F(Form("%s_AB",itna->second.c_str()), Form("%s_AB",itna->second.c_str()), 4096, -0.5, 16383.5, 4096, -0.5, 16383.5);
            mGG_AddBack[itna->first]->GetXaxis()->SetTitle("keV");
            mGG_AddBack[itna->first]->GetYaxis()->SetTitle("keV");
            fOutput->Add(mGG_AddBack[itna->first]);
            
//             mGG_CS[itna->first] = new TH2F(Form("%s_CS",itna->second.c_str()), Form("%s_CS",itna->second.c_str()), 4096, -0.5, 16383.5, 4096, -0.5, 16383.5);
//             mGG_CS[itna->first]->GetXaxis()->SetTitle("keV");
//             mGG_CS[itna->first]->GetYaxis()->SetTitle("keV");
//             fOutput->Add(mGG_CS[itna->first]);
                
            mGG_DC[itna->first] = new TH2F(Form("%s_DC",itna->second.c_str()), Form("%s_DC",itna->second.c_str()), 4096, -0.5, 16383.5, 4096, -0.5, 16383.5);
            mGG_DC[itna->first]->GetXaxis()->SetTitle("keV");
            mGG_DC[itna->first]->GetYaxis()->SetTitle("keV");
            fOutput->Add(mGG_DC[itna->first]);
   
//             mGG_CS_DC[itna->first] = new TH2F(Form("%s_CS_DC",itna->second.c_str()), Form("%s_CS_DC",itna->second.c_str()), 4096, -0.5, 16383.5, 4096, -0.5, 16383.5);
//             mGG_CS_DC[itna->first]->GetXaxis()->SetTitle("keV");
//             mGG_CS_DC[itna->first]->GetYaxis()->SetTitle("keV");
//             fOutput->Add(mGG_CS_DC[itna->first]);
//             
            //////////////////////////////
            
            if (blLong){
            
                mGG_long[itna->first] = new TH2F(Form("%s_long",itna->second.c_str()), Form("%s_long",itna->second.c_str()), 4096, -0.5,  65535.5, 4096, -0.5,  65535.5);
                mGG_long[itna->first]->GetXaxis()->SetTitle("keV");
                mGG_long[itna->first]->GetYaxis()->SetTitle("keV");
                fOutput->Add(mGG_long[itna->first]);
    
//                 mGG_CS_long[itna->first] = new TH2F(Form("%s_CS_long",itna->second.c_str()), Form("%s_CS_long",itna->second.c_str()), 4096, -0.5,  65535.5, 4096, -0.5,  65535.5);
//                 mGG_CS_long[itna->first]->GetXaxis()->SetTitle("keV");
//                 mGG_CS_long[itna->first]->GetYaxis()->SetTitle("keV");
//                 fOutput->Add(mGG_CS_long[itna->first]);
                    
                mGG_DC_long[itna->first] = new TH2F(Form("%s_DC_long",itna->second.c_str()), Form("%s_DC_long",itna->second.c_str()), 4096, -0.5,  65535.5, 4096, -0.5,  65535.5);
                mGG_DC_long[itna->first]->GetXaxis()->SetTitle("keV");
                mGG_DC_long[itna->first]->GetYaxis()->SetTitle("keV");
                fOutput->Add(mGG_DC_long[itna->first]);
    
          /*      mGG_CS_DC_long[itna->first] = new TH2F(Form("%s_CS_DC_long",itna->second.c_str()), Form("%s_CS_DC_long",itna->second.c_str()), 4096, -0.5,  65535.5, 4096, -0.5,  65535.5);
                mGG_CS_DC_long[itna->first]->GetXaxis()->SetTitle("keV");
                mGG_CS_DC_long[itna->first]->GetYaxis()->SetTitle("keV");
                fOutput->Add(mGG_CS_DC_long[itna->first])*/;
            };
            ///////////////////////////////
            
            hMult[itna->first] = new TH1F(Form("%s_hMult",itna->second.c_str()), Form("%s_hMult",itna->second.c_str()), 20,0,20);
            hMult[itna->first]->GetXaxis()->SetTitle("Multiplicity");
            hMult[itna->first]->GetYaxis()->SetTitle("Counts");
            fOutput->Add(hMult[itna->first]);


  
            if (itna->first == 13){
                mGG[itna->first]->GetXaxis()->SetTitle("LaBr, keV"); mGG[itna->first]->GetYaxis()->SetTitle("HPGe, keV");
//                 mGG_CS[itna->first]->GetXaxis()->SetTitle("LaBr, keV"); mGG_CS[itna->first]->GetYaxis()->SetTitle("HPGe, keV");
                mGG_DC[itna->first]->GetXaxis()->SetTitle("LaBr, keV"); mGG_DC[itna->first]->GetYaxis()->SetTitle("HPGe, keV");
//                 mGG_CS_DC[itna->first]->GetXaxis()->SetTitle("LaBr, keV"); mGG_CS_DC[itna->first]->GetYaxis()->SetTitle("HPGe, keV");

                if (blLong){
                    mGG_long[itna->first]->GetXaxis()->SetTitle("LaBr, keV"); mGG_long[itna->first]->GetYaxis()->SetTitle("HPGe, keV");
    //                 mGG_CS_long[itna->first]->GetXaxis()->SetTitle("LaBr, keV"); mGG_CS_long[itna->first]->GetYaxis()->SetTitle("HPGe, keV");
                    mGG_DC_long[itna->first]->GetXaxis()->SetTitle("LaBr, keV"); mGG_DC_long[itna->first]->GetYaxis()->SetTitle("HPGe, keV");
    //                 mGG_CS_DC_long[itna->first]->GetXaxis()->SetTitle("LaBr, keV"); mGG_CS_DC_long[itna->first]->GetYaxis()->SetTitle("HPGe, keV");
                };
            };
            

            if (itna->first == 11){//for core-core
            //     mGG_time_diff[itna->first] = new TH2F(Form("%s_time_diff",itna->second.c_str()), Form("%s_time_diff",itna->second.c_str()), 100, 0, 100, 10e3, -2e6, 2e6);//was tuned like that
                mGG_time_diff[itna->first] = new TH2F(Form("%s_time_diff",itna->second.c_str()), Form("%s_time_diff",itna->second.c_str()), max_domain, 0, max_domain, 4e2, -2e6, 2e6);                       
                mGG_AB_time_diff[itna->first] = new TH2F(Form("%s_AB_time_diff",itna->second.c_str()), Form("%s_AB_time_diff",itna->second.c_str()), max_domain, 0, max_domain, 4e2, -2e6, 2e6);
            }else{
                mGG_time_diff[itna->first] = new TH2F(Form("%s_time_diff",itna->second.c_str()), Form("%s_time_diff",itna->second.c_str()), max_domain, 0, max_domain, 4e4, -2e6, 2e6);
                mGG_AB_time_diff[itna->first] = new TH2F(Form("%s_AB_time_diff",itna->second.c_str()), Form("%s_AB_time_diff",itna->second.c_str()), max_domain, 0, max_domain, 4e2, -2e6, 2e6);
            };
                
            mGG_time_diff[itna->first]->GetXaxis()->SetTitle("coreID"); mGG_time_diff[itna->first]->GetYaxis()->SetTitle("ps");
            mGG_AB_time_diff[itna->first]->GetXaxis()->SetTitle("coreID"); mGG_AB_time_diff[itna->first]->GetYaxis()->SetTitle("ps");
           
            
            fOutput->Add(mGG_time_diff[itna->first]);
            fOutput->Add(mGG_AB_time_diff[itna->first]);
            
            continue;
        };
   
//    };
//   std::cout<<itna->first<<" "<< Form("%s",itna->second.c_str())<<" Initialized \n" ;
        
};

      hLaBrElissa = new TH1F("hLaBrElissa","hLaBrElissa", 4096, -0.5, 16383.5);
        hLaBrElissa->GetYaxis()->SetTitle("counts");
        hLaBrElissa->GetXaxis()->SetTitle("keV");
        fOutput->Add(hLaBrElissa);
   
  std::map<UShort_t,std::string>::iterator itna1 =  detector_name.begin();

  for(;itna1!=detector_name.end();++itna1){
      
        
      if (blExtTrigger and blCheckBunching){
            mCheckBunching[itna1->first] = new TH2F(Form("mCheckBunching%s",itna1->second.c_str()), Form("mCheckBunching%s",itna1->second.c_str()), 1e3, 0, 1e3,  14000, -2e5, 5e5);
            mCheckBunching[itna1->first]->GetXaxis()->SetTitle("bunch number");
            mCheckBunching[itna1->first]->GetYaxis()->SetTitle("Calculated Time for Bunch, ps");
            fOutput->Add(mCheckBunching[itna1->first]);
      };
      
     if (blExtTrigger && ((itna1->first == 5) && has_detector["BGOs"] )
        || ((itna1->first == 7) && has_detector["Elissa"] )
        || ((itna1->first == 17) && has_detector["dElissa"] )
        
    ){
        mEnergyTimeDiff[itna1->first] = new TH2F(Form("mEnergyTimeDiff%s",itna1->second.c_str()), Form("mEnergyTimeDiff%s",itna1->second.c_str()), 256, -0.5, 32767.5, 14000, -2e5, 5e5);//128 keV per bin
        mEnergyTimeDiff[itna1->first]->GetXaxis()->SetTitle("Energy, 128 keV/bin");
        mEnergyTimeDiff[itna1->first]->GetYaxis()->SetTitle("Time diff, 200 ps/bin");
        fOutput->Add(mEnergyTimeDiff[itna1->first]); 
        
//         std::cout<<"itna1->first "<<itna1->first<<"\n";
        

    }; 

      
      if (((itna1->first == 7) || (itna1->first == 17) ) && has_detector["Elissa"]){
        hDelila0[itna1->first] = new TH1F(Form("%s",itna1->second.c_str()), Form("%s before EventB",itna1->second.c_str()), 4096, -0.5, 32767.5);
        hDelila0[itna1->first]->GetYaxis()->SetTitle("counts");
        hDelila0[itna1->first]->GetXaxis()->SetTitle("keV");
        fOutput->Add(hDelila0[itna1->first]);
        
        hDelila_single[itna1->first] = new TH1F(Form("%s_single",itna1->second.c_str()), Form("%s_single",itna1->second.c_str()), 4096, -0.5, 32767.5);
        hDelila_single[itna1->first]->GetYaxis()->SetTitle("counts");
        hDelila_single[itna1->first]->GetXaxis()->SetTitle("keV");
        fOutput->Add(hDelila_single[itna1->first]);
        
        
        if (blExtTrigger){
        
//             mEnergyTimeDiff[7] = new TH2F("mEnergyTimeDiffSiE", "mEnergyTimeDiffSiE", 4096, -0.5, 16383.5, 14000, -2e5, 5e5);
//             mEnergyTimeDiff[7]->GetXaxis()->SetTitle("Energy, keV");
//             mEnergyTimeDiff[7]->GetYaxis()->SetTitle("Time diff, ps");
//             fOutput->Add(mEnergyTimeDiff[7]);
//             
//             
//             mEnergyTimeDiff[17] = new TH2F("mEnergyTimeDiffSiDE", "mEnergyTimeDiffSiDE", 4096, -0.5, 16383.5, 14000, -2e5, 5e5);
//             mEnergyTimeDiff[17]->GetXaxis()->SetTitle("Energy, keV");
//             mEnergyTimeDiff[17]->GetYaxis()->SetTitle("Time diff, ps");
//             fOutput->Add(mEnergyTimeDiff[17]); 
        };
        
        continue;
        
      };
      
      
          

      
       if ((itna1->first == 1) && has_detector["HPGe"] && (addBackMode > 0)){
//           
         for (int k=1; k<=addBackMode; k++){
           hAddBack[k] = new TH1F(Form("AddBackMode_%i",k), Form("AddBackMode_%i",k), 4096, -0.5, 16383.5);
           hAddBack[k]->GetYaxis()->SetTitle("counts");
           hAddBack[k]->GetXaxis()->SetTitle("4 keV/bin");
               fOutput->Add(hAddBack[k]);
           };
         };
         
         
         
         if (blExtTrigger && ((itna1->first == 3) && has_detector["LaBr"]) //for Oliver
             /*|| ((itna1->first == 1) && has_detector["HPGe"])*/ 
            ){
         
            
            double max_e = 32000; int n_bin_e = 320; int kev_per_bin = max_e / n_bin_e;   //was  256, -0.5, 32767.5,
            //    mEnergyTimeDiff[3] = new TH2F("mEnergyTimeDiffLaBr", "mEnergyTimeDiffLaBr", 4096, -0.5, 16383.5, 7000, -2e5, 5e5);
            mEnergyTimeDiff[itna1->first] = new TH2F(Form("mEnergyTimeDiff%s",itna1->second.c_str()), Form("mEnergyTimeDiff%s",itna1->second.c_str()), n_bin_e, -0.5, max_e-0.5, 14000, -2e5, 5e5);//128 keV per bin
            mEnergyTimeDiff[itna1->first]->GetXaxis()->SetTitle(Form("Energy, %i keV/bin", kev_per_bin));
            mEnergyTimeDiff[itna1->first]->GetYaxis()->SetTitle("Time diff, 200 ps/bin");
            fOutput->Add(mEnergyTimeDiff[itna1->first]);
         
            mEnergyTimeDiffCS[itna1->first] = new TH2F(Form("mEnergyTimeDiffCS%s",itna1->second.c_str()), Form("mEnergyTimeDiffCS%s",itna1->second.c_str()), n_bin_e, -0.5, max_e-0.5, 14000, -2e5, 5e5);//128 keV per bin
            mEnergyTimeDiffCS[itna1->first]->GetXaxis()->SetTitle(Form("Energy, %i keV/bin", kev_per_bin));
            mEnergyTimeDiffCS[itna1->first]->GetYaxis()->SetTitle("Time diff, 200 ps/bin");
            fOutput->Add(mEnergyTimeDiffCS[itna1->first]);  
             
             
            if (my_params["beta"] > 0) {
                mEnergyTimeDiffDC[itna1->first] = new TH2F(Form("mEnergyTimeDiffDC_%s",itna1->second.c_str()), Form("mEnergyTimeDiffDC_%s",itna1->second.c_str()), n_bin_e, -0.5, max_e-0.5, 14000, -2e5, 5e5);//128 keV per bin
                mEnergyTimeDiffDC[itna1->first]->GetXaxis()->SetTitle(Form("Energy, %i keV/bin", kev_per_bin));
                mEnergyTimeDiffDC[itna1->first]->GetYaxis()->SetTitle("Time diff, 200 ps/pin");
                fOutput->Add(mEnergyTimeDiffDC[itna1->first]);

                mEnergyTimeDiffCS_DC[itna1->first] = new TH2F(Form("mEnergyTimeDiffCS_DC%s",itna1->second.c_str()), Form("mEnergyTimeDiffCS_DC%s",itna1->second.c_str()), n_bin_e, -0.5, max_e-0.5, 14000, -2e5, 5e5);//128 keV per bin
                mEnergyTimeDiffCS_DC[itna1->first]->GetXaxis()->SetTitle(Form("Energy, %i keV/bin", kev_per_bin));
                mEnergyTimeDiffCS_DC[itna1->first]->GetYaxis()->SetTitle("Time diff, 200 ps/pin");
                fOutput->Add(mEnergyTimeDiffCS_DC[itna1->first]);
                
                mEnergyTimeDiffCS_DC_E[itna1->first] = new TH2F(Form("mEnergyTimeDiffCS_DC_E_%s",itna1->second.c_str()), Form("mEnergyTimeDiffCS_DC_E_%s",itna1->second.c_str()), n_bin_e, -0.5, max_e-0.5, 14000, -2e5, 5e5);//128 keV per bin
                mEnergyTimeDiffCS_DC_E[itna1->first]->GetXaxis()->SetTitle(Form("Energy, %i keV/bin", kev_per_bin));
                mEnergyTimeDiffCS_DC_E[itna1->first]->GetYaxis()->SetTitle("Time diff, 200 ps/pin");
                fOutput->Add(mEnergyTimeDiffCS_DC_E[itna1->first]);
                
                mEnergyTimeDiffCS_DC_noE[itna1->first] = new TH2F(Form("mEnergyTimeDiffCS_DC_noE_%s",itna1->second.c_str()), Form("mEnergyTimeDiffCS_DC_noE_%s",itna1->second.c_str()), n_bin_e, -0.5, max_e-0.5, 14000, -2e5, 5e5);//128 keV per bin
                mEnergyTimeDiffCS_DC_noE[itna1->first]->GetXaxis()->SetTitle(Form("Energy, %i keV/bin", kev_per_bin));
                mEnergyTimeDiffCS_DC_noE[itna1->first]->GetYaxis()->SetTitle("Time diff, 200 ps/pin");
                fOutput->Add(mEnergyTimeDiffCS_DC_noE[itna1->first]);
                          
                
                std::map<int, TDelilaDetector > ::iterator it_lut_ = LUT_ELIADE.begin();
                std::deque<int> list_of_theta;
                
                
                for (; it_lut_ != LUT_ELIADE.end(); ++it_lut_) {
                    if (LUT_ELIADE[it_lut_->first].detType == 3){
                        
                        int dom = static_cast<int>(LUT_ELIADE[it_lut_->first].dom);
                        
                        mEnergyTimeDiffCS_DC_domain[dom] = new TH2F(Form("mEnergyTimeDiffCS_DC_domain%i",dom), Form("mEnergyTimeDiffCS_DC_domain%i",dom), n_bin_e, -0.5, max_e-0.5, 14000, -2e5, 5e5);//128 keV per bin
                        mEnergyTimeDiffCS_DC_domain[dom]->GetXaxis()->SetTitle(Form("Energy, %i keV/bin", kev_per_bin));
                        mEnergyTimeDiffCS_DC_domain[dom]->GetYaxis()->SetTitle("Time diff, 200 ps/pin");
                        fOutput->Add(mEnergyTimeDiffCS_DC_domain[dom]);
                        
                        
                        hEnergyTimeDiffCS_DC_domain[dom] = new TH1F(Form("hEnergyTimeDiffCS_DC_domain%i",dom), Form("hEnergyTimeDiffCS_DC_domain%i",dom), 14000, -2e5, 5e5);//128 keV per bin
                        hEnergyTimeDiffCS_DC_domain[dom]->GetXaxis()->SetTitle(Form("Energy, %i keV/bin", kev_per_bin));
                        hEnergyTimeDiffCS_DC_domain[dom]->GetYaxis()->SetTitle("Time diff, 200 ps/pin");
                        fOutput->Add(hEnergyTimeDiffCS_DC_domain[dom]);
                        
                         mEnergyTimeDiffCS_DC_E_domain[dom] = new TH2F(Form("mEnergyTimeDiffCS_DC_E_domain%i",dom), Form("mEnergyTimeDiffCS_DC_E_domain%i",dom), n_bin_e, -0.5, max_e-0.5, 14000, -2e5, 5e5);//128 keV per bin
                        mEnergyTimeDiffCS_DC_E_domain[dom]->GetXaxis()->SetTitle(Form("Energy, %i keV/bin", kev_per_bin));
                        mEnergyTimeDiffCS_DC_E_domain[dom]->GetYaxis()->SetTitle("Time diff, 200 ps/pin");
                        fOutput->Add(mEnergyTimeDiffCS_DC_E_domain[dom]);
                        
                        mEnergyTimeDiffCS_DC_noE_domain[dom] = new TH2F(Form("mEnergyTimeDiffCS_DC_noE_domain%i",dom), Form("mEnergyTimeDiffCS_DC_noE_domain%i",dom), n_bin_e, -0.5, max_e-0.5, 14000, -2e5, 5e5);//128 keV per bin
                        mEnergyTimeDiffCS_DC_noE_domain[dom]->GetXaxis()->SetTitle(Form("Energy, %i keV/bin", kev_per_bin));
                        mEnergyTimeDiffCS_DC_noE_domain[dom]->GetYaxis()->SetTitle("Time diff, 200 ps/pin");
                        fOutput->Add(mEnergyTimeDiffCS_DC_noE_domain[dom]);
                        



                        
                        
                        std::cout << Form("mEnergyTimeDiffCS_DC_domain%i",dom)<<" Initialized \n";

                        
                        
                        Float_t theta = LUT_ELIADE[it_lut_->first].theta;
                        int theta_int = static_cast<int>(theta);
                        bool found = false;
                        for (int value : list_of_theta) {
                            if (value == theta_int) {
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            list_of_theta.push_back(theta_int); // Add the element if not found
                            mEnergyTimeDiffCS_DC_theta[theta_int] = new TH2F(Form("mEnergyTimeDiffCS_DC_theta%i",theta_int), Form("mEnergyTimeDiffCS_DC_theta%i",theta_int), n_bin_e, -0.5, max_e-0.5, 14000, -2e5, 5e5);//128 keV per bin
                            mEnergyTimeDiffCS_DC_theta[theta_int]->GetXaxis()->SetTitle(Form("Energy, %i keV/bin", kev_per_bin));
                            mEnergyTimeDiffCS_DC_theta[theta_int]->GetYaxis()->SetTitle("Time diff, 200 ps/pin");
                            fOutput->Add(mEnergyTimeDiffCS_DC_theta[theta_int]);
                              
                            
                            hEnergyTimeDiffCS_DC_theta[theta_int] = new TH1F(Form("hEnergyTimeDiffCS_DC_theta%i",theta_int), Form("hEnergyTimeDiffCS_DC_theta%i",theta_int), 14000, -2e5, 5e5);//128 keV per bin
                            hEnergyTimeDiffCS_DC_theta[theta_int]->GetXaxis()->SetTitle(Form("Energy, %i keV/bin", kev_per_bin));
                            hEnergyTimeDiffCS_DC_theta[theta_int]->GetYaxis()->SetTitle("Time diff, 200 ps/pin");
                            fOutput->Add(hEnergyTimeDiffCS_DC_theta[theta_int]);
                            
                            std::cout << Form("mEnergyTimeDiffCS_DC_theta%i",theta_int)<<" Initialized \n";
                            };
                        };
                    };
                };
            };  
         
         
    
         
//         double max_e = 16383.5; int n_bin_e = 4096; int kev_per_bin = max_e / n_bin_e;
//         if (blExtTrigger){ max_e = 32768; n_bin_e = 8192; kev_per_bin = 4;}
            
        hDelila0[itna1->first] = new TH1F(Form("%s",itna1->second.c_str()), Form("%s before EventB",itna1->second.c_str()),my_hists["mDelila"][3], my_hists["mDelila"][4]-0.5, my_hists["mDelila"][5]-0.5);
        hDelila0[itna1->first]->GetYaxis()->SetTitle(Form("%i keV/bin ",int(my_hists["mDelila"][5]/my_hists["mDelila"][3])));
        hDelila0[itna1->first]->GetXaxis()->SetTitle("keV");
        fOutput->Add(hDelila0[itna1->first]);
        
        hDelila_single[itna1->first] = new TH1F(Form("%s_single",itna1->second.c_str()), Form("%s_single",itna1->second.c_str()), my_hists["mDelila"][3], my_hists["mDelila"][4]-0.5, my_hists["mDelila"][5]-0.5);
        hDelila_single[itna1->first]->SetTitle(Form("%i keV/bin ",int(my_hists["mDelila"][5]/my_hists["mDelila"][3])));
        hDelila_single[itna1->first]->GetXaxis()->SetTitle("keV");
        fOutput->Add(hDelila_single[itna1->first]);
   
        hDelilaCS[itna1->first] = new TH1F(Form("%s_CS",itna1->second.c_str()), Form("%s_CS",itna1->second.c_str()),my_hists["mDelila"][3], my_hists["mDelila"][4]-0.5, my_hists["mDelila"][5]-0.5);
        hDelilaCS[itna1->first]->GetYaxis()->SetTitle(Form("%i keV/bin ",int(my_hists["mDelila"][5]/my_hists["mDelila"][3])));
        hDelilaCS[itna1->first]->GetXaxis()->SetTitle("keV");
        hDelilaCS[itna1->first]->SetLineColor(2);
        fOutput->Add(hDelilaCS[itna1->first]);
        
        hDelilaDC[itna1->first] = new TH1F(Form("%s_DC",itna1->second.c_str()), Form("%s_DC",itna1->second.c_str()), my_hists["mDelila"][3], my_hists["mDelila"][4]-0.5, my_hists["mDelila"][5]-0.5);
        hDelilaDC[itna1->first]->GetYaxis()->SetTitle(Form("%i keV/bin ",int(my_hists["mDelila"][5]/my_hists["mDelila"][3])));
        hDelilaDC[itna1->first]->GetXaxis()->SetTitle("keV");
        hDelilaDC[itna1->first]->SetLineColor(kBlack);
        fOutput->Add(hDelilaDC[itna1->first]);
        
        hDelilaCS_DC[itna1->first] = new TH1F(Form("%s_CS_DC",itna1->second.c_str()), Form("%s_CS_DC",itna1->second.c_str()),my_hists["mDelila"][3], my_hists["mDelila"][4]-0.5, my_hists["mDelila"][5]-0.5);
        hDelilaCS_DC[itna1->first]->GetYaxis()->SetTitle(Form("%i keV/bin ",int(my_hists["mDelila"][5]/my_hists["mDelila"][3])));
        hDelilaCS_DC[itna1->first]->GetXaxis()->SetTitle("keV");
        fOutput->Add(hDelilaCS_DC[itna1->first]);
     
  };
  
  
  itna1 =  detector_name.begin();

   hTimeMinusTriggerLaBr = new TH1F("hTimeMinusTriggerLaBr", "hTimeMinusTriggerLaBr", 14000, -2e5, 5e5);
   fOutput->Add(hTimeMinusTriggerLaBr);
  
  

   
   
   mDomainTimeDiff_trigger = new TH2F("mDomainTimeDiff_trigger", "mDomainTimeDiff_trigger", max_domain, 0, max_domain, 10e3, -1e6, 9e6);
//    mDomainTimeDiff_trigger = new TH2F("mDomainTimeDiff_trigger", "mDomainTimeDiff_trigger", max_domain, 0, max_domain, 3e2, 0, 3e5);
   mDomainTimeDiff_trigger->GetXaxis()->SetTitle("domain");
   mDomainTimeDiff_trigger->GetYaxis()->SetTitle("Time diff, ps");
   fOutput->Add(mDomainTimeDiff_trigger);
   
   mDomainTimeDiff_bunch = new TH2F("mDomainTimeDiff_bunch", "mDomainTimeDiff_bunch", max_domain, 0, max_domain, 10e3, -1e6, 9e6);
//    mDomainTimeDiff_bunch = new TH2F("mDomainTimeDiff_bunch", "mDomainTimeDiff_bunch", max_domain, 0, max_domain, 3e2, 0, 3e5);
   mDomainTimeDiff_bunch->GetXaxis()->SetTitle("domain");
   mDomainTimeDiff_bunch->GetYaxis()->SetTitle("Time diff, ps");
   fOutput->Add(mDomainTimeDiff_bunch);
   
 //  hTriggerTrigger = new TH1F("hTriggerTrigger", "hTriggerTrigger", 100e4, 0, 100e6);
     hTriggerTrigger = new TH1F("hTriggerTrigger", "hTriggerTrigger", 1e4, 0, 1e10);
   hTriggerTrigger->GetYaxis()->SetTitle("Counts");
   hTriggerTrigger->GetXaxis()->SetTitle("Time diff, ps");
   hTriggerTrigger->SetTitle("Time between two trigger signals");
   fOutput->Add(hTriggerTrigger);
   
   hBunchBunch = new TH1F("hBunchBunch", "hBunchBunch", 10e4, -1e6, 9e6);
   hBunchBunch->GetYaxis()->SetTitle("Counts");
   hBunchBunch->GetXaxis()->SetTitle("Time diff, ps");
   hBunchBunch->SetTitle("Time between two Bunch signals");
   fOutput->Add(hBunchBunch);
   
   
//    hBeamCurrent = new TH1F("hBeamCurrent", "hBeamCurrent", 300, 50000, 53000);
//    hBeamCurrent->GetYaxis()->SetTitle("Counts");
//    hBeamCurrent->GetXaxis()->SetTitle("Beam kind of energy");
//    hBeamCurrent->SetTitle("Beam current");
//    fOutput->Add(hBeamCurrent);
   
   hCoincID= new TH1F("hCoincID", "hCoincID", 100,0,100);
   hCoincID->GetYaxis()->SetTitle("Counts");
   hCoincID->GetXaxis()->SetTitle("CoincID");
   hCoincID->SetTitle("CoincID");
   fOutput->Add(hCoincID);
   
   hTriggerDomain= new TH1F("hTriggerDomain", "hTriggerDomain", max_domain,-0.5,max_domain-0.5);
   hTriggerDomain->GetYaxis()->SetTitle("Counts");
   hTriggerDomain->GetXaxis()->SetTitle("Trigger Domain");
   hTriggerDomain->SetTitle("Trigger Domain");
   fOutput->Add(hTriggerDomain);
   
   hNBunch = new TH1F("hNBunch", "hNBunch", 100, 0, 100);
   hNBunch->GetYaxis()->SetTitle("Counts");
   hNBunch->GetXaxis()->SetTitle("N bunches after Trigger");
   hNBunch->SetTitle("N bunches");
   fOutput->Add(hNBunch);
   
   hTimeInBunch = new TH1F("hTimeInBunch", "hTimeInBunch", 10e3, -1e5, 9e5);
   hTimeInBunch->GetYaxis()->SetTitle("Counts");
   hTimeInBunch->GetXaxis()->SetTitle("hTimeInBunch");
   hTimeInBunch->SetTitle("time, ps");
   fOutput->Add(hTimeInBunch);
   
   hEventsPerTrigger = new TH1F("hEventsPerTrigger", "hEventsPerTrigger", 100, 0, 100);
   hEventsPerTrigger->GetYaxis()->SetTitle("Counts");
   hEventsPerTrigger->GetXaxis()->SetTitle("hEventsPerTrigger");
   hEventsPerTrigger->SetTitle("trigger");
   fOutput->Add(hEventsPerTrigger);
   
   hBunchFold = new TH1F("hBunchFold", "hBunchFold", 10, 0, 10);
   hBunchFold->GetYaxis()->SetTitle("Counts");
   hBunchFold->GetXaxis()->SetTitle("Det fold in bunch");
   hBunchFold->SetTitle("fold");
   fOutput->Add(hBunchFold);
   
   mFoldEnergy = new TH2F("mFoldEnergy", "mFoldEnergy", 10, 0, 10, 4096, -0.5, 65535.5);
   mFoldEnergy->GetXaxis()->SetTitle("fold");
   mFoldEnergy->GetYaxis()->SetTitle("Energy, 16 keV/pin");
   mFoldEnergy->SetTitle("Fold vs Energy");
   fOutput->Add(mFoldEnergy);

if (has_detector["neutron"]) {mTimeCalibTrigger = new TH2F("mTimeCalibTrigger", "mTimeCalibTrigger", max_domain, -0.5, max_domain-0.5,  1e2, 0, 256e6);}
    else {mTimeCalibTrigger = new TH2F("mTimeCalibTrigger", "mTimeCalibTrigger", max_domain, -0.5, max_domain-0.5, 1e3,-1e6, 9e6);};//usually i use this
//     else {mTimeCalibTrigger = new TH2F("mTimeCalibTrigger", "mTimeCalibTrigger", max_domain, -0.5, max_domain-0.5, 1e3,-1e7, 9e7);}; //for Dragos

   mTimeCalibTrigger->GetXaxis()->SetTitle("coinc ID");
   mTimeCalibTrigger->GetYaxis()->SetTitle("ps");
   mTimeCalibTrigger->SetTitle(Form("TimeDiff domain%i vs domain", channel_trg));
   fOutput->Add(mTimeCalibTrigger);
   
   mTimeCalibTriggerCores = new TH2F("mTimeCalibTriggerCores", "mTimeCalibTriggerCores", max_domain, -0.5, max_domain-0.5, 1e3,-1e6, 9e6);
   mTimeCalibTriggerCores->GetXaxis()->SetTitle("coinc ID");
   mTimeCalibTriggerCores->GetYaxis()->SetTitle("ps");
   mTimeCalibTriggerCores->SetTitle(Form("TimeDiff domain%i vs domain", channel_trg));
   fOutput->Add(mTimeCalibTriggerCores);
   
//    mTimeCalibInsideEvent = new TH2F("mTimeCalibInsideEvent", "mTimeCalibInsideEvent", max_domain, -0.5, max_domain-0.5, 1e4,-5e5, 5e5);
//    mTimeCalibInsideEvent = new TH2F("mTimeCalibInsideEvent", "mTimeCalibInsideEvent", max_domain, -0.5, max_domain-0.5, 4e3,-2e6, 2e6);
   mTimeCalibInsideEvent = new TH2F("mTimeCalibInsideEvent", "mTimeCalibInsideEvent", my_hists["mTimeCalibInsideEvent"][0], my_hists["mTimeCalibInsideEvent"][1]-0.5,  my_hists["mTimeCalibInsideEvent"][2]-0.5,  my_hists["mTimeCalibInsideEvent"][3], my_hists["mTimeCalibInsideEvent"][4],  my_hists["mTimeCalibInsideEvent"][5]);
   mTimeCalibInsideEvent->GetXaxis()->SetTitle("domain");
   mTimeCalibInsideEvent->GetYaxis()->SetTitle("ps");
//    mTimeCalibInsideEvent->SetTitle(Form("TimeDiff domain%i vs domain", trigger_domains.front()));
   mTimeCalibInsideEvent->SetTitle(Form("TimeDiff domain%i vs domain", int(my_params["reference_dom"])));
   fOutput->Add(mTimeCalibInsideEvent);
   
   mTimeCalibInsideEventCores = new TH2F("mTimeCalibInsideEventCores", "mTimeCalibInsideEventCores", 100, -0.5, 99.5, 1e3,-1e6, 9e6);
   mTimeCalibInsideEventCores->GetXaxis()->SetTitle("coreID");
   mTimeCalibInsideEventCores->GetYaxis()->SetTitle("ps");
//    mTimeCalibInsideEventCores->SetTitle(Form("TimeDiff domain%i vs domain", trigger_domains.front()));
   mTimeCalibInsideEventCores->SetTitle(Form("TimeDiff domain%i vs domain", int(my_params["reference_dom"])));
   fOutput->Add(mTimeCalibInsideEventCores);
   
   if (has_detector["neutron"]){
       
//        hTimeSort->GetXaxis()->TAxis::Set(121e2, -1e6, 120e6);
              
       mNN_TimeDiff = new TH2F("mNN_TimeDiff", "mNN_TimeDiff", max_domain, -0.5, max_domain-0.5,  3e2, 0, 300e6);
       mNN_TimeDiff->GetXaxis()->SetTitle("domain");
       mNN_TimeDiff->GetYaxis()->SetTitle("ps");
       fOutput->Add(mNN_TimeDiff);
       
       mNN_TimeDiff_counter = new TH2F("mNN_TimeDiff_counter", "mNN_TimeDiff_counter", max_domain, -0.5, max_domain-0.5,  3e2, 0, 300e6);
       mNN_TimeDiff_counter->GetXaxis()->SetTitle("domain");
       mNN_TimeDiff_counter->GetYaxis()->SetTitle("ps");
       fOutput->Add(mNN_TimeDiff_counter);
       
       hNN_Mult = new TH1F("hNN_Mult", "hNN_Mult", 20,-0.5,19.5);
       hNN_Mult->GetXaxis()->SetTitle("Multiplicity");
       hNN_Mult->GetYaxis()->SetTitle("Counts");
       fOutput->Add(hNN_Mult);
       
       hNN_fired = new TH1F("hNN_fired", "hNN_fired", 40,-0.5,39.5);
       hNN_fired->GetXaxis()->SetTitle("Domain");
       hNN_fired->GetYaxis()->SetTitle("Counts");
       fOutput->Add(hNN_fired);
       
       hNN_TimeDiff = new TH1F("hNN_TimeDiff", "hNN_TimeDiff",  3e2, 0, 300e6);
       hNN_TimeDiff->GetXaxis()->SetTitle("Domain");
       hNN_TimeDiff->GetYaxis()->SetTitle("Counts");
       fOutput->Add(hNN_TimeDiff);
       
       hNN_ring = new TH1F("hNN_ring", "hNN_ring", 5,-0.5,4.5);
       hNN_ring->GetXaxis()->SetTitle("ring");
       hNN_ring->GetYaxis()->SetTitle("Counts");
       fOutput->Add(hNN_ring);
       
       hTheta["neutron"] = new TH1F("hTheta_neutron", "hTheta_neutron", 40,-200, 200);
       hTheta["neutron"]->GetXaxis()->SetTitle("#Theta, degrees");
       fOutput->Add(hTheta["neutron"]);
       
       
       for (int i=0;i<=50;i++) {
           last_neutron_det[i] = 0;
           CounterIsFired[i] = 0;
       };
       LastNeutronTime = 0;
   };
   
   
//    mTimeDiffCS = new TH2F("mTimeDiffCS", "mTimeDiffCS", 100, 0, 100, 4e3, -2e6, 2e6);
   mTimeDiffCS = new TH2F("mTimeDiffCS", "mTimeDiffCS", 1000, 0, 1000, 4e3, -2e6, 2e6);
   mTimeDiffCS->GetXaxis()->SetTitle("domain");
   mTimeDiffCS->GetYaxis()->SetTitle("ps");
   mTimeDiffCS->SetTitle("LaBr_BGO time diff");
   fOutput->Add(mTimeDiffCS);
   
//    mLaBr_LabBr_time_diff = new TH2F("mLaBr_LabBr_time_diff", "mLaBr_LabBr_time_diff", 100, 0, 100, 4e3, -2e5, 2e5);
   mLaBr_LabBr_time_diff = new TH2F("mLaBr_LabBr_time_diff", "mLaBr_LabBr_time_diff", 100, 0, 100, 4e3, -4e4, 4e4);
   mLaBr_LabBr_time_diff->GetXaxis()->SetTitle("domain");
   mLaBr_LabBr_time_diff->GetYaxis()->SetTitle("ps");
   mLaBr_LabBr_time_diff->SetTitle("LaBr_LabBr time diff");
   fOutput->Add(mLaBr_LabBr_time_diff);

   mPulser0TimeDiff = new TH2F("mPulser0TimeDiff", "mPulser0TimeDiff", 100, 0.5, 100.5, 6e3, -3e6, 3e6);
   mPulser0TimeDiff->GetXaxis()->SetTitle("domain");
   mPulser0TimeDiff->GetYaxis()->SetTitle("ps");
   mPulser0TimeDiff->SetTitle("PulsePulser time diff");
   fOutput->Add(mPulser0TimeDiff);//time_diff relevant to the 1st channel (101), i.e. ch 101 is a trigger
   
   mTimeDiff_gg = new TH2F("mTimeDiff_gg", "mTimeDiff_gg", 200, 0.5, 200.5, 500, -99.5, 399.5);
   fOutput->Add(mTimeDiff_gg);
   
   mTimeDiff_gg_CS = new TH2F("mTimeDiff_gg_CS", "mTimeDiff_gg_CS", 200, 0.5, 200.5, 500, -99.5, 399.5);
   fOutput->Add(mTimeDiff_gg_CS);
       
   mPulserPulser = new TH2F("mPulserPulser", "mPulserPulser",4096, -0.5, 8191.5, 4096, -0.5, 8195.5);
   fOutput->Add(mPulserPulser);
   
   mNeutron = new TH2F("mNeutron", "mNeutron", 4096, 0, 16394, 4096, 0,1);
   mNeutron->GetXaxis()->SetTitle("Charge, a.u.");
   mNeutron->GetYaxis()->SetTitle("Short / Long");
   mNeutron->SetTitle("Neutron/#gamma separation");
   fOutput->Add(mNeutron);
   
   mShortLong = new TH2F("mShortLong", "mShortLong", 4096, 0, 16394, 4096, 0,16394);
   mShortLong->GetXaxis()->SetTitle("LongCharge, a.u.");
   mShortLong->GetYaxis()->SetTitle("ShortCharge, a.u.");
   mShortLong->SetTitle("Long vs short");
   fOutput->Add(mShortLong);
       
  if (my_confs["Fold"]){
    std::map<int, TDelilaDetector > ::iterator it_lut_ = LUT_ELIADE.begin();
        for (; it_lut_ != LUT_ELIADE.end(); ++it_lut_) {
            if ((LUT_ELIADE[it_lut_->first].detType == 1)||(LUT_ELIADE[it_lut_->first].detType == 3)){
                int dom = LUT_ELIADE[it_lut_->first].dom;
                mEnergy_time_diff[dom] = new TH2F(Form("mEnergy_time_diff_dom%i",dom), Form("mEnergy_time_diff_dom%i",dom), 4096, -0.5, 65535.5, 10e3, -1e5, 9e5);
                mEnergy_time_diff[dom] ->GetXaxis()->SetTitle("16 keV / bin");
                mEnergy_time_diff[dom] ->GetYaxis()->SetTitle("100 ps / bin");
                mEnergy_time_diff[dom]->SetTitle(" Time Diff Event.Time - Bunch.Time ");
                fOutput->Add(mEnergy_time_diff[dom]); 
            };
        };
  };
  
  std::cout<<"Hists are defined \n";

//   TString option = GetOption();
//   toks = option.Tokenize(",");
//   TString RunID = ((TObjString*) toks->At(0))->GetString();
//   TString VolID = ((TObjString*) toks->At(1))->GetString();
//   TString ServerID = ((TObjString*) toks->At(3))->GetString();
    
  //std::stringstream OutputFile;
  OutputFile.str(std::string());
//   OutputFile << "selected_trg_run" << "_" << RunID <<"_"<<VolID;
    OutputFile << "selected_run" << "_" << RunID <<"_"<<VolID;
    if (atoi(ServerID) != 0) {OutputFile<<"_eliadeS"<<ServerID;};
//     if (atoi(ServerID) != 0) {OutputFile<<"_elifant";};
  OutputFile << ".root";
//   std::cout <<"ServerID "<<ServerID<<" "<< OutputFile.str().c_str() <<std::endl;
   lastTime = 0;
   
  foutFile = new TFile (OutputFile.str().c_str(),"recreate"); 
   
   std::cout <<"ServerID "<<ServerID<<" "<< OutputFile.str().c_str() <<std::endl;
    start = std::clock();
    
    lastDelilaEvent.Time = 0;
    LastTriggerEvent.Time = 0;
    PulserEvent.Time = 0;
    DomainZeroEvent.Time = 0;
    LastBunchEvent.Time = 0;
//     nn_current_bunch = 0;
    delilaQu.clear();
    
    blFirst_event = false;
    

 
    
    if (my_confs["Time_Alignement"] && blAddTriggerToQueue) {
        blAddTriggerToQueue = false;
        std::cout<<"blAddTriggerToQueue is set false because incompatibe with  blTimeAlignement true \n";
    }
    
    std::cout << "SlaveBegin() is done \n";

//    std::cout<<"event_length "<<event_length<<" ps \n";
//    std::cout<<"bunch_reset "<<bunch_reset <<" ps \n";
//    std::cout<<"Beta is "<<beta<<" % \n"; 

    
//     outputQu.clear();
}

Bool_t DelilaSelectorEliade::Process(Long64_t entry)
{

  
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

  // fReader.SetLocalEntry(entry);

    GetEntry(entry);
    nb_entries = GetEntries();
    if (debug){std::cout<<"I just have got new entry "<< entry<<" fMod="<<fMod<<" fChannel="<<fChannel<<" TS "<<fTimeStampFS<< " at l.1481 "<<"\n";}

    int daq_ch = (fMod)*100+fChannel;
    
    if (LUT_ELIADE.empty()){std::cout<<"no LUT entry for daq ch "<< daq_ch <<" \n"; return kTRUE;};//did not work well
    
    //Check that daq_ch is defined in LUT
      bool check_daq_ch = false;
      std::map<int, TDelilaDetector >::iterator it_daq_ch_ = LUT_ELIADE.begin();
      for (; it_daq_ch_!= LUT_ELIADE.end();++it_daq_ch_){
//           std::cout <<LUT_ELIADE[it_daq_ch_->first].ch<<" fffffff \n";
             if (LUT_ELIADE[it_daq_ch_->first].ch == daq_ch){
               check_daq_ch = true;
               break;
             };
     };
     if (!check_daq_ch) {
         hChannelHitNotDefined->Fill(daq_ch);         
         if (daq_ch > 3216 && daq_ch!=25755) std::cout<<"daq_ch "<<daq_ch<<" is not defined in LUT_ELIADE.dat, skipping the entry \n" ;
         return kTRUE;
     };
     
     if (LUT_ELIADE[daq_ch].enable == 0)  return kTRUE;
    
//     if (LUT_ELIADE.find(daq_ch) == LUT_ELIADE.end()){std::cout<<"daq_ch "<<daq_ch<<" is not defined in LUT_ELIADE.dat, skipping the entry \n" ;return kTRUE;};//did not work well
    
    DelilaEvent_.det_def = LUT_ELIADE[daq_ch].detType;

    //check if the detector was defined in the LUT_CONF
//     if (!has_detector[detector_name[DelilaEvent_.det_def]] && DelilaEvent_.det_def != 10){
//       std::cout<<" daq_ch "<<daq_ch<<" Detector type "<<DelilaEvent_.det_def<<" is not defined in LUT_CONF.dat, skipping the entry \n" ;
//       return kTRUE;
//     };
    
    if (!has_detector[detector_name[DelilaEvent_.det_def]]) return kTRUE;
    
    DelilaEvent_.channel = daq_ch;
 	hChannelHit->Fill(daq_ch);

    
    if (debug){std::cout<<"I am doing new entry "<<entry <<" ch:"<< daq_ch << " at l. 1412 \n";};

    
    DelilaEvent_.domain = LUT_ELIADE[daq_ch].dom;   
    int domain = DelilaEvent_.domain;

//     if (debug){std::cout<<"I am doing entry here, ch:"<< daq_ch << "\n";}
    //     if (domain != channel_trg) DelilaEvent_.fEnergy = fEnergyLong;//why i was doing this??
    DelilaEvent_.fEnergy = fEnergyLong;
    
    
    if ((DelilaEvent_.fEnergy < LUT_ELIADE[daq_ch].threshold)&&(DelilaEvent_.det_def < 9)) return kTRUE;
    
    if (!has_detector[detector_name[DelilaEvent_.det_def]]) return kTRUE;
        
    DelilaEvent_.cs_domain = LUT_ELIADE[daq_ch].cs_dom;
    DelilaEvent_.theta= LUT_ELIADE[daq_ch].theta;
    DelilaEvent_.phi= LUT_ELIADE[daq_ch].phi;
    DelilaEvent_.ElasticEnergy = LUT_ELIADE[daq_ch].ElasticEnergy;
    
    
    hDomainHit->Fill(domain);
    hDetTypeHit->Fill(DelilaEvent_.det_def);   
    mDelila_raw->Fill(domain,DelilaEvent_.fEnergy);
    
    if (debug){std::cout<<"I am doing new entry l.1435, ch:"<< daq_ch <<" det_def "<<DelilaEvent_.det_def << "\n";}

     //Check if the tree is time sorted
     if (my_confs["UseFineTS"]){
         DelilaEvent_.Time = fTimeStampFS;
     }
     else{
        DelilaEvent_.Time=fTimeStamp;
     }
     double time_diff_last = DelilaEvent_.Time - lastDelilaTime;
     
     //Check that the Tree is sorted in Time
     if (time_diff_last<0){std::cout<<"\nWarning time_diff_last: .Time  TTree may be not sorted by time "<< time_diff_last<<" \n";};
     if (DelilaEvent_.Time == 0) {hTimeZero->Fill(daq_ch);return kTRUE;};
     hTimeSort->Fill(time_diff_last);
     
     if (debug){std::cout<<"I am doing new entry l.1084, ch:"<< daq_ch << "\n";}
     
     lastDelilaTime = DelilaEvent_.Time;     
     //Apply time correction, usually should not be done for ExtTrigger, so it is not specified in LUT timeoffset for him
     
     if (blLUT_TA && !blExtTrigger){
            DelilaEvent_.Time= DelilaEvent_.Time + LUT_TA[domain];//*1e3; //from ns in lut to ps
        }
     else if (!blExtTrigger){
        DelilaEvent_.Time= DelilaEvent_.Time + LUT_ELIADE[daq_ch].time_offset;//*1e3; //from ns in lut to ps
     };
     

     if (blExtTrigger && DelilaEvent_.det_def != 99) {
//            std::cout<<"Time: "<<DelilaEvent_.Time<<" Trigger Time: "<<LastTriggerEvent.Time<< " Time -Trigger: "<<DelilaEvent_.Time - LastTriggerEvent.Time <<"\n";
//            DelilaEvent_.TimeBunch = DelilaEvent_.Time - LastTriggerEvent.Time;
         
            DelilaEvent_.TimeTrg = DelilaEvent_.Time - LastTriggerEvent.Time;
            
            DelilaEvent_.TimeTrg = DelilaEvent_.TimeTrg + LUT_TA[domain]  + my_params["pre_window"] ;
            
            int nn =  DelilaEvent_.TimeTrg  / rf_time;
            DelilaEvent_.TimeBunch = DelilaEvent_.TimeTrg  - nn * rf_time;
//             DelilaEvent_.TimeBunch= DelilaEvent_.TimeBunch + LUT_TA[domain];//*1e3; //from ns in lut to ps
//             std::cout<<"nn "<<nn<<" number_of_bunch "<< number_of_bunch << "\n";
            blNewBunch = (nn != number_of_bunch);
            number_of_bunch = nn;
            blNewBunch_Event_Length = (DelilaEvent_.TimeBunch > my_params["window_length"] );
//             if (blNewBunch_Event_Length) std::cout<<"DelilaEvent_.TimeBunch "<<DelilaEvent_.TimeBunch<<" event_length "<< event_length << "\n";
            
//             if (blTimeAlignement) mTimeCalibInsideEvent->Fill(DelilaEvent_.domain, DelilaEvent_.TimeBunch);
            if (my_confs["Time_Alignement"]) mTimeCalibInsideEvent->Fill(DelilaEvent_.domain, DelilaEvent_.TimeTrg);
            if (blCheckBunching) mCheckBunching[DelilaEvent_.det_def] ->Fill(nn, DelilaEvent_.TimeBunch);
       };
     
     switch (DelilaEvent_.det_def){
          case 1:  { 
             if (has_detector["HPGe"]) {TreatHpGeSingle();}
                 else return kTRUE;
             break;
         };case 2:  { 
             if (has_detector["SEG"]) {TreatHPGeSegmentSingle();}
                 else return kTRUE;
             break;
         };case 3:  { 
             if (has_detector["LaBr"]) {TreatLaBrSingle();}
                else return kTRUE;
             break;
         };case 4:  { 
             if (has_detector["CsI"]) {TreatBGOSingle();}
                else return kTRUE;
             break;
         };case 5:  { 
             if (has_detector["BGOs"]) {TreatBGOSingle();}
                else return kTRUE;
             break;
         };case 6:  { 
             if (has_detector["BGOf"]) {TreatBGOSingle();}
                else return kTRUE;
             break;
         };case 7: case 17:  { 
             if (has_detector["Elissa"]) {TreatElissaSingle();}
                else return kTRUE;
             break;
         };case 8:  { 
             if (has_detector["neutron"]) {TreatNeutronSingle3He();/*TreatNeutronSingle()*/;}
                else return kTRUE;
             break;
         };case 87: case 88:  { 
             if (has_detector["psdL"]||has_detector["psdP"]) {TreatPSD();/*TreatNeutronSingle()*/;}//TreatPSD should replce TreatNeutronSingle
                else return kTRUE;
             break;
         };case 9:  { 
             if (has_detector["pulser"]) //CheckPulserAllignement(90);
                return kTRUE;
             break;
         };case 10:  { 
             if (has_detector["core10"]) {TreatHpGeSingle();}
                 else return kTRUE;
             break;
         }; 
          case 90: { //beam current
              
              return kTRUE;
          };         
          case 98:  { 
             return kTRUE;
             break;
         };
         case 11:  { //second core
             return kTRUE;
             break;
         };
         case 99:  { //trigger
             if (has_detector["ExtTrigger"]) { ;}
                 else return kTRUE;
             break;
         };
         
     };
  
  if (debug){std::cout<<"I did TreatDelilaEvent_() \n";}
  
  if (EVENT_BUILDER && blExtTrigger) {EventBuilderForOliver();}
    else if (EVENT_BUILDER) EventBuilderPreTrigger();

  if ((entry) % int(nb_entries / 100) == 0 || (entry) % 100000 == 0) {
    duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    double eventsPerSec = entry / duration;
    double eta = (nb_entries - entry) / eventsPerSec;
    std::cout << "                      \r" << entry << " / " << nb_entries
	      << " ====> " << round((float) entry / nb_entries * 100.)
	      << " % " << " (" << eventsPerSec << " ev/s, " << round(eta / 60)
	      << ":" << std::setw(2) << std::setfill('0')
	      << round(((int) eta) % 60) << std::setw(8) << " min ETA)";
    std::cout.flush();
    };
   
   nevents++;
   return kTRUE;
}

void DelilaSelectorEliade::FillOutputTree(){
    
  std::deque<DelilaEvent>::iterator it_delila_ = delilaQu.begin();
  
  if (has_detector["HPGe"])   HPGeEvent     ->clear();
  if (has_detector["SEG"])    HPGeSegEvent  ->clear();
  if (has_detector["LaBr"])   LabrEvent     ->clear();
  if (has_detector["Elissa"]) ElissaEvent   ->clear();;
  
  for (; it_delila_!= delilaQu.end();++it_delila_){
      switch (it_delila_->det_def){
          case 1:{
              if (has_detector["HPGe"]){
                  hpge_tree_event.SetEnergy(it_delila_->Energy_kev);
                  hpge_tree_event.SetEnergyDC(it_delila_->EnergyDC);
                  hpge_tree_event.SetChannel(it_delila_->channel);
                  hpge_tree_event.SetDomain(it_delila_->domain);
                  HPGeEvent->push_back(hpge_tree_event);
              };
              break;
          }
          case 2:{
              if (has_detector["SEG"]){
                  hpge_seg_tree_event.SetEnergy(it_delila_->Energy_kev);
                  hpge_seg_tree_event.SetEnergyDC(it_delila_->EnergyDC);
                  hpge_seg_tree_event.SetChannel(it_delila_->channel);
                  hpge_seg_tree_event.SetDomain(it_delila_->domain);
                  HPGeSegEvent->push_back(hpge_seg_tree_event);
              };
              break;
          }
          case 3:{
              if (has_detector["LaBr"]){
                  labr_tree_event.SetEnergy(it_delila_->Energy_kev);
                  labr_tree_event.SetEnergyDC(it_delila_->EnergyDC);
                  labr_tree_event.SetChannel(it_delila_->channel);
                  labr_tree_event.SetDomain(it_delila_->domain);
                  LabrEvent->push_back(labr_tree_event);
              };
              break;
          }
          case 7:{//ELISSA
               if (has_detector["Elissa"]){
                  elissa_tree_event.SetEnergy(it_delila_->Energy_kev);
                  elissa_tree_event.SetAmax(it_delila_->Amax);
                  elissa_tree_event.SetChannel(it_delila_->channel);
                  elissa_tree_event.SetDomain(it_delila_->domain);
                  ElissaEvent->push_back(elissa_tree_event);
               };
              break;
          }
          default : {
           
              break;   
          }
      };
   };
   
   
   if((has_detector["HPGe"]   && HPGeEvent->size()>0)    ||
      (has_detector["SEG"]    && HPGeSegEvent->size()>0)    ||
      (has_detector["LaBr"]   && LabrEvent->size()>0)    ||
      (has_detector["Elissa"] && ElissaEvent->size()>0))  {
        outputTree->Fill();
    };
   
   
   
  outputTree->Fill();
  return;
}


void DelilaSelectorEliade::TreatFold(int det)
{
   if (delilaQu.empty())return;
//      Int_t fold_size = foldQu.size();
     int nfold = 0;
     std::deque<DelilaEvent>::iterator it_delila_ = delilaQu.begin();

     for (; it_delila_!= delilaQu.end();++it_delila_){
         if (it_delila_->det_def == det) nfold++;
        };
         
    hBunchFold->Fill(nfold);
    it_delila_ = delilaQu.begin();
     
     for (; it_delila_!= delilaQu.end();++it_delila_){
         if (it_delila_->det_def == det) {
             it_delila_->fold = nfold;
             mFoldEnergy->Fill(nfold,it_delila_->Energy_kev);
//              if (blOutTree) {
//                 DelilaEvent_Treated = *it_delila_;                
//                 outputTree->Fill();
//              };
            };
        };
    nfold = 0;
}

// bool check_gamma_det(std::deque<DelilaEvent>::iterator it_check_) {return ((it_check_->det_def != 1 )&&(it_check_->det_def != 2 )&&(it_check_->det_def != 3 )&&(it_check_->det_def != 7 ));};

void DelilaSelectorEliade::TreatGammaGammaCoinc()
{
   if (delilaQu.empty())return;
   std::map<int, int> nmult;
   std::map<UInt_t, std::string>  ::iterator it_mult_ =  gg_coinc_id.begin();

   for(;it_mult_!=gg_coinc_id.end();++it_mult_) {
        if ((it_mult_->first != 11 )&&(it_mult_->first != 13 )&&(it_mult_->first != 33   )&&(it_mult_->first != 37 )&&(it_mult_->first != 17 )) continue;
        nmult[it_mult_->first] = 0;   
   };
   
   std::deque<DelilaEvent>::iterator it1_= delilaQu.begin();
   std::deque<DelilaEvent>::iterator it2_= delilaQu.begin();
   
   std::deque<DelilaEvent>::iterator it_dom1_= delilaQu.begin();
   std::deque<DelilaEvent>::iterator it_dom2_= delilaQu.begin();
   
     for (; it1_!= delilaQu.end();++it1_){
          if ((it1_->det_def != 1 )&&(it1_->det_def != 3 )&&(it1_->det_def != 7 )) continue;
          if (it1_->CS != 0) continue;
          it2_= delilaQu.begin();
//          int ttt=0;       
         for (; it2_!= delilaQu.end();++it2_){
             if (it1_ == it2_) continue;              
             if ((it2_->det_def != 1 )&&(it2_->det_def != 3 )&&(it2_->det_def != 7 )) continue;
             if (it2_->CS != 0) continue;
             int coinc_id = GetCoinc_det_def(it1_->det_def, it2_->det_def);
;
//              if (coinc_id == 77) continue;
             //Check that daq_ch is defined in LUT
            std::map<int, Float_t> ::iterator it_c_gates_ = coinc_gates.find(coinc_id);
            if(it_c_gates_ == coinc_gates.end()) continue;
            
            it_dom1_ = it1_;
            it_dom2_ = it2_;

            if ((it1_->det_def == 1)&&(it2_->det_def == 3)){//ge-labr the time of labr is first
                it_dom1_ = it2_;
                it_dom2_ = it1_;
            };
            
            if ((it1_->det_def == 7)&&(it2_->det_def == 3)){//Si-labr the time of labr is first
                it_dom1_ = it2_;
                it_dom2_ = it1_;
            };
            double_t time_diff_gg = it_dom2_->Time - it_dom1_->Time;
            
            if (coinc_id == 11) {
            	if (my_confs["Eliade"]){
	                int core_id1 =  (*it_dom1_).domain/100 * 10 +(*it_dom1_).domain/10%10;
        	        mGG_time_diff[coinc_id]->Fill(core_id1,time_diff_gg);
        	    };
         	 mGG_time_diff[coinc_id]->Fill(it_dom1_->domain,time_diff_gg);   
            }else {
                mGG_time_diff[coinc_id]->Fill(it_dom1_->domain,time_diff_gg);
            };
            
            hCoincID->Fill(coinc_id);
               double_t delta_theta = it1_->theta - it2_->theta;
            if (abs(time_diff_gg) < coinc_gates[coinc_id]){
                  mGG[coinc_id]->Fill(it_dom1_->Energy_kev, it_dom2_->Energy_kev);
                  nmult[coinc_id]++;
                  if (coinc_id == 37) it_dom1_->coincID = 7;
                  if (coinc_id == 17) it_dom1_->coincID = 7;
              };
        };
     };
//         std::cout<<"line 1268 \n";
        
   it_mult_ =  gg_coinc_id.begin();
   for(;it_mult_!=gg_coinc_id.end();++it_mult_){
       if ((it_mult_->first != 11 )&&(it_mult_->first != 13 )&&(it_mult_->first != 33   )&&(it_mult_->first != 37 )&&(it_mult_->first != 17 )) continue;
       hMult[it_mult_->first]->Fill(nmult[it_mult_->first]);          
   };

   
   
    if (has_detector["Elissa"]){ 
        it1_= delilaQu.begin();
        for (; it1_!= delilaQu.end();++it1_){ 
           if (it1_->coincID == 7) hLaBrElissa->Fill(it1_->Energy_kev); 
        }
    };
   
};

void DelilaSelectorEliade::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void DelilaSelectorEliade::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the clieint coreID, nt, it can be used to present
   // the results graphically or save the results to file.
    
  std::cout<<"I  will terminate soon... "<<std::endl;  

  TIter iter(fOutput);
      

  std::cout << std::endl << "Finished after " << round(duration / 60) << ":"
	    << std::setw(2) << round(((int) duration) % 60) << std::setw(8)
	    << ". Write spectra to file \n" 
//         << " A box of valpolicella can be sent to \n"
        << " 2 plates of tikka masala can be sent to \n"
        << " Strada Reactorului 30, Magurele 077125 \n"
        << " office 420 \n" << std::endl;


      TObject *obj; 
      
      if (blGammaGamma)             foutFile->mkdir("GammaGamma","GammaGamma");
      if (blLong)                   foutFile->mkdir("long","long");
      if (my_confs["Fold"])                   foutFile->mkdir("Energy_time_diff","Energy_time_diff");
      if (blAddBack)                foutFile->mkdir("AddBack","AddBack");
      if (blCS)                     foutFile->mkdir("CS","CS");
      if (has_detector["neutron"])  foutFile->mkdir("Neutron","Neutron");
      if (blExtTrigger)             foutFile->mkdir("CheckBunching","CheckBunching");
      //For Oliver----------------------------------------------------------------------------
      if (blExtTrigger)             foutFile->mkdir("EnergyTimeDiffCS_DC_domain","mEnergyTimeDiffCS_DC_domain");
      if (blExtTrigger)             foutFile->mkdir("EnergyTimeDiffCS_DC_theta","CheckBunching");
      //---------------------------------------------------------------------------------------
      if (my_confs["ParticleCutGate"] ||my_confs["TOF"] )        foutFile->mkdir("particle_cuts","particle_cuts");     
      if (has_detector["Elissa"]) foutFile->mkdir("dee","dee");
      
      outputTree->Write();
      if (blAddBack) addbackTree->Write();
      
       while ((obj = iter())) {
           TString name = obj->GetName();
//             std::cout<<"name "<<name<<std::endl;

           if((name.Contains("mCoreSegments")   ||
               name.Contains("hCoreFold")        ||
               name.Contains("mFoldSpec")        ||
               name.Contains("mGGCoreSegments")  ||
               name.Contains("mTimeDiffCoreSegments") ||
               name.Contains("mTimeDiffCoreCore")) && blAddBack){
                foutFile->cd(Form("%s:/AddBack", OutputFile.str().c_str()));
           }else if (name.Contains("EnergyTimeDiffCS_DC_domain") || name.Contains("EnergyTimeDiffCS_DC_E_domain")||name.Contains("EnergyTimeDiffCS_DC_noE_domain")){
                foutFile->cd(Form("%s:/EnergyTimeDiffCS_DC_domain", OutputFile.str().c_str()));
           }else if (name.Contains("EnergyTimeDiffCS_DC_theta")){
                foutFile->cd(Form("%s:/EnergyTimeDiffCS_DC_theta", OutputFile.str().c_str()));
           }else if (name.Contains("NN_")&&has_detector["neutron"]){
                foutFile->cd(Form("%s:/Neutron", OutputFile.str().c_str()));
           }else if (name.Contains("gate_check")&&my_confs["ParticleCutGate"] ){
                foutFile->cd(Form("%s:/particle_cuts", OutputFile.str().c_str())); 
            }else if (name.Contains("pid_gamma_")&&my_confs["ParticleCutGate"] ){
                foutFile->cd(Form("%s:/particle_cuts", OutputFile.str().c_str()));          
           }else if(name.Contains("mDelila_long") && blLong){
                foutFile->cd(Form("%s:/", OutputFile.str().c_str()));
           }else if(name.Contains("mEnergy_time_diff") && my_confs["Fold"]){
                foutFile->cd(Form("%s:/Energy_time_diff", OutputFile.str().c_str()));      
           }else if(name.Contains("CheckBunching")){
                foutFile->cd(Form("%s:/CheckBunching", OutputFile.str().c_str()));      
           }else if ((name.Contains("mCoreACS")         || 
                      name.Contains("mTimeDiffCoreACS") ||
                      name.Contains("mCoreSpecACS") ||
                      name.Contains("mAcsFold")         ||
                      name.Contains("ACS")         || 
                      name.Contains("hACSFold"))  && blCS){
               foutFile->cd(Form("%s:/CS", OutputFile.str().c_str()));      
           }else if((name.Contains("mAmaxEnergy_dom")) && (has_detector["Elissa"]) ){
               foutFile->cd(Form("%s:/AmaxEnergy", OutputFile.str().c_str()));      
           } else if (name.Contains("mgg_") && blGammaGamma){
                foutFile->cd(Form("%s:/GammaGamma", OutputFile.str().c_str()));      
//              std::cout<<Form("%s:/GammaGamma", OutputFile.str().c_str())<<std::endl;
            }else if (name.Contains("long") && blLong){
                foutFile->cd(Form("%s:/long", OutputFile.str().c_str()));      
            }else if (name.Contains("mDee")){
                foutFile->cd(Form("%s:/dee", OutputFile.str().c_str()));      
            }else {
                foutFile->cd(Form("%s:/", OutputFile.str().c_str()));
            };  
            
           
            
           if ( obj->IsA()->InheritsFrom(TH1F::Class())){
            TH1 *h1 = (TH1*)obj;
                if (h1->GetEntries()>0) obj->Write();
            }else if (obj->IsA()->InheritsFrom(TH2F::Class())){
            TH2 *h2 = (TH2*)obj;
                if (h2->GetEntries()>0) obj->Write();
            }else if (obj->IsA()->InheritsFrom(TH3F::Class())){
            TH3 *h3 = (TH3*)obj;
                if (h3->GetEntries()>0) obj->Write();
            }
        };
        
        if (my_confs["ParticleCutGate"] || my_confs["TOF"] ){
            foutFile->cd(Form("%s:/particle_cuts", OutputFile.str().c_str()));
            std::map<UInt_t, string>::iterator it_pid_=particle_name_in_cut.begin();
            for(;it_pid_!=particle_name_in_cut.end();++it_pid_){
                if (particle_cut[it_pid_->second] == 0x0) continue;
                particle_cut[it_pid_->second]->Write(); 
        }
        }
        
        
        if (has_detector["neutron"]) GetNMultiplicity();
        
//        outputTree->Write();
       foutFile->Close();
       delilaQu.clear();
       
//        std::cout<<"trigger_cnt "<<trigger_cnt<<std::endl;
   

}


int DelilaSelectorEliade::GetCoincTimeCorrection(int dom1, int dom2)
{
 int coin_id = GetCoincID(dom1, dom2);
//  if (dom1 >= dom2 ){coin_id = dom1*100+dom2;}else {coin_id = dom2*100+dom1;};
    
//  int coin_id = dom1*100+dom2;
 int time_corr = 0;
 std::map<int, int >::iterator it = LUT_TA.find(coin_id);
 if(it != LUT_TA.end()){time_corr = LUT_TA[coin_id];};
 return time_corr;
}

void DelilaSelectorEliade::cs()
{
    if (delilaQu.empty()) return;
    
    //BGO-GAMMA forward
    std::map<int, double> last_bgo_time;
    
    std::map<int, TDelilaDetector >::iterator it_lut_ = LUT_ELIADE.begin();
    for (; it_lut_ != LUT_ELIADE.end(); ++it_lut_){
        last_bgo_time[LUT_ELIADE[it_lut_->first].cs_dom] = -1;
    }
    
//     std::cout<<" DelilaSelectorEliade::cs() \n";
    double time_diff_bgo = -1;
    std::deque<DelilaEvent>  ::iterator it_ev__ = delilaQu.begin();
    
    for (; it_ev__ != delilaQu.end(); ++it_ev__){
        int cs_dom = (*it_ev__).cs_domain;
        int det = (*it_ev__).det_def;
        double time = 0;
        
//         if (det != 99)  std::cout<<" aaaaaaaaaaaaaaaa "<< det <<" \n";
        
        if (blExtTrigger){time = (*it_ev__).TimeBunch;}
        else time = (*it_ev__).Time; 
            
        

        switch (det)
        {
            case 4: case 5: case 6:{
                last_bgo_time[cs_dom] = time; 
//                  std::cout<<" csssssssssssssss \n";
                break;
            };
            case 1: case 2: case 3: {
//                  std::cout<<" csssssssssssssss \n";
               if (last_bgo_time[cs_dom] != -1)
               {
                  time_diff_bgo =  time - last_bgo_time[cs_dom];
//                  time_diff_bgo =  last_bgo_time[cs_dom] - time;
                 mTimeDiffCS ->Fill(cs_dom, time_diff_bgo);
//                   std::cout<<" coinc_gates "<<det<<" "<< coinc_gates[det*10+5] <<" "<<time_diff_bgo << " \n";
                  if (abs(time_diff_bgo) < coinc_gates[det*10+5]){
                    (*it_ev__).CS = 1; 
                    }else{
                    last_bgo_time[cs_dom] = -1;
                    };                 
               };
               break;
            };
            default: break;
        };
        
    };

     //GAMMA-BGO backward
    time_diff_bgo = -1;
    it_lut_ = LUT_ELIADE.begin();
    for (; it_lut_ != LUT_ELIADE.end(); ++it_lut_){
        last_bgo_time[LUT_ELIADE[it_lut_->first].cs_dom] = -1;
    }
    time_diff_bgo = 0;
    it_ev__ = delilaQu.end();
    
//     for (; it_ev__ != delilaQu.begin(); --it_ev__){
//         int cs_dom = (*it_ev__).cs_domain;
//         int det = (*it_ev__).det_def;
//                 double time = 0;
//         
//         if (blExtTrigger){time = (*it_ev__).TimeBunch;}
//         else time = (*it_ev__).Time; 
//         
//         switch (det)
//         {
//             case 4: case 5: case 6: {
//                 last_bgo_time[cs_dom] = time; 
//                 break;
//             };
//             case 1: case 2: case 3: {
//                if ((last_bgo_time[cs_dom] == -1)||((*it_ev__).CS == 1))
//                {
//                  break;  
//                }else{
//                   time_diff_bgo =  time - last_bgo_time[cs_dom];
// //                   time_diff_bgo =  last_bgo_time[cs_dom] - time;
//                  mTimeDiffCS ->Fill(cs_dom, time_diff_bgo); 
// //                  std::cout<<"time_diff_bgo back "<<time_diff_bgo<<" id "<<det*10+5 <<" "<<coinc_gates[det*10+5]<<"\n";
//                   if (abs(time_diff_bgo) < coinc_gates[det*10+5]) //10000)
//                  {
//                       (*it_ev__).CS = 1; //  std::cout<<" here cs \n";
//                  }else{
//                      last_bgo_time[cs_dom] = -1;
//                  };
//                  
//                };
//                break;
//             };
//             default: break;
//         };
//         
//     };
    
//     it_ev__ = delilaQu.begin();
//     
//     for (; it_ev__ != delilaQu.end(); ++it_ev__){
//          if (((*it_ev__).CS == 0)&&((*it_ev__).det_def == 3)) hDelilaCS[(*it_ev__).det_def]->Fill((*it_ev__).Energy_kev);
//          if (((*it_ev__).CS == 0)&&((*it_ev__).det_def == 1)) hDelilaCS[(*it_ev__).det_def]->Fill((*it_ev__).Energy_kev);
//     };
    
};


 int DelilaSelectorEliade::GetCoinc_det_def(int det_def1, int det_def2)
 {
     int id=0;
     if (det_def1<=det_def2) {id = det_def1*10+det_def2;}
         else {id = det_def2*10+det_def1; };
     return id;
 }


int DelilaSelectorEliade::GetCoincID(int dom1, int dom2){
    int id=0;
    if (dom1<=dom2) {id = dom1*100+dom2;}
        else {id = dom2*100+dom1; };
    return id;
}


void DelilaSelectorEliade::CheckPulserAllignement(int zero_dom)
{
   double time_diff_pulser;
   int cur_dom = DelilaEvent_.domain;
   
   if (cur_dom != zero_dom){
       time_diff_pulser =  DelilaEvent_.Time - PulserEvent.Time - GetCoincTimeCorrection(PulserEvent.domain, DelilaEvent_.domain);;
       mPulser0TimeDiff->Fill(cur_dom, time_diff_pulser);
//        std::cout<<" CheckPulserAllignement "<<cur_dom <<"  DelilaEvent_.domain "<<DelilaEvent_.Time<<" PulserEvent.Time "<<PulserEvent.Time<<" dt ="<<time_diff_pulser<<" \n";
//        if (DelilaEvent_.domain == 57) std::cout<<" time_diff_pulser #50 - #57 "<<time_diff_pulser<<" \n";
//        if (DelilaEvent_.domain == 51) std::cout<<" time_diff_pulser #50 - #51 "<<time_diff_pulser<<" \n";
   }else PulserEvent = DelilaEvent_;
   return;
}

void DelilaSelectorEliade::PrintDelilaEvent(DelilaEvent &ev_)
{
    std::cout<<" === DelilaEvent_ === \n"<<
    "  ev_.domain "<<ev_.domain<<" \n"<<
    "  ev_.Energy_kev "<<ev_.Energy_kev<<" \n"<<
    "  ev_.fold "<<ev_.fold<<" \n";
//     "  ============= \n";
}

void DelilaSelectorEliade::TimeAlignementInsideEvent()
{
     std::deque<DelilaEvent>::iterator it1_= delilaQu.begin();
     double time_diff_temp = 0;
     double ref_time = -1;
     for (; it1_!= delilaQu.end();++it1_){
      if ((*it1_).domain == my_params["reference_dom"] )
      {
          ref_time = (*it1_).Time;       
          break;
      }
     };
     
     if (ref_time == -1) return;
     it1_= delilaQu.begin();
     for (; it1_!= delilaQu.end();++it1_){
         if ((*it1_).domain == my_params["reference_dom"]) continue;
         time_diff_temp = ref_time - (*it1_).Time;
         mTimeCalibInsideEvent->Fill((*it1_).domain, time_diff_temp);
     };     
}

void DelilaSelectorEliade::TreatLaBrSingle()
{
    UShort_t daq_ch = DelilaEvent_.channel;
    UShort_t domain = DelilaEvent_.domain;
    
    DelilaEvent_.Energy_kev = CalibDet(DelilaEvent_.fEnergy, daq_ch);

//     Double_t theta = (180 - LUT_ELIADE[daq_ch].theta) * TMath::DegToRad();
    Double_t theta = LUT_ELIADE[daq_ch].theta * TMath::DegToRad();
    Double_t costheta = TMath::Cos(theta);
    if (my_params["beta"] >0) {
        DelilaEvent_.EnergyDC = DelilaEvent_.Energy_kev*(1./sqrt(1 - my_params["beta"]*my_params["beta"]) * (1 - my_params["beta"]*costheta));
//         if (LUT_ELIADE[daq_ch].theta == 90)
//         std::cout<<DelilaEvent_.Energy_kev<<" "<<DelilaEvent_.EnergyDC<<" "<<DelilaEvent_.Energy_kev -DelilaEvent_.EnergyDC<<" "<<LUT_ELIADE[daq_ch].theta<<" "<<costheta<<"\n";
//         mDelilaDC->Fill(domain,DelilaEvent_.EnergyDC);
//         hDelilaDC[DelilaEvent_.det_def]->Fill(DelilaEvent_.EnergyDC); 
//         if (blLong){mDelilaDC_long ->Fill(domain,DelilaEvent_.EnergyDC);}//do need any more
//         mEnergyTimeDiffDC[3]->Fill(DelilaEvent_.EnergyDC, DelilaEvent_.TimeBunch);
    };
    
    hDelila0[DelilaEvent_.det_def]->Fill(DelilaEvent_.Energy_kev); 
    mDelila->Fill(domain,DelilaEvent_.Energy_kev);
    
    
//     if (blLong){mDelila_long->Fill(domain,DelilaEvent_.Energy_kev);}
    
//      mEnergyTimeDiff[3]->Fill(DelilaEvent_.Energy_kev, DelilaEvent_.TimeBunch);
//     hTimeMinusTriggerLaBr->Fill(DelilaEvent_.Time);
//     std::cout<<DelilaEvent_.Time<<" "<<DelilaEvent_.Energy_kev<<"\n";

    return;
}

void DelilaSelectorEliade::TreatHpGeSingle()//clover
{
    UShort_t daq_ch = DelilaEvent_.channel;
    UShort_t domain = DelilaEvent_.domain;
    
    DelilaEvent_.Energy_kev = CalibDet(DelilaEvent_.fEnergy, daq_ch);
    double costheta = TMath::Cos(LUT_ELIADE[daq_ch].theta);

    mDelila->Fill(domain,DelilaEvent_.Energy_kev);
    hDelila0[DelilaEvent_.det_def]->Fill(DelilaEvent_.Energy_kev); 
    mEliade_raw->Fill(domain,DelilaEvent_.fEnergy);
    mEliade->Fill(domain,DelilaEvent_.Energy_kev);
    
    
    if (my_confs["IsEliade"]){
        if (DelilaEvent_.det_def == 1) mEliadeCores->Fill(DelilaEvent_.coreID, DelilaEvent_.Energy_kev);
        DelilaEvent_.cloverID =  domain/100;
        DelilaEvent_.coreID   =  domain/100 * 10 + domain/10%10;
        
    };
    
    if (my_params["beta"] >0) DelilaEvent_.EnergyDC = DelilaEvent_.Energy_kev*(1./sqrt(1 - my_params["beta"]*my_params["beta"]) * (1 - my_params["beta"]*costheta));
     
}

void DelilaSelectorEliade::TreatHPGeSegmentSingle()
{
    UShort_t daq_ch = DelilaEvent_.channel;
    UShort_t domain = DelilaEvent_.domain;
    
    DelilaEvent_.cloverID =  domain/100;
    DelilaEvent_.segmentID   =  domain/100;
    
    DelilaEvent_.Energy_kev = CalibDet(DelilaEvent_.fEnergy, daq_ch);

    double costheta = TMath::Cos(LUT_ELIADE[daq_ch].theta);
    if (my_params["beta"] >0) DelilaEvent_.EnergyDC = DelilaEvent_.Energy_kev*(1./sqrt(1 - my_params["beta"]*my_params["beta"]) * (1 - my_params["beta"]*costheta));
          
    
    mDelila->Fill(domain,DelilaEvent_.Energy_kev);
    
    hDelila0[DelilaEvent_.det_def]->Fill(DelilaEvent_.Energy_kev); 
    
    mEliade_raw->Fill(domain,DelilaEvent_.fEnergy);
    mEliade->Fill(domain,DelilaEvent_.Energy_kev);
}

void DelilaSelectorEliade::TreatNeutronSingle()
{
    DelilaEvent_.fEnergyShort = fEnergyShort;
    mShortLong->Fill(DelilaEvent_.fEnergy, DelilaEvent_.fEnergyShort);
//     std::cout<<DelilaEvent_.det_def<<" "<<" Long "<<DelilaEvent_.fEnergy<<" short "<< DelilaEvent_.fEnergyShort <<" n/g "<<(DelilaEvent_.fEnergy - DelilaEvent_.fEnergyShort)*1.0/DelilaEvent_.fEnergy<<"\n";
    mNeutron->Fill(DelilaEvent_.fEnergy*1.0, ((DelilaEvent_.fEnergy*1.0 - DelilaEvent_.fEnergyShort*1.0)/DelilaEvent_.fEnergy*1.0));
    hDelila0[DelilaEvent_.det_def]->Fill(DelilaEvent_.Energy_kev); 
}

void DelilaSelectorEliade::TreatNeutronSingle3He()
{
    std::string sz_ring = LUT_ELIADE[DelilaEvent_.channel].serial;
    sz_ring = sz_ring.substr(0,1);
    std::map<std::string, int> nnring;
    nnring["A"] = 0; nnring["B"] = 1;  nnring["C"] = 2;
//     std::cout<<nnring[sz_ring] <<"\n";
    hNN_TimeDiff->Fill(DelilaEvent_.Time - LastNeutronTime);
    LastNeutronTime = DelilaEvent_.Time;
    hNN_ring->Fill(nnring[sz_ring]);
    mNN_TimeDiff_counter->Fill(DelilaEvent_.domain, DelilaEvent_.Time - last_neutron_det[DelilaEvent_.domain]);
    last_neutron_det[DelilaEvent_.domain] = DelilaEvent_.Time; 
    DelilaEvent_.ring = nnring[sz_ring];
    hTheta["neutron"]->Fill(DelilaEvent_.theta);
//     std::cout<<DelilaEvent_.theta<<std::endl;
}

void DelilaSelectorEliade::TreatNeutronNeutron()
{
   if (delilaQu.empty())return;
   
   std::deque<DelilaEvent>::iterator it_n_= delilaQu.begin();

   double time_start =  (*it_n_).Time;
   int nn_mult = 0;
   bool blOnlyOnce = true;//true - each counter is allowed to fire onle once in the gate
   
   std::map<int, int> ::iterator it_fired_ = CounterIsFired.begin();
   for (;it_fired_!=CounterIsFired.end();++it_fired_)(*it_fired_).second = 0;
   
   for (; it_n_!= delilaQu.end();++it_n_){
       
//        if (it_n_ == delilaQu.begin())           continue;
       if ((*it_n_).det_def != 8)               continue;
       if ((CounterIsFired[(*it_n_).domain] > 0) && blOnlyOnce && ((*it_n_).ring == 0))    {
           CounterIsFired[(*it_n_).domain]++;
           hNN_fired->Fill((*it_n_).domain);
           continue; 
        };
       mNN_TimeDiff->Fill((*it_n_).domain, (*it_n_).Time - time_start);
       nn_mult++;
       CounterIsFired[(*it_n_).domain]++;
  };
   
//    std::map<int, int> ::iterator it_fired_ = CounterIsFired.begin();
//    
//    for (;it_fired_!=CounterIsFired.end();++it_fired_){
//        if ((*it_fired_).second < 2) continue;
// //        hNN_fired->SetBinContent((*it_fired_).first, hNN_fired->GetBinContent((*it_fired_).first) + (*it_fired_).second);
//         for (int k=0;k<=(*it_fired_).second;k++){
//             hNN_fired->Fill((*it_fired_).first);
//             std::cout<<"<<< "<<(*it_fired_).first<<" "<<(*it_fired_).second<<"\n";
//             (*it_fired_).second = 0;
//         }
//    };

   
   hNN_Mult->Fill(nn_mult);
}

void DelilaSelectorEliade::TreatElissaSinglePSA()
{
    vector<float> data_fil=DelilaSelectorEliade::trapezoidal(fSignal, 1000, 20, 0);//to be checked if signals do not present
    float trap_max=*max_element(data_fil.begin(),data_fil.end());
    float trap_min=*min_element(data_fil.begin(),data_fil.end());
    DelilaEvent_.Amax=trap_max-trap_min;
    
    DelilaEvent_.Energy_kev = CalibDet(DelilaEvent_.fEnergy, DelilaEvent_.channel);

    
    hAmax->Fill(DelilaEvent_.Amax);
//     hAmax->Fill(DelilaEvent_.Amax/DelilaEvent_.Energy_kev);
    mAmaxEnergy->Fill(DelilaEvent_.Energy_kev,DelilaEvent_.Amax);
    hDelila0[DelilaEvent_.det_def]->Fill(DelilaEvent_.Energy_kev); 
    mElissa->Fill(DelilaEvent_.domain, DelilaEvent_.Energy_kev);

    
    if (blFillAmaxEnergyDom) mAmaxEnergyDom[DelilaEvent_.domain]->Fill(DelilaEvent_.Energy_kev,DelilaEvent_.Amax);
    
    if (blDebugElissa) cout << trap_max << " " << trap_min <<" Amax "<<DelilaEvent_.Amax<< endl;   
    
}

void DelilaSelectorEliade::TreatElissaSingle()
{
    DelilaEvent_.Energy_kev = CalibDet(DelilaEvent_.fEnergy, DelilaEvent_.channel);

    
//     hAmax->Fill(DelilaEvent_.Amax/DelilaEvent_.Energy_kev);
    hDelila0[DelilaEvent_.det_def]->Fill(DelilaEvent_.Energy_kev); 
    mElissa->Fill(DelilaEvent_.domain, DelilaEvent_.Energy_kev);
    
//     if (blExtTrigger) mEnergyTimeDiff[DelilaEvent_.det_def]->Fill(DelilaEvent_.Energy_kev, DelilaEvent_.TimeBunch);

    
}

bool DelilaSelectorEliade::TriggerDecision()
{

   if (debug) std::cout<<" channel_trg "<< channel_trg <<" domain "<<DelilaEvent_.domain <<" det_def "<< DelilaEvent_.det_def<< " \n";
   
   if (det_def_trg == 999) return  true;
   
   if (!trigger_det_defs.empty()){
       std::vector<int>::iterator it_trg_ = trigger_det_defs.begin();
       bool blTRG = false;
       for (; it_trg_!=trigger_det_defs.end(); ++it_trg_){
            if (DelilaEvent_.det_def == *it_trg_){
                blTRG = true;
                break;
            };
        };
        return blTRG;
    } else if (!trigger_domains.empty()){
        std::vector<int>::iterator it_trg_ = trigger_domains.begin();
        bool blTRG = false;
        for (; it_trg_!=trigger_domains.end(); ++it_trg_){
            if (DelilaEvent_.domain == *it_trg_){
                blTRG = true;
                break;
            };
        };
        return blTRG;
    };
    std::cout<<"Warning: TriggerDecision returns false because the conditions were strange \n";
    return false;
};

void DelilaSelectorEliade::CheckPreQu()
{
    if (delilaPreQu.empty()) return;
    
    std::deque<DelilaEvent>::iterator it_= delilaPreQu.begin();
    double time_diff_temp;
    
//     for (; it_!= delilaPreQu.end();++it_){
    while ((it_!= delilaPreQu.end())&&(!delilaPreQu.empty())){
        time_diff_temp = it_->Time - DelilaEvent_.Time;  
       if (abs(time_diff_temp) > pre_event_length) delilaPreQu.erase(it_);
       ++it_;
//        if (delilaPreQu.empty()) break;
    };
}

void DelilaSelectorEliade::MovePreQu2Qu()
{
    if (delilaPreQu.empty()) return;
    if (pre_event_length == 0) return;
    if (!delilaQu.empty()) {
        std::cout<<"CheckPreQu delilaQu is not empty \n";
        delilaQu.clear();
    };
    
    std::deque<DelilaEvent>::iterator it_= delilaPreQu.begin();
    double time_diff_temp;
    
    int nval = 0;
    
    for (; it_!= delilaPreQu.end();++it_){
           time_diff_temp = it_->Time - DelilaEvent_.Time;                      
           if (abs(time_diff_temp) < pre_event_length) {delilaQu.push_back(*it_); nval++;};
    };
    
    hdelilaPreQu_size->Fill(nval);
    
//     std::cout<<"PreQueSize is "<< delilaPreQu.size() << " \n";
//     std::cout<<"delilaQueSize is "<< delilaQu.size() << " \n";
    
    delilaPreQu.clear();
}




void DelilaSelectorEliade::EventBuilderPreTrigger()
{

    if (blIsWindow){//open

       double time_diff_trigger;
       bool   blCloseCondition = false;
       
       if (blExtTrigger){
           
           if (DelilaEvent_.det_def == external_trigger_det_def)           {blCloseCondition = true; blIsWindow = false;}
           else if (blNewBunch)                                            {blCloseCondition = true;}

       }
       else{
           time_diff_trigger = DelilaEvent_.Time - LastTriggerEvent.Time;
           blCloseCondition = (abs(time_diff_trigger) > my_params["window_length"] );
           if (blCloseCondition) blIsWindow = false;
       };
        if (blCloseCondition){
            
            if (my_confs["Time_Alignement"] && !blExtTrigger) TimeAlignementInsideEvent();
//             if (blTimeAlignement)        TimeAlignementTrigger();//we do not use
            if (blCS)                cs_simple(15);
            if (blCS)                cs_simple(35);
//            if (blCS)                    ViewACS();
//            if (blCS)                    ViewACS_segments();
            if (my_confs["Fold"])                  TreatFold(3);
            
//           if (blAddBack)               ViewAddBackDetector();//for segments
//            if (blAddBack)               ViewAddBackDetectorCS();
//            if (blAddBack)               ViewAddBackCrystal();
            if (blAddBack)               ViewAddBackCoreCore();
           
           if (blGammaGamma)            TreatGammaGammaCoinc();
           
           if (my_confs["DeeSector"])		           ViewDeESector();
           if (my_confs["DeeRing"] )		           ViewDeERings();
           if (my_confs["DeeEx"])			           ViewDeeEx();
           if (my_confs["ParticleCutGate"] )          TreatGammaPartCoinc(1771);
           
           
           if (has_detector["neutron"]) TreatNeutronNeutron();
           
           if (blFillSingleSpectra)     FillSingleSpectra();
           if (blOutTree)               FillOutputTree();
           
           
           hdelilaQu_size->Fill(delilaQu.size());          
           delilaQu.clear();
           delilaPreQu.clear();
           
           
           if (blIsWindow || blNewBunch) {delilaQu.push_back(DelilaEvent_); /*blNewBunch = false;*/};
           
           
//             blIsWindow = false;
           
            
           if (!blIsWindow && TriggerDecision()) {
               SetUpNewTrigger();
           }else {
//                delilaPreQu.push_back(DelilaEvent_);
           };
           
        }else{//closed condition is not fullfilled
//            hDelila_single[DelilaEvent_.det_def]->Fill(DelilaEvent_.Energy_kev);
//            if (DelilaEvent_.TimeBunch > event_length) std::cout<<"DelilaEvent_.TimeBunch "<<DelilaEvent_.TimeBunch<<" event_length (!) "<< event_length << "\n"; 
            
           DelilaEvent_.trg = trigger_cnt;
           delilaQu.push_back(DelilaEvent_);
           hTimeMinusTriggerLaBr->Fill(DelilaEvent_.TimeBunch);
            
       };
       
   }else{//window is closed
       if (TriggerDecision()) {
           if (debug)std::cout<<" EventBuilderPreTrigger new trigger condition is fulfilled \n";
           SetUpNewTrigger(); 
       }else{ 
//            CheckPreQu();
           DelilaEvent_.trg = trigger_cnt;
//            delilaPreQu.push_back(DelilaEvent_);
        };
    };
}

void DelilaSelectorEliade::SetUpNewTrigger(){

    MovePreQu2Qu();
    
    hTriggerTrigger->Fill(DelilaEvent_.Time - LastTriggerEvent.Time);
    hTriggerDomain->Fill(DelilaEvent_.domain);
    
    LastTriggerEvent = DelilaEvent_;
    
    delilaQu.push_back(DelilaEvent_);//here uncomment
    
    
    
//     std::cout<<" TriggerTimeFlag is "<<TriggerTimeFlag<<" DelilaEvent_.Time "<<DelilaEvent_.Time<<"  \n";  
    
//     if (rf_time != 0) RF_N++;
//I do not remember  what is for TriggerTimeFlag   
//     TriggerTimeFlag = DelilaEvent_.Time - pre_event_length;
//     if (TriggerTimeFlag < 0){
//       std::cout<<"SetUpNewTrigger() TriggerTimeFlag is < 0 \n";  
//       TriggerTimeFlag = 0;
//     };
    
    blIsWindow = true;
    trigger_cnt++;
    if (debug)std::cout<<" New trigger is setup \n";
    return;
}


void DelilaSelectorEliade::SetUpNewTriggerSimple(){

    hTriggerTrigger->Fill(DelilaEvent_.Time - LastTriggerEvent.Time);
    hTriggerDomain->Fill(DelilaEvent_.domain);
    
    LastTriggerEvent = DelilaEvent_;
    
    TriggerTimeFlag = DelilaEvent_.Time;
    
    if (blAddTriggerToQueue) delilaQu.push_back(DelilaEvent_);
    
    blIsTrigger = true;

    trigger_cnt++;
    return;
}



void DelilaSelectorEliade::TreatACS()
{
//     int gate = 20000;
//     double_t time_diff_cs = 0;
//     int cs_dom = DelilaEvent_.cs_domain;
//     
//     if (DelilaEvent_.det_def == 1) gate = 100000;
// 
//     if ((DelilaEvent_.det_def == 3)||(DelilaEvent_.det_def == 1)) {
//         waitingQu_gamma[cs_dom].push_back(DelilaEvent_); 
//         hDelila_single[DelilaEvent_.det_def]->Fill(DelilaEvent_.Energy_kev);
// //         hDelilaDC[DelilaEvent_.det_def]->Fill(DelilaEvent_.EnergyDC);
// //         hDelila_long[DelilaEvent_.det_def]->Fill(DelilaEvent_.EnergyCal);
// //         hDelilaDC_long[DelilaEvent_.det_def]->Fill(DelilaEvent_.EnergyDC);
//     }
//     if (DelilaEvent_.det_def == 5) waitingQu_bgo[cs_dom].push_back(DelilaEvent_);
//         
//         if ((!waitingQu_gamma[cs_dom].empty())&&(!waitingQu_bgo.empty()))
//         {
//             std::deque<DelilaEvent>  ::iterator it_g__ = waitingQu_gamma[cs_dom].begin();
//             std::deque<DelilaEvent>  ::iterator it_bgo__ = waitingQu_bgo[cs_dom].begin();
//             for (; it_g__ != waitingQu_gamma[cs_dom].end();++it_g__){
//                 for (; it_bgo__ != waitingQu_bgo[cs_dom].end();++it_bgo__){   
//                 time_diff_cs = it_g__->Time - it_bgo__->Time ;//-  GetCoincTimeCorrection(it_g__->domain,it_bgo__->domain);
//                 mTimeCalibBGO->Fill(it_g__->domain, time_diff_cs);
//                 if (abs(time_diff_cs)<gate){
//                     if (it_g__->CS == 1) continue;
//                     it_g__->CS = 1;   
// //                     it_g__->bgo_time_diff = time_diff_cs;
// //                      if (it_g__->det_def == 1) {std::cout<<time_diff_cs<<" time_diff_cs \n";};
//                     //mLaBr_BGO_time_diff->Fill(it_g__->domain, time_diff_cs);
//                     }
//                 }
//             }
//         }
//         
//          if (!waitingQu_bgo[cs_dom].empty())
//          {
//              std::deque<DelilaEvent>  ::iterator it1_ = waitingQu_bgo[cs_dom].begin();
//              for (; it1_ != waitingQu_bgo[cs_dom].end();)
//              {
//               if (abs(DelilaEvent_.Time - it1_->Time)>gate) {it1_=waitingQu_bgo[cs_dom].erase(it1_);}
//                   else ++it1_;
//              }
//          };
//          
//          if (!waitingQu_gamma[cs_dom].empty())
//          {
//              std::deque<DelilaEvent>  ::iterator it2_ = waitingQu_gamma[cs_dom].begin();
//              for (; it2_ != waitingQu_gamma[cs_dom].end();)
//              {
//               if (abs(DelilaEvent_.Time - it2_->Time)>gate) 
//               {
//                  // output_pQu.push(*it2_);                  
//                   if (it2_->CS ==0){
// //                       hDelilaCS[it2_->det_def]->Fill(it2_->Energy_kev);//just removed 20220610
// //                       hDelilaCS_long[it2_->det_def]->Fill(it2_->EnergyCal);
// //                       mDelilaCS->Fill(it2_->cs_domain, it2_->EnergyCal);
// //                       gamma_gamma_cs(*it2_);
// //                       hDelilaCS_DC[it2_->det_def]->Fill(it2_->EnergyDC);
// //                       hDelilaCS_DC_long[it2_->det_def]->Fill(it2_->EnergyDC);
// //                       mDelilaCS_DC->Fill(it2_->cs_domain, it2_->EnergyDC);
//                 };
//                   
//                  if (blOutTree) {/*DelilaEventCS = *it2_; outputTree->Fill();*/};
//                   
//                   it2_=waitingQu_gamma[cs_dom].erase(it2_);
//               }
//               else ++it2_;
//              };
//          };
    return;

};


// add by saka
std::vector<float> DelilaSelectorEliade::trapezoidal(short wave[],int length, int L, int G)//L = 20; G = 0
{
  vector<float> ret;
  for(int j=0,n=length-(2*L+G); j<n; j++){
    float tmp=0;
    for(int k=0;k<L;k++){
      tmp+=(float)wave[j+k]*(-1/(float)L);
    }
    for(int k=0;k<G;k++){
      tmp+=wave[j+k+G]*0;
    }
    for(int k=0;k<L;k++){
      tmp+=(float)wave[j+k+G+L]*(1/(float)L);
    }
    ret.push_back(tmp);
  }
  return ret;

}
// add by saka

void DelilaSelectorEliade::TreatBGOSingle()
{
     DelilaEvent_.Energy_kev = CalibDet(DelilaEvent_.fEnergy, DelilaEvent_.channel);
     hDelila0[DelilaEvent_.det_def]->Fill(DelilaEvent_.Energy_kev);
     mDelila->Fill(DelilaEvent_.domain,DelilaEvent_.Energy_kev);
}
void DelilaSelectorEliade::FillSingleSpectra()
{
     std::deque<DelilaEvent>::iterator it_ev__= delilaQu.begin();
   
     for (; it_ev__!= delilaQu.end();++it_ev__){
         
         int domain = (*it_ev__).domain;
         
         hDelila_single[(*it_ev__).det_def]->Fill((*it_ev__).Energy_kev);
         
         
//          if (blExtTrigger) {FillSpectraForOliver((*it_ev__));continue;}
         if (blExpIsElifant) {FillSpectraForElifant((*it_ev__));continue;}
         
         
//           if ((blExtTrigger) && ( ((*it_ev__).det_def == 3) 
//                                  || ((*it_ev__).det_def == 5)
//  //                                 || ((*it_ev__).det_def == 7) 
//  //                                 || ((*it_ev__).det_def == 17)
//                                 )
//              ) mEnergyTimeDiff[(*it_ev__).det_def]->Fill(DelilaEvent_.Energy_kev, DelilaEvent_.TimeBunch);

//              if ((blExtTrigger) && (*it_ev__).det_def == 3) std::cout<<"det_type "<< (*it_ev__).det_def<<"\n";
         
//          if (!blCS) continue;
         
//          CheckIfObject(mSingleCoreCS);
         
        if ((*it_ev__).det_def == 1 && my_confs["IsEliade"]) {
              
             int core_id = (*it_ev__).domain/100 * 10 +(*it_ev__).domain/10%10;
//              mSingleCore->Fill(core_id, (*it_ev__).Energy_kev);
             
             if (((*it_ev__).CS == 0) && blCS) {
                   hDelilaCS[(*it_ev__).det_def]->Fill((*it_ev__).Energy_kev);
                   mSingleCoreCS->Fill(core_id, (*it_ev__).Energy_kev);
              }else {
                 int acs_id = (*it_ev__).CS%10;//this to be checked//20240529
//                   mCoreSpecACS[core_id]->Fill(acs_id,(*it_ev__).Energy_kev);
              };
         };
//          std::cout<<"here2 \n";
         if ((*it_ev__).det_def == 3 || (*it_ev__).det_def == 1) {
             if (((*it_ev__).CS == 0) && blCS)  {
                 mDelilaCS->Fill(domain, DelilaEvent_.Energy_kev);
                 hDelilaCS[(*it_ev__).det_def]->Fill((*it_ev__).Energy_kev);
             };
             if (my_params["beta"] > 0) {
                 mDelilaDC->Fill(domain, DelilaEvent_.EnergyDC); 
                 hDelilaDC[(*it_ev__).det_def]->Fill((*it_ev__).EnergyDC);
                 if (((*it_ev__).CS == 0) && blCS)  {
                     mDelilaCS_DC->Fill(domain, DelilaEvent_.EnergyDC);
                     hDelilaCS_DC[(*it_ev__).det_def]->Fill((*it_ev__).EnergyDC);
                 };
             };
         };
         
         
//          if (((*it_ev__).CS == 0)&&((*it_ev__).det_def == 3)) {
//              hDelilaCS[(*it_ev__).det_def]->Fill((*it_ev__).Energy_kev);
//              mDelilaCS->Fill(domain, DelilaEvent_.Energy_kev); 
//              if (beta > 0){
//               mDelilaDC->Fill(domain, DelilaEvent_.EnergyDC);   
//               mDelilaCS_DC->Fill(domain,DelilaEvent_.EnergyDC); 
//               hDelilaCS_DC[(*it_ev__).det_def]->Fill((*it_ev__).EnergyDC);
//              };             
//          };
         
//          if (((*it_ev__).CS == 0)&&((*it_ev__).det_def == 2)) hDelilaCS[(*it_ev__).det_def]->Fill((*it_ev__).Energy_kev);
         
//          if ((blExtTrigger) && (*it_ev__).det_def == 3) mEnergyTimeDiffCS[(*it_ev__).det_def]->Fill(DelilaEvent_.Energy_kev, DelilaEvent_.TimeBunch);

//          if (((*it_ev__).CS == 0)&&((*it_ev__).det_def == 1)) {};
     };
}


void DelilaSelectorEliade::ViewAddBackCrystal()
{
     std::deque<DelilaEvent>::iterator it1_= delilaQu.begin();
     std::deque<DelilaEvent>::iterator it2_= delilaQu.begin();
     for (; it1_!= delilaQu.end();++it1_){
         
         if ((*it1_).det_def != 1) continue;         
         int core_id1 =  (*it1_).domain/100 * 10 +(*it1_).domain/10%10;
         it2_= delilaQu.begin();
         
         std::deque<DelilaEvent> SegQu;
         SegQu.clear();
         it2_= delilaQu.begin();
         for (; it2_!= delilaQu.end();++it2_){
             
           if (it1_ == it2_) continue; 
           if ((*it2_).det_def != 2) continue;
           int core_id2 =  (*it2_).domain/100 * 10 +(*it2_).domain/10%10;
           if (core_id1 != core_id2) continue;
           
           int seg_id = (*it2_).domain%10;
//            std::cout<<core_id1<<" "<<core_id2<<" "<<seg_id<<" \n";           
           
           double time_diff_core_seg = (*it1_).Time - (*it2_).Time;
           
           
           mTimeDiffCoreSegments[core_id1]->Fill(seg_id,time_diff_core_seg);
           
           if (time_diff_core_seg < coinc_gates[12]) {
               mCoreSegments[core_id1]->Fill(seg_id,(*it2_).Energy_kev);
               mGGCoreSegments[core_id1]->Fill((*it1_).Energy_kev,(*it2_).Energy_kev);
               hCoreFold[core_id1]->SetBinContent(seg_id, hCoreFold[core_id1]->GetBinContent(seg_id)+1);
               SegQu.push_back(*it2_);
           };
         };
        
        std::deque<DelilaEvent>::iterator it3_= SegQu.begin();
        int nnfold = SegQu.size();
//         double foldsum = 0;
        for (; it3_!= SegQu.end();++it3_){
          mFoldSpec[core_id1]->Fill(nnfold, (*it3_).Energy_kev);  
//           foldsum+= (*it3_).Energy_kev;
        };
        SegQu.clear();
//         mFoldSpecSum[core_id1]->Fill(nnfold, foldsum); 

         
     };
    
}

void DelilaSelectorEliade::ViewAddBackDetector()//it is for segments
{
     std::deque<DelilaEvent>::iterator itd_= delilaQu.begin();
     
     delilaQuAddedBack.clear();//safe event for later g-g, similar to delilaQu back with addback
     
     std::deque<DelilaEvent> SegQu;
     SegQu.clear();

     for (; itd_!= delilaQu.end();++itd_){
         
         if ((*itd_).det_def != 2) continue;
         if ((*itd_).CS != 0) continue;
         
         SegQu.push_back(*itd_);
     }

     std::deque<DelilaEvent>::iterator it1_= SegQu.begin();
     std::deque<DelilaEvent>::iterator it2_= SegQu.begin();
     
     /*for (; it1_!= SegQu.end();++it1_){
         if ((*it1_).det_def != 2) {SegQu.erase(it1_); continue;}
         if ((*it1_).CS != 0) {SegQu.erase(it1_); continue;}
         int det_id1  =  (*it1_).cloverID;
         int core_id1 =  (*it1_).coreID;

         int nnfold = 1;
         double foldsum = (*it1_).Energy_kev;
         if (addback_tree>0){
           vDomain->clear();
           vEAddback->clear();
           vTime->clear();
           vDomain->push_back((*it1_).domain);
           vEAddback->push_back((*it1_).Energy_kev);
           vTime->push_back((*it1_).Time);
         }

         it2_= it1_;
         for (; it2_!= SegQu.end();++it2_){           
           if (it1_ == it2_) continue; 
           if ((*it2_).det_def != 2) continue;
           if ((*it2_).CS != 0) continue;  
           int det_id2 =  (*it2_).cloverID;
           int core_id2 = (*it2_).coreID;
           if (det_id1 != det_id2) continue;
           if (core_id1/10 != core_id2/10) continue;
//             std::cout<<core_id1<<" "<<core_id2<<" "<<" \n";           
           
           double time_diff_seg_seg = (*it1_).Time - (*it2_).Time;
           
           int id1 = core_id1;
           int id2 = core_id2;
           //mTimeDiffCoreSegments[id1]->Fill(id2,time_diff_seg_seg);
           
           if (time_diff_seg_seg < coinc_gates[12]) {
               nnfold++;
               foldsum += (*it2_).Energy_kev;

               if (addback_tree>0){
                 vDomain->push_back((*it2_).domain);
                 vEAddback->push_back((*it2_).Energy_kev);
                 vTime->push_back((*it2_).Time);
               }

               SegQu.erase(it2_);
               it1_ = SegQu.begin();
               it2_ = SegQu.begin();
           }
         };

         mFoldSpecSum[det_id1]->Fill(nnfold, foldsum);

         (*it1_).Energy_kev = foldsum;
         delilaQuAddedBack.push_back((*it1_));

         if (addback_tree>0){
             nfoldAddback = nnfold;
             EAddback = foldsum;
             addbackTree->Fill();
             vDomain->clear();
             vEAddback->clear();
             vTime->clear();
         }

         SegQu.erase(it1_);
         it1_ = SegQu.begin() - 1;
    }*/

     for (; it1_!= SegQu.end();++it1_){
         if ((*it1_).det_def != 2) {SegQu.erase(it1_); continue;}
         if ((*it1_).CS != 0) {SegQu.erase(it1_); continue;}
         int det_id1  =  (*it1_).cloverID;
         int core_id1 =  (*it1_).coreID;

         int nnfold = 1;
         double foldsum = (*it1_).Energy_kev;
         vEAddback->clear();
         vDomain->clear();
         if (addback_tree>0){
           vTime->clear();
           vTime->push_back((*it1_).Time);
         }
         vDomain->push_back((*it1_).domain);
         vEAddback->push_back((*it1_).Energy_kev);

         it2_= it1_;
         for (; it2_!= SegQu.end();++it2_){           
           if (it1_ == it2_) continue; 
           if ((*it2_).det_def != 2) continue;
           if ((*it2_).CS != 0) continue;  
           int det_id2 =  (*it2_).cloverID;
           int core_id2 = (*it2_).coreID;
           if (det_id1 != det_id2) continue;
           if (core_id1/10 != core_id2/10) continue;
//             std::cout<<core_id1<<" "<<core_id2<<" "<<" \n";           
           
           double time_diff_seg_seg = (*it1_).Time - (*it2_).Time;
           
           int id1 = core_id1;
           int id2 = core_id2;
           //mTimeDiffCoreSegments[id1]->Fill(id2,time_diff_seg_seg);

           if (time_diff_seg_seg < coinc_gates[12] && AddBack_distances[(*it1_).domain%100][(*it2_).domain%100]<addback_distance) {
               nnfold++;
               foldsum += (*it2_).Energy_kev;

               if (addback_tree>0){
                 vTime->push_back((*it2_).Time);
               }
               vDomain->push_back((*it2_).domain);
               vEAddback->push_back((*it2_).Energy_kev);

               SegQu.erase(it2_);
               it1_ = SegQu.begin();
               it2_ = SegQu.begin();
           }
         };

         foldsum = 0;
         for (int v_it = 0; v_it < int(vEAddback->size()); v_it++){
           for (int v_it2 = 0; v_it2 < int(vEAddback->size()); v_it2++){
             foldsum += vEAddback->at(v_it2) * Crosstalk_matrix[vDomain->at(v_it)%100][vDomain->at(v_it2)%100];
           }
         }

         mFoldSpecSum[det_id1]->Fill(nnfold, foldsum);

         (*it1_).Energy_kev = foldsum;
         delilaQuAddedBack.push_back((*it1_));

         if (addback_tree>0){
             nfoldAddback = nnfold;
             EAddback = foldsum;
             addbackTree->Fill();
             vDomain->clear();
             vEAddback->clear();
             vTime->clear();
         }

         SegQu.erase(it1_);
         it1_ = SegQu.begin() - 1;
    }
     /*std::deque<DelilaEvent>::iterator it1_= delilaQu.begin();
     std::deque<DelilaEvent>::iterator it2_= delilaQu.begin();

     for (; it1_!= delilaQu.end();++it1_){
         
         if ((*it1_).det_def != 1) continue;         
//          int core_id1 =  (*it1_).domain/100 * 10 +(*it1_).domain/10%10;
         int det_id1 =  (*it1_).domain/100;
         it2_= delilaQu.begin();
         
         std::deque<DelilaEvent> SegQu;
         SegQu.clear();
         
         for (; it2_!= delilaQu.end();++it2_){
             
           if (it1_ == it2_) continue; 
           if ((*it2_).det_def != 2) continue;
//            int core_id2 =  (*it2_).domain/100 * 10 +(*it2_).domain/10%10;
           int det_id2 =  (*it2_).domain/100;
           if (det_id1 != det_id2) continue;
           
           int seg_id = (*it2_).domain%100;
//            std::cout<<core_id1<<" "<<core_id2<<" "<<seg_id<<" \n";           
           
           double time_diff_core_seg = (*it1_).Time - (*it2_).Time;
           
           mTimeDiffCoreSegments[det_id1]->Fill(seg_id,time_diff_core_seg);
           
           if (time_diff_core_seg < coinc_gates[12]) {
               SegQu.push_back(*it2_);
           };
         };
         
        std::deque<DelilaEvent>::iterator it3_= SegQu.begin();
        int nnfold = SegQu.size();
        float foldsum = 0;
        bool goAddBack = true;
        for (; it3_!= SegQu.end();++it3_){
          std::deque<DelilaEvent>::iterator it4_= it3_ + 1;
          for (; it4_!= SegQu.end();++it4_){
            //std::cout<<(*it3_).domain%100<<" "<<(*it4_).domain%100<<" "<<AddBack_distances[(*it3_).domain%100][(*it4_).domain%100]<<" "<<(nnfold-1)*AddBack_distances[01][05]<<endl;
            //if (AddBack_distances[(*it3_).domain%100][(*it4_).domain%100]>(nnfold-1.)*AddBack_distances[01][05]){
            if (AddBack_distances[(*it3_).domain%100][(*it4_).domain%100]>addback_distance){
            //if (false){//AddBack_all
            //if ((*it3_).domain/10%10!=(*it4_).domain/10%10){//Same as core signal fold 1
              goAddBack = false;
              break;
            }
          }
        }
        it3_= SegQu.begin();
        if (addback_tree>0){
          vDomain->clear();
          vEAddback->clear();
          vTime->clear();
        }
        if (goAddBack){
          if (nnfold==2){
            if ((*it3_).domain%100>(*(it3_+1)).domain%100){
              mFoldSpec[det_id1]->Fill((*it3_).domain%100,(*(it3_+1)).domain%100);
            }
            else{
              mFoldSpec[det_id1]->Fill((*(it3_+1)).domain%100,(*it3_).domain%100);
            }
          } 
          for (; it3_!= SegQu.end();++it3_){
            mCoreSegments[det_id1]->Fill((*it3_).domain%100,(*it3_).Energy_kev);
            mGGCoreSegments[det_id1]->Fill((*it1_).Energy_kev,(*it3_).Energy_kev);
            hCoreFold[det_id1]->SetBinContent((*it3_).domain%100, hCoreFold[det_id1]->GetBinContent((*it3_).domain%100)+1);
            //mFoldSpec[det_id1]->Fill(nnfold, (*it3_).Energy_kev); 
            foldsum+= (*it3_).Energy_kev;
            if (addback_tree>0){
              vDomain->push_back((*it3_).domain);
              vEAddback->push_back((*it3_).Energy_kev);
              vTime->push_back((*it3_).Time);
            }
          };
          mFoldSpecSum[det_id1]->Fill(nnfold, foldsum);
          if (addback_tree>0){
            nfoldAddback = nnfold;
            EAddback = foldsum;
            addbackTree->Fill();
            vDomain->clear();
            vEAddback->clear();
            vTime->clear();
          }
        }
        else{
          for (; it3_!= SegQu.end();++it3_){
            mCoreSegments[det_id1]->Fill((*it3_).domain%100,(*it3_).Energy_kev);
            mGGCoreSegments[det_id1]->Fill((*it1_).Energy_kev,(*it3_).Energy_kev);
            hCoreFold[det_id1]->SetBinContent((*it3_).domain%100, hCoreFold[det_id1]->GetBinContent((*it3_).domain%100)+1);
            mFoldSpecSum[det_id1]->Fill(1, (*it3_).Energy_kev);
            if (addback_tree>0){
              vDomain->push_back((*it3_).domain);
              vEAddback->push_back((*it3_).Energy_kev);
              vTime->push_back((*it3_).Time);
              nfoldAddback = nnfold;
              EAddback = (*it3_).Energy_kev;
              addbackTree->Fill();
              vDomain->clear();
              vEAddback->clear();
              vTime->clear();
            }
          };
          /*for (int domfold1 = 0; domfold1 < 4; domfold1++){//Same as core signal fold 1
            nnfold = 0.;
            foldsum = 0.;
            for (; it3_!= SegQu.end();++it3_){
              if ((*it3_).domain/10%10==domfold1){
                mCoreSegments[det_id1]->Fill((*it3_).domain%100,(*it3_).Energy_kev);
                mGGCoreSegments[det_id1]->Fill((*it1_).Energy_kev,(*it3_).Energy_kev);
                hCoreFold[det_id1]->SetBinContent((*it3_).domain%100, hCoreFold[det_id1]->GetBinContent((*it3_).domain%100)+1);
                //mFoldSpec[det_id1]->Fill(nnfold, (*it3_).Energy_kev); 
                mFoldSpecSum[det_id1]->Fill(1, (*it3_).Energy_kev);
                vDomain->push_back((*it3_).domain);
                vEAddback->push_back((*it3_).Energy_kev);
                vTime->push_back((*it3_).Time);
                foldsum += (*it3_).Energy_kev;
              }
            }
            nfoldAddback = nnfold;
            EAddback = foldsum;
            if (EAddback > 0){
              addbackTree->Fill();
            }
            vDomain->clear();
            vEAddback->clear();
            vTime->clear();
            it3_= SegQu.begin();
          }
        }
        SegQu.clear();
     };*/
}

void DelilaSelectorEliade::ViewAddBackDetectorCS()
{
     std::deque<DelilaEvent>::iterator it1_= delilaQu.begin();
     std::deque<DelilaEvent>::iterator it2_= delilaQu.begin();
     for (; it1_!= delilaQu.end();++it1_){
         
         if ((*it1_).det_def != 1) continue;
         if ((*it1_).CS != 0) continue;                  
//          int core_id1 =  (*it1_).domain/100 * 10 +(*it1_).domain/10%10;
         int det_id1 =  (*it1_).domain/100;
         it2_= delilaQu.begin();
         
//          std::deque<DelilaEvent> SegQu;
//          SegQu.clear();
         
         std::deque<DelilaEvent> SegQuCS;
         SegQuCS.clear();
         it2_= delilaQu.begin();
         for (; it2_!= delilaQu.end();++it2_){
             
           if (it1_ == it2_)            continue; 
           if ((*it2_).det_def != 2)    continue;
           if ((*it2_).CS != 0)         continue;
//            int core_id2 =  (*it2_).domain/100 * 10 +(*it2_).domain/10%10;
           int det_id2 =  (*it2_).domain/100;
           if (det_id1 != det_id2) continue;
           
           int seg_id = (*it2_).domain%100;
//            std::cout<<core_id1<<" "<<core_id2<<" "<<seg_id<<" \n";           
           
           double time_diff_core_seg = (*it1_).Time - (*it2_).Time;
           
           mTimeDiffCoreSegmentsCS[det_id1]->Fill(seg_id,time_diff_core_seg);
           
           if (time_diff_core_seg < coinc_gates[12]) {
//                mCoreSegments[det_id1]->Fill(seg_id,(*it2_).Energy_kev);
//                mGGCoreSegments[det_id1]->Fill((*it1_).Energy_kev,(*it2_).Energy_kev);
//                hCoreFold[det_id1]->SetBinContent(seg_id, hCoreFold[det_id1]->GetBinContent(seg_id)+1);
                SegQuCS.push_back(*it1_);
           };
         };
        
        std::deque<DelilaEvent>::iterator it3_= SegQuCS.begin();
        int nnfold = SegQuCS.size();
        for (; it3_!= SegQuCS.end();++it3_){
          mFoldSpecCS[det_id1]->Fill(nnfold, (*it3_).Energy_kev);  
        };
        SegQuCS.clear();
     };
    
}

void DelilaSelectorEliade::ViewACS_cores()
{
     std::deque<DelilaEvent>::iterator it_core_= delilaQu.begin();
     std::deque<DelilaEvent>::iterator it_acs_= delilaQu.begin();
     for (; it_core_!= delilaQu.end();++it_core_){
         
         if ((*it_core_).det_def != 1) continue;      
         int core_id =  (*it_core_).domain/100 * 10 +(*it_core_).domain/10%10;
         it_acs_= delilaQu.begin();
         
         std::deque<DelilaEvent> AcsQu;
         AcsQu.clear();
         
         
         for (; it_acs_ != delilaQu.end();++it_acs_){
           if (it_core_ == it_acs_) continue; 
           if (((*it_acs_).det_def != 4) && ((*it_acs_).det_def != 5) && ((*it_acs_).det_def != 6)) continue;         
           
           int acs_id = (*it_acs_).domain%10;
//            std::cout<<core_id<<" "<<acs_id<<" \n";
           
           int coincID = it_core_->domain%100+acs_id*100;//core_id+acs_id*100
           int time_corr = 0;
           std::map<int, double_t >::iterator it = LUT_TA_COINC.find(coincID);
           if(it != LUT_TA_COINC.end()){time_corr = LUT_TA_COINC[coincID];};
           
           double time_diff_core_acs = (*it_core_).Time - (*it_acs_).Time - time_corr;
           mTimeDiffCoreACS[core_id]->Fill(acs_id,time_diff_core_acs);
           
           if (time_diff_core_acs < coinc_gates[15]) {
               mCoreACS[core_id]->Fill(acs_id,(*it_acs_).Energy_kev);               
               hACSFold[core_id]->SetBinContent(acs_id, hACSFold[core_id]->GetBinContent(acs_id)+1);
               AcsQu.push_back(*it_acs_);
               (*it_core_).CS = (*it_acs_).det_def;
           };
         };
         
            it_acs_= AcsQu.begin();
            int nnfold = AcsQu.size();
            for (; it_acs_!= AcsQu.end();++it_acs_){
                mAcsFold[core_id]->Fill(nnfold, (*it_acs_).Energy_kev);  
            };
            AcsQu.clear();
     };
    
}

void DelilaSelectorEliade::ViewACS_segments()
{
     std::deque<DelilaEvent>::iterator it_seg_= delilaQu.begin();
     std::deque<DelilaEvent>::iterator it_acs_= delilaQu.begin();
     for (; it_seg_!= delilaQu.end();++it_seg_){
         
         if ((*it_seg_).det_def != 2) continue;      
         int seg_id =  (*it_seg_).domain;
         it_acs_= delilaQu.begin();
         
         std::deque<DelilaEvent> AcsQu;
         AcsQu.clear();
         
         for (; it_acs_ != delilaQu.end();++it_acs_){
           if (it_seg_ == it_acs_) continue; 
           if (((*it_acs_).det_def != 4) && ((*it_acs_).det_def != 5) && ((*it_acs_).det_def != 6)) continue;         
           
           int acs_id = (*it_acs_).domain%10;
//            std::cout<<core_id<<" "<<acs_id<<" \n";           
           
           double time_diff_seg_acs = (*it_seg_).Time - (*it_acs_).Time;
           
           
           mTimeDiffSegACS[seg_id]->Fill(acs_id,time_diff_seg_acs);
           
           if (time_diff_seg_acs < coinc_gates[15]) {
                mSegACS[seg_id]->Fill(acs_id,(*it_acs_).Energy_kev);               
//                hACSFold[core_id]->SetBinContent(acs_id, hACSFold[core_id]->GetBinContent(acs_id)+1);
               AcsQu.push_back(*it_acs_);
               (*it_seg_).CS = (*it_acs_).det_def;
           };
         };
         
            it_acs_= AcsQu.begin();
            int nnfold = AcsQu.size();
            for (; it_acs_!= AcsQu.end();++it_acs_){
                mAcsFoldSeg[seg_id]->Fill(nnfold, (*it_acs_).Energy_kev);  
            };
            AcsQu.clear();
     };
    
}


void DelilaSelectorEliade::ViewAddBackCoreCore() //addback on the core level
{
     std::deque<DelilaEvent>::iterator itd_= delilaQu.begin();
     
     delilaQuAddedBack.clear();//safe event for later g-g, similar to delilaQu back with addback
     
     std::deque<DelilaEvent> CoreQu;
     CoreQu.clear();

     for (; itd_!= delilaQu.end();++itd_){
         
         if ((*itd_).det_def != 1) continue;
         if ((*itd_).CS != 0) continue;
         
         CoreQu.push_back(*itd_);
     }

     std::deque<DelilaEvent>::iterator it1_= CoreQu.begin();
     std::deque<DelilaEvent>::iterator it2_= CoreQu.begin();
     
     for (; it1_!= CoreQu.end();++it1_){
         if ((*it1_).det_def != 1) {CoreQu.erase(it1_); continue;}
         if ((*it1_).CS != 0) {CoreQu.erase(it1_); continue;}
         int det_id1  =  (*it1_).cloverID;
         int core_id1 =  (*it1_).coreID;

         int nnfold = 1;
         double foldsum = (*it1_).Energy_kev;
         if (addback_tree>0){
           vDomain->clear();
           vEAddback->clear();
           vTime->clear();
           vDomain->push_back((*it1_).domain);
           vEAddback->push_back((*it1_).Energy_kev);
           vTime->push_back((*it1_).Time);
         }

         it2_= it1_;
         for (; it2_!= CoreQu.end();++it2_){           
           if (it1_ == it2_) continue; 
           if ((*it2_).det_def != 1) continue;
           if ((*it2_).CS != 0) continue;  
           int det_id2 =  (*it2_).cloverID;
           int core_id2 = (*it2_).coreID;
           if (det_id1 != det_id2) continue;
           if (core_id1/10 != core_id2/10) continue;
//             std::cout<<core_id1<<" "<<core_id2<<" "<<" \n";           
           
           double time_diff_core_core = (*it1_).Time - (*it2_).Time;
           
           int id1 = core_id1;
           int id2 = core_id2;
           if (id1 < id2){
             mTimeDiffCoreCore[id1]->Fill(id2,time_diff_core_core);
           }else{
             mTimeDiffCoreCore[id2]->Fill(id1,-time_diff_core_core);
           }

           if (abs(time_diff_core_core) < coinc_gates[11]) {
               nnfold++;
               foldsum += (*it2_).Energy_kev;

               if (addback_tree>0){
                 vDomain->push_back((*it2_).domain);
                 vEAddback->push_back((*it2_).Energy_kev);
                 vTime->push_back((*it2_).Time);
               }

               CoreQu.erase(it2_);
               it1_= CoreQu.begin();
               it2_= CoreQu.begin();
           }
         };

         mFoldSpecSum[det_id1]->Fill(nnfold, foldsum);

         (*it1_).Energy_kev = foldsum;
         delilaQuAddedBack.push_back((*it1_));

         if (addback_tree>0){
             nfoldAddback = nnfold;
             EAddback = foldsum;
             addbackTree->Fill();
             vDomain->clear();
             vEAddback->clear();
             vTime->clear();
         }

         CoreQu.erase(it1_);
         it1_= CoreQu.begin()-1;
    }
        
/*        std::deque<DelilaEvent>::iterator it3_= CoreQu.begin();
        std::deque<DelilaEvent>::iterator it4_= CoreQu.begin();
        int nnfold = CoreQu.size();
        double foldsum = 0;
        double foldE = 0;
        
        if (addback_tree>0){
          vDomain->clear();
          vEAddback->clear();
          vTime->clear();
        }

        switch (nnfold){
            case 1:{
              mFoldSpec[det_id1]->Fill(nnfold, CoreQu[0].Energy_kev);
              mFoldSpecSum[det_id1]->Fill(nnfold, CoreQu[0].Energy_kev);
             
              delilaQuAddedBack.push_back(CoreQu[0]);

              if (addback_tree>0){
                vDomain->push_back(CoreQu[0].domain);
                vEAddback->push_back(CoreQu[0].Energy_kev);
                vTime->push_back(CoreQu[0].Time);
                nfoldAddback = nnfold;
                EAddback = CoreQu[0].Energy_kev;
                addbackTree->Fill();
                vDomain->clear();
                vEAddback->clear();
                vTime->clear();
              }

              break ;  
            };
            default :{
              it3_= CoreQu.begin();
              foldsum = 0;
              if (nnfold==2){
                if ((*it3_).coreID>(*(it3_+1)).coreID){
                  mFoldSpec[det_id1]->Fill((*it3_).coreID,(*(it3_+1)).coreID);
                }
                else{
                  mFoldSpec[det_id1]->Fill((*(it3_+1)).coreID,(*it3_).coreID);
                }
              }
              for (; it3_!= CoreQu.end();++it3_){
                //mFoldSpec[det_id1]->Fill(nnfold, (*it3_).Energy_kev);  
                /*foldE = 0.;
                it4_= CoreQu.begin();
                for (; it4_!= CoreQu.end();++it4_){
                  foldE += (*it4_).Energy_kev*Crosstalk_matrix[(*it3_).coreID][(*it4_).coreID];
                }
                foldsum+= foldE;*/
                /*foldsum+= (*it3_).Energy_kev;
                if (addback_tree>0){
                  vDomain->push_back((*it3_).domain);
                  //vEAddback->push_back(foldE);
                  vEAddback->push_back((*it3_).Energy_kev);
                  vTime->push_back((*it3_).Time);
                }
              };
              mFoldSpecSum[det_id1]->Fill(nnfold, foldsum);
               
              CoreQu[0].Energy_kev = foldsum;
              delilaQuAddedBack.push_back(CoreQu[0]);

              if (addback_tree>0){
                nfoldAddback = nnfold;
                EAddback = foldsum;
                addbackTree->Fill();
                vDomain->clear();
                vEAddback->clear();
                vTime->clear();
              }

            };
        };
     };*/
     
     
     std::deque<DelilaEvent>::iterator it_gg1_= delilaQuAddedBack.begin();
     std::deque<DelilaEvent>::iterator it_gg2_= delilaQuAddedBack.begin();
     int coinc_id = 11;
     
      for (; it_gg1_!= delilaQuAddedBack.end();++it_gg1_){
          it2_= delilaQuAddedBack.begin();
          for (; it_gg2_!= delilaQuAddedBack.end();++it_gg2_){  
              if (it_gg1_ == it_gg2_) continue;
              int core_id1 =  (*it_gg1_).domain/100 * 10 +(*it_gg1_).domain/10%10;
              double time_diff_gg = (*it_gg1_).Time - (*it_gg2_).Time;
                              
              if (abs(time_diff_gg) < coinc_gates[coinc_id]){
                  mGG_AB_time_diff[coinc_id]->Fill(core_id1,time_diff_gg);
                  mGG_AddBack[coinc_id]->Fill(it_gg1_->Energy_kev, it_gg2_->Energy_kev);
              };
          }        
      }
}

void DelilaSelectorEliade::Read_ELIADE_JSONLookUpTable()
{
     if (blLUT_ELIADE) return;
     
     std::cout<<"I am reading LUT_ELIADE.json \n";
     char* pLUT_Path;
     pLUT_Path = getenv ("ELIADE_LUT");
     if (pLUT_Path!=NULL)
     printf ("The JSON - LookUpTable path is: %s \n",pLUT_Path);
 
     std::stringstream fileName;
 //     fileName <<"/home/testov/EliadeSorting/LUT_ELIADE_CL29_new.json";
     fileName <<pLUT_Path<<"/LUT_ELIADE.json";
     std::ifstream fin = std::ifstream(fileName.str().c_str());
     
     if (!fin.good()) {
         std::ostringstream os;
         os << "Could not open " << fileName.str().c_str() << " and I need it ;(\n";
         Abort(os.str().c_str());
     };
    
     nlohmann::json data = nlohmann::json::parse(fin);
 
     std::cout << data.size() << " entries in the array" << std::endl;
  
 
     for(unsigned long int i = 0; i < data.size(); i++) {
       TDelilaDetector curDet;
        curDet.ch             =  data[i]["channel"];
        curDet.dom            =  data[i]["domain"];
        curDet.detType        =  data[i]["detType"];
        curDet.serial         =  data[i]["serial"];
        curDet.time_offset     =  data[i]["TimeOffset"];
        curDet.theta          =  data[i]["theta"];
        curDet.phi            =  data[i]["phi"];
        curDet.threshold      =  data[i]["threshold"];
        curDet.cs_dom         =  data[i]["cs_dom"];
        curDet.enable         =  data[i]["on"];
        
       curDet.pol_order      = 0;
       
     bool blPrintJson = false;
       if (blPrintJson){
       
           std::cout <<" Entry : \n"//<< "channel: " <<  curDet.ch  << "\n"
//   	      << "domain: " << curDet.dom << "\n"
//   	      << "detType: " << curDet.detType << "\n"
//   	      << "serial: " << curDet.serial << "\n"
          << "channel: " << data[i]["channel"] << "\n"
          << "domain: " << data[i]["domain"] << "\n"
          << "detType: " << data[i]["detType"] << "\n"
          << "serial: " << data[i]["serial"] << "\n"
  	      << "TimeOffset: " << data[i]["TimeOffset"] << "\n"
  	      << "theta: " << data[i]["theta"] << "\n"
          << "phi: " << data[i]["phi"] << "\n"
          << "threshold: " << data[i]["threshold"] << "\n"
          << "cs_dom: " << data[i]["cs_dom"] << "\n"
          << "enable: " << data[i]["enable"] << "\n"
          << "pol_order: " << data[i]["pol_order"] << "\n"
          << "pol_list: " << data[i]["pol_list"]// << "\n"
 	      << std::endl;
          
      auto poly = data[i]["pol_list"];
      if (poly.size() == 0) //does not work to be changed here on in python
      { poly.push_back(0);
        poly.push_back(1);
        
        std::cout<<"Warning for domain 0 no calibration is found; settung defaults 0+ch*1 \n";
      }
      unsigned long int poly_order = poly.size();
      
      for (unsigned long int j = 0;j<poly_order; j++){
            curDet.calibE.push_back(poly[j]);
           std::cout<<poly[j]<<" ";
       };
       std::cout<<"\n----- \n";
      }
       else{
             
         auto poly = data[i]["pol_list"];
         unsigned long int poly_order = poly.size();
         
         for (unsigned long int j = 0;j<poly_order; j++){
             curDet.calibE.push_back(poly[j]);
         };
           
       }
    	LUT_ELIADE[curDet.ch] = curDet;
          
  }
 
  fin.close();
  blLUT_ELIADE = true;
 
}

void DelilaSelectorEliade::TreatBeamCurrent()
{
    return;
}


// void DelilaSelectorEliade::GetNMultiplicity(TH1 *hh, int nn_max = 15)
void DelilaSelectorEliade::GetNMultiplicity()
{

  std::cout << "Welcome to the automatic GetMultiplicity macro \n"
	    << "I will calculate Multiplicity ratios from the analyszed file\n"
	    << "and will put it in the file"
	    << std::endl;
  fstream outputFile;
//   TH1F *hh = hNN_Mult.Clone();
  int nn_max = 15;
//   outputFile.open("multiplicity.dat", ios_base::out);
  
  outputFile.open(Form("run_selected_%i_%i.dat", atoi(RunID), atoi(VolID)), ios_base::out);
  float mult[nn_max];
  
  for (int i = 1; i<=nn_max; i++)mult[i]=-1;  
  
  for (int i = 1; i<=nn_max; i++){
    mult[i-1] = hNN_Mult->GetBinContent(i);  
    std::cout<< i-1 <<" "<<mult[i-1]<<"\n";
    outputFile<< i-1 <<" "<<mult[i-1]<<"\n";    
  };
  
  int nn_tot = 0;
  for (int i = 1; i<=nn_max; i++) nn_tot+=i*mult[i];  
  
//   for (int i = 1; i<=nn_max; i++)   std::cout	<<i<<" ddd "<< mult[i] <<"\n";
   std::map<int, std::vector<Float_t>> neutron_efficiency;
   std::vector<int> vecNN_mult = { 12, 13, 23, 24, 34};
   std::vector<int> :: iterator it_vecNN_ = vecNN_mult.begin();
   
   if (has_detector["neutron"]) Read_SeaTable();
   
   
      
   for (;it_vecNN_ != vecNN_mult.end();++it_vecNN_){
//        int index = 0;
        std::vector<Float_t> :: iterator it_ = mapSeaTable[(*it_vecNN_)].begin();
        std::vector<Float_t> :: iterator it_found_ =  mapSeaTable[(*it_vecNN_)].end();
        
        Float_t exp_ratio = (mult[(*it_vecNN_)/10])/(mult[(*it_vecNN_%10)]);
        Float_t last_diff = 100;
        
        for (; it_ !=  mapSeaTable[(*it_vecNN_)].end();++it_){
            
//            std::cout<<" last_diff "<< last_diff <<" abs((*it_) - exp_ratio " << abs((*it_) - exp_ratio) <<" \n"; //<< <<" "<<" "<< <<" "<<" "<< <<" "<<" "<< <<" "
            
           if ((  abs((*it_) - exp_ratio)) < last_diff ) {
                it_found_ = it_;
                last_diff = abs((*it_) - exp_ratio);
                }
            };
           int index = std::distance(mapSeaTable[(*it_vecNN_)].begin(),it_found_); 
            
            
           std::cout<< " NN = "<<(*it_vecNN_) <<" Found ratio "<<(*it_found_)<<" Exp Ratio "<< exp_ratio<<" index "<< " eff "<<  mapSeaTable[0][index] <<"\n";
           outputFile << " NN = "<<(*it_vecNN_) <<" Found ratio "<<(*it_found_)<<" Exp Ratio "<< exp_ratio<<" index "<< " eff "<<  mapSeaTable[0][index] <<"\n";

   }
    std::cout<<"\n TOT   " << nn_tot <<"\n";
    outputFile<<"\n TOT   " << nn_tot <<"\n";
/*   std::cout	<<" N1/N2 " <<mult[1]/mult[2]
     		<<"\n N1/N3 " <<mult[1]/mult[3]
     		<<"\n N2/N3 " <<mult[2]/mult[3]
     		<<"\n N2/N4 " <<mult[2]/mult[4]
     		<<"\n N3/N4 " <<mult[3]/mult[4]
     		    		
     		<<"\n N2/N5 " <<mult[2]/mult[5]
     		<<"\n N3/N5 " <<mult[3]/mult[5]
     		<<"\n N4/N5 " <<mult[5]/mult[5]
 		<<"\n TOT   " << nn_tot <<"\n";
//     */ 		
// //     
//    outputFile	<<" N1/N2 " <<mult[1]*1.0/mult[2]
//      		<<"\n N1/N3 " <<mult[1]*1.0/mult[3]
//      		<<"\n N2/N3 " <<mult[2]*1.0/mult[3]
//      		<<"\n N2/N4 " <<mult[2]*1.0/mult[4]
//      		<<"\n N3/N4 " <<mult[3]*1.0/mult[4]
//      		    		
//      		<<"\n N2/N5 " <<mult[2]*1.0/mult[5]
//      		<<"\n N3/N5 " <<mult[3]*1.0/mult[5]
//      		<<"\n N4/N5 " <<mult[4]*1.0/mult[5]
//  		<<"\n TOT   " << nn_tot <<"\n";
   
  outputFile.close();
}


void DelilaSelectorEliade::SimpleRun()
{
    if (DelilaEvent_.det_def == 3 && !blFirst_event) 
        {
            blFirst_event = true;
            ev_trg = DelilaEvent_;
            return;
        };
    
    if (!blFirst_event) return;
    
    if (DelilaEvent_.det_def == 3 && ev_trg.det_def == 3)
    {
        ev_trg = DelilaEvent_;
        return;
    }
    
    else if (DelilaEvent_.det_def == 7 && ev_trg.det_def == 7)
    {
        ev_trg = DelilaEvent_;
        return;
    }
    
    else if (DelilaEvent_.det_def == 7 && ev_trg.det_def == 3)
    {
        hhGammaSi->Fill(DelilaEvent_.Time - ev_trg.Time );
        ev_trg = DelilaEvent_;
        return;
    }
    
    else if (DelilaEvent_.det_def == 3 && ev_trg.det_def == 7)
    {
        hhGammaSi->Fill(DelilaEvent_.Time - ev_trg.Time );
        ev_trg = DelilaEvent_;
        return;
    };
    
    
    
    
    
    
    return;
}

void DelilaSelectorEliade::ViewDeESector()
{
  
  std::deque<DelilaEvent>::iterator it_de_= delilaQu.begin();
  std::deque<DelilaEvent>::iterator it_e_= delilaQu.begin();
  
  
    for (; it_de_!= delilaQu.end();++it_de_){
        
      bool bl_dee = false;
      if ((*it_de_).domain > 116)  continue; //not dEs-Es
      if ((*it_de_).det_def != 17) continue;
      
      int dee_mult = 0;


      it_e_ = delilaQu.begin();
      
      for (; it_e_  != delilaQu.end();++it_e_){   
          if ((*it_e_).det_def > 116) continue;
          if ((*it_e_).det_def != 7)  continue;
          if ((*it_de_).cs_domain != (*it_e_).cs_domain) continue;
          double time_diff;
          if (blExtTrigger) 
          {
              time_diff = ((*it_de_).TimeBunch - (*it_e_).TimeBunch);
          }else time_diff = (*it_de_).Time - (*it_e_).Time;
          
          mDee_Sector_TimeDiff -> Fill((*it_de_).domain, time_diff);
          hDee_SectorAll_TimeDiff->Fill(time_diff);
          
          if (abs(time_diff) > coinc_gates[177]) continue;
 
          dee_mult++;

          mDee_Sector[(*it_de_).cs_domain]->Fill((*it_e_).Energy_kev, (*it_de_).Energy_kev);
          mDee_SectorAll->Fill((*it_e_).Energy_kev, (*it_de_).Energy_kev);          
          (*it_de_).e_energy = (*it_e_).Energy_kev;
          
         //get ID
           std::map<UInt_t, string>::iterator it_pid_=particle_name_in_cut.begin();
            for(;it_pid_!=particle_name_in_cut.end();++it_pid_){
                if (particle_cut[it_pid_->second] == 0x0) continue;
                
                if (particle_cut[it_pid_->second]->IsInside(it_de_->e_energy, it_de_->Energy_kev))//Energy_kev is E energy
                {
                    it_de_->particleID = it_pid_->first;
                    hPID_dee->Fill(it_de_->particleID);
                    mdee_gate_check[it_de_->particleID]->Fill(it_de_->e_energy, it_de_->Energy_kev);
                    break;
        //                 it1_->particleID+= it_pid_->first;
        //                 mdee_gate_check[it_pid_->first]->Fill(it1_->e_energy, it1_->Energy_kev);
        //                 hGG_particle[it_pid_->second]->Fill((*it_g_).Energy_kev);
                    }
                };
      }
      hMult_dee->Fill(dee_mult);
  }
 return;
}

void DelilaSelectorEliade::ViewDeERings()
{
  std::deque<DelilaEvent>::iterator it_de_= delilaQu.begin();
  std::deque<DelilaEvent>::iterator it_e_= delilaQu.begin();
  
  
  for (; it_de_!= delilaQu.end();++it_de_){
      
      if ((*it_de_).det_def != 17) continue;
      if (((*it_de_).det_def >=100) && ((*it_de_).det_def <= 115)) continue;//for sectors dEs there is separate proceedure
      it_e_ = delilaQu.begin();
      
      for (; it_e_  != delilaQu.end();++it_e_){   //any if E sector is okay
          if ((*it_e_).det_def != 7)  continue;
          
          double time_diff;          
          
          if (blExtTrigger) 
          {
              time_diff = (*it_de_).TimeBunch - (*it_e_).TimeBunch;
          }else time_diff = (*it_de_).Time - (*it_e_).Time;
                    
          
          mDee_Ring_TimeDiff -> Fill((*it_de_).domain, time_diff);
          hDee_RingAll_TimeDiff ->Fill(time_diff);
          
          if (abs(time_diff) > coinc_gates[177]) continue;
          mDee_Ring[(*it_de_).domain]->Fill((*it_e_).Energy_kev, (*it_de_).Energy_kev);
          mDee_RingAll->Fill((*it_e_).Energy_kev, (*it_de_).Energy_kev);
          (*it_de_).e_energy = (*it_e_).Energy_kev;

      }
  }
 return;
}

void DelilaSelectorEliade::ViewDeeEx()
{
   std::deque<DelilaEvent>::iterator it1_= delilaQu.begin();
   std::deque<DelilaEvent>::iterator it2_= delilaQu.begin();
    
    
    for (; it1_  != delilaQu.end();++it1_){
        
        if ((*it1_).det_def != dname["dElissa"]) continue;
        if ((*it1_).e_energy == 0)           continue;
        
        it2_= delilaQu.begin();
        
        for (; it2_  != delilaQu.end();++it2_){
          
            
            if ((*it2_).det_def != dname["LaBr"]) continue;
            if ((*it2_).CS != 0) continue;
            
            double time_diff ;
            
            if (blExtTrigger) 
            {
                time_diff = (*it1_).TimeBunch - (*it2_).TimeBunch;
            }else time_diff = (*it1_).Time - (*it2_).Time; 
            
            
            
            
            if (abs(time_diff) > coinc_gates[1773]) continue;
            mGG_time_diff[1773]->Fill((*it1_).domain,time_diff);
           
            float Ex = (*it1_).ElasticEnergy - ((*it1_).e_energy + (*it1_).Energy_kev);
            
            mGG[1773]->Fill((*it2_).Energy_kev, Ex);

            
//             std::cout<<" Eel "<< (*it1_).ElasticEnergy << " Ee "<< (*it1_).e_energy << " Ede "<< (*it1_).Energy_kev<<" Ex = "<<Ex<< " \n";
            
//             mGG_time_diff[1773]->Fill(Ex, (*it2_).Energy_kev);
        }
    }
    
}



void DelilaSelectorEliade::EventBuilder()
{
//     if (blIsWindow){
//      // check if is inside the gate
//      //increment queue   
//     }
//     
    return;
}

void DelilaSelectorEliade::cs_simple(int coinc_id)
{//see cs(), should work
    if (delilaQu.empty()) return;
    
   
    double time_diff_bgo = -1;
    std::deque<DelilaEvent>  ::iterator it_ev1__ = delilaQu.begin();
    
    int current_coinc_id = 0;
    double time_diff= 0;
    
   
     for (; it_ev1__ != delilaQu.end(); ++it_ev1__){
         if ((*it_ev1__).Time < 0) continue;
         if ((*it_ev1__).det_def > 3)   continue; //not bgo (4,5,6) or gamma (1,2,3)
//          if ((*it_ev1__).CS == 1 )      continue; 
         std::deque<DelilaEvent>  ::iterator it_ev2__ = delilaQu.begin();
         
         bool bgo1 = EventIsBGO((*it_ev1__));
         
 //                     std::cout<<"current_coinc_id x "<<current_coinc_id<<"\n";
 
        for (; it_ev2__ != delilaQu.end(); ++it_ev2__){
             if (it_ev1__ == it_ev2__)                           continue;
             if ((*it_ev2__).Time < 0)                           continue;
             if (!EventIsBGO((*it_ev2__)))                        continue;
             if ((*it_ev2__).CS == 1)                            continue;              
             if ((*it_ev1__).det_def == (*it_ev2__).det_def)     continue;
             if ((*it_ev1__).cs_domain != (*it_ev2__).cs_domain) continue;
             
             //bool bgo2 = EventIsBGO((*it_ev2__));
             //if ((bgo1 && bgo2) || (!bgo1 && !bgo2)) continue;
             
             
             //if ((*it_ev1__).det_def < (*it_ev2__).det_def){current_coinc_id = (*it_ev1__).det_def*10+(*it_ev2__).det_def ;}
             //    else current_coinc_id = (*it_ev2__).det_def*10+(*it_ev1__).det_def;
             current_coinc_id = (*it_ev1__).det_def*10+(*it_ev2__).det_def;
// //             std::cout<<"current_coinc_id "<<current_coinc_id<<"\n";
 
             if (current_coinc_id != coinc_id) continue;
             
             
             if (blExtTrigger) 
             {
              time_diff = ((*it_ev1__).TimeBunch - (*it_ev2__).TimeBunch);
             }else time_diff = (*it_ev1__).Time - (*it_ev2__).Time;
//              time_diff =  ((*it_ev1__).Time - (*it_ev2__).Time);
             mTimeDiffCS ->Fill((*it_ev1__).cs_domain, time_diff);
             
             if (abs(time_diff) < coinc_gates[current_coinc_id]) 
//                  if ((abs(time_diff) < 250e3) && (abs(time_diff) > 150e3) )
                 {
                     if ((*it_ev1__).det_def <= 3) {(*it_ev1__).CS = 1;}
                        else if ((*it_ev2__).det_def <= 3) (*it_ev2__).CS = 1;
                 };
         };
    };
};


void DelilaSelectorEliade::Read_CutFile(){
    
    std::cout << "Reading Cut File... " ;
    
    char* pLUT_Path;
    pLUT_Path = getenv ("ELIADE_LUT");
    if (pLUT_Path!=NULL)
    printf ("The LookUpTable path is: %s \n",pLUT_Path);

    std::stringstream CUTFile;
    CUTFile << pLUT_Path <<"/"<<"cut_charged_particles.root";
//     std::ifstream lookuptable(LUTFile.str().c_str());
    
    if(gSystem->AccessPathName(CUTFile.str().c_str())){
        std::cout << "Warning Cut File does not exist: " << CUTFile.str().c_str() <<std::endl;
        return;
    } else {
        std::cout << "Cut File  found" <<  CUTFile.str().c_str() << std::endl;
    }
    
     std::map<UInt_t, string>::iterator p_name;
    
     TFile *file_cut_charged = TFile::Open(CUTFile.str().c_str());
     
     for (p_name = particle_name_in_cut.begin();
 	     p_name != particle_name_in_cut.end(); ++p_name) {
        
//          TCutG *cut_temp = (TCutG*) file_cut_charged->Get(Form("dom_%i_%s", 999, p_name->second.c_str()));                                                                                                                                                                                                                                                           
//          particle_cut[ p_name->second] = (TCutG*) cut_temp->Clone();
        
	  if (debug)
	    std::cout << std::endl << "Looking for cut name "
		      << Form("dom_%i_%s", 999,
			      p_name->second.c_str()) << " ... ";
                  
      if (file_cut_charged->GetListOfKeys()->Contains(Form("dom_%i_%s", 999, p_name->second.c_str()))) {
          TCutG *cut_temp = (TCutG*) file_cut_charged->Get(Form("dom_%i_%s", 999, p_name->second.c_str()));
     
        if (cut_temp != 0x0) {
            std::cout<<"cut_charged_particles.root file contains "<<Form("dom_%i_%s", 999, p_name->second.c_str())<<"\n";
            particle_cut[p_name->second.c_str()] = (TCutG*) cut_temp->Clone();
            cut_temp->Delete();
        }
      }
 	}

	return;
}

#include "src/TriggerOliver.C"

bool DelilaSelectorEliade::EventIsBGO(DelilaEvent ev_)
{
    return ((ev_.det_def == 4)||(ev_.det_def == 5)||(ev_.det_def == 6));
}

void DelilaSelectorEliade::TreatGammaPartCoinc(int coinc_id)//1773 - de-e-LaBr; 1771 de-e-HPge
{
    
   if (coinc_id != 1771 && coinc_id != 1773) {
       std::cout<<"TreatGammaPartCoinc wrong coinc_id \n";
       return;
   }
    
   std::deque<DelilaEvent>::iterator it1_= delilaQu.begin();
   std::deque<DelilaEvent>::iterator it_g_= delilaQu.begin();
   
   int id_gamma_det=1;//coinc_id%10;
//    std::cout<<"id_gamma_det "<<id_gamma_det<<" \n";
    
    
    for (; it1_  != delilaQu.end();++it1_){
        
        
        if ((*it1_).det_def != 17) continue;
        
//         std::cout<<"(*it1_).det_def "<<(*it1_).det_def<<" (*it1_).e_energy "<<(*it1_).e_energy<<" \n";

        
        if ((*it1_).e_energy == 0)           continue;
        
        if (it1_->particleID == 0)           continue;
        
        it_g_= delilaQu.begin();
        
        for (; it_g_  != delilaQu.end();++it_g_){
          
            
//             if ((*it2_).det_def != dname["LaBr"]) continue;
            if ((*it_g_).det_def != id_gamma_det) continue;
            if ((*it_g_).CS != 0) continue;
            
            double time_diff ;
            
            if (blExtTrigger) 
            {
                time_diff = (*it1_).TimeBunch - (*it_g_).TimeBunch;
            }else time_diff = (*it1_).Time - (*it_g_).Time; 
            
            
            if (abs(time_diff) > coinc_gates[coinc_id]) continue;
            
            it_g_->particleID+= it1_->particleID;

        }   
    }
    
     it_g_= delilaQu.begin();
        
        for (; it_g_  != delilaQu.end();++it_g_){
            if (it_g_->det_def != id_gamma_det) continue;
            hPID->Fill(it_g_->particleID);
//             std::map<std::string, TH1F*>:: iterator it_hist_ = particle_name_in_cut.begin();
            std::map<UInt_t, string>:: iterator it_pname_ = particle_name_in_cut.begin();
            
//             if (it_g_->particleID != 10 && it_g_->particleID != 100 && it_g_->particleID != 1 && it_g_->particleID != 1000) continue;
            
            if (particle_name_in_cut.find(it_g_->particleID) != particle_name_in_cut.end()) hGG_particle[particle_name_in_cut[it_g_->particleID]]->Fill(it_g_->Energy_kev);
            if (particle_name_without_cut.find(it_g_->particleID) != particle_name_without_cut.end()) hGG_particle[particle_name_without_cut[it_g_->particleID]]->Fill(it_g_->Energy_kev);
            
//             std::string h_name = Form("%ip%id%ia%iLi",(it_g_->particleID)%10,(it_g_->particleID/10)%10,(it_g_->particleID/100)%10,(it_g_->particleID/1000)%10);
   
             std::string h_name;

//              //creating missing hists for cuts
//              if (particle_name_without_cut.find(it_g_->particleID) == particle_name_without_cut.end()) {
//                 int nbins = 16384; float bin_max = 16383.5; int kev_bin = 1;
//                 std::map<string, int> map_hist_name;
//                 if ((it_g_->particleID)%10 >0) map_hist_name["p"] = (it_g_->particleID)%10;
//                 if ((it_g_->particleID/10)%10 >0) map_hist_name["d"] = (it_g_->particleID/10)%10;
//                 if ((it_g_->particleID/100)%10 >0) map_hist_name["a"] = (it_g_->particleID/100)%10;
//                 if ((it_g_->particleID/1000)%10 >0) map_hist_name["Li"] = (it_g_->particleID/1000)%10;
//                 std::map<string, int>::iterator it_hname_=map_hist_name.begin();
//                 for (;it_hname_!= map_hist_name.end();++it_hname_) h_name.append(Form("%i%s", it_hname_->second, it_hname_->first.c_str()));
//                 
//                 hGG_particle[h_name] = new TH1F(Form("pid_gamma_%i_%s",it_g_->particleID, h_name.c_str()), Form("pid_%i_#gamma_%s",it_g_->particleID, h_name.c_str()), 16384, -0.5, bin_max);
//                 hGG_particle[h_name]->GetXaxis()->SetTitle(Form("keV, %i keV/bin", kev_bin)); 
//                 hGG_particle[h_name]->GetYaxis()->SetTitle("counts");
//                 fOutput->Add(hGG_particle[h_name]);
//                 particle_name_without_cut[it_g_->particleID]=h_name;
//                 std::cout<<" hGG_particle_"<<h_name<<" created \n";
//             }
//             hGG_particle[h_name]->Fill(it_g_->Energy_kev);
           if (!blParticleGammaGamma) continue;
           
            std::deque<DelilaEvent>::iterator it2_g_= delilaQu.begin();
              for (; it2_g_  != delilaQu.end();++it2_g_){
                  if (it2_g_->det_def != id_gamma_det) continue;
                  if (it_g_ == it2_g_) continue;
                  if (it_g_->particleID != it2_g_->particleID) continue;
                    
                  double time_diff;
                  if (blExtTrigger){
                        time_diff = (*it_g_).TimeBunch - (*it2_g_).TimeBunch;
                    }else time_diff = (*it_g_).Time - (*it2_g_).Time; 
                    
                    
                    if (abs(time_diff) > coinc_gates[11]) continue;//coing HpGe-Hpge
                    
                    if (particle_name_in_cut.find(it_g_->particleID) != particle_name_in_cut.end()) mGG_particle[particle_name_in_cut[it_g_->particleID]]->Fill(it_g_->Energy_kev, it2_g_->Energy_kev);
//                     if (particle_name_without_cut.find(it_g_->particleID) != particle_name_without_cut.end()) mGG_particle[particle_name_without_cut[it_g_->particleID]]->Fill(it_g_->Energy_kev, it2_g_->Energy_kev);
                        
                    
            };


            
//             std::string hh_name =  particle_name_in_cut[it_pname_->second] ;
            
//             string hh_str =  particle_name_in_cut[it_g_->particleID]
//             hGG_particle[particle_name_in_cut[it_g_->particleID]]->Fill(it_g_->Energy_kev);
            
        };
    
    
    
    return;  

};

void DelilaSelectorEliade::TreatPSD()//clover
{
     UShort_t daq_ch = DelilaEvent_.channel;
     UShort_t domain = DelilaEvent_.domain;
    
     DelilaEvent_.Energy_kev = CalibDet(DelilaEvent_.fEnergy, daq_ch);
     DelilaEvent_.fEnergyShort = fEnergyShort;
     float psd = (1.0*DelilaEvent_.fEnergy - DelilaEvent_.fEnergyShort) / (DelilaEvent_.fEnergy + DelilaEvent_.fEnergyShort);
     mPSD[domain]->Fill(DelilaEvent_.fEnergy, psd);
     
     if (my_confs["TOF"]){
      	mTOF_ch_long[domain]->Fill(DelilaEvent_.fEnergy, DelilaEvent_.TimeBunch);
  	mTOF_ch_short[domain]->Fill(psd, DelilaEvent_.TimeBunch);
  	 };
     
     
}

double DelilaSelectorEliade::Time2Energy(double d, double t)
{
	//E in MeV, time in s, distance in m
	double v = 0;
 	double gamma = 0;
 	double c = 299792458.0;
 	double e = 0;
	v = d/t;	
	gamma = 1./pow(1.-v*v/(c*c),0.5);
	e = 939.565*(gamma-1); //939.565 = Mn*c^2
	//std::cout<<"gamma "<<gamma<<" v "<<v<<" E "<<e<<" \n";
	return e;	
};

bool DelilaSelectorEliade::CheckKey(std::map<int, TH2F*> dic, int key)
{
    //----------------------------------------------------------------------------------------------------------------
    // Check if the key exists in the map
    //----------------------------------------------------------------------------------------------------------------
    auto it = dic.find(key);
    if (it != dic.end()) {
        std::cout << "Key " << key << " exists in the map.\n";
        TH2F* hist = it->second;
        std::cout << "Histogram Name: " << hist->GetName() << "\n";
        std::cout << "Histogram Title: " << hist->GetTitle() << "\n";
        std::cout << "Entries in histogram: " << hist->GetEntries() << "\n";
        return true;
    } else {
        std::cout << "Hist " <<  Form("Key %i", key) << " does not exist \n";
        return false;
    }
    //----------------------------------------------------------------------------------------------------------------
}



