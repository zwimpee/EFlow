{
  gROOT->Reset();
  TFile* f=TFile::Open("readMap_AlCaPhiSym_Run2012_v5_6400M_alphaFit.root");
  cout<<"ok"<<endl;
  TTree* inputTree=(TTree*)f->Get("outTree_barl");
  cout<<"ok"<<endl;
  gROOT->ProcessLine(".L makeIntervalsPlots.C+");
  cout<<"ok"<<endl;
  makeIntervalsPlots t(inputTree);
  cout<<"ok"<<endl;
  t.setOutfile("intervalPlots_2012_readMap_AlCaPhiSym_Run2012_v5_6400M_alphaFit.root");
  cout<<"ok"<<endl;
  t.Loop();
  
}
