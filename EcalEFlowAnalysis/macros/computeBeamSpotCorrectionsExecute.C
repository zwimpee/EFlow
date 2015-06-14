{
  gROOT->ProcessLine(".L computeBeamSpotCorrections.C++");
  //  gSystem->Load("computeBeamSpotCorrectionsFull_C.so");
  TChain *t=new TChain("variablesTree_barl");
  std::ifstream fileList("mcMinBias.list", ios::in);

  if (!fileList.is_open()) {
    cout<<"file not found"<<endl;
    exit(0);
  }

  int sum=0;
  while(!fileList.eof()){
    string nameFile;
    getline(fileList,nameFile);
    t->Add(nameFile.c_str());
    //  cout<<nameFile<<endl;
    sum++;
    cout<<sum<<endl;
  }

  //  t->Add("root://eoscms///eos/cms/store/user/meridian/EFlow/MinBias_TuneZ2_7TeV-pythia6_Summer11-NoPileup_START42_V11-v2/variablesTree_*.root");
  //t->Add("root://pccmsrm27///cms/local/meridian/EFlow/v1/MinBias_TuneZ2_7TeV-pythia6_Summer11-NoPileup_START42_V11-v2/variablesTree_3*.root");
  //t->Add("root://eoscms///eos/cms/store/user/meridian/EFlow/MinBias_TuneZ2_7TeV-pythia6_Summer11-NoPileup_START42_V11-v2/variablesTree_3*.root");
  //  TTree *t=f->Get("variablesTree_barl");
  computeBeamSpotCorrections a(t);
  a.setOutFile("beamSpotCorrections.root");
  a.Loop();
}
