//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Mar 23 13:02:03 2021 by ROOT version 6.22/06
// from TTree EliadeSorting/Energy Station
// found on file: run1005_03.root
//////////////////////////////////////////////////////////

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


//#include "TObjString.h."
// Headers needed by this particular selector


class EliadeSorting : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

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
    UChar_t	 fMod; 
    UChar_t	 fChannel;    
    ULong64_t	 fTimeStamp;
    UShort_t	 fEnergy;  
    float	 EnergyCal;
    int	 domain;
    int	 channel;//ch daq
    //int make_board_ID(){return fMod*100}
    TEliadeEvent(): domain(-1),channel(-1),fTimeStamp(0),fEnergy(-1){};
 };

  class TEliadeDetector { 
  public:
    Int_t	 dom;
    Int_t	 ch;//ch daq
    Float_t 	 theta;
    Float_t	 phi;  
    Int_t	 TimeOffset; 
    Int_t 	 upperThreshold; 
    Int_t	 pol_order;
    std::vector<float> calibE;
    TEliadeDetector(): dom(-1),phi(-1),theta(-1),TimeOffset(0),calibE(0),upperThreshold(-1),ch(-1),pol_order(-1){};
 };

  struct TEliadeEventCoinc {
      TEliadeEvent LastEvent;
      bool coinc;
      int mult;
 };
 
  std::deque<TEliadeEvent> eliadeQu;
  std::deque<TEliadeEvent> eliadeQu_sorted;
  std::map<unsigned int, TEliadeDetector > LUT_ELIADE  ;
 
 
  TEliadeEvent EliadeEvent;  
  TEliadeEvent lastEliadeEvent;  
  TEliadeEvent lastEliadeZeroEvent;  
  TEliadeEventCoinc EliadeCoincEvent[4];

  TBranch *b_channel;
  TBranch *b_tstmp;
  TBranch *b_energ;  
  TBranch *b_mod;  
  
  Long64_t nb_entries;
  
 // std::map<UInt_t,TH1F*> hEnergy_raw;
  //std::map<UInt_t,TH1F*> hEnergy_cal;
 
  std::map<UInt_t, TEliadeEvent> last_board_event;
 
  TH1F* hHitPattern;
  TH2F* mEliade;
  TH2F* mEliade_raw;
  TH2F* mCores;
  TH2F* mSegments;
  TH2F* mEliadeTD;
  TH2F* mEliadeMULT;
  TH1F* hTimeSort;
  TH1F* hTimeZero;
  TH1F* hTest;
  TH2F* mBoardTimeDiff;
  TH2F* mZeroTimeDiff;
  TH2F* mZeroTimeDiff_vs_Enegy;
   
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
