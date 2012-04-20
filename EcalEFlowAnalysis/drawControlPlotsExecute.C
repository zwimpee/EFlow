{
  gROOT->Reset();
  gROOT->ProcessLine(".L drawControlPlots.C+");

  TString prefix="root://pccmsrm27///cms/local/meridian/EFlow/histories/histories_RUN2011_800M__";
  int X0=1300000000+86400*25;
  int X1=1320105600+86400*4;
  float axisLower=0.93;
  float axisUp=1.05;
  float axisLowerXtal=0.9;
  float axisUpXtal=1.1;

  drawControlPlots(prefix,1,0,0,0,1,0,X0,X1,axisLower,axisUp,axisLowerXtal,axisUpXtal);
  drawControlPlots(prefix,1,0,0,0,1,1,X0,X1,axisLower,axisUp,axisLowerXtal,axisUpXtal);
  drawControlPlots(prefix,0,1,0,1,1,1,X0,X1,axisLower,axisUp,axisLowerXtal,axisUpXtal);
  drawControlPlots(prefix,0,0,1,1,1,1,X0,X1,axisLower,axisUp,axisLowerXtal,axisUpXtal);
  drawControlPlots(prefix,0,1,0,0,0,1,X0,X1,axisLower,axisUp,axisLowerXtal,axisUpXtal);
  drawControlPlots(prefix,0,0,1,0,0,1,X0,X1,axisLower,axisUp,axisLowerXtal,axisUpXtal);
}
