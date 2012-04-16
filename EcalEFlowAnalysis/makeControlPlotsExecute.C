{
  gROOT->Reset();
  //  TFile* f= TFile::Open("root://pccmsrm27.cern.ch:1094://cms/local/meridian/phiSimmetry/finalOutput_2011A.root");
  //TFile* f= TFile::Open("/xrootdfs/cms/local/meridian/EFlow/fullHistoryTree/finalTree_RUN2011AB.root");
  //TFile* f= TFile::Open("/xrootdfs/cms/local/meridian/EFlow/fullHistoryTree/finalTree_RUN2011AB_noBscCorr.root");
  //  TFile* f= TFile::Open("/xrootdfs/cms/local/meridian/EFlow/fullHistoryTree/finalTree_RUN2012_test.root");
  TFile* f= TFile::Open("/xrootdfs/cms/local/meridian/EFlow/fullHistoryTree/finalTree_RUN2011_800M.root");
  TTree* intree= (TTree*)f->Get("finalTree_barl");
  gSystem->Load("lib/libUtils.so");
  gROOT->ProcessLine(".L makeControlPlots.C++");
  gROOT->ProcessLine("makeControlPlots t(intree)");
  //t.setLumiIntervals("readMap_out_barl_2011AB.root");  
  //t.setLumiIntervals("readMap_out_barl_2012_190456-190679.root");
  t.setLumiIntervals("readMap_out_barl_2011AB_800M.root");
  //  t.setOutfile("/xrootdfs/cms/local/meridian/EFlow/histories/histories_RUN2011AB_bsCorr_kfactors");  
  //t.setOutfile("/xrootdfs/cms/local/meridian/EFlow/histories/histories_RUN2011AB_noBsCorr_kfactors");  
  //t.setOutfile("/xrootdfs/cms/local/meridian/EFlow/histories/histories_RUN2012_test");  
  t.setOutfile("/xrootdfs/cms/local/meridian/EFlow/histories/histories_RUN2011_800M_");  
  t.kfactorCorr=true;
  t.kFactorsFile="/afs/cern.ch/work/m/meridian/EFlow/kFactors.root";
  t.kfactor_alpha=1.;
  t.kfactorABCorr=false;
  t.kfactorAB_alpha=1.;
  t.errEtCorr_factor=1.;
  //  t.normalizationType="allEB";
  t.normalizationType="ring";
  //Remember for 2011 to remove the first intervals!!
  t.historyNormalizationInterval=10;
  t.ringRefRegion=10;
  gROOT->ProcessLine("t.Loop()");
}
