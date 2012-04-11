{

  gROOT->Reset();
  TChain*  c=new TChain("tree_barl");
  c->Add("/xrootdfs/cms/local/meridian/EFlow/historyTree/createHistoryOut_1*root");
  //  c->Add("createHistoryOut_2011B_*.root");
  //  TTree* inputTree=(TTree*)f->Get("tree_barl");
  // gSystem->Load("libJSON.so");
  gSystem->Load("lib/libUtils.so");
  //  gROOT->ProcessLine(".L lumiIntervals.C++");
  gROOT->ProcessLine(".L createLastTree.C++");
  createLastTree t(c);
  t.setLumiIntervals("readMap_out_barl_2011AB.root");  
  //  TString outDir="root://pccmsrm27//cms/local/meridian/EFlow/fullHistoryTree/";
  t.setOutfile("/xrootdfs/cms/local/meridian/EFlow/fullHistoryTree/finalTree_RUN2011AB.root");
  t.Loop();
  cout<<"all done"<<endl;
  
}
