#define EliadeSorting_cxx
// The class definition in data.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after hTimeDiffCoreCoreBegin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("EliadeSorting.C")
// root> T->Process("EliadeSorting.C","some options")
// root> T->Process("EliadeSorting.C+")
//


#include "EliadeSorting.h"
#include <TH2.h>
#include <TStyle.h>
#include <TString.h>
#include <TObjString.h>
#include <unordered_set>
#include <iomanip>      // std::setw
using namespace std;


int addBackMode = 1; //0 - no addback; 1- addback
bool Trigger = false;

//TString LUT_Directory = "/data/live/IT/dsoft/EliadeSorting/";
//TString LUT_Directory = "/home/eliade/EliadeSorting/";

// TString LUT_Directory = "/home/eliade/EliadeSorting/";
//TString LUT_Directory = "/home/testov/EliadeSorting/";
//TString LUT_Directory = "/home/work/EliadeSorting/";
//TString LUT_Directory = "~/EliadeSorting/";


bool debug = false;

const int dom_nbr = 1;
const int current_clover = 1;
const int nbr_of_boards = 8;
const int nbr_of_ch = 200;
// const int zero_channel = 101; //for time allignement
ULong64_t lastTime_pulser = 0;
ULong64_t lastTime_dom0 = 0;
ULong64_t lastTimeStamp = 0;

// std::unordered_set<int> cores = { 109,119,129,139,171,172};
std::unordered_set<int> pulsers = { 150,151,152,153};
std::unordered_set<int> crystal2mask = {1,3,5};
//std::unordered_set<int> frontsegments = {};
//std::unordered_set<int> cores = { 101,111,121};

void EliadeSorting::Read_ELIADE_LookUpTable() {
  std::cout << "I am Reading ELIADE LookUpTable ... ";
//  std::stringstream CUTFile;
//  CUTFile << CUTG_Directory.Data() << "cut_EeEw_galileo.root";
//  TFile *file_EeEw = TFile::Open(CUTFile.str().c_str());

 char* pLUT_Path;
  pLUT_Path = getenv ("ELIADE_LUT");
  if (pLUT_Path!=NULL)
    printf ("The LookUpTable path is: %s \n",pLUT_Path);


  std::stringstream LUTFile;
  LUTFile << pLUT_Path <<"/"<<"LUT_ELIADE.dat";
  //LUTFile << LUT_Directory << "LUT_ELIADE.dat";
  const int nbr_of_ch = 200;
  std::ifstream lookuptable(LUTFile.str().c_str());

  if (!lookuptable.good()) {
    std::ostringstream os;
    os << "Could not open " << LUTFile.str().c_str()
       << " and I need it ;(\n";
    Abort(os.str().c_str());
  } else {
    while (lookuptable.good()) {
      std::string oneline;
      std::getline(lookuptable, oneline);
      if (!lookuptable.good()) continue;
      if (oneline[0] == '#') continue; // ignore lines stating with #
      if (oneline.empty())   continue; // ignore empty lines
      TEliadeDetector curDet;
      Float_t theta(-1.), phi(-1.);
      int upperThreshold = 1e6;
      std::istringstream is(oneline);
      if (debug) std::cout << is.str().c_str() << std::endl;
//       is >> curDet.ch >> curDet.dom >> curDet.theta >> curDet.phi >> curDet.TimeOffset >> curDet.upperThreshold;
      is >> curDet.ch >> curDet.dom >> curDet.detType >> curDet.phi >> curDet.TimeOffset >> curDet.upperThreshold;
    //  std::cout<<" curDet.ch  "<<curDet.ch <<" curDet.TimeOffset " <<curDet.TimeOffset<<std::endl;
      
      if (curDet.ch >= 0) {
	//theta *= TMath::DegToRad();
	//phi *= TMath::DegToRad();
//	TVector3 DetPos;
//	curDet.direction.SetMagThetaPhi(210, theta, phi);
	int pol_order = 0;
	//Now we sorted_run_354.roottry to get the EeEw selection with a simple line
	float offset_gate(0.),slope_gate(1.);
	is >> offset_gate >> slope_gate;
	//curDet.rejectionEeEw = new TF1(Form("Ge_%2i_EeEw",curDet.domain),
	//			       "pol1");
	//curDet.rejectionEeEw->FixParameter(0,offset_gate);
	//curDet.rejectionEeEw->FixParameter(1,slope_gate);
	is >> pol_order;
	curDet.pol_order = pol_order;
	if (debug) std::cout << "Cal order " << pol_order << "  ";
	std::vector<float> DetCal_sub0;
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
  }
  lookuptable.close();
  std::cout << " done" << std::endl;
  //  std::exit(1);
}


void EliadeSorting::PrioritySorting(TEliadeEvent ev_)
{
   
   if (eliadeQu.empty()){eliadeQu.push_back(ev_);return;};  
   std::deque<TEliadeEvent>  ::iterator it__ = eliadeQu.begin();

   for (; it__ != eliadeQu.end(); ++it__) {
   	if (ev_.fTimeStamp >= (*it__).fTimeStamp ) {;}
   	else break;
   };
  it__ = eliadeQu.insert(it__++, ev_);
  return;
};

void EliadeSorting::CheckSorting(std::deque<TEliadeEvent> myQu)
{
 if (eliadeQu.empty()){return;};
 bool debug_check_sorting = false;
 std::cout<<"I will start CheckSorting ...\n";
 std::deque<TEliadeEvent>  ::iterator it__ = myQu.begin();
 std::deque<TEliadeEvent>  ::iterator it_last__ = myQu.begin();
 it__++;
 
 int time_diff_ev = 0;
 
 for (; it__ != myQu.end(); ++it__) {
//   	if ((*it__).fTimeStamp < (*it_last__).fTimeStamp ) {std::cout<<" (*it__).fTimeStamp < (*it_last__).fTimeStamp is wrong" << std::endl;}
        time_diff_ev = (*it__).fTimeStamp - (*it_last__).fTimeStamp ;
	it_last__ = it__;
	hTimeSort->Fill(time_diff_ev);
	if ( debug_check_sorting) {std::cout<<(*it__).fTimeStamp<<" "<<(*it_last__).fTimeStamp<<" "<<time_diff_ev<<std::endl;};
	// std::cout<<" check " <<(*it__).fTimeStamp<<" "<<std::endl;
   };
  // std::cout<<"check" <<(*it__).fTimeStamp<<" "<<std::endl;
 //  std::cout<<(*it__).fTimeStamp<<" "<<(*it_last__).fTimeStamp<<std::endl;
	return;
}

void EliadeSorting::Print_ELIADE_LookUpTable()
{
    std::cout<<"Print_ELIADE_LookUpTable \n";		
    std::map<unsigned int, TEliadeDetector > ::iterator it__ = LUT_ELIADE.begin();
    for (; it__ != LUT_ELIADE.end(); ++it__) {
     // is >> curDet.ch >> curDet.dom >> theta >> phi >> curDet.TimeOffset >> curDet.upperThreshold;
	std::cout<<" Ch "<<LUT_ELIADE[it__->first].ch<<" Dom "<< LUT_ELIADE[it__->first].dom<<" "<< LUT_ELIADE[it__->first].theta<<" "<< LUT_ELIADE[it__->first].phi <<" offset "<< LUT_ELIADE[it__->first].TimeOffset<<" Thr "<< LUT_ELIADE[it__->first].upperThreshold<<" "<<LUT_ELIADE[it__->first].pol_order <<std::endl;
    }
};



float EliadeSorting::CalibDet(float val, int ch)
{

 Float_t Randomize = 0.;
 std::vector<float> cal_par;
 cal_par = LUT_ELIADE[ch].calibE;  
 float cal_value = 0.;
 float flou = (float)rand () / RAND_MAX - 0.5; 
  for (UInt_t j = 0; j < cal_par.size(); j++) {
    //    std::cout << j <<"  " << cal_par[j] ;
    //cal_value += cal_par[j] * TMath::Power(val + Randomize, (Int_t) j);
    cal_value += cal_par[j] * TMath::Power(val + flou, (Int_t) j);
   // if (ch==102){std::cout<<cal_par[0]<<" "<<cal_par[1]<<" "<<std::endl;}
  }
 //if (domain == 103){
  //std::cout <<" val "<<val << " cal_value " << cal_value <<std::endl;
 // cal_value = -0.037564457391682+ val* 3.86683661082787;
  //}
  return cal_value;

};


void EliadeSorting::CheckCoincInCrystal(TEliadeEvent ev_)
{


 if ((ev_.domain == 102)||(ev_.domain == 112)||(ev_.domain == 122)||(ev_.domain == 132))
 {
	mEliadeMULT->Fill(ev_.fMod, EliadeCoincEvent[ev_.fMod].mult);
 	EliadeCoincEvent[ev_.fMod].coinc = true;
	EliadeCoincEvent[ev_.fMod].mult = 0;
	EliadeCoincEvent[ev_.fMod].LastEvent = ev_;
 }
 else if (EliadeCoincEvent[ev_.fMod].coinc) 
 {	 	
 	int time_diff = ev_.fTimeStamp -  EliadeCoincEvent[ev_.fMod].LastEvent.fTimeStamp;
 	if (time_diff < 30)
 	{
 	 EliadeCoincEvent[ev_.fMod].mult++;
 	 mEliadeTD->Fill(ev_.fMod,time_diff);
 	}
 	else  	EliadeCoincEvent[ev_.fMod].coinc = false; 	
 }
 return;
};

void EliadeSorting::Begin(TTree * tree)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

  
  TString option = GetOption();
  toks = option.Tokenize(",");
  TString RunID = ((TObjString*) toks->At(0))->GetString();
  addBackMode = atoi(((TObjString*) toks->At(2))->GetString());
  std::cout << "addBackMode  " << addBackMode <<std::endl;

//   TString VolID = ((TObjString*) toks->At(1))->GetString();
// 
//   std::stringstream OutputFile;
//   OutputFile << "selectror_run" << "_" << RunID <<"_"<<VolID<< ".root";
//   std::cout << "OUTFILE  run" << "_" << RunID<<"_"<<VolID<< ".root"<<std::endl;
  
   if(!tree) {std::cout<<" TTree NOT found "<<std::endl; return;};
  
  std::cout<<" TTree found "<<std::endl;
  fChain = tree;
  fChain->SetMakeClass(1);
  
  fChain->SetBranchAddress("Energy", 	&EliadeEvent.fEnergy, 	&b_energ);
//  fChain->SetBranchAddress("Ch", 	&uChannel,  &b_channel);
//  fChain->SetBranchAddress("Mod", 	&uMod,  &b_mod);
  fChain->SetBranchAddress("TimeStamp",&EliadeEvent.fTimeStamp,&b_tstmp); 
  fChain->SetBranchAddress("Ch", 	&EliadeEvent.fChannel,  &b_channel);
  fChain->SetBranchAddress("Mod", 	&EliadeEvent.fMod,  &b_mod);
  
  
  
  
  auto index = new TTreeIndex(fChain,"TimeStamp", "Ch");
  fChain->SetTreeIndex(index);
  ULong64_t lastStamp = 0;
  ULong64_t timeStamp = 0;
  const auto nEvents = fChain->GetEntries();

  for(auto iEve = 0; iEve < nEvents; iEve++) {
    auto local = fChain->GetEntryNumberWithIndex(iEve);
    fChain->GetEntry(local);

    if(lastStamp > timeStamp){
      std::cout << "hit: " << lastStamp <<"\t"<< timeStamp << std::endl;
    }
    //std::cout << "hit: " << timeStamp  << std::endl;
    lastStamp = timeStamp;
  }


  Read_ELIADE_LookUpTable();
  Print_ELIADE_LookUpTable();

//  EliadeEvent.fChannel = uChannel;
 // EliadeEvent.fMod = uMod;
  //uMod = 0;
  
  
   
}




void EliadeSorting::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

 nevents = 0;
 nevents_reset=0;
 reset_counter = 0;
 if ( !eliadeQu.empty()) 
 {
    eliadeQu.clear();
 }


   TString option = GetOption();
   int ch_num = 0;
   for (int i = 1; i<=dom_nbr;i++){   
  	    for (int j = 0; j<=4; j++){
  	 	for (int k = 0; k<=9; k++){//board number
  		  //ch_num = i*100+j*10+k;
  		  ch_num = 100*current_clover+j*10+k;
	   	  //std::cout<<"ch_num = "<<ch_num<<" i= "<<i<<" j= "<<k<<" ch= "<<k<<std::endl;
		 // hEnergy_raw[ch_num] = new TH1F(Form("hEnergy_raw_dom_%i_ch_%i",i,ch_num), Form("hEnergy_raw_dom_%i_%i",i,ch_num), 16384, -0.5, 16383.5);
		  //fOutput->Add(hEnergy_raw[j]);        	
	   	};     
	   };
 	};
 
   // for (int i=0;i<=3;i++){EliadeCoincEvent[0].fMod = 999};
   
   hCheckCore2AddBack = new TH1F("hCheckCore2AddBack", "hCheckCore2AddBack",40,0,40);
   fOutput->Add(hCheckCore2AddBack);
 	
   hChannelHit = new TH1F("hChannelHit", "hChannelHit",3216,0,3216);
   fOutput->Add(hChannelHit);
   
   hSegmentHit = new TH1F("hSegmentHit", "hSegmentHit",20,0,20);
   fOutput->Add(hSegmentHit);
   
   hDomainHit = new TH1F("hDomainHit", "hDomainHit",200,0,200);
   fOutput->Add(hDomainHit);
   
   hCoreHit = new TH1F("hCoreHit", "hCoreHit",10,0,10);
   fOutput->Add(hCoreHit);
   
   hDetTypeHit = new TH1F("hDetTypeHit", "hDetTypeHit",200,0,200);
   fOutput->Add(hDetTypeHit);
     
   hEliade_no_addback = new TH1F("hEliade_no_addback", "hEliade_no_addback", 4096, -0.5, 4095.5);
   fOutput->Add(hEliade_no_addback);
   	
   hEliade = new TH1F("hEliade", "hEliade", 4096, -0.5, 4095.5);
   fOutput->Add(hEliade);
   
   hCeBr = new TH1F("hCeBr", "hCeBr", 4096, -0.5, 4095.5);
   fOutput->Add(hCeBr);
   
   hCeBr_CS = new TH1F("hCeBr_CS", "hCeBr_CS", 4096, -0.5, 4095.5);
   fOutput->Add(hCeBr_CS);
   
   hTimeSort = new TH1F("hTimeSort", "hTimeSort", 1000, -500, 500);
   fOutput->Add(hTimeSort);
   
   hTimeZero = new TH1F("hTimeZero", "hTimeZero", 40, -40, 40);
   fOutput->Add(hTimeZero);
      
   mEliade_raw = new TH2F("mEliade_raw", "mEliade_raw", 200, 0.5, 200.5, 16384, -0.5, 16383.5);
   mEliade_raw->GetXaxis()->SetTitle("domain");
   mEliade_raw->GetYaxis()->SetTitle("ADC channels");   
   fOutput->Add(mEliade_raw);
   
   mEliade = new TH2F("mEliade", "mEliade", 200, 0.5, 200.5, 16384, -0.5, 16383.5);
   mEliade->GetXaxis()->SetTitle("domain");
   mEliade->GetYaxis()->SetTitle("keV");
   fOutput->Add(mEliade);
      
   mSegments = new TH2F("mSegments", "mSegments", 200, 0, 200, 16384, -0.5, 16383.5);
   mSegments->GetXaxis()->SetTitle("domain");
   mSegments->GetYaxis()->SetTitle("keV");
   fOutput->Add(mSegments);
   
   mCores = new TH2F("mCores", "mCores", 200, 0, 200, 16384, -0.5, 16383.5);
   mCores->GetXaxis()->SetTitle("domain");
   mCores->GetYaxis()->SetTitle("keV");
   fOutput->Add(mCores);
   
   mSegmentsPerCore = new TH2F("mSegmentsPerCore", "mSegmentsPerCore", 10, 0, 10, 20, 0, 20);
   mSegmentsPerCore->GetXaxis()->SetTitle("segments");
   mSegmentsPerCore->GetYaxis()->SetTitle("counts");   
   fOutput->Add(mSegmentsPerCore);
   
//    mEliadeTD = new TH2F("mEliadeTD", "mEliadeTD", 4, 0, 4, 300, -0.5, 299.5);
//    fOutput->Add(mEliadeTD);
   
   mEliadeMULT = new TH2F("mEliadeMULT", "mEliadeMULT", 4, 0, 4, 10, -0.5, 9.5);
   fOutput->Add(mEliadeMULT);
   
   mBoardTimeDiff = new TH2F("mBoardTimeDiff", "mBoardTimeDiff", nbr_of_boards, 0, nbr_of_boards, 1000, -99.5, 899.5);
   fOutput->Add(mBoardTimeDiff);
   
   mDom0TimeDiff = new TH2F("mDom0TimeDiff", "mDom0TimeDiff",200, 0.5, 200.5, 500, -99.5, 899.5);
   fOutput->Add(mDom0TimeDiff);//time_diff relevant to the 1st channel (101), i.e. ch 101 is a trigger
    
   mDom0TimeDiffEnergy = new TH2F("mDom0TimeDiffEnergy", "mDom0TimeDiffEnergy",16384, -0.5, 16383.5, 500, -99.5, 899.5);
   fOutput->Add(mDom0TimeDiffEnergy);//time_diff relevant to the 1st channel (101), i.e. ch 101 is a trigger
   
   mPulser0TimeDiff = new TH2F("mPulser0TimeDiff", "mPulser0TimeDiff", 200, 0.5, 200.5, 500, -99.5, 899.5);
   fOutput->Add(mPulser0TimeDiff);//time_diff relevant to the 1st channel (101), i.e. ch 101 is a trigger
       
   mZeroTimeDiff_vs_Enegy = new TH2F("mZeroTimeDiff_vs_Enegy", "mZeroTimeDiff_vs_Enegy", 16384, -0.5, 16383.5, 1000, -99.5, 899.5);
   fOutput->Add(mZeroTimeDiff_vs_Enegy);//time_diff relevant to the 1st channel (101), i.e. ch 101 is a trigger
   
   hMultCores = new TH1F("hMultCores", "hMultCores", 100, 0, 100);
   fOutput->Add(hMultCores);
    
   hMultSegments = new TH1F("hMultSegments", "hMultSegments", 100, 0, 100);
   fOutput->Add(hMultSegments);
    
   mCoreCore = new TH2F("mCoreCore", "mCoreCore",4096, -0.5, 4095.5, 4096, -0.5, 4095.5);
   fOutput->Add(mCoreCore);
    
   mSegmentSegment = new TH2F("mSegmentSegment", "mSegmentSegment",4096, -0.5, 4095.5, 4096, -0.5, 4095.5);
   fOutput->Add(mSegmentSegment);
    
//     hTimeDiffCoreCore = new TH1F("hTimeDiffCoreCore", "hTimeDiffCoreCore", 1000, -99.5, 899.5);
   hTimeDiffCoreCore = new TH1F("hTimeDiffCoreCore", "hTimeDiffCoreCore", 1000, -99.5, 899.5);
   fOutput->Add(hTimeDiffCoreCore);
   
   hTimeDiffCeBrCebr = new TH1F("hTimeDiffCeBrCebr", "hTimeDiffCeBrCebr", 1000, -99.5, 899.5);
   fOutput->Add(hTimeDiffCeBrCebr);
   
   hTimeDiffCeBrCebr = new TH1F("hTimeDiffCeBrCebr", "hTimeDiffCeBrCebr", 1000, -99.5, 899.5);
   fOutput->Add(hTimeDiffCeBrCebr);
    
   hTimeDiffBGOCeBr = new TH1F("hTimeDiffBGOCeBr", "hTimeDiffBGOCeBr", 1000, -99.5, 899.5);
   fOutput->Add(hTimeDiffBGOCeBr);
    
   hTimeDiffPulser = new TH1F("hTimeDiffPulser", "hTimeDiffPulser", 1000, -99.5, 899.5);
   fOutput->Add(hTimeDiffPulser);
    
   mPulserPulser = new TH2F("mPulserPulser", "mPulserPulser",4096, -0.5, 8191.5, 4096, -0.5, 8195.5);
   fOutput->Add(mPulserPulser);
    
    
   
   
    start = std::clock();
    
    lastEliadeEvent.fTimeStamp = 0;
    lastEliadeZeroEvent.fTimeStamp = 0;
    
    /*std::map<UInt_t, TEliadeEvent> ::iterator it__ = LUT_ELIADE.begin();
    for (; it__ != LUT_ELIADE.end(); ++it__) {
    //      particle_id[it__->second] = it__->first;
	std::cout<<LUT_ELIADE[it__->first].ch<<" "<< LUT_ELIADE[it__->first].pol_order <<std::endl;
    }*/
    for (int i=0; i<nbr_of_boards; i++ ){
        last_board_event[i].fTimeStamp=0;
    ;}
    
    
    

}

Bool_t EliadeSorting::Process(Long64_t entry)
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
//   if(nb_entries == 0) {
    nb_entries = GetEntries();
    
    if (EliadeEvent.fTimeStamp == 0) {hTimeZero->Fill(EliadeEvent.fChannel);};
    

  // if (nevents>20000) {reset_counter=1;nevents_reset=0;};
  //}
  //std::cout<<"nb_entries "<<nb_entries<<std::endl;
//   int test =   EliadeEvent.fChannel;
   
   // std::cout<<Form("%i", EliadeEvent.fChannel) <<std::end;
	int mod = EliadeEvent.fMod;
	int ch = EliadeEvent.fChannel;
	
       if (mod > 3) return kTRUE;
	
	//int num = 100*current_clover+(mod)*10+ch;
        int daq_ch = (mod)*100+ch;
	EliadeEvent.channel = daq_ch;
	EliadeEvent.EnergyCal = CalibDet(EliadeEvent.fEnergy, daq_ch);
	EliadeEvent.domain = LUT_ELIADE[daq_ch].dom;
    EliadeEvent.det_def = LUT_ELIADE[daq_ch].detType;
    hDetTypeHit->Fill(EliadeEvent.det_def);
//     EliadeEvent.domain = LUT_ELIADE[daq_ch].dom;
	EliadeEvent.core = (EliadeEvent.domain  - 100*current_clover)/10; // a % 10 = reminder
	EliadeEvent.segment = (EliadeEvent.domain  - 100*current_clover) % 10; // a % 10 = reminder
	hCoreHit->Fill(EliadeEvent.core);
    hSegmentHit->Fill(EliadeEvent.segment);
	int domain = EliadeEvent.domain;
    if ((EliadeEvent.fEnergy < LUT_ELIADE[daq_ch].upperThreshold))  {/*std::cout<<EliadeEvent.fEnergy<< " "<< LUT_ELIADE[daq_ch].upperThreshold<<std::endl; */return kTRUE;}
            
   	hChannelHit->Fill(daq_ch);
  	hDomainHit->Fill(domain);
	mEliade_raw->Fill(domain,EliadeEvent.fEnergy);
	mEliade->Fill(domain,EliadeEvent.EnergyCal);

    //Check if the tree is time sorted
    if (lastTimeStamp > EliadeEvent.fTimeStamp){std::cout<<"Warning: TTree may be not sorted by time \n";};
    lastTimeStamp = EliadeEvent.fTimeStamp;
    
    if (EliadeEvent.det_def == 1) hEliade_no_addback->Fill(EliadeEvent.EnergyCal);

	//std::cout<<" EliadeEvent.domain "<<EliadeEvent.domain<<"  LUT_ELIADE[num].TimeOffset "<< LUT_ELIADE[num].TimeOffset<<" EliadeEvent.fTimeStamp "<<EliadeEvent.fTimeStamp <<" "<<EliadeEvent.fTimeStamp + LUT_ELIADE[num].TimeOffset<<std::endl;
     EliadeEvent.fTimeStamp = EliadeEvent.fTimeStamp + LUT_ELIADE[daq_ch].TimeOffset;
     hTimeSort->Fill(EliadeEvent.fTimeStamp - lastEliadeEvent.fTimeStamp);   

    if (EliadeEvent.det_def == 1){mCores -> Fill(mod, EliadeEvent.EnergyCal);}
       else if (EliadeEvent.det_def == 2) {mSegments->Fill(domain, EliadeEvent.EnergyCal);};
    
    //Check Pulser TimeAlignment

       if (EliadeEvent.det_def == 9){
            int time_diff_pulser = EliadeEvent.fTimeStamp - lastTime_pulser;
            mPulser0TimeDiff->Fill(EliadeEvent.domain, time_diff_pulser);
            if (EliadeEvent.domain == 150) lastTime_pulser = EliadeEvent.fTimeStamp;        
            return kTRUE;
           
        } else if (EliadeEvent.det_def == 1)
          {
   //Check Eliade TimeAlignment
            int time_diff_dom0 = EliadeEvent.fTimeStamp - lastTime_dom0;
            mDom0TimeDiff->Fill(EliadeEvent.domain, time_diff_dom0);
            mDom0TimeDiffEnergy->Fill(EliadeEvent.EnergyCal, time_diff_dom0);
            if (EliadeEvent.domain == 109) lastTime_dom0 = EliadeEvent.fTimeStamp;
            
//             return kTRUE;
          };
  
    //gamma-gamma between different cores
//      if ((cores.count(domain))&&(addBackMode == 0)){
    if ((EliadeEvent.det_def == 1)&&(addBackMode == 0)){ //noaddback
//  if ((domain >= 140)&&(addBackMode == 0)){ //for the pulser to check the time
//          std::cout<<" no add back \n";
         if (coincQu_cores.empty()){coincQu_cores.push_back(EliadeEvent);/*std::cout<<"Empty Coic \n";*/}
         else
         {
//              std::cout<<" no add back \n";
             int time_diff = EliadeEvent.fTimeStamp - coincQu_cores.front().fTimeStamp;
//                std::cout<<time_diff<<" time_diff \n";
             hTimeDiffCoreCore->Fill(time_diff);
             if (std::abs(time_diff) < 20) 
             {
                 coincQu_cores.push_back(EliadeEvent);
             }
             else
             {
                hMultCores->Fill(coincQu_cores.size());             
                std::deque<TEliadeEvent>  ::iterator it1__ = coincQu_cores.begin();
                std::deque<TEliadeEvent>  ::iterator it2__ = coincQu_cores.begin(); 
                for (; it1__ != coincQu_cores.end(); ++it1__){
                     hEliade->Fill((*it1__).EnergyCal);
                  for (; it2__ != coincQu_cores.end(); ++it2__){
                      if (it1__ == it2__) continue;
                      mCoreCore->Fill((*it1__).EnergyCal, (*it2__).EnergyCal);
                  };
                };
                coincQu_cores.clear();
                coincQu_cores.push_back(EliadeEvent);
             }
         }
     }else if ((EliadeEvent.det_def == 33)&&(addBackMode == 0)){ //noaddback
//  if ((domain >= 140)&&(addBackModze == 0)){ //for the pulser to check the time
         // std::cout<<" CeBr \n";
         if (coincQu_cores.empty()){coincQu_cores.push_back(EliadeEvent);/*std::cout<<"Empty Coic \n";*/}
         else
         {
//              std::cout<<" no add back \n";
             int time_diff = EliadeEvent.fTimeStamp - coincQu_cores.front().fTimeStamp;
//                std::cout<<time_diff<<" time_diff \n";
             hTimeDiffCeBrCebr->Fill(time_diff);
             if (std::abs(time_diff) < 40000) 
             {
                 coincQu_cores.push_back(EliadeEvent);
             }
             else
             {
                hMultCores->Fill(coincQu_cores.size());             
                std::deque<TEliadeEvent>  ::iterator it1__ = coincQu_cores.begin();
                std::deque<TEliadeEvent>  ::iterator it2__ = coincQu_cores.begin(); 
                for (; it1__ != coincQu_cores.end(); ++it1__){
                     hEliade->Fill((*it1__).EnergyCal);
                  for (; it2__ != coincQu_cores.end(); ++it2__){
                      if (it1__ == it2__) continue;
                      mCoreCore->Fill((*it1__).EnergyCal, (*it2__).EnergyCal);
                  };
                };
                coincQu_cores.clear();
                coincQu_cores.push_back(EliadeEvent);
             }
         }
     }  if ((EliadeEvent.det_def == 3)&&(addBackMode == 0)){ //noaddback
//  if ((domain >= 140)&&(addBackMode == 0)){ //for the pulser to check the time
//          std::cout<<" no add back \n";
         if (coincQu_cores.empty()){coincQu_cores.push_back(EliadeEvent);/*std::cout<<"Empty Coic \n";*/}
         else
         {
//              std::cout<<" no add back \n";
             int time_diff = EliadeEvent.fTimeStamp - coincQu_cores.front().fTimeStamp;
//                std::cout<<time_diff<<" time_diff \n";
             hTimeDiffCoreCore->Fill(time_diff);
             if (std::abs(time_diff) < 20) 
             {
                 coincQu_cores.push_back(EliadeEvent);
             }
             else
             {
                hMultCores->Fill(coincQu_cores.size());             
                std::deque<TEliadeEvent>  ::iterator it1__ = coincQu_cores.begin();
                std::deque<TEliadeEvent>  ::iterator it2__ = coincQu_cores.begin(); 
                for (; it1__ != coincQu_cores.end(); ++it1__){
                     hEliade->Fill((*it1__).EnergyCal);
                  for (; it2__ != coincQu_cores.end(); ++it2__){
                      if (it1__ == it2__) continue;
                      mCoreCore->Fill((*it1__).EnergyCal, (*it2__).EnergyCal);
                  };
                };
                coincQu_cores.clear();
                coincQu_cores.push_back(EliadeEvent);
             }
         }
     };
     
     
     if (EliadeEvent.det_def == 5) {bgo_Qu.push_back(EliadeEvent);
         
        }else if ((EliadeEvent.det_def == 3)&&(addBackMode == 0)){ 
         
         bool bgo_pass = bgo_Qu.empty();
         if (bgo_pass) hCeBr_CS->Fill(EliadeEvent.EnergyCal);
         if (bgo_pass) std::cout<<"Empty "<< EliadeEvent.EnergyCal<<" \n";
         
          if ((EliadeEvent.domain == 172)&&(!bgo_pass)){
             std::deque<TEliadeEvent>  ::iterator it1__ = bgo_Qu.begin();
             int time_diff;
             for (; it1__ != bgo_Qu.end(); ++it1__){
                  time_diff = EliadeEvent.fTimeStamp - (*it1__).fTimeStamp;
                  hTimeDiffBGOCeBr->Fill(time_diff);
                  if (time_diff < 10){
 //                   bgo_Qu.erase(it1__,it1__);
                      bgo_Qu.pop_front();
                  }
              };
             bgo_pass = bgo_Qu.empty();            
             if (bgo_pass) hCeBr_CS->Fill(EliadeEvent.EnergyCal);
         };
        if (EliadeEvent.domain == 172) hCeBr->Fill(EliadeEvent.EnergyCal);
     };
     
     
    //Trying add-back
    
    // if ((addBackMode == 1)&&(!pulsers.count(domain))){
      if ((addBackMode == 1)&&(EliadeEvent.det_def != 9)){

     	if (!Trigger){
	     	if (EliadeEvent.det_def == 1){coincQu_cores.push_back(EliadeEvent);Trigger=true;}
     	}
     	else //Trigger==true
     	{
     		int time_diff = EliadeEvent.fTimeStamp - coincQu_cores.front().fTimeStamp;
            if (EliadeEvent.det_def == 1){hTimeDiffCoreCore->Fill(time_diff);};
     		if (std::abs(time_diff) < 40)
     		{
     			if (EliadeEvent.det_def == 1){coincQu_cores.push_back(EliadeEvent);}
     				//else coincQu_segments.push_back(EliadeEvent);
     				else coincQu_seg[EliadeEvent.core].push_back(EliadeEvent);
     		}
     		else 
     		{

            for (int i=0;i<=3;i++){mSegmentsPerCore->Fill(i,coincQu_seg[i].size());};
            //Run AddBack
 			 AddBack();
 			//Add to hist and mat 
 		    std::deque<float>  ::iterator it1__ = enrergyQu.begin();
		    std::deque<float>  ::iterator it2__ = enrergyQu.begin();
                       for (; it1__ != enrergyQu.end(); ++it1__){
                          hEliade->Fill((*it1__));
	                  for (; it2__ != enrergyQu.end(); ++it2__){
        	              if (it1__ == it2__) continue;
        	              mCoreCore->Fill((*it1__), (*it2__));
                  	};
             };
   			 coincQu_cores.clear();
   			 coincQu_segments.clear();
             for (int j=0;j<=3;j++) coincQu_seg[j].clear();
    			 Trigger = false;
   			 
   			 if (EliadeEvent.det_def == 1)
   			 {
   			 coincQu_cores.push_back(EliadeEvent);Trigger=true;
   			 }

     		}
     	};
     	//if (coincQu_cores.empty()){
	//	if ((cores.count(num))){coincQu_cores.push_back(EliadeEvent);}
     	//};
     };
   	 
   	 

    
 
    
   
//    	lastEliadeEvent = EliadeEvent;
   
// 	mBoardTimeDiff->Fill(mod, EliadeEvent.fTimeStamp - last_board_event[mod].fTimeStamp);
// 	last_board_event[mod] = EliadeEvent;

/*
	mZeroTimeDiff->Fill(domain,EliadeEvent.fTimeStamp - lastEliadeZeroEvent.fTimeStamp);
	mZeroTimeDiff_vs_Enegy->Fill(EliadeEvent.EnergyCal, EliadeEvent.fTimeStamp - lastEliadeZeroEvent.fTimeStamp);
	if  (domain == zero_channel) {lastEliadeZeroEvent = EliadeEvent;};*/


   /*if ((EliadeEvent.fMod == 0)&&(EliadeEvent.fChannel==1)){
   std::cout<<"EliadeEvent.fMod     "<<EliadeEvent.fMod*1.0<<" "<<std::endl;
   std::cout<<"EliadeEvent.fCh      "<<EliadeEvent.fChannel*1.0<< " "<<std::endl;
   std::cout<<"EliadeEvent.fEnergy  "<<EliadeEvent.fEnergy<< std::endl;
   std::cout<<"Num  "<< num << std::endl;
   std::cout<<" ================  "<< std::endl;
   //std::cout<<"EliadeEvent.fTimeStamp  "<<EliadeEvent.fTimeStamp<< std::endl;
   };*/


   
//    if (num == 102){
//  std::cout <<" num "<<  num <<" EliadeEvent.fEnergy "<<EliadeEvent.fEnergy << " EliadeEvent.EnergyCal  " << EliadeEvent.EnergyCal  <<std::endl;
//hTest->Fill(EliadeEvent.EnergyCal);}
   
//float EliadeSorting::CalibDet(float val, int domain)   
   
  // if (EliadeEvent.EnergyCal == -1) break;
   
   //std::cout<<num<<std::endl;
 
   //hEnergy_raw[num]->Fill(EliadeEvent.fEnergy);
//   if (EliadeEvent.fChannel <=1) {mCores->Fill(num,EliadeEvent.fEnergy);}
//   else if (EliadeEvent.fChannel<=9) {mSegments->Fill(num,EliadeEvent.fEnergy);};

   //CheckCoincInCrystal(EliadeEvent);   



   //hHitPattern->Fill(EliadeEvent.fChannel);
   
   
   
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
  }
   

   nevents++;
   nevents_reset++;
   return kTRUE;
}


void EliadeSorting::AddBack()
{
 if (coincQu_cores.empty()){std::cout<<"Strange Empty Queue "<<std::endl;return;};
//std::deque<TEliadeEvent>  ::iterator it2__ = coincQu_cores.begin();  
 enrergyQu.clear();
 int ncores = coincQu_cores.size();
 hMultCores->Fill(ncores);
 if (ncores == 1) {enrergyQu.push_back(coincQu_cores.front().EnergyCal);return;}
 
 if (ncores == 2) {
     if (coincQu_cores[0].core == coincQu_cores[1].core) 
     {        
         enrergyQu.push_back(coincQu_cores.front().EnergyCal);
         std::cout<<"Warning the same core fired twice \n";
         return;
     };    
     
     int coreCoincID = 0;
     
     if (coincQu_cores[0].core < coincQu_cores[1].core) 
     {
         coreCoincID =  coincQu_cores[1].core *10 +  coincQu_cores[0].core;
//          CoreSegmentHitID(coincQu_cores[0].core, 2376);
//          CoreSegmentHitID(coincQu_cores[1].core, 3478);
     }
     else 
     {
         coreCoincID =  coincQu_cores[0].core *10 +  coincQu_cores[1].core;
//          CoreSegmentHitID(coincQu_cores[1].core, 2376);
//          CoreSegmentHitID(coincQu_cores[0].core, 3478);
     };
     
     hCheckCore2AddBack->Fill(coreCoincID);
     
     switch (coreCoincID){
         case 20: case 31: //diagonal
         {
          enrergyQu.push_back(coincQu_cores[0].EnergyCal);
          enrergyQu.push_back(coincQu_cores[1].EnergyCal);   
         }
     /*    case 10: case 21: case 32:
         {
            CoreSegmentHitID(coincQu_seg[coreCoincID % 10], 2376); //smaller core
            CoreSegmentHitID(coincQu_seg[coreCoincID / 10], 3478); // bigger core
            //std::cout<<coreCoincID<<" coreCoincID smaller "<< (coreCoincID % 10)<<" bigger" << (coreCoincID / 10) <<" \n";
         }
         case 30:
          {
            CoreSegmentHitID(coincQu_seg[coreCoincID / 10], 2376);
            CoreSegmentHitID(coincQu_seg[coreCoincID % 10], 3478); 
            //std::cout<<(coreCoincID / 10)<<" \n";
         } */  
     };
     
     
     
//      switch (coreCoincID){
//          case 4: case 6: //segments are on diagonal
//              enrergyQu.push_back(coincQu_cores[0].EnergyCal);
//              enrergyQu.push_back(coincQu_cores[1].EnergyCal);
//          case 1: case 5: //core 0 and 1
//           {
//               if (coincQu_cores[0].core < coincQu_cores[1].core) 
//               {
//                   CoreSegmentHitID(coincQu_cores[0].core, 2376);
//                   CoreSegmentHitID(coincQu_cores[1].core, 3478);
//               }
//               else 
//               {
//                   CoreSegmentHitID(coincQu_cores[1].core, 2376);
//                   CoreSegmentHitID(coincQu_cores[0].core, 3478);   
//               }
//           };
//          case 3:
//          {
//              if (coincQu_cores[0].core < coincQu_cores[1].core) 
//               {
//                   CoreSegmentHitID(coincQu_cores[0].core, 2376);
//                   CoreSegmentHitID(coincQu_cores[1].core, 3478);
//               }
//               else 
//               {
//                   CoreSegmentHitID(coincQu_cores[1].core, 2376);
//                   CoreSegmentHitID(coincQu_cores[0].core, 3478);   
//               }
//          };
//              
//      };
//      
     
// 	 if (!crystal2mask.count(coreCoincID))//segments are on diagonal
// 	 //if cores are on diagonal: 4 or 6 ; (side :  1 3 or 5)
// 	 {
// 	 	
// 	 	//std::cout<<"core coinc " <<coincQu_cores[0].core + coincQu_cores[1].core <<std::endl;
// 	 	enrergyQu.push_back(coincQu_cores[0].EnergyCal);
// 	    enrergyQu.push_back(coincQu_cores[1].EnergyCal);CoreSegmentHitID
//         //return;
// 	 }
// 	 else {
//         int segmentCoincID;// = pow(2, coincQu_cores[0].core); 
//          //return;
//          
//         }
	};
 return;
	
}

void EliadeSorting::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void EliadeSorting::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the clieint coreID, nt, it can be used to present
   // the results graphically or save the results to file.

  std::cout<<"I  will terminate soon... "<<std::endl;   
//  CheckSorting(eliadeQu);

  
  //std::sort(((eliadeQu.begin()).fTimeStamp, (eliadeQu.end()).fTimeStamp);
  
  TString option = GetOption();
  toks = option.Tokenize(",");
  TString RunID = ((TObjString*) toks->At(0))->GetString();
  TString VolID = ((TObjString*) toks->At(1))->GetString();
  
  std::stringstream OutputFile;
  OutputFile << "sorted_run" << "_" << RunID <<"_"<<VolID<< ".root";
  std::cout << "OUTFILE  sorted_run" << "_" << RunID<<"_"<<VolID<< ".root"<<std::endl;
 
  /*
  std::stringstream OutputFile;
  OutputFile << "sorted_run" << "_" << RunID << ".root";
  std::cout << "sorted_run" << "_" << RunID << ".root"<<std::endl;*/
  
   
  TFile ofile(OutputFile.str().c_str(),"recreate"); 
      TIter iter(fOutput);
      

  std::cout << std::endl << "Finished after " << round(duration / 60) << ":"
	    << std::setw(2) << round(((int) duration) % 60) << std::setw(8)
	    << ". Write spectra to file" << std::endl;


      TObject *obj; 
       while ((obj = iter())) {
           obj->Write();
       };
   

}

int EliadeSorting::CheckTimeAlignment(int to_domain)
{
//          if (coincQu_pulser.empty()){coincQu_pulser.push_back(EliadeEvent);/*std::cout<<"Empty Coic \n";*/}
//          else 
//          { 
//              int time_diff_pulser = EliadeEvent.fTimeStamp - coincQu_pulser.front().fTimeStamp;
//              hTimeDiffPulser->Fill(time_diff_pulser);
//              
//              if (time_diff_pulser < 10 )
//              {
//                 coincQu_pulser.push_back(EliadeEvent);
//              }
//              else{
//                 std::deque<TEliadeEvent>  ::iterator it1__ = coincQu_pulser.begin();
//                 std::deque<TEliadeEvent>  ::iterator it2__ = coincQu_pulser.begin();
//                 for (; it1__ != coincQu_pulser.end(); ++it1__){
//                   for (; it2__ != coincQu_pulser.end(); ++it2__){
//                       if (it1__ == it2__) continue;
//                       mPulserPulser->Fill((*it1__).EnergyCal, (*it2__).EnergyCal);
//                   };
//                 };
//                 coincQu_pulser.clear();
//                 coincQu_pulser.push_back(EliadeEvent);    
//                 };
//              };
//     coincQu_pulser  is not needed to check TimeAllignement
    //does not work ULong64_t lastTime = 0; should be outside of here
    ULong64_t lastTime = 0;
    int time_diff_pulser = EliadeEvent.fTimeStamp - lastTime;;
    if (EliadeEvent.domain == to_domain) lastTime = EliadeEvent.fTimeStamp;        
 	return time_diff_pulser;
};

int  EliadeSorting::CoreSegmentHitID(std::deque<TEliadeEvent> segQu_, int coincID)
{
    std::deque<TEliadeEvent>  ::iterator it__ = segQu_.begin();  
    
    std::deque<UShort_t> SegmentHitMask;
    for (int i=1;i<=8;i++) SegmentHitMask.push_back(0);
    for (; it__ != segQu_.end(); ++it__)SegmentHitMask.at(it__->segment = 1);                  
    
   // for (int i=1;i<=8;i++) {std::cout<<SegmentHitMask.at(0)<<" ";};std::cout<<"\n";

    
    int result = 0;    
    
    /*switch (coincID){
        case 2376:
        {
            result = pow(2,SegmentHitMask.at(2))+pow(2,SegmentHitMask.at(3))+pow(2,SegmentHitMask.at(6))+pow(3,(7));//204 max
            std::cout<<SegmentHitMask.at(2)<<" "<<SegmentHitMask.at(3)<<" "<<SegmentHitMask.at(6)<<" "<<SegmentHitMask.at(7)<<"\n";
        }
        case 3478:
        {
            result = pow(2,SegmentHitMask.at(3))+pow(2,SegmentHitMask.at(4))+pow(2,SegmentHitMask.at(7))+pow(3,(8));//408 max
            std::cout<<SegmentHitMask.at(3)<<" "<<SegmentHitMask.at(4)<<" "<<SegmentHitMask.at(7)<<" "<<SegmentHitMask.at(8)<<"\n";

        };
    };*/
    
    if (result > 408) {std::cout<<"Warning something is wrong with SegmentHitMask "<< result << " \n";};
    
 return result;   
}

void EliadeSorting::AddBackCoreAB() //0 and 1
{
//     for (int i = 0; i<=1; i++)
//         switch (coincQu_cores[1].core)
//         {
//             case (coincQu_cores[i].core == 0):
//                 CoreSegmentHitID(coincQu_cores[0].core, 2376);
//             case (coincQu_cores[i].core == 1):
//                 CoreSegmentHitID(coincQu_cores[0].core, 3478);    
//         }
 return;   
}

void EliadeSorting::AddBackCoreBC()
{
 return;   
}

void EliadeSorting::AddBackCoreCD()
{
 return;   
}

void EliadeSorting::AddBackCoreDA()
{
 return;   
}
