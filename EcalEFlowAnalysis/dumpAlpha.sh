#!/bin/sh 

printFile() {
    nGoodFits=`cat $1 | grep -c ok`
    nBadFits=`cat $1 | grep -v -c ok`
    echo "$1: goodFits $nGoodFits !goodFits $nBadFits"   
}

#echo "Dumping 2012C v6 firstPeriod"
#root -l -b -q dumpAlpha.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012C_v6_12000M_alphaFit_newLaser_2012dataBsCorr_EB_iterative.root\",0,0,21\) | sed '1,9d' > fit_AlCaPhiSym_Run2012C_v6_firstPeriod_12000M_alphaFit_2012dataBsCorr_EB_iterative.txt
#printFile fit_AlCaPhiSym_Run2012C_v6_firstPeriod_12000M_alphaFit_2012dataBsCorr_EB_iterative.txt
#root -l -b -q dumpAlpha.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012C_v6_12000M_alphaFit_newLaser_2012dataBsCorr_EB_TT_iterative.root\",0,0,550\) | sed '1,9d' > fit_AlCaPhiSym_Run2012C_v6_firstPeriod_12000M_alphaFit_2012dataBsCorr_EB_TT_iterative.txt
#printFile fit_AlCaPhiSym_Run2012C_v6_firstPeriod_12000M_alphaFit_2012dataBsCorr_EB_TT_iterative.txt

#echo "Dumping 2012C v6 fullPeriod"
#root -l -b -q dumpAlpha.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012C_v6_12000M_alphaFit_newLaser_2012dataBsCorr_EB_iterative_fullPeriod.root\",0,0,36\) | sed '1,9d' > fit_AlCaPhiSym_Run2012C_v6_fullPeriod_12000M_alphaFit_2012dataBsCorr_EB_iterative.txt
#printFile fit_AlCaPhiSym_Run2012C_v6_fullPeriod_12000M_alphaFit_2012dataBsCorr_EB_iterative.txt
#root -l -b -q dumpAlpha.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012C_v6_12000M_alphaFit_newLaser_2012dataBsCorr_EB_TT_iterative_fullPeriod.root\",0,0,860\) | sed '1,9d' > fit_AlCaPhiSym_Run2012C_v6_fullPeriod_12000M_alphaFit_2012dataBsCorr_EB_TT_iterative.txt
#printFile fit_AlCaPhiSym_Run2012C_v6_fullPeriod_12000M_alphaFit_2012dataBsCorr_EB_TT_iterative.txt

#echo "Dumping 2012B v6"
#root -l -b -q dumpAlpha.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v6_12000M_alphaFit_newLaser_2012dataBsCorr_EB_iterative.root\",0,0,29\) | sed '1,9d' > fit_AlCaPhiSym_Run2012B_v6_12000M_alphaFit_2012dataBsCorr_EB_iterative.txt
#printFile fit_AlCaPhiSym_Run2012B_v6_12000M_alphaFit_2012dataBsCorr_EB_iterative.txt
#root -l -b -q dumpAlpha.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v6_12000M_alphaFit_newLaser_2012dataBsCorr_EB_TT_iterative.root\",0,0,700\) | sed '1,9d' > fit_AlCaPhiSym_Run2012B_v6_12000M_alphaFit_2012dataBsCorr_EB_TT_iterative.txt
#printFile fit_AlCaPhiSym_Run2012B_v6_12000M_alphaFit_2012dataBsCorr_EB_TT_iterative.txt

echo "Dumping 2012C v6 firstPeriod"
root -l -b -q dumpAlpha.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012C_v6_12000M_alphaFit_newLaser_2012dataBsCorr_EE_iterative.root\",0,0,16\) | sed '1,9d' > fit_AlCaPhiSym_Run2012C_v6_firstPeriod_12000M_alphaFit_2012dataBsCorr_EE_iterative.txt
printFile fit_AlCaPhiSym_Run2012C_v6_firstPeriod_12000M_alphaFit_2012dataBsCorr_EE_iterative.txt
root -l -b -q dumpAlpha.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012C_v6_12000M_alphaFit_newLaser_2012dataBsCorr_EE_SC_iterative.root\",0,0,250\) | sed '1,9d' > fit_AlCaPhiSym_Run2012C_v6_firstPeriod_12000M_alphaFit_2012dataBsCorr_EE_SC_iterative.txt
printFile fit_AlCaPhiSym_Run2012C_v6_firstPeriod_12000M_alphaFit_2012dataBsCorr_EE_SC_iterative.txt

echo "Dumping 2012B v6"
root -l -b -q dumpAlpha.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v6_12000M_alphaFit_newLaser_2012dataBsCorr_EE_iterative.root\",0,0,22\) | sed '1,9d' > fit_AlCaPhiSym_Run2012B_v6_12000M_alphaFit_2012dataBsCorr_EE_iterative.txt
printFile fit_AlCaPhiSym_Run2012B_v6_12000M_alphaFit_2012dataBsCorr_EE_iterative.txt
root -l -b -q dumpAlpha.C\(\"/xrootdfs/cms/local/meridian/EFlow/alphaFits/fit_histories_AlCaPhiSym_Run2012B_v6_12000M_alphaFit_newLaser_2012dataBsCorr_EE_SC_iterative.root\",0,0,250\) | sed '1,9d' > fit_AlCaPhiSym_Run2012B_v6_12000M_alphaFit_2012dataBsCorr_EE_SC_iterative.txt
printFile fit_AlCaPhiSym_Run2012B_v6_12000M_alphaFit_2012dataBsCorr_EE_SC_iterative.txt