void fitHistoriesExecute()
{
  gROOT->ProcessLine(".L fitHistories.C+");

//   d888888b d8b   db d888888b d88888b d8888b. db    db  .d8b.  db            .o88b.  .d88b.  d8b   db d88888b d888888b  d888b  db    db d8888b.  .d8b.  d888888b d888888b  .d88b.  d8b   db 
//     `88'   888o  88 `~~88~~' 88'     88  `8D 88    88 d8' `8b 88           d8P  Y8 .8P  Y8. 888o  88 88'       `88'   88' Y8b 88    88 88  `8D d8' `8b `~~88~~'   `88'   .8P  Y8. 888o  88 
//      88    88V8o 88    88    88ooooo 88oobY' Y8    8P 88ooo88 88           8P      88    88 88V8o 88 88ooo      88    88      88    88 88oobY' 88ooo88    88       88    88    88 88V8o 88 
//      88    88 V8o88    88    88~~~~~ 88`8b   `8b  d8' 88~~~88 88           8b      88    88 88 V8o88 88~~~      88    88  ooo 88    88 88`8b   88~~~88    88       88    88    88 88 V8o88 
//     .88.   88  V888    88    88.     88 `88.  `8bd8'  88   88 88booo.      Y8b  d8 `8b  d8' 88  V888 88        .88.   88. ~8~ 88b  d88 88 `88. 88   88    88      .88.   `8b  d8' 88  V888 
//   Y888888P VP   V8P    YP    Y88888P 88   YD    YP    YP   YP Y88888P       `Y88P'  `Y88P'  VP   V8P YP      Y888888P  Y888P  ~Y8888P' 88   YD YP   YP    YP    Y888888P  `Y88P'  VP   V8P 


  int intStart[6];
  int intEnd[6];

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

//   // 1600M RUN2011 Intervals 
//     int nDelta=4;
//     int excl_int=2;

//     intStart[0]=2-2;
//     intEnd[0]=7-2;

//     intStart[1]=8-2;
//     intEnd[1]=20-2;

//     intStart[2]=21-2;
//     intEnd[2]=32-2;

//     intStart[3]=33-2;
//     intEnd[3]=61-2;
  

  
  // 1600M RUN2012 alphaFit_v2
//   int nDelta=3;
//   int excl_int=1;
  
//   intStart[0]=1-1;
//   intEnd[0]=16-1;
  
//   intStart[1]=17-1;
//   intEnd[1]=28-1;
  
//   intStart[2]=29-1;
//   intEnd[2]=999;

// 1600M RUN2012 alphaFit_EE
//   int nDelta=4;
//   int excl_int=0;
  
//   intStart[0]=0;
//   intEnd[0]=16;
//   //  
//   intStart[1]=17;
//   intEnd[1]=27;
//   //  
//   intStart[2]=28;
//   intEnd[2]=40;
  
//   intStart[3]=41;
//   intEnd[3]=117;

  //
  // 1600M RUN2011+2012 alphaFit_v2
//   int nDelta=6;
//   int excl_int=2;
  
//   intStart[0]=2-2;
//   intEnd[0]=10-2;
  
//   intStart[1]=11-2;
//   intEnd[1]=30-2;
  
//   intStart[2]=31-2;
//   intEnd[2]=48-2;
  
//   intStart[3]=49-2;
//   intEnd[3]=93-2;
  
//   intStart[4]=94-2;
//   intEnd[4]=121-2;
  
//   intStart[5]=122-2;
//   intEnd[5]=218-2;

// 3200M RUN2012B 
//      int nDelta=1;
//      int excl_int=0;

//      intStart[0]=0;
//      intEnd[0]=125;

// 12000M RUN2012B 
  int nDelta=1;
  int excl_int=0;
  int nPoints=36;
  
  intStart[0]=0;
  intEnd[0]=35;


// 3200M RUN2012B onlyClusters 
//  int nDelta=1;
//  int excl_int=0;
//  int nPoints=21;
//  
//  intStart[0]=0;
//  intEnd[0]=21;



// 6400M RUN2012B 
//  int nDelta=1;
//  int excl_int=0;
//  int nPoints=66;
//
//  intStart[0]=0;
//  intEnd[0]=65;


// 12000M RUN2012C (first part)
//   int nDelta=1;
//   int excl_int=0;
//   int nPoints=26;
//
//   intStart[0]=0;
//   intEnd[0]=25;

// 12000M RUN2012C (full period)
// int nDelta=2;
// int excl_int=0;
// int nPoints=99999;
//
// intStart[0]=0;
// intEnd[0]=26;
//
// intStart[1]=0;
// intEnd[1]=999;

// 3200M RUN2012C 
//    int nDelta=1;
//    int excl_int=0;
  
//    intStart[0]=0;
//    intEnd[0]=73;

//   d888888b d8b   db d888888b d888888b      db    db  .d8b.  db      db    db d88888b .d8888. 
//     `88'   888o  88   `88'   `~~88~~'      88    88 d8' `8b 88      88    88 88'     88'  YP 
//      88    88V8o 88    88       88         Y8    8P 88ooo88 88      88    88 88ooooo `8bo.   
//      88    88 V8o88    88       88         `8b  d8' 88~~~88 88      88    88 88~~~~~   `Y8b. 
//     .88.   88  V888   .88.      88          `8bd8'  88   88 88booo. 88b  d88 88.     db   8D 
//   Y888888P VP   V8P Y888888P    YP            YP    YP   YP Y88888P ~Y8888P' Y88888P `8888Y' 


//  TString region="xtal";
   TString region="TT";
//  TString region="eextal";
//  TString region="EESC";
  int nxtals;
  if (region=="xtal")
    nxtals=1;
  else if (region=="eextal")
    nxtals=1;
  else if (region=="TT")
    nxtals=25;
  else if (region=="HV")
    nxtals=50;
  else if (region=="EESC")
    nxtals=25;

  int fixedPar[100];
  double startValue[100];
  double stepValue[100];

   for (int i=0;i<nDelta;++i)
     {
       fixedPar[i]=1;
       startValue[i]=0.;
       stepValue[i]=0.000001;
     }

   for (int i=nDelta;i<nDelta+nxtals;++i)
     {
       fixedPar[i]=0;
       startValue[i]=0.;
       stepValue[i]=0.001;
     }
  
   fitHistories(nDelta,&intStart[0],&intEnd[0],excl_int,nPoints,
	       &fixedPar[0],&startValue[0],&stepValue[0],

	       // error scale factor (to make chi2/ndof 1)
	       1.2, //this value for data 
	       //1., //this value for toys 
	       1, // one single alpha for the entire region
	       //toy parameters
	       0, // 0 to set it off
	       0, // to shift around data point
	       0., // delta_alpha shift
	       0., // laser normalization smearing
	       0., //error on each point 

	       //iterative fit parameters
	       1, //switch for iterative fit
	       6, //chi2Cut
	       1.5, //minChi2Cut to be reached by the fit 
	       //region
	       region,

	       //Input && Output files

//                  22   000   11   11  
//                 2  2 0  00 111  111  
//                   2  0 0 0  11   11  
//                  2   00  0  11   11  
//                 2222  000  11l1 11l1 

//     	       "/xrootdfs/cms/local/meridian/EFlow/histories/histories_RUN2011_GR_R_42_V24_AlphaTest1_1600M_2012dataBsCorr_newKfactorsEB_ixtal.root",
//     	       "/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_RUN2011_GR_R_42_V24_AlphaTest1_1600M_EB.root");

// 	       "/xrootdfs/cms/local/meridian/EFlow/histories/histories_RUN2011_GR_R_42_V24_rusAlpha118_1600M_2012dataBsCorr_newKfactorsEB_ixtal.root",
// 	       "/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_RUN2011_GR_R_42_V24_rusAlpha118_1600M_EB_onlyDelta.root");

//        	       "/xrootdfs/cms/local/meridian/EFlow/histories/histories_RUN2011_GR_R_42_V24_rusAlpha118_1600M_2012dataBsCorr_newKfactorsEB_allEBRef_ixtalEndc.root",
//        	       "/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_RUN2011_GR_R_42_V24_rusAlpha118_1600M_allEBRef_EE_normSmeared_v3.root");

//                 22   000   11   22  
//                2  2 0  00 111  2  2 
//                  2  0 0 0  11    2  
//                 2   00  0  11   2   
//                2222  000  11l1 2222 

//       	       "/xrootdfs/cms/local/meridian/EFlow/histories/histories_AlCaPhiSym_Run2012A-v1_RAW_EcalLaserAPDPNRatios_data_20120516_190380_193761_P_p1_tbc_EcalLaserAlphas_EE_sic1_btcp128_1600M_noBsCorr_2012dataBsCorr_ixtal.root",
//       	       "/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_AlCaPhiSym_Run2012A-v1_RAW_EcalLaserAPDPNRatios_data_20120516_190380_193761_P_p1_tbc_EcalLaserAlphas_EE_sic1_btcp128_1600M_fixed.root");
	       
//       	       "/xrootdfs/cms/local/meridian/EFlow/histories/histories_AlCaPhiSym_Run2012A-v1_RAW_EcalLaserAPDPNRatios_data_20120516_190380_193761_P_p1_tbc_EcalLaserAlphas_EE_sic1_btcp128_1600M_noBsCorr_2012dataBsCorr_ixtal.root",
//       	       "/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_AlCaPhiSym_Run2012A-v1_RAW_EcalLaserAPDPNRatios_data_20120516_190380_193761_P_p1_tbc_EcalLaserAlphas_EE_sic1_btcp128_1600M.root");

//        	       "/xrootdfs/cms/local/meridian/EFlow/histories/histories_AlCaPhiSym_Run2012_v4_1600M_alphaFit_2012dataBsCorr_ixtal.root",
//        	       "/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_AlCaPhiSym_Run2012_v4_1600M_alphaFit_2012dataBsCorr_EB_fixed.root");
	       
//  	       "/xrootdfs/cms/local/meridian/EFlow/histories/histories_AlCaPhiSym_Run2012_EcalLaserAPDPNRatios_data_20120516_190380_193761_P_p1_tbc_EcalLaserAlphas_EE_sic1_btcp116_1600M_noBsCorr_2012dataBsCorr_ixtalEndc.root",
//  	       "/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012_EcalLaserAPDPNRatios_data_20120516_190380_193761_P_p1_tbc_EcalLaserAlphas_EE_sic1_btcp116_1600M_noBsCorr_2012dataBsCorr_EE_v1.root");
  
//   	       "/xrootdfs/cms/local/meridian/EFlow/histories/histories_AlCaPhiSym_Run2012B_v5_3200M_alphaFit_2012dataBsCorr_ixtal.root",
//        	       "/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_3200M_alphaFit_2012dataBsCorr_EB_TT_iterative.root");

     	       "/xrootdfs/cms/local/meridian/EFlow/histories/histories_AlCaPhiSym_Run2012B_v6_12000M_alphaFit_newLaser_2012dataBsCorr_KFactorsVsTime_ixtal.root",
//      	       "/xrootdfs/cms/local/meridian/EFlow/histories/histories_AlCaPhiSym_Run2012B_v6_12000M_alphaFit_newLaser_2012dataBsCorr_ixtalEndc.root",
   	       "/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v6_12000M_alphaFit_newLaser_2012dataBsCorr_KFactorsVsTime_EB_TT_iterative_fixedDelta.root");
//   	       "/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v6_12000M_alphaFit_newLaser_2012dataBsCorr_KFactorsPerRing_EE_iterative_fixedDelta.root");

//     	       "/xrootdfs/cms/local/meridian/EFlow/histories/histories_AlCaPhiSym_Run2012B_v6fromClusters_3200M_alphaFit_newLaser_2012dataBsCorr_ixtal.root",
//      	       "/xrootdfs/cms/local/meridian/EFlow/histories/histories_AlCaPhiSym_Run2012B_v6_12000M_alphaFit_newLaser_2012dataBsCorr_ixtalEndc.root",
//   	       "/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v6fromClusters_3200M_alphaFit_newLaser_2012dataBsCorr_EB_iterative.root");

// 		"/xrootdfs/cms/local/meridian/EFlow/histories/histories_AlCaPhiSym_Run2012C_v6_12000M_alphaFit_newLaser_bugFix_2012dataBsCorr_KFactorsPerRing_ixtal.root",
// 		"/xrootdfs/cms/local/meridian/EFlow/histories/histories_AlCaPhiSym_Run2012C_v6_12000M_alphaFit_newLaser_bugFix_2012dataBsCorr_KFactorsPerRing_ixtalEndc.root",
		//		"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012C_v6_12000M_alphaFit_newLaser_2012dataBsCorr_EB_TT_iterative.root");
		//		"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012C_v6_12000M_alphaFit_newLaser_bugFix_2012dataBsCorr_KFactorsPerRing_EB_TT_iterative_fixedDelta.root");
		//		"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012C_v6_12000M_alphaFit_newLaser_bugFix_2012dataBsCorr_KFactorsPerRing_EE_iterative_fixedDelta.root");
   //		"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012C_v6_12000M_alphaFit_newLaser_2012dataBsCorr_EE_iterative.root");

//    	       "/xrootdfs/cms/local/meridian/EFlow/histories/histories_AlCaPhiSym_Run2012B_v6_6400M_alphaFit_newLaser_2012dataBsCorr_ixtal.root",
//	       // 	       "/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v6_6400M_alphaFit_newLaser_2012dataBsCorr_EB_TT_iterative.root");
// 	       "/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v6_6400M_alphaFit_newLaser_2012dataBsCorr_EB_iterative.root");

//    	       "/xrootdfs/cms/local/meridian/EFlow/histories/histories_AlCaPhiSym_Run2012B_v6_12000M_alphaFit_newLaser_2012dataBsCorr_ixtal.root",
// 	       "/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v6_12000M_alphaFit_newLaser_2012dataBsCorr_EB_iterative.root");
  
//  	       "/xrootdfs/cms/local/meridian/EFlow/histories/histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_ixtal.root",
//       	       "/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_HV_fixedDelta_iterative.root");

// 	       "/xrootdfs/cms/local/meridian/EFlow/histories/histories_AlCaPhiSym_Run2012C_v5_3200M_alphaFit_2012dataBsCorr_ixtal.root",
//      	       "/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012C_v5_3200M_alphaFit_2012dataBsCorr_EB_TT_fixedDelta.root");

//    	       "/xrootdfs/cms/local/meridian/EFlow/histories/histories_2011Plus2012_v3_1600M_noBsCorr_2012dataBsCorr_newKFactors_allEBRef_ixtal.root",
//    	       "/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_2011Plus2012_v3_1600M_noBsCorr_2012dataBsCorr_newKFactors_allEBReg_EB_TT_normSmeared_v2.root");
  
//        	       "/xrootdfs/cms/local/meridian/EFlow/histories/histories_AlCaPhiSym_Run2012_v4_1600M_alphaFitEE_2012dataBsCorr_ixtalEndc.root",
//        	       "/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_AlCaPhiSym_Run2012_v4_1600M_alphaFitEE_2012dataBsCorr_EE_fixed.root");
}
