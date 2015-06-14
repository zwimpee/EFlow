void combineAlphaFits_systStudy(
		      TString file1="",
		      TString file2="",
		      TString suffix1="",
		      TString suffix2="",
		      int det=0
		      )
{
  float alpha1[61200];
  float alpha2[61200];
  float alphaStart1[61200];
  float alphaStart2[61200];
  float deltaAlpha1[61200];
  float deltaAlpha2[61200];
  float chi2red1[61200];
  float chi2red2[61200];
  float ndof1[61200];
  float ndof2[61200];
  float err1_alpha[61200];
  float err2_alpha[61200];
  float ic[61200];
  int status1[61200];
  int status2[61200];
  int bad1[61200];
  int bad2[61200];
  int index1[61200];
  int index2[61200];
  int ieta1[61200];
  int ieta2[61200];
  int iphi1[61200];
  int iphi2[61200];
  int sign1[61200];
  int sign2[61200];


  TFile *f1=TFile::Open(file1);
  if (!f1)
    {
      std::cout << "Cannot open " << file1 << std::endl;
      exit(-1);
    }
  TTree* res1= (TTree*)f1->Get("fitResults");
  
  int status,index,bad,ieta,iphi,sign;
  double alpha,delta_alpha,err_alpha,chi2Min,ndof;
  
  TBranch *b_ieta=res1->GetBranch("ieta");
  TBranch *b_iphi=res1->GetBranch("iphi");
  TBranch *b_sign=res1->GetBranch("sign");
  TBranch *b_alpha=res1->GetBranch("alpha0");
  TBranch *b_dalpha=res1->GetBranch("delta_alpha");
  TBranch *b_erralpha=res1->GetBranch("err_alpha");
  TBranch *b_status=res1->GetBranch("status");
  TBranch *b_bad=res1->GetBranch("badXtal");
  TBranch *b_index=res1->GetBranch("index");
  TBranch *b_chi2=res1->GetBranch("chi2Min");
  TBranch *b_ndof=res1->GetBranch("ndof");
  
  res1->SetBranchAddress("ieta", &ieta, &b_ieta);
  res1->SetBranchAddress("iphi", &iphi, &b_iphi);
  res1->SetBranchAddress("sign", &sign, &b_sign);
  res1->SetBranchAddress("alpha0", &alpha, &b_alpha);
  res1->SetBranchAddress("delta_alpha", &delta_alpha, &b_dalpha);
  res1->SetBranchAddress("err_alpha", &err_alpha, &b_erralpha);
  res1->SetBranchAddress("status", &status, &b_status);
  res1->SetBranchAddress("badXtal", &bad, &b_bad);
  res1->SetBranchAddress("index", &index, &b_index);
  res1->SetBranchAddress("chi2Min", &chi2Min, &b_chi2);
  res1->SetBranchAddress("ndof", &ndof, &b_ndof);
  
  int nentries1 = res1->GetEntries();

  for(int jentry=0;jentry<nentries1;++jentry)
    {
      res1->GetEntry(jentry);
      int ii=index-1;
      if (det==1)
	ii=index;

      alpha1[ii]=alpha+delta_alpha;
      alphaStart1[ii]=alpha;
      deltaAlpha1[ii]=delta_alpha;
      err1_alpha[ii]=err_alpha;
      status1[ii]=status;
      bad1[ii]=bad;
      index1[ii]=index;
      ieta1[ii]=ieta;
      iphi1[ii]=iphi;
      sign1[ii]=sign;
      chi2red1[ii]=chi2Min/(float)ndof;
      ndof1[ii]=ndof;
    }
    
  std::cout << "Read alpha fits for " << nentries1 << " xtals " << std::endl;



  TFile *f2=TFile::Open(file2);
  if (!f2)
    {
      std::cout << "Cannot open " << file2 << std::endl;
      exit(-1);
    }
  TTree* res2= (TTree*)f2->Get("fitResults");
  

  TBranch *b_ieta=res2->GetBranch("ieta");
  TBranch *b_iphi=res2->GetBranch("iphi");
  TBranch *b_sign=res2->GetBranch("sign");
  TBranch *b_alpha=res2->GetBranch("alpha0");
  TBranch *b_dalpha=res2->GetBranch("delta_alpha");
  TBranch *b_erralpha=res2->GetBranch("err_alpha");
  TBranch *b_status=res2->GetBranch("status");
  TBranch *b_bad=res2->GetBranch("badXtal");
  TBranch *b_index=res2->GetBranch("index");
  TBranch *b_chi2=res2->GetBranch("chi2Min");
  TBranch *b_ndof=res2->GetBranch("ndof");
  
  res2->SetBranchAddress("ieta", &ieta, &b_ieta);
  res2->SetBranchAddress("iphi", &iphi, &b_iphi);
  res2->SetBranchAddress("sign", &sign, &b_sign);
  res2->SetBranchAddress("alpha0", &alpha, &b_alpha);
  res2->SetBranchAddress("delta_alpha", &delta_alpha, &b_dalpha);
  res2->SetBranchAddress("err_alpha", &err_alpha, &b_erralpha);
  res2->SetBranchAddress("status", &status, &b_status);
  res2->SetBranchAddress("badXtal", &bad, &b_bad);
  res2->SetBranchAddress("index", &index, &b_index);
  res2->SetBranchAddress("chi2Min", &chi2Min, &b_chi2);
  res2->SetBranchAddress("ndof", &ndof, &b_ndof);
  
  int nentries2 = res2->GetEntries();

  TFile *_file2 = TFile::Open("EBMap.root");  
  TH2F* icMap=(TH2F*) _file2->Get("EBMapCoeff1");

  for(int jentry=0;jentry<nentries2;++jentry)
    {
      res2->GetEntry(jentry);
      int ii=index-1;
      if (det==1)
	ii=index;
      alpha2[ii]=alpha+delta_alpha;
      alphaStart2[ii]=alpha;
      deltaAlpha2[ii]=delta_alpha;
      err2_alpha[ii]=err_alpha;
      status2[ii]=status;
      bad2[ii]=bad;
      index2[ii]=index;
      ieta2[ii]=ieta;
      iphi2[ii]=iphi;
      sign2[ii]=sign;
      chi2red2[ii]=chi2Min/(float)ndof;
      ndof2[ii]=ndof;
      ic[ii]=icMap->GetBinContent(iphi,ieta+86);
    }  
    
  std::cout << "Read alpha fits for " << nentries2 << " xtals " << std::endl;

  int nXtal=TMath::Max(nentries1,nentries2);



  TFile *outFile= new TFile("outAlpha_"+suffix1+"_"+suffix2+".root","RECREATE");
  TTree* outTree=new TTree("combineAlphaTree","");
  outTree->Branch("alpha1",&alpha1, "alpha1[61200]/F");
  outTree->Branch("alpha2",&alpha2, "alpha2[61200]/F");
  outTree->Branch("alphaStart1",&alphaStart1, "alphaStart1[61200]/F");
  outTree->Branch("alphaStart2",&alphaStart2, "alphaStart2[61200]/F");
  outTree->Branch("deltaAlpha1",&deltaAlpha1, "deltaAlpha1[61200]/F");
  outTree->Branch("deltaAlpha2",&deltaAlpha2, "deltaAlpha2[61200]/F");
  outTree->Branch("chi2red1",&chi2red1, "chi2red1[61200]/F");
  outTree->Branch("chi2red2",&chi2red2, "chi2red2[61200]/F");
  outTree->Branch("ndof1",&ndof1, "ndof1[61200]/F");
  outTree->Branch("ndof2",&ndof2, "ndof2[61200]/F");
  outTree->Branch("errAlpha1",&err1_alpha, "errAlpha1[61200]/F");
  outTree->Branch("errAlpha2",&err2_alpha, "errAlpha2[61200]/F");
  outTree->Branch("status1",&status1, "status1[61200]/I");
  outTree->Branch("status2",&status2, "status2[61200]/I");
  outTree->Branch("bad1",&bad1, "bad1[61200]/I");
  outTree->Branch("bad2",&bad2, "bad2[61200]/I");
  outTree->Branch("ic",&ic, "ic[61200]/F");
  outTree->Branch("index",&index1, "index[61200]/I");
  outTree->Branch("ieta",&ieta1, "ieta[61200]/I");
  outTree->Branch("iphi",&iphi1, "iphi[61200]/I");
  outTree->Branch("sign",&sign1, "sign[61200]/I");

  outTree->Fill();
  outFile->Write();
  outFile->Close();


}
