{
  TFile *_file0 = TFile::Open("data/beamSpotAsymm_fromData.root");
  double x[85];
  double x_err[85];
  double slope[85];
  double slope_err[85];
  for (int i=1;i<=85;++i)
    {
      TF1* f=_file0->Get(Form("f_bsAsymm_ieta_%d",i));
      x[i-1]=i;
      x_err[i-1]=0;
      slope[i-1]=fabs(f->GetParameter(1));
      slope_err[i-1]=f->GetParError(1);
    }
  TGraphErrors* slopeVsIeta=new TGraphErrors(85,x,slope,x_err,slope_err);
  slopeVsIeta->SetTitle("slopeVsIeta");
  slopeVsIeta->SetMarkerColor(1);
  slopeVsIeta->SetMarkerSize(1.2);
  slopeVsIeta->SetMarkerStyle(20);
  slopeVsIeta->Draw("APL");
  slopeVsIeta->GetXaxis()->SetTitle("i#eta");
  c1->SaveAs("bsPosSlopeVsIeta.png");
}
