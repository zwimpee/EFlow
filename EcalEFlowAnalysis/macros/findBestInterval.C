{
  //  TFile *f=TFile::Open("root://eoscms//eos/cms/store/group/alca_ecalcalib/micheli/phisym/Run2011A_1/variablesTree_49_1_ct5.root");
  //  TFile *f=TFile::Open("root://eoscms//eos/cms/store/group/alca_ecalcalib/micheli/phisym/Run2011B_4/variablesTree_83_1_SCE.root");
  //  TFile *f=TFile::Open("root://eoscms//eos/cms/store/group/alca_ecalcalib/micheli/phisym/Run2011A_3/variablesTree_73_1_l2S.root");
  TFile *f=TFile::Open("root://eoscms//eos/cms/store/group/alca_ecalcalib/micheli/phisym/Run2011A_2/variablesTree_49_1_0S8.root");

  TH1F* h[85];
  float bestCut[85];
  float ieta[85];
  for (int i=1;i<=85;++i)
    {
      TString histoName="h_";
      histoName+=i;
      h[i-1]=new TH1F(histoName,histoName,600,0.,1.5);
      TString drawString="et_barl>>h_";
      drawString+=i;
      TString cutString="ieta==";
      cutString+=i;
      variablesTree_barl->Draw(drawString,cutString);
      bestCut[i-1]=-1.;
      //      ieta[i-1]=0.0175/2 + (i-1)*0.0175;
      ieta[i-1]=i;
      for (int ibin=40;ibin<160;++ibin)
	{
	  if (h[i-1]->Integral(1,ibin)>0)
	    if (fabs(h[i-1]->Integral(ibin+1,h[i-1]->GetNbinsX())/h[i-1]->Integral(1,ibin)-1.)<0.05)
	      {
		bestCut[i-1]=(float)ibin/400.;
		std::cout << "ieta " << i << " bestCut " << bestCut[i-1] << std::endl;
		break;
	      }
	  
	}
    }
  TGraph graphBestCut(85,ieta,bestCut);
  graphBestCut.SetName("graphBestCut");
  graphBestCut.SaveAs("graphBestCut.root");
  
  TFile *fOut=new TFile("etDistro.root","RECREATE");
  fOut->cd();
  for (int i=0;i<85;++i)
    h[i]->Write();
  fOut->Close();
}
