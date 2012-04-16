{
  gROOT->Reset();
  TFile* f=TFile::Open("readMap_out_barl_2012_190456_190679.root");
  cout<<"ok"<<endl;
  TTree* inputTree=(TTree*)f->Get("outTree_barl");
  cout<<"ok"<<endl;
  gROOT->ProcessLine(".L makeIntervalsPlots.C+");
  cout<<"ok"<<endl;
  makeIntervalsPlots t(inputTree);
  cout<<"ok"<<endl;
  t.setOutfile("intervalPlots_2012_until190949.root");
  cout<<"ok"<<endl;
  t.Loop();
  
}
