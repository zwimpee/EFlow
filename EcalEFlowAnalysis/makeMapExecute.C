{
  gROOT->Reset();
  TChain inputChain_barl("variablesTree_barl");
  

  std::ifstream fileList("filelist2011A_6.txt", ios::in);
  
  if (!fileList.is_open()) {
    cout<<"file not found"<<endl;
    exit(0);
  }
  
  
  
  int sum=0;
  while(!fileList.eof()){
    string nameFile;
    getline(fileList,nameFile);
  inputChain_barl.Add(nameFile.c_str());
  
  sum++;
  cout<<sum<<endl;
  }
  
  
  

  
  
  gROOT->ProcessLine(".L makeMap.C+");
  gROOT->ProcessLine("makeMap t(&inputChain_barl)");
  gROOT->ProcessLine("t.Loop()");
  
}
