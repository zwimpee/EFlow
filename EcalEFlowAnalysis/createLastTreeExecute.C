{
  gROOT->Reset();
  TChain*  c=new TChain("tree_barl");
  //  c->Add("/xrootdfs/cms/local/meridian/EFlow/historyTree/createHistoryOut_1*root");
  //  c->Add("/xrootdfs/cms/local/meridian/EFlow/historyTree_2012/createHistoryOut*root");
  //  c->Add("/xrootdfs/cms/local/meridian/EFlow/historyTree_Run2011_800M/*root");
  c->Add("/xrootdfs/cms/local/meridian/EFlow/historyTree_2012/*root");
  //  c->Add("createHistoryOut_2011B_*.root");
  //  TTree* inputTree=(TTree*)f->Get("tree_barl");
  // gSystem->Load("libJSON.so");
  gSystem->Load("lib/libUtils.so");
  //  gROOT->ProcessLine(".L lumiIntervals.C++");
  gROOT->ProcessLine(".L createLastTree.C++");
  createLastTree t(c);
  //  t.setLumiIntervals("readMap_out_barl_2011AB.root");  
  //t.setLumiIntervals("readMap_out_barl_2012_190456-190679.root");  
  //t.setLumiIntervals("readMap_out_barl_2011AB_800M.root");
  //t.setLumiIntervals("readMap_out_barl_2011AB_800M.root");
  t.setLumiIntervals("readMap_Run2012A_v1_190456_191277.root");
  //  TString outDir="root://pccmsrm27//cms/local/meridian/EFlow/fullHistoryTree/";
  //t.setOutfile("/xrootdfs/cms/local/meridian/EFlow/fullHistoryTree/finalTree_RUN2012_test.root");
  t.setOutfile("/xrootdfs/cms/local/meridian/EFlow/fullHistoryTree/finalTree_RUN2012A_v1_190456_191277.root");
  t.Loop();
  cout<<"all done"<<endl;
}
