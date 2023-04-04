{
  TString home_path = gSystem->GetFromPipe("echo $HOME");
  gROOT->ProcessLine(Form(".L %s/EliadeSorting/lib/libDelilaEvent.so-m64",home_path.Data()));
  gROOT->ProcessLine(Form(".L %s/EliadeSorting/lib/libHPGeTreeEvent.so-m64",home_path.Data()));
  gROOT->ProcessLine(Form(".L %s/EliadeSorting/lib/libHPGeSegTreeEvent.so-m64",home_path.Data()));
  gROOT->ProcessLine(Form(".L %s/EliadeSorting/lib/libLaBrTreeEvent.so-m64",home_path.Data()));
  gROOT->ProcessLine(Form(".L %s/EliadeSorting/lib/libElissaTreeEvent.so-m64",home_path.Data()));
   
  }
