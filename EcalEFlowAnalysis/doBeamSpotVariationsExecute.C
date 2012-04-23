{
  gROOT->Reset();

  TFile* f= TFile::Open("/xrootdfs/cms/local/meridian/EFlow/fullHistoryTree/finalTree_AlCaPhiSym_Run2012A-v1_RAW_EcalLaser_20120419_600M_noBsCorr.root");
  TTree* intree= (TTree*)f->Get("finalTree_barl");
  gSystem->Load("lib/libUtils.so");
  gROOT->ProcessLine(".L doBeamSpotVariations.C++");


  //First calculate bsPos corrections
  gROOT->ProcessLine("doBeamSpotVariations t(intree)");
  t.setLumiIntervals("readMap_AlCaPhiSym_Run2012A-v1_RAW_EcalLaser_20120419_600M_noBsCorr.root");
  t.applyBSCorrection=0;
  t.outFileMode=TString("RECREATE");
  t.setOutfile("beamSpotAsymm.root");  
  t.bsInfoFile=TString("/xrootdfs/cms/local/meridian/EFlow/fullHistoryTree/bsInfo_AlCaPhiSym_Run2012A-v1_RAW_EcalLaser_20120419_600M_noBsCorr.root");
  gROOT->ProcessLine("t.Loop()");

  //Then apply bsPos corrections and calculate residual dependence on bsWid
  gROOT->ProcessLine("doBeamSpotVariations t1(intree)");
  t1.setLumiIntervals("readMap_AlCaPhiSym_Run2012A-v1_RAW_EcalLaser_20120419_600M_noBsCorr.root");
  t1.applyBSCorrection=1;
  t1.outFileMode=TString("UPDATE");
  t1.bsCorrectionFile=TString("beamSpotAsymm.root");
  t1.setOutfile("beamSpotAsymm.root");  
  t1.bsInfoFile=TString("/xrootdfs/cms/local/meridian/EFlow/fullHistoryTree/bsInfo_AlCaPhiSym_Run2012A-v1_RAW_EcalLaser_20120419_600M_noBsCorr.root");
  gROOT->ProcessLine("t1.Loop()");


}
