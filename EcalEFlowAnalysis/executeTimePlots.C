{

TChain inputChain_barl("variablesTree_barl");


std::ifstream fileList("filelist.txt", ios::in);

  if (!fileList.is_open()) {
    cout<<"file not found"<<endl;
    exit(0);
  }
  


int sum=0;
 while(!fileList.eof() && sum<30){
  string nameFile;
  getline(fileList,nameFile);
  inputChain_barl.Add(nameFile.c_str());

  sum++;
  cout<<sum<<endl;
 }






 gROOT->ProcessLine(".L timePlots_barl.C+");
 gROOT->ProcessLine("timePlots_barl t(&inputChain_barl)");
 gROOT->ProcessLine("t.Loop()");

}
