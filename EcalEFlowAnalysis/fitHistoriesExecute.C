void fitHistoriesExecute()
{
  gROOT->ProcessLine(".L fitHistories.C+");
//   int nDelta=4;
//   int intStart[4];
//   int intEnd[4];

  int nDelta=4;
  int intStart[4];
  int intEnd[4];

  //#include fit_RUN2011_16Jan_3200M.conf
//  intStart[0]=1-1;
//  intEnd[0]=3-1;
//  
//  intStart[1]=4-1;
//  intEnd[1]=10-1;
//  
//  intStart[2]=11-1;
//  intEnd[2]=17-1;
//  
//  intStart[3]=18-1;
//  intEnd[3]=31-1;
//  
//  int excl_int=1;

  // 1600M RUN2011 Intervals 
  //#include fit_RUN2011_16Jan_1600M.conf
  intStart[0]=2-2;
  intEnd[0]=7-2;
  
  intStart[1]=8-2;
  intEnd[1]=20-2;
  
  intStart[2]=21-2;
  intEnd[2]=32-2;
  
  intStart[3]=33-2;
  intEnd[3]=60-2;
  
  int excl_int=2;
  
//   bool fixedPar[5];
//   double startValue[5];
//   double stepValue[5];
  
//   for (int i=0;i<nDelta;++i)
//     {
//       fixedPar[i]=false;
//       startValue[i]=0.;
//       stepValue[i]=0.000001;
//     }

//   fixedPar[nDelta]=false;
//   startValue[nDelta]=0.;
//   stepValue[nDelta]=0.001;
  int nxtals;

  TString region="HV";

  if (region=="xtal")
    nxtals=1;
  else if (region=="TT")
    nxtals=25;
  else if (region=="HV")
    nxtals=50;

  int fixedPar[54];
  double startValue[54];
  double stepValue[54];

   for (int i=0;i<nDelta;++i)
     {
       fixedPar[i]=0;
       startValue[i]=0.;
       stepValue[i]=0.000001;
     }

   for (int i=nDelta;i<54;++i)
     {
       fixedPar[i]=0;
       startValue[i]=0.;
       stepValue[i]=0.001;
     }
  
  fitHistories(nDelta,&intStart[0],&intEnd[0],excl_int,
	       &fixedPar[0],&startValue[0],&stepValue[0],
	       region,
	       "/xrootdfs/cms/local/meridian/EFlow/histories/histories_RUN2011_Jan16_1600M_2012dataBsCorr_newKfactorsEB_ixtal.root",
	       "/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_RUN2011_16Jan_1600M_EB_newKfactors_test.root");
}
