void combineAlphaFits(
		      TString file1="",
		      TString file2="",
		      TString suffix1="",
		      TString suffix2="",
		      float errAlphaCut=999.,
		      float ndof1Cut=0,
		      float ndof2Cut=0,
		      float chi2Cut=999.,
		      int det=0
		      )
{


  float alpha1[61200];
  float alpha2[61200];
  float chi2red1[61200];
  float chi2red2[61200];
  float ndof1[61200];
  float ndof2[61200];
  float err1_alpha[61200];
  float err2_alpha[61200];
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

  for(int jentry=0;jentry<nentries2;++jentry)
    {
      res2->GetEntry(jentry);
      int ii=index-1;
      if (det==1)
	ii=index;
      alpha2[ii]=alpha+delta_alpha;
      err2_alpha[ii]=err_alpha;
      status2[ii]=status;
      bad2[ii]=bad;
      index2[ii]=index;
      ieta2[ii]=ieta;
      iphi2[ii]=iphi;
      sign2[ii]=sign;
      chi2red2[ii]=chi2Min/(float)ndof;
      ndof2[ii]=ndof;
    }  
    
  std::cout << "Read alpha fits for " << nentries2 << " xtals " << std::endl;

  int nXtal=TMath::Max(nentries1,nentries2);


  TH1F diff("diff","diff",500,-10,10);
  TH2F* diffMap;
  if (det==0)
    diffMap=new TH2F("diffMap","diffMap",360,0.5,360.5,171,-85.5,85.5);
  else if (det==1)
    diffMap=new TH2F("diffMap","diffMap",200,0.5,200.5,100,0.5,100.5);
  
  TProfile p("p","p",100,-1,4);

  float alpha1Array[61200];
  float alpha2Array[61200];
  float erralpha1Array[61200];
  float erralpha2Array[61200];
  int nGoodXtals=0;
  for (int ixtal=0;ixtal<61200;++ixtal)
    {
      //      if (bad1[ixtal]==0 && bad2[ixtal]==0 && status1[ixtal]==0 && status2[ixtal]==0 && alpha1[ixtal]>0.01 && alpha2[ixtal]>0.01 && err1_alpha[ixtal]<0.12 && err2_alpha[ixtal]<0.12)
      //      if (bad1[ixtal]==0 && bad2[ixtal]==0 && status1[ixtal]==0 && status2[ixtal]==0 && alpha1[ixtal]>0.01 && alpha2[ixtal]>0.01 && chi2red1[ixtal]<3. && chi2red2[ixtal]<3.)
      if (bad1[ixtal]==0 && bad2[ixtal]==0 && status1[ixtal]==0 && status2[ixtal]==0 && alpha1[ixtal]>0.1 && alpha2[ixtal]>0.1 && ndof1[ixtal]>ndof1Cut && ndof2[ixtal]>ndof2Cut && chi2red1[ixtal]<chi2Cut && chi2red2[ixtal]<chi2Cut && err1_alpha[ixtal]<errAlphaCut && err2_alpha[ixtal]<errAlphaCut)
	{
	  alpha1Array[nGoodXtals]=alpha1[ixtal];
	  alpha2Array[nGoodXtals]=alpha2[ixtal];
	  erralpha1Array[nGoodXtals]=err1_alpha[ixtal];
	  erralpha2Array[nGoodXtals]=err2_alpha[ixtal];
	  //  diff.Fill((alpha1Array[nGoodXtals]-alpha2Array[nGoodXtals])/(sqrt(err1_alpha[ixtal]*err1_alpha[ixtal]+err2_alpha[ixtal]*err2_alpha[ixtal])));
	  diff.Fill((alpha1Array[nGoodXtals]-alpha2Array[nGoodXtals]));
	  p.Fill(alpha1Array[nGoodXtals],alpha2Array[nGoodXtals]);
	  if (det==0)
	    {
	      int etaIndex=(index1[ixtal]-1)/360+1;
	      int phiIndex=(index1[ixtal]-1)%360+1;
	      if (etaIndex>85)
		etaIndex++;
	      else
		etaIndex=86-etaIndex;
	      diffMap->SetBinContent(phiIndex,etaIndex,(alpha1Array[nGoodXtals]-alpha2Array[nGoodXtals])+1.);
	    }
	  else if (det==1)
	    {
	      diffMap->SetBinContent(ieta1[ixtal]+100*sign1[ixtal],iphi1[ixtal],(alpha1Array[nGoodXtals]-alpha2Array[nGoodXtals])+1.);
	    }
	  nGoodXtals++;
	}
    }
  
  std::cout << "Found " << nGoodXtals << " good xtals " << std::endl;

  gStyle->SetOptTitle(0);
  gStyle->SetOptFit(11);
  TH2F a("a","a",10,-1,4,10,-1,4);
  a.GetXaxis()->SetTitle(suffix1);
  a.GetYaxis()->SetTitle(suffix2);
  TGraphErrors corr(nGoodXtals,alpha1Array,alpha2Array,erralpha1Array,erralpha2Array);
  a.SetStats(false);
  a.Draw();
  corr.Draw("PXSAME");
  corr.Fit("pol1","0+");
  TF1* f=(TF1*) corr.GetFunction("pol1");
  f->SetLineColor(2);
  f->SetLineWidth(4);
  f->Draw("SAME");
//   p.SetMarkerColor(kRed);
//   p.SetLineColor(kRed);
//   p.SetMarkerStyle(20);
//   p.SetMarkerSize(1.2);
//   p.Draw("ESAME");
  TString suffix=suffix1+"_vs_"+suffix2;

  c1->SaveAs("plotsFitDiff/corrAlpha_"+suffix+".png");

  diff.Draw();
  diff.Fit("gaus","","",diff.GetMean()-3*diff.GetRMS(),diff.GetMean()+3*diff.GetRMS());
  c1->SaveAs("plotsFitDiff/diffAlpha_"+suffix+".png");

  gStyle->SetOptStat(0);
  diffMap->GetZaxis()->SetRangeUser(0.5,1.5);
  diffMap->Draw("COLZ");
  c1->SaveAs("plotsFitDiff/diffAlphaMap_"+suffix+".png");

}
