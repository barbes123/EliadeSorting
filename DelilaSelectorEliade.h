//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Mar 23 13:02:03 2021 by ROOT version 6.22/06
// from TTree DelilaSelectorEliade/Energy Station
// found on file: run1005_03.root
//////////////////////////////////////////////////////////

#pragma once

#ifndef DelilaSelectorEliade_h
#define DelilaSelectorEliade_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TH2.h>
#include <TString.h>
#include <TObjString.h>

#include <map>
#include <vector>
#include <cmath>
#include <iostream>
#include <sstream>
#include <utility>
#include <fstream>
#include <limits>
#include <csignal>
#include <ctime>
#include <TEntryList.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TVector3.h>
#include <TH3.h>
#include <TH2.h>
#include <TH1.h>
#include <TF1.h>
#include <TMath.h>
#include <TObjArray.h>
#include <TRandom.h>
#include <TCutG.h>
#include <TList.h>
#include <TKey.h>
#include <TSysEvtHandler.h>
#include <TSystem.h>
#include <TApplication.h>
#include <deque>
#include "TTreeIndex.h"
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* getenv */
#include <map>
#include <functional>
#include <queue>
#include <vector>

#include "TDatime.h"
#include "nlohmann/json.hpp"
#include <cfloat>

#include "DelilaEvent.h"
#include "HPGeTreeEvent.h"
#include "HPGeSegTreeEvent.h"
#include "LaBrTreeEvent.h"
#include "ElissaTreeEvent.h"

//#include "TObjString.h."
// Headers needed by this particular selector


class DelilaSelectorEliade : public TSelector {
public :
    TTreeReader     fReader;  //!the tree reader
    TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain
    TFile          *foutFile;
    TTree          *outputTree;
    ULong64_t	    fTimeStamp;
    double_t	    fTimeStampFS;//FineTS
    UShort_t	    fEnergyShort;
    UShort_t	    fEnergyLong;

  class TDelilaDetector { 
  public:
    Int_t	 dom;
    int	 ch;//ch daq
//     Int_t	 serial;
    string	 serial;
    Float_t  theta;
    Float_t	 phi;  
//     UShort_t detType;//0 - nothing; 1 - core; 2 - segment; 3 - CeBr; 4 - CsI; 5 - BGO1; 6 - BGO2; 9 - pulser
    int detType;//0 - nothing; 1 - core; 2 - segment; 3 - CeBr; 4 - CsI; 5 - BGO1; 6 - BGO2; 9 - pulser
    Int_t	 TimeOffset;
    double_t bgo_time_offset;
    Int_t 	 threshold; 
    Int_t	 pol_order;
    Int_t    cs_dom;
    int      enable;
    std::vector<float> calibE;
    TDelilaDetector(): dom(-1),phi(-1),theta(-1),TimeOffset(0),calibE(0),threshold(-1),ch(-1),pol_order(-1),enable(1){};
 };
 
//   DelilaEvent     delila_tree_event;
  HPGeTreeEvent     hpge_tree_event;
  HPGeSegTreeEvent  hpge_seg_tree_event;
  LaBrTreeEvent     labr_tree_event;
  ElissaTreeEvent   elissa_tree_event;
  
  DelilaEvent       ev_trg;
  bool              blFirst_event;
  TH1F              *hhGammaSi;
 
  std::vector<HPGeTreeEvent>    *HPGeEvent;
  std::vector<LaBrTreeEvent>    *LabrEvent;
  std::vector<ElissaTreeEvent>  *ElissaEvent;
  std::vector<HPGeSegTreeEvent> *HPGeSegEvent;
  
  std::deque<DelilaEvent>      delilaQu;
  std::deque<DelilaEvent>      delilaQuAddedBack; //filled by AddBack
  std::deque<DelilaEvent>      delilaPreQu;
  
  std::map<int,std::deque<DelilaEvent>> waitingQu_gamma; //for CeBr
  std::map<int,std::deque<DelilaEvent>> waitingQu_bgo; //for CS
  
  //Part for PHA
  short          fSignal[2000]; // add by saka //RecordLength branch is not read we assume rl = 2000
  
//   std::map<unsigned int, TDelilaDetector > LUT_ELIADE;
  std::map<int, TDelilaDetector >       LUT_ELIADE;    
  std::map<int, int >                   LUT_TA;
  std::map<int, double_t >              LUT_TA_COINC;

  DelilaEvent DelilaEvent_;  
//   DelilaEvent DelilaEventTreated ;
  DelilaEvent lastDelilaEvent;  
  DelilaEvent lastEliadeZeroEvent;
  DelilaEvent LastTriggerEvent;
  DelilaEvent LastBunchEvent;    
  
  DelilaEvent PulserEvent;
  DelilaEvent DomainZeroEvent;    
  

  TBranch *b_channel;
  TBranch *b_tstmp;
  TBranch *b_tstmp_fine;
  TBranch *b_energ;  //ChargeLong
  TBranch *b_energ_short;  //ChargeShot
  TBranch *b_mod;
  TBranch *b_signal;
  
  Long64_t nb_entries;
  
  Long64_t event_length;
  Long64_t pre_event_length;
  Long64_t bunch_reset;
  
  UChar_t	        fMod; 
  UChar_t	        fChannel; 
 
  TH1F* hChannelHit;
  TH1F* hDomainHit;
  TH1F* hSegmentHit;
  TH1F* hDetTypeHit;
  TH1F* hChannelHitNotDefined; // channels which are not defined in lut

  std::map<int, TH1F*> hDelila0; //before event_builder
  std::map<int, TH1F*> hDelila_single; //after event builder

  std::map<int, TH1F*> hDelilaCS;
  std::map<int, TH1F*> hDelilaDC;
  std::map<int, TH1F*> hDelilaCS_DC;
  
  TH1F* hLaBrElissa;
  
  std::map<int, TH1F*> hDelila_long;
  std::map<int, TH1F*> hDelilaCS_long;
  std::map<int, TH1F*> hDelilaDC_long;
  std::map<int, TH1F*> hDelilaCS_DC_long;
  
  std::map<int, TH1F*> hAddBack;

  std::map<UInt_t, std::string> detector_name;

  TH1F* hTriggerTrigger;
  TH1F* hBunchBunch;
  TH1F* hNBunch;
  TH1F* hBunchFold;
  TH1F* hTimeInBunch;
  TH1F* hEventsPerTrigger;
  
  
  TH2F* mFoldEnergy;
  
//   TH1F* hBeamCurrent;
  
  TH1F* hCoincID;
  TH1F* hTriggerDomain;
  
  TH1F* hdelilaQu_size;
  TH1F* hdelilaPreQu_size; //number of validated events in preQu

  
  TH2F* mDelila;
  TH2F* mEliade;
  TH2F* mEliadeCores;
  TH2F* mEliade_raw;
//   TH2F* mEliadeCores;
  TH2F* mElissa;
  TH2F* mDelila_raw;
  TH2F* mDelilaCS;
  TH2F* mDelilaDC;//keV
  TH2F* mDelilaCS_DC;//keV
  
  TH2F* mDelila_long;
  TH2F* mDelila_raw_long;
  TH2F* mDelilaCS_long;
  TH2F* mDelilaDC_long;//keV
  TH2F* mDelilaCS_DC_long;//keV

  TH2F* mGammaGammaDC;
//   TH2F* mGammaGammaCS_DC;
  
  TH2F* mEnergyTimeDiff_trigger;
  TH2F* mDomainTimeDiff_trigger;
  TH2F* mDomainTimeDiff_bunch;
  
  std::map<int, TH2F*> mGG;
  std::map<int, TH2F*> mGG_AddBack;
  std::map<int, TH2F*> mGG_theta;
  std::map<int, TH2F*> mGG_DC;
  std::map<int, TH2F*> mGG_time_diff;
  std::map<int, TH2F*> mGG_AB_time_diff;

  std::map<int, TH2F*> mGG_long;
  std::map<int, TH2F*> mGG_DC_long;
  
//   std::map<int, TH2F*> mTimeDiffselected_run_2022_0.root;
  std::map<int, TH1F*> hMult;

  std::map<UInt_t, std::string> gg_coinc_id;
  std::map<int, Float_t> coinc_gates;//in ps

  
  std::map<int, std::string> domain_list;
  std::map<int, TH2F*> mEnergy_time_diff;
   
  std::map<int, TH2F*> mAmaxEnergyDom;

  
  TH2F* mTimeDiffCS;
  
  TH2F* mThetaPhi; 
  TH2F* mGammaGamma;
  TH2F* mTimeDiff_gg;
  TH1F* hMult_gg;
//   TH2F* mGammaGammaCS;
  TH2F* mTimeDiff_gg_CS;
  TH1F* hMult_gg_CS;
  
  TH2F* mLaBr_LabBr_time_diff;

  
  TH2F *mPulser0TimeDiff;
  
  TH1F* hTimeDiffPulser;
  TH2F* mPulserPulser;
  
  TH1F* hTimeZero;
  TH1F* hTimeSort;
    
//   TH2F* mTimeCalibDomain0;
  TH2F* mTimeCalibTrigger;
  TH2F* mTimeCalibTriggerCores;
  TH2F* mTimeCalibInsideEvent;
  TH2F* mTimeCalibInsideEventCores;
  TH2F* mTimeCalibCoincCoinc;
  
//   TH2F* mTimeCalib;
  TH2F* mTimeCalibBGO;
  TH2F* mTimeCalibBGO_cs_dom;
  
  TH2F* mNeutron;
  TH2F* mShortLong;
  TH2F* mNN_TimeDiff;
//   TH2F* mNN_rings;
  TH2F* mNN_TimeDiff_counter;//time_diff withint the same detector
  TH1F* hNN_Mult;
  TH1F* hNN_fired;//fired more than once
  TH1F* hNN_ring;
  std::map<int, double> last_neutron_det;
  std::map<int, int> CounterIsFired;
  
  std::map<int, std::vector<Float_t>> mapSeaTable;

//   TH2F* mTimeCalibDomain0;
  
  TH1F *hAmax;
  TH2F *mAmaxEnergy;

  int det_def_trg;
  int channel_trg;
  std::vector<int> trigger_domains;
  double TriggerTimeFlag;
  double lastDelilaTime;
  
  bool blCS;
  bool blGammaGamma;
  bool blTimeAlignement;
  bool blAddBack;
  double beta;

  //options of the selector
  int addBackMode;
  TString RunID;
  TString VolID;
  TString ServerID;
  
  
  std::vector<int>     ListOfCores;
  std::map<int, TH1F*> hCoreFold;
//   std::map<int, TH2F*> mCoreCore;//add back core core
  std::map<int, TH2F*> mCoreSegments;//spectra: core0-seg1..8
  std::map<int, TH2F*> mGGCoreSegments;//core-seg1..8
  std::map<int, TH2F*> mFoldSpec;//Crystal;
  std::map<int, TH2F*> mFoldSpecSum;//summing all energies inside the gate for add back
  std::map<int, TH2F*> mFoldSpecCS;//Crystal;
//   std::map<int, TH2F*> mFoldSpecClover;
  std::map<int, TH2F*> mTimeDiffCoreCore;
  std::map<int, TH2F*> mTimeDiffCoreSegments;
  std::map<int, TH2F*> mTimeDiffCoreSegmentsCS;
  
  std::map<int, TH2F*> mCoreACS;//spectra acs: core0-bgo1...10
  std::map<int, TH2F*> mSegACS;//spectra acs: seg0-bgo1...10
  std::map<int, TH2F*> mCoreSpecACS;//spectra core: core0-bgo1...10
  std::map<int, TH2F*> mSegSpecACS;//spectra seg: seg0-bgo1...10
  TH2F* mSingleCore;
  TH2F* mSingleCoreCS;
  std::map<int, TH2F*> mTimeDiffCoreACS;
  std::map<int, TH2F*> mTimeDiffSegACS;
  std::map<int, TH1F*> hACSFold;
  std::map<int, TH2F*> mAcsFold;
  std::map<int, TH2F*> mAcsFoldSeg;
  
  std::map<int, TH2F*> mDee;
  TH2F* mDeeTimeDiff;


    
  std::clock_t start;
  double duration;
 
  ULong64_t nevents;
//   ULong64_t nevents_reset;
  int reset_counter;
 
  ULong64_t lastTime;
  
  TObjArray *toks;

   DelilaSelectorEliade(TTree * /*tree*/ =0) { }
   virtual ~DelilaSelectorEliade() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();
   virtual Long64_t GetEntries() { return fChain ? fChain->GetEntries() : 0;}

   virtual void  Read_ELIADE_LookUpTable();
   virtual void  Read_SeaTable();
   virtual void  Read_ELIADE_JSONLookUpTable();
   virtual void  Read_TimeAlignment_LookUpTable();
   virtual void  Read_CoincCoinc_TimeAlignment_LookUpTable();
   virtual void  Read_Confs();
   virtual void  Print_ELIADE_LookUpTable();
   virtual void  Print_TimeAlignment_LookUpTable();
   virtual void  Print_CoincCoinc_TimeAlignment_LookUpTable();

   virtual float CalibDet(float,int);
   virtual int GetCoincTimeCorrection(int dom1, int dom2);
   virtual void cs();
   
   virtual void TreatDelilaEvent();
   virtual void TreatFold(int det); 
   virtual int  GetCoincID(int dom1, int dom2);
   virtual int  GetCoinc_det_def(int det_def1, int det_def2);
   virtual void CheckPulserAllignement(int zero_dom);
   virtual void PrintDelilaEvent(DelilaEvent &ev_);
   
   virtual void FillOutputTree();
   virtual void FillSingleSpectra();
   virtual void TimeAlignementTrigger();
   virtual void TimeAlignementInsideEvent();//
   virtual void TimeAlignementCoincCoinc();//based on pairs det-det coinc
   
   
   virtual void EventBuilderSimple();
   virtual void EventBuilderPreTrigger();
   virtual void EventBuilder();//many conditions
   virtual bool TriggerDecision();
   virtual void SetUpNewTriggerSimple();
   virtual void SetUpNewTrigger();
   virtual void CheckPreQu();
   virtual void MovePreQu2Qu();

   
   virtual void TreatLaBrSingle();
   virtual void TreatHpGeSingle();
   virtual void TreatHPGeSegmentSingle();
   virtual void TreatNeutronSingle();
   virtual void TreatNeutronNeutron();
   virtual void TreatNeutronSingle3He();
   virtual void TreatBeamCurrent();//not yet implemented 

   virtual void TreatElissaSingle();
   virtual void TreatElissaSinglePSA();
   virtual void TreatBGOSingle();
   virtual void TreatACS();
   
   virtual void TreatGammaGammaCoinc();
   virtual void TreatSolarLaBrCoinc();
   
   virtual void ViewAddBackCoreCore();
   virtual void ViewAddBackCrystal();
   virtual void ViewAddBackDetector();
   virtual void ViewAddBackDetectorCS();
   virtual void ViewACS();//for cores
   virtual void ViewACS_segments(); //for segments
   
   virtual void ViewDeE();
   
//    virtual void GetNMultiplicity(TH1 *hh, int nn_max = 15);
   virtual void GetNMultiplicity();
   
   virtual void SimpleRun(); //to run without EventBuilder

   
   virtual std::vector<float> trapezoidal(short wave[],int length, int L, int G);//L = 20; G = 0
   
   std::map<std::string, bool> has_detector;


   ClassDef(DelilaSelectorEliade,0);

  
};

#endif

#ifdef DelilaSelectorEliade_cxx
void DelilaSelectorEliade::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).
  
  if(!tree) {std::cout<<" TTree NOT found "<<std::endl; return;};
  
//   std::cout<<" Delila Sorting "<<std::endl;
  std::cout<<" TTree found "<<std::endl;
  fChain = tree;
  fChain->SetMakeClass(1);  
  fChain->SetBranchAddress("ChargeLong", 	&fEnergyLong,            	   &b_energ);
  fChain->SetBranchAddress("ChargeShort", 	&fEnergyShort, 	               &b_energ_short);
  fChain->SetBranchAddress("FineTS", 	    &fTimeStampFS, 	               &b_tstmp_fine);
  fChain->SetBranchAddress("TimeStamp",     &fTimeStamp,                   &b_tstmp); 
  fChain->SetBranchAddress("Ch", 	        &fChannel,                     &b_channel);
  fChain->SetBranchAddress("Mod", 	        &fMod,                         &b_mod);
  fChain->SetBranchAddress("Signal",        &fSignal,                      &b_signal);// add by saka 
  
  fReader.SetTree(tree);
       
   
  foutFile->cd();
  outputTree = new TTree("SelectedDelila","SelectedDelila");
  

  lastDelilaTime = 0;
  
//   Read_ELIADE_LookUpTable();
//   Read_TimeAlignment_LookUpTable();
  
//   Print_ELIADE_LookUpTable();
  
  if (has_detector["HPGe"]){
      HPGeEvent= new std::vector<HPGeTreeEvent>;
      outputTree->Branch("HPGeEvents",&HPGeEvent);
    };
    
  if (has_detector["SEG"]){
      HPGeSegEvent= new std::vector<HPGeSegTreeEvent>;
      outputTree->Branch("HPGeSegEvents",&HPGeSegEvent);
    };
  
  if (has_detector["LaBr"]){
      LabrEvent= new std::vector<LaBrTreeEvent>;
      outputTree->Branch("LaBrEvents",&LabrEvent);
    };
  
  if (has_detector["Elissa"]){
      ElissaEvent= new std::vector<ElissaTreeEvent>;
      outputTree->Branch("ElissaEvents",&ElissaEvent);
  };
  
  
  
  
  
  std::cout<<" === Present detectors === \n";
  
   std::map<std::string, bool> ::iterator it_has_=has_detector.begin();
   for (; it_has_!= has_detector.end();++it_has_){
       if (has_detector[it_has_->first]) std::cout<< it_has_->first<< "  enabled  \n";
  };

  
  
//    std::cout<<" HPGe     " << has_detector["HPGe"] <<"  \n";
//    std::cout<<" HPGeSeg  " << has_detector["SEG"] <<"  \n";
//    std::cout<<" LaBr     " << has_detector["LaBr"] <<"  \n";
//    std::cout<<" CsI      " << has_detector["CsI"] <<"  \n";
//    std::cout<<" BGOs     " << has_detector["BGOs"] <<"  \n";
//    std::cout<<" BGOf     " << has_detector["BGOf"] <<"  \n";
//    std::cout<<" Elissa   " << has_detector["Elissa"] <<" \n";
//    std::cout<<" neutron   " << has_detector["neutron"] <<" \n";
//    std::cout<<" neutronTN   " << has_detector["neutronTN"] <<" \n";
  
  std::cout<<" === Settings === \n";
  
  std::cout<<" AddBack option: "<< addBackMode <<" \n";
  std::cout<<" Beta: "<< beta <<" \n";
  
  
    
  std::cout<<" Trigger: ";
  if (det_def_trg == -1){std::cout<<" No trigger \n";}
//   else if (det_def_trg == 0){std::cout<<" Domain: "<<detector_name[channel_trg]<< "\n";}//I am not sure one should comment, i will see how it goes
  else if (det_def_trg == 0 && !trigger_domains.empty()){
            std::cout<<" Domain(s): ";
            std::vector<int> ::iterator it_trg_dom_ = trigger_domains.begin();
            for (;it_trg_dom_!=trigger_domains.end(); ++it_trg_dom_)
              {
                std::cout<<*it_trg_dom_<<" "; 
             }
             std::cout<<"\n";
      }
  else if (det_def_trg > 0){std::cout<<" any of "<<" "<<detector_name[det_def_trg]<< " detectors ("<< det_def_trg<<") \n";}
  else {std::cout<<" trigger is not set correctly \n";};
  
  blCS = false;
  if (coinc_gates.find(15) != coinc_gates.end()){blCS = true;};
  if (coinc_gates.find(35) != coinc_gates.end()){blCS = true;};
  if (blCS && (has_detector["BGOs"] || has_detector["BGOf"] || has_detector["CsI"] ) ) {std:cout<<" Compton Supression: enabled \n";}
  else {cout<<" Compton Supression: disabled \n"; blCS = false;};
  
  
  
  blGammaGamma = false;
  if (coinc_gates.find(11) != coinc_gates.end() && has_detector["HPGe"]){blGammaGamma = true;};
  if (coinc_gates.find(13) != coinc_gates.end() && has_detector["HPGe"] && has_detector["LaBr"]){blGammaGamma = true;};
  if (coinc_gates.find(33) != coinc_gates.end() && has_detector["LaBr"]){blGammaGamma = true;};
  if (coinc_gates.find(37) != coinc_gates.end() && has_detector["Elissa"] && has_detector["LaBr"]){blGammaGamma = true;};
  
  if (blGammaGamma) {cout<<" GammaGamma: enabled \n";}
  else {cout<<" GammaGamma: disabled \n";};
  
  if (blTimeAlignement) {cout<<" Time Alignement matrix: enabled \n";}
  
  std::cout<<" === Time settings ps === \n";
  std::map<int, Float_t> ::iterator itcc_ = coinc_gates.begin();
  for (; itcc_ != coinc_gates.end(); ++itcc_) {
     std::cout<<" coin_id " << itcc_->first <<" "<< itcc_->second <<" ps \n";
  }
  std::cout<<" event_length     " << event_length      <<" ps \n";
  std::cout<<" pre_event_length " << pre_event_length  <<" ps \n";
  
  
  std::cout<<" ===                   === \n";
  
//   blAddBack  = false;
//   if (addBackMode > 0){
//     blAddBack  = true;
//     std::cout<<"Making List of Cores from LUT_ELIADE.dat \n";
//     std::map<int, TDelilaDetector > ::iterator it__ = LUT_ELIADE.begin();
//     for (; it__ != LUT_ELIADE.end(); ++it__) {
//         if (LUT_ELIADE[it__->first].detType == 1){
//              int core_id = LUT_ELIADE[it__->first].dom/100 * 10 +LUT_ELIADE[it__->first].dom/10%10;
//              std::cout<<core_id<<" ";
//              ListOfCores.push_back(core_id);
// //           std::cout<<LUT_ELIADE[it__->first].dom<<" "<<LUT_ELIADE[it__->first].dom/100 * 10 +LUT_ELIADE[it__->first].dom/10%10 <<std::endl;
//         };
//    };
//    std::cout<<" \n";
// 
//  };
  
  
}

Bool_t DelilaSelectorEliade::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef DelilaSelectorEliade_cxx
