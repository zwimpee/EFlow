{
  gROOT->Reset();
  //  TFile* f= TFile::Open("root://pccmsrm27.cern.ch:1094://cms/local/meridian/phiSimmetry/finalOutput_2011A.root");
  TFile* f= TFile::Open("/xrootdfs/cms/local/meridian/EFlow/fullHistoryTree/finalTree_RUN2011AB.root");
  TTree* intree= (TTree*)f->Get("finalTree_barl");
  gSystem->Load("lib/libUtils.so");
  gROOT->ProcessLine(".L makeControlPlots.C++");
  gROOT->ProcessLine("makeControlPlots t(intree)");
  t.setLumiIntervals("readMap_out_barl_2011AB.root");  
  t.setOutfile("/xrootdfs/cms/local/meridian/EFlow/histories/histories_RUN2011AB_bsCorr_kfactors");  
  t.kfactorCorr=true;
  t.kFactorsFile="/afs/cern.ch/work/m/meridian/EFlow/kFactors.root";
  t.kfactor_alpha=1.;
  t.kfactorABCorr=false;
  t.kfactorAB_alpha=1.;
  t.errEtCorr_factor=1.;
  gROOT->ProcessLine("t.Loop()");
}
