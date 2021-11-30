//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Mar 23 13:02:03 2021 by ROOT version 6.22/06
// from TTree EliadeSorting/Energy Station
// found on file: run1005_03.root
//////////////////////////////////////////////////////////

#pragma once

#ifndef EliadeSorting_h
#define EliadeSorting_h

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

//#include "TObjString.h."
// Headers needed by this particular selector


class EliadeSorting : public TSelector {
public :
    TTreeReader     fReader;  //!the tree reader
    TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain
    TTree          *outputTree;
    TFile          *outputFile;
   // Readers to access the data (delete the ones you do not need).
  // TTreeReaderValue<UChar_t> fMod = {fReader, "Mod"};
   //TTreeReaderValue<UChar_t> fCh = {fReader, "Ch"};
//   TTreeReaderValue<ULong64_t> fTimeStamp = {fReader, "TimeStamp"};
  /* TTreeReaderValue<UShort_t> fEnergy = {fReader, "Energy"};
   TTreeReaderValue<UInt_t> fRecordLength = {fReader, "RecordLength"};
   TTreeReaderArray<UShort_t> fTrace1 = {fReader, "Signal"};
*/

  UChar_t	 uMod; 
  UChar_t	 uChannel; 

 class TEliadeEvent {
 public:
    UChar_t	        fMod; 
    UChar_t	        fChannel;    
    ULong64_t	    fTimeStamp;
    UShort_t	    fEnergy;  
    UShort_t        det_def;//0 - nothing; 1 - core; 2 - segment; 3 - CeBr; 4 - CsI; 5 - BGO1; 6 - BGO2; 9 - pulser
    float	        EnergyCal;
    UShort_t        domain;
    UShort_t        cs_domain;
    UShort_t        channel;//ch daq
    UShort_t        core;
    UShort_t        segment;
    UShort_t        CS;//0 - no; 1 - yes

    //int make_board_ID(){return fMod*100}
    TEliadeEvent(): domain(-1),channel(-1),fTimeStamp(0),fEnergy(-1),CS(0),cs_domain(0){};
 };

  class TEliadeDetector { 
  public:
    Int_t	 dom;
    Int_t	 ch;//ch daq
    Float_t  theta;
    Float_t	 phi;  
    UShort_t detType;//0 - nothing; 1 - core; 2 - segment; 3 - CeBr; 4 - CsI; 5 - BGO1; 6 - BGO2; 9 - pulser
    Int_t	 TimeOffset; 
    Int_t 	 upperThreshold; 
    Int_t	 pol_order;
    Int_t    cs_dom;
    std::vector<float> calibE;
    TEliadeDetector(): dom(-1),phi(-1),theta(-1),TimeOffset(0),calibE(0),upperThreshold(-1),ch(-1),pol_order(-1){};
 };

  struct TEliadeEventCoinc {
      TEliadeEvent LastEvent;
      bool coinc;
      int mult;
 };
 
  std::deque<TEliadeEvent> eliadeQu;
  std::deque<TEliadeEvent> coincQu_pulser;
  std::deque<TEliadeEvent> coincQu_cores;
  std::deque<TEliadeEvent> coincQu_segments;
  std::deque<TEliadeEvent> bgo_Qu;
  std::deque<float> enrergyQu;
  
   std::map<int,std::deque<TEliadeEvent>> waitingQu;

//   std::deque<TEliadeEvent> eliadeQu_sorted;
  std::map<unsigned int, TEliadeDetector > LUT_ELIADE  ;
  std::map<unsigned int, std::deque<TEliadeEvent> > coincQu_seg ;

 
 
  TEliadeEvent EliadeEvent;  
  TEliadeEvent EliadeEventCS;
  TEliadeEvent lastEliadeEvent;  
  TEliadeEvent lastEliadeZeroEvent;  
  TEliadeEventCoinc EliadeCoincEvent[4];
  
  TEliadeEvent startEventCore;  
  TEliadeEvent startEventSegment;  

  TBranch *b_channel;
  TBranch *b_tstmp;
  TBranch *b_energ;  
  TBranch *b_mod;  
  
  Long64_t nb_entries;
  
 // std::map<UInt_t,TH1F*> hEnergy_raw;
  //std::map<UInt_t,TH1F*> hEnergy_cal;
 
  std::map<UInt_t, TEliadeEvent> last_board_event;
 
  TH1F* hChannelHit;
  TH1F* hDomainHit;
  TH1F* hSegmentHit;
  TH1F* hDetTypeHit;
  TH2F* mEliade;//keV
  TH2F* mEliadeCS;//keV
  TH1F* hEliade;//keV
  TH1F* hEliadeCS;//keV
  TH2F* mEliade_raw;
  TH2F* mCores;//keV
  TH2F* mSegments;//keV
  TH2F* mEliadeTD;
  
  //Part For RoSPHERE
  TH2F* mLaBr_raw;  
  TH2F* mLaBr_kev;
  TH1F* hLaBr_kev;
  TH1F* hLaBrCS_kev;
  
  
  
//  TH2F* mEliadeSegEnergy;
  TH2F* mEliadeMULT;
  TH1F* hTimeSort;
  TH1F* hTimeZero;
  TH1F* hEliade_no_addback;
  TH1F* hCheckCore2AddBack;
  TH1F* hCoreHit;
  TH2F* mBoardTimeDiff;
  TH2F* mZeroTimeDiff_vs_Enegy;
  TH2F* mSegmentsPerCore;
  
  TH2F *mDomTimeDiff;
  TH2F *mPulser0TimeDiff;
  TH2F *mDom0TimeDiff;
  TH2F *mDom0TimeDiffEnergy;
  
  TH1F* hTimeDiffCoreCore;//Eg-Eg between cores; trigger any core
  TH1F* hTimeDiffCeBrCebr;//Eg-Eg between cores; trigger any core
  TH2F* mCoreCore;
  TH1F* hMultCores;
  
  TH1F* hTimeDiffSegSeg;//Eg-Eg between segments; trigger any core
  TH2F* mSegmentSegment;
  TH1F* hMultSegments;
  TH1F* hTimeDiffBGOCeBr;
  
  TH1F* hTimeDiffPulser;
  TH2F* mPulserPulser;
  
  TH2F* mTimeDiffCS;

  
  
  
    
  std::clock_t start;
  double duration;
 
  ULong64_t nevents;
  ULong64_t nevents_reset;
  int reset_counter;
  
  TObjArray *toks;

   EliadeSorting(TTree * /*tree*/ =0) { }
   virtual ~EliadeSorting() { }
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

   virtual void  CheckCoincInCrystal(TEliadeEvent ev_);
   virtual void  Read_ELIADE_LookUpTable();
   virtual void  Print_ELIADE_LookUpTable();
   virtual float CalibDet(float,int);
   virtual void PrioritySorting(TEliadeEvent ev_);
   virtual void CheckSorting(std::deque<TEliadeEvent> myQu);
   virtual void AddBack();
   virtual void AddBackCoreAB();//0 and 1
   virtual void AddBackCoreBC();//1 and 2
   virtual void AddBackCoreCD();//2 and 3
   virtual void AddBackCoreDA();//3 and 0
   virtual int CheckTimeAlignment(int to_domain);
   virtual int  CoreSegmentHitID(std::deque<TEliadeEvent>, int coincID);
   

   ClassDef(EliadeSorting,0);

};

#endif

#ifdef EliadeSorting_cxx
void EliadeSorting::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
   
   
  outputFile->cd();
  outputTree = new TTree("SelectedDelila","SelectedDelila");
  outputTree->Branch("fTEventTS",&EliadeEventCS.fTimeStamp,"TimeStamp/l");
  outputTree->Branch("fEnergy",&EliadeEventCS.EnergyCal,"Energy/F");
  outputTree->Branch("fDomain",&EliadeEventCS.domain,"Domain/b");
  outputTree->Branch("fDetType",&EliadeEventCS.det_def,"def/b");
  outputTree->Branch("fCS",&EliadeEventCS.CS,"CS/b");
}

Bool_t EliadeSorting::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef EliadeSorting_cxx
// R G // 2 1
// W B // 3 0
// B0 G1 R2 W3
