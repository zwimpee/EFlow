{
  //  TFile *_file0 = TFile::Open("/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_2011Plus2012_v2_1600M_noBsCorr_2012dataBsCorr_newKFactors_EB_TT.root");
  TFile *_file0 = TFile::Open("/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_2011Plus2012_v2_1600M_noBsCorr_2012dataBsCorr_newKFactors_allEBReg_EB_TT_oneAlphaPerRegion.root");
  TGraphErrors *lc=_file0->Get("lc_ixtal_30001");
  TGraphErrors *et=_file0->Get("etSumOverRef_ixtal_30001");
  TGraphErrors *lc_corr=_file0->Get("lc_ixtal_30001_corr");
  TGraphErrors *et_corr=_file0->Get("etSumOverRef_ixtal_30001_corr");
  lc->Draw("AP");
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  lc->Draw("AP");
  et->Draw("PSAME");
  et_corr->Draw("PSAME");
  lc->Print();
  TLine l(1.30291e+09,1,1.33735e+09,1);
  l.Draw();
  lc_corr->Draw("PSAME");
}
