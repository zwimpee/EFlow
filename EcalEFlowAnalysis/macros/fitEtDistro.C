{
    // Exp + Pow fit model
  TFile *_file0 = TFile::Open("etDistro.root");

  float par0[85];
  float par1[85];
  float par2[85];
  float ieta[85];
  for (int i=1;i<=85;++i)
    {
      ieta[i-1]=i;
      TString histo="h_";
      histo+=i;
      TH1F* h=(TH1F*)_file0->Get(histo);
      TF1 f("f","[0]*((1-[2])*TMath::Exp(x*[1])+[2]*TMath::Power(x,[3]))",0.25/ TMath::CosH(0.0175/2. + (i-1)*0.0175),1.+ 0.25/TMath::CosH(0.0175/2. + (i-1)*0.0175));
      f.SetParameter(0,1e+03);
      f.SetParameter(1,-5);
      f.SetParameter(2,0.1);
      f.SetParameter(3,-3);
      f.SetParLimits(1,-30,-1);
      f.SetParLimits(2,0.,1.);
      f.SetParLimits(3,-30,-1);
      h->Fit(&f,"RB+");
      par0[i-1]=f.GetParameter(1);
      par1[i-1]=f.GetParameter(2);
      par2[i-1]=f.GetParameter(3);
    }

  TGraph par0Graph(85,ieta,par0);
  par0Graph.SetName("expSlope");
  par0Graph.SetTitle("expSlope");
  TGraph par1Graph(85,ieta,par1);
  par1Graph.SetName("powFrac");
  par1Graph.SetTitle("powFrac");
  TGraph par2Graph(85,ieta,par2);
  par2Graph.SetName("powExponent");
  par2Graph.SetTitle("powExponent");
  
  TFile *fOut=new TFile("fitValues.root","RECREATE");
  fOut->cd();
  par0Graph.Write();
  par1Graph.Write();
  par2Graph.Write();
  fOut->Close();
  
}
