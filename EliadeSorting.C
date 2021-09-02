#define EliadeSorting_cxx
// The class definition in data.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
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
using namespace std;


TString LUT_Directory = "/data/live/IT/dsoft/EliadeSorting/";
//TString LUT_Directory = "";

bool debug = false;

const int dom_nbr = 1;
const int current_clover = 1;
const int nbr_of_boards = 8;
const int nbr_of_ch = 200;
const int zero_channel = 101; //for time allignement

//std::unordered_set<int> cores = { 101,111,121,131};
std::unordered_set<int> cores = { 101,111,121};

void EliadeSorting::Read_ELIADE_LookUpTable() {
  std::cout << "I am Reading ELIADE LookUpTable ... ";
//  std::stringstream CUTFile;
//  CUTFile << CUTG_Directory.Data() << "cut_EeEw_galileo.root";
//  TFile *file_EeEw = TFile::Open(CUTFile.str().c_str());
  std::stringstream LUTFile;
  LUTFile << LUT_Directory << "LUT_ELIADE.dat";
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
      is >> curDet.ch >> curDet.dom >> curDet.theta >> curDet.phi >> curDet.TimeOffset >> curDet.upperThreshold;
      
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
	std::cout<<LUT_ELIADE[it__->first].ch<<" "<< LUT_ELIADE[it__->first].dom<<" "<< LUT_ELIADE[it__->first].theta<<" "<< LUT_ELIADE[it__->first].phi <<" "<< LUT_ELIADE[it__->first].TimeOffset<<" "<< LUT_ELIADE[it__->first].upperThreshold<<" "<<LUT_ELIADE[it__->first].pol_order <<std::endl;
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
   

 	
   hHitPattern = new TH1F("ChannelHits", "ChannelHits",200,0,200);
   fOutput->Add(hHitPattern);
   
   
   	
   hTest = new TH1F("hTest", "hTest", 16384, -0.5, 16383.5);
   fOutput->Add(hTest);
   
   hTimeSort = new TH1F("hTimeSort", "hTimeSort", 1000, -500, 500);
   fOutput->Add(hTimeSort);
   
   hTimeZero = new TH1F("hTimeZero", "hTimeZero", 40, -40, 40);
   fOutput->Add(hTimeZero);
      
   mEliade_raw = new TH2F("mEliade_raw", "mEliade_raw", 200, 0.5, 200.5, 16384, -0.5, 16383.5);
   fOutput->Add(mEliade_raw);
   
   mEliade = new TH2F("mEliade", "mEliade", 200, 0.5, 200.5, 16384, -0.5, 16383.5);
   fOutput->Add(mEliade);
      
   mSegments = new TH2F("mSegments", "mSegments", 200, 0, 200, 16384, -0.5, 16383.5);
   fOutput->Add(mSegments);
   
   mCores = new TH2F("mCores", "mCores", 200, 0, 200, 16384, -0.5, 16383.5);
   fOutput->Add(mCores);
   
   mEliadeTD = new TH2F("mEliadeTD", "mEliadeTD", 4, 0, 4, 300, -0.5, 299.5);
   fOutput->Add(mEliadeTD);
   
   mEliadeMULT = new TH2F("mEliadeMULT", "mEliadeMULT", 4, 0, 4, 10, -0.5, 9.5);
   fOutput->Add(mEliadeMULT);
   
   mBoardTimeDiff = new TH2F("mBoardTimeDiff", "mBoardTimeDiff", nbr_of_boards, 0, nbr_of_boards, 1000, -99.5, 899.5);
   fOutput->Add(mBoardTimeDiff);
   
   mZeroTimeDiff = new TH2F("mZeroTimeDiff", "mZeroTimeDiff", nbr_of_ch, 0, nbr_of_ch, 1000, -99.5, 899.5);
   fOutput->Add(mZeroTimeDiff);//time_diff relevant to the 1st channel (101), i.e. ch 101 is a trigger
   
    
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
    
    hTimeDiffCoreCore = new TH1F("hTimeDiffCoreCore", "hTimeDiffCoreCore", 1000, -99.5, 899.5);
    fOutput->Add(hTimeDiffCoreCore);
    
    hTimeDiffSegSeg = new TH1F("hTimeDiffSegSeg", "hTimeDiffSegSeg", 1000, -99.5, 899.5);
    fOutput->Add(hTimeDiffSegSeg);
   
   
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
  
 // if (nevents == 10)  {std::cout<<" Reached 1e6 events I am terminating "<<std::endl; return kTRUE;}
   
//   int test =   EliadeEvent.fChannel;
   
   // std::cout<<Form("%i", EliadeEvent.fChannel) <<std::end;
	int mod = EliadeEvent.fMod;
	int ch = EliadeEvent.fChannel;
	int num = 100*current_clover+(mod)*10+ch;
	EliadeEvent.channel = num;
	EliadeEvent.EnergyCal = CalibDet(EliadeEvent.fEnergy, num);
	EliadeEvent.domain = LUT_ELIADE[num].dom;
    
        
   	hHitPattern->Fill(num);
	mEliade_raw->Fill(num,EliadeEvent.fEnergy);
	mEliade->Fill(num,EliadeEvent.EnergyCal);
	
	
		//std::cout<<" EliadeEvent.domain "<<EliadeEvent.domain<<"  LUT_ELIADE[num].TimeOffset "<< LUT_ELIADE[num].TimeOffset<<" EliadeEvent.fTimeStamp "<<EliadeEvent.fTimeStamp <<" "<<EliadeEvent.fTimeStamp + LUT_ELIADE[num].TimeOffset<<std::endl;
	
	
	//EliadeEvent.fTimeStamp = EliadeEvent.fTimeStamp + LUT_ELIADE[num].TimeOffset;

   
//   hTimeSort->Fill(EliadeEvent.fTimeStamp - lastEliadeEvent.fTimeStamp);   
 
    //gamma-gamma between different cores
//     ULong64_t TimeDiff = 0; 
     if ((cores.count(num))){
         if (coincQu_cores.empty()){coincQu_cores.push_back(EliadeEvent);/*std::cout<<"Empty Coic \n";*/}
         else if (EliadeEvent.fTimeStamp - coincQu_cores.front().fTimeStamp < 100) {
             coincQu_cores.push_back(EliadeEvent);
             hTimeDiffCoreCore->Fill(EliadeEvent.fTimeStamp - coincQu_cores.front().fTimeStamp);
         }
         else {
             hMultCores->Fill(coincQu_cores.size());
             
             std::deque<TEliadeEvent>  ::iterator it1__ = coincQu_cores.begin();
             std::deque<TEliadeEvent>  ::iterator it2__ = coincQu_cores.begin();
             
              for (; it1__ != coincQu_cores.end(); ++it1__){
                  for (; it2__ != coincQu_cores.end(); ++it2__){
                      if (it1__ == it2__) continue;
                      mCoreCore->Fill((*it1__).EnergyCal, (*it2__).EnergyCal);
                  };
             };
             
             coincQu_cores.clear();
             coincQu_cores.push_back(EliadeEvent);
         };
     };
    
    

    
    if ((cores.count(num))){mCores -> Fill(mod, EliadeEvent.fEnergy);}
        else {mSegments->Fill(num, EliadeEvent.fEnergy);};
    
   
   	lastEliadeEvent = EliadeEvent;
   
	mBoardTimeDiff->Fill(mod, EliadeEvent.fTimeStamp - last_board_event[mod].fTimeStamp);
	last_board_event[mod] = EliadeEvent;


	mZeroTimeDiff->Fill(num,EliadeEvent.fTimeStamp - lastEliadeZeroEvent.fTimeStamp);
	mZeroTimeDiff_vs_Enegy->Fill(EliadeEvent.EnergyCal, EliadeEvent.fTimeStamp - lastEliadeZeroEvent.fTimeStamp);
	if  (num == zero_channel) {lastEliadeZeroEvent = EliadeEvent;};


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

void EliadeSorting::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void EliadeSorting::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
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
