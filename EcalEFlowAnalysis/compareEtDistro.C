{
    // Exp + Pow fit model
  TFile *_file0 = TFile::Open("etDistro_2011A_2.root");
  TFile *_file1 = TFile::Open("etDistro_2011B.root");

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetOptFit(1111111);

  float slope[85];
  float ieta[85];
  for (int i=1;i<=85;++i)
    {
      ieta[i-1]=i;
      TString histo="h_";
      histo+=i;
      TH1F* h=(TH1F*)_file0->Get(histo);
      TH1F* h1=(TH1F*)_file1->Get(histo);
      
      h->Sumw2();
      h1->Sumw2();
      h->Rebin(10);
      h1->Rebin(10);
      h->Scale(1./(float)h->GetEntries());
      h1->Scale(1./(float)h1->GetEntries());

      h->SetLineWidth(2);
      h->SetMaximum(TMath::Max(h->GetMaximum(),h1->GetMaximum())*1.2);
      h->GetXaxis()->SetTitle("Et [GeV]");
      h1->GetXaxis()->SetTitle("Et [GeV]");
      h->Draw("H");
      h1->SetMarkerColor(2);
      h1->SetMarkerStyle(20);
      h1->SetMarkerSize(0.7);
      h1->Draw("EPSAME");

      c1->SaveAs(histo+".png");

      h1->Divide(h);
      h1->SetMarkerColor(1);
      h1->SetMarkerSize(1.2);
      
      TF1 f1("f1","pol1");
      h1->Fit(&f1);
      slope[i-1]=f1.GetParameter(1);
      h1->SetMaximum(1.5);
      h1->SetMinimum(0.5);
      h1->Draw("EP");
      c1->SaveAs(histo+"_ratio.png");
    }

  TGraph slopeGraph(85,ieta,slope);
  slopeGraph.SetName("slope");
  slopeGraph.SetTitle("slope");

  
  TFile *fOut=new TFile("compareEtDistro.root","RECREATE");
  fOut->cd();
  slopeGraph.Write();
  fOut->Close();
}
