{
  gROOT->Reset();
  TChain inputChain_barl("variablesTree");
  
  std::ifstream fileList("test.txt", ios::in);
  
  if (!fileList.is_open()) {
    cout<<"file not found"<<endl;
    exit(0);
  }
  
  int sum=0;
  while(!fileList.eof()){
    string nameFile;
    getline(fileList,nameFile);
    inputChain_barl.Add(nameFile.c_str());
    //  cout<<nameFile<<endl;
    sum++;
    cout<<sum<<endl;
  }
  gSystem->Load("lib/libUtils.so");
//   gROOT->ProcessLine(".L lumiIntervals.C+");
//   gROOT->ProcessLine(".L alcaPhiSym_tree_v3.C+");
//   gROOT->ProcessLine(".L createHistoryPlots.C+");
  createHistoryPlots t(&inputChain_barl);
  t.setLumiIntervals("readMap_out_barl_2011AB.root");
  t.setJSON("Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt");
  t.setOutfile("createHistoryOut_test.root");
  t.Loop();
}
