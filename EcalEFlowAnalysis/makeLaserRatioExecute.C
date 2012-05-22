{
  gROOT->Reset();
  gROOT->ProcessLine(".L makeLaserRatio.C++");
  gROOT->ProcessLine("makeLaserRatio t");
  t.setFiles("histories_RUN2011AB_400Mnew_","histories_RUN2011AB_400MnewDUMMY_");
  t.setOutFile("laserRatioTest.root");
  gROOT->ProcessLine("t.Loop()");
}
