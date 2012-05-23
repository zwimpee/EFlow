{
  gROOT->Reset();
  gROOT->ProcessLine(".L makeLaserRatio.C++");
  gROOT->ProcessLine("makeLaserRatio t");
  t.setFiles("/xrootdfs/cms/local/meridian/EFlow/histories/histories_AlCaPhiSym_Run2012A-v1_RAW_EcalLaserAPDPNRatios_data_20120512_189410_193761_cbt_EcalLaserAlphas_EE_sic1_btcp128_1200M_noBsCorr_2012dataBsCorr_","/xrootdfs/cms/local/meridian/EFlow/histories/histories_AlCaPhiSym_Run2012A-v1_RAW_v4_800M_noBsCorr_2012dataBsCorr_");
  t.setOutFile("PhotonicsV1overNormal_");
  gROOT->ProcessLine("t.Loop()");
}
