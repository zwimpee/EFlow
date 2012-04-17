{
  gROOT->Reset();
  TFile* f=TFile::Open("readMap_Run2012A_v1_190456_191277.root");
  cout<<"ok"<<endl;
  TTree* inputTree=(TTree*)f->Get("outTree_barl");
  cout<<"ok"<<endl;
  gROOT->ProcessLine(".L makeIntervalsPlots.C+");
  cout<<"ok"<<endl;
  makeIntervalsPlots t(inputTree);
  cout<<"ok"<<endl;
  t.setOutfile("intervalPlots_2012_readMap_Run2012A_v1_190456_191277.root");
  cout<<"ok"<<endl;
  t.Loop();
  
}
