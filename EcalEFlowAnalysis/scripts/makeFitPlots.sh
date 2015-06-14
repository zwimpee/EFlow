#!/bin/bash


#TT iterative (reference)
#rm -f plotsFit
#mkdir -p plotsFit_Run2012B_v5_12000M_EB_TT_iterative
#ln -s plotsFit_Run2012B_v5_12000M_EB_TT_iterative plotsFit
#root -l -b -q drawFitResults.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_TT_iterative.root\",0,0.12,0,0,\"ndof\>850\"\)
#root -l -b -q drawCorrelations.C\(\"BTCP\",\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_TT_iterative.root\",0.12,850\)
#mkdir -p plotsFitDiff
#root -l -b -q combineAlphaFits.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_TT_iterative.root\",\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_TT_fixedDelta_iterative.root\",\"12000M_EB_TT_iterative_vs_12000M_EB_TT_fixedDelta_iterative\",0.12,850,850,2.3\)
#root -l -b -q combineAlphaFits.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_TT_iterative.root\",\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_iterative.root\",\"12000M_EB_TT_iterative_vs_12000M_EB_iterative\",0.12,850,36,2.3\)
#root -l -b -q combineAlphaFits.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_TT_iterative.root\",\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_HV_iterative.root\",\"12000M_EB_TT_iterative_vs_12000M_EB_HV_iterative\",0.12,850,1700,2.3\)
#root -l -b -q combineAlphaFits.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_TT_iterative.root\",\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_TT.root\",\"12000M_EB_TT_iterative_vs_12000M_EB_TT\",0.12,850,850,2.3\)
#
#rm -f plotsFit
#mkdir -p plotsFit_Run2012B_v5_12000M_EB_iterative
#ln -s plotsFit_Run2012B_v5_12000M_EB_iterative plotsFit
#root -l -b -q drawFitResults.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_iterative.root\",0,0.12,0,0,\"ndof\>36\"\)
#root -l -b -q drawCorrelations.C\(\"BTCP\",\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_iterative.root\",0.12,36\)
#root -l -b -q combineAlphaFits.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_iterative.root\",\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_fixedDelta_iterative.root\",\"12000M_EB_iterative_vs_12000M_EB_fixedDelta_iterative\",0.12,36,36,2.3\)
#root -l -b -q combineAlphaFits.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_iterative.root\",\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB.root\",\"12000M_EB_iterative_vs_12000M_EB\",0.12,36,36,2.3\)
#root -l -b -q combineAlphaFits.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_iterative.root\",\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_3200M_alphaFit_2012dataBsCorr_EB_iterative.root\",\"12000M_EB_iterative_vs_3200M_EB_iterative\",0.12,36,36,2.3\)
#
#
#rm -f plotsFit
#mkdir -p plotsFit_Run2012B_v5_12000M_EB_fixedDelta_iterative
#ln -s plotsFit_Run2012B_v5_12000M_EB_fixedDelta_iterative plotsFit
#root -l -b -q drawFitResults.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_fixedDelta_iterative.root\",0,0.12,0,0,\"ndof\>36\"\)
#root -l -b -q drawCorrelations.C\(\"BTCP\",\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_fixedDelta_iterative.root\",0.12,36\)
#

#root -l -b -q combineAlphaFits.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_TT_iterative.root\",\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_3200M_alphaFit_2012dataBsCorr_EB_TT.root\",\"12000M_EB_TT_iterative_vs_3200M_EB_TT\",0.12,850,850,2.3\)
#root -l -b -q combineAlphaFits.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_iterative.root\",\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_3200M_alphaFit_2012dataBsCorr_EB.root\",\"12000M_EB_iterative_vs_3200M_EB\",0.12,36,110,2.3\)
#root -l -b -q combineAlphaFits.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_fixedDelta_iterative.root\",\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_3200M_alphaFit_2012dataBsCorr_EB.root\",\"12000M_EB_fixedDelta_iterative_vs_3200M_EB\",0.12,36,110,2.3\)
root -l -b -q combineAlphaFits.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_12000M_alphaFit_2012dataBsCorr_EB_iterative.root\",\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v5_3200M_alphaFit_2012dataBsCorr_EB_fixedDelta.root\",\"12000M_EB_iterative_vs_3200M_EB_fixedDelta\",0.12,36,110,2.3\)
