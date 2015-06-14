{
  gROOT->Reset();
  gROOT->ProcessLine(".L makeLaserRatio.C++");
  gROOT->ProcessLine("makeLaserRatio t");
  t.setFiles("/xrootdfs/cms/local/meridian/EFlow/histories/histories_AlCaPhiSym_Run2012A-v1_RAW_EcalLaserAPDPNRatios_data_20120516_190380_193761_P_p1_tbc_EcalLaserAlphas_EE_sic1_btcp128_1200M_noBsCorr_2012dataBsCorr_","/xrootdfs/cms/local/meridian/EFlow/histories/histories_AlCaPhiSym_Run2012_v4_1200M_forLaserRatio_2012dataBsCorr_");
  t.setOutFile("PhotV2overPrompt");
  gROOT->ProcessLine("t.Loop()");
}
