{
  gROOT->ProcessLine(".L fitHistories.C+");
  int nDelta=4;
  int intStart[4];
  int intEnd[4];

  //#include fit_RUN2011_16Jan_3200M.conf
  intStart[0]=1-1;
  intEnd[0]=3-1;
  
  intStart[1]=4-1;
  intEnd[1]=10-1;
  
  intStart[2]=11-1;
  intEnd[2]=17-1;
  
  intStart[3]=18-1;
  intEnd[3]=31-1;
  
  int excl_int=1;

  // 1600M RUN2011 Intervals 
  //#include fit_RUN2011_16Jan_1600M.conf
  //   intStart[0]=2-2;
  //   intEnd[0]=7-2;
  
  //   intStart[1]=8-2;
  //   intEnd[1]=20-2;
  
  //   intStart[2]=21-2;
  //   intEnd[2]=32-2;
  
  //   intStart[3]=33-2;
  //   intEnd[3]=60-2;
  
  //   int excl_int=2;

  fitHistories(nDelta,&intStart[0],&intEnd[0],excl_int,
	       "/xrootdfs/cms/local/meridian/EFlow/histories/histories_RUN2011_Jan16_3200M_2012dataBsCorr_ixtal.root",
	       "/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_RUN2011_16Jan_3200M_EB.root");
}
