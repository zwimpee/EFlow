#include "TROOT.h"
#include "TSystem.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include <iostream>

#define kBarlRings 85
#define kEndcRings 39

#define kBarlWedges 360
#define kEndcWedgesX 100
#define kEndcWedgesY 100

#define kSides 2

#define kTowerPerSM 68
#define kXtalPerSM 1700
#define kSM 36

#define kEndcSCs 624
#define kEndcXtals 14648

using namespace std;

void dumpHistoryTree(
		     int ieta=1,
		     int iphi=1, 
		     int sign=1,
		     TString det="EB",
		     TString historyFile="/xrootdfs/cms/local/meridian/EFlow/histories/histories_RUN2011_Jan16_1600M_2012dataBsCorr_fullTree.root",
		     TString eeIndexMap="/afs/cern.ch/user/m/meridian/public/eeIndicesMap.root",
		     TString valuesToPrint="etSumMeanVsRef:etSumMeanVsRefRMS:unixTime",
		     TString dumpFormat="colsize=20 col=6.5:6.5"
		     )
{
  TFile *f=TFile::Open(historyFile); 
  if (!f)
    {
      std::cout << "Cannot open " << historyFile << std::endl;
      return;
    }
  TTree* historyTree=(TTree*) f->Get("historyTree");

  int index=0;
  if (det=="EB")
    {
      int etaIndex=ieta-1;
      if (sign==1)
	etaIndex+=85;
      index=etaIndex*360+iphi;
    }
  else if (det=="EE")
    {
      TFile *fee=TFile::Open(eeIndexMap);
      if (!fee)
	{
	  std::cout << "Cannot open " << eeIndexMap << std::endl;
	  return;
	}
      TTree* eeIndicesTree= (TTree*)fee->Get("eeIndices");
  
      int ixVar,iyVar,signVar,ixtalVar;
      TBranch *b_ix=eeIndicesTree->GetBranch("ix");
      TBranch *b_iy=eeIndicesTree->GetBranch("iy");
      TBranch *b_sig=eeIndicesTree->GetBranch("sign");
//       TBranch *b_iring=eeIndicesTree->GetBranch("iring");
      TBranch *b_ixtal=eeIndicesTree->GetBranch("ixtal");
//       TBranch *b_isc=eeIndicesTree->GetBranch("isc");
//       TBranch *b_ilm=eeIndicesTree->GetBranch("ilm");
  
      eeIndicesTree->SetBranchAddress("ix", &ixVar, &b_ix);
      eeIndicesTree->SetBranchAddress("iy", &iyVar, &b_iy);
      eeIndicesTree->SetBranchAddress("sign", &signVar, &b_sig);
//       eeIndicesTree->SetBranchAddress("iring", &iringVar, &b_iring);
      eeIndicesTree->SetBranchAddress("ixtal", &ixtalVar, &b_ixtal);
//       eeIndicesTree->SetBranchAddress("isc", &iSCVar, &b_isc);
//       eeIndicesTree->SetBranchAddress("ilm", &iLMVar, &b_ilm);
  
      int nentries_ee = eeIndicesTree->GetEntries();
//       int endcapRings[kEndcWedgesX][kEndcWedgesY][kSides];
      int endcapXtals[kEndcWedgesX][kEndcWedgesY][kSides];
//       int endcapSCs[kEndcWedgesX][kEndcWedgesY][kSides];
//       int endcapLMs[kEndcWedgesX][kEndcWedgesY][kSides];
  
      for (int isign=0;isign<kSides;++isign)
	for (int ix=0;ix<kEndcWedgesX;++ix)
	  for (int iy=0;iy<kEndcWedgesX;++iy)
	    {
// 	      endcapRings[ix][iy][isign]=-1;
	      endcapXtals[ix][iy][isign]=-1;
// 	      endcapSCs[ix][iy][isign]=-1;
// 	      endcapLMs[ix][iy][isign]=-1;
	    }
  
      for(int jentry=0;jentry<nentries_ee;++jentry)
	{
	  eeIndicesTree->GetEntry(jentry);
// 	  endcapRings[ixVar-1][iyVar-1][signVar]=iringVar;
	  endcapXtals[ixVar-1][iyVar-1][signVar]=ixtalVar;
// 	  endcapSCs[ixVar-1][iyVar-1][signVar]=iSCVar;
// 	  endcapLMs[ixVar-1][iyVar-1][signVar]=iLMVar;
	}
      index=endcapXtals[ieta-1][iphi-1][sign]+1;
    }
  else
    {
      std::cout << "Det is unknown. Giving up" << std::endl;
      return;
    }

  TString query="indexType==";
  if (det=="EB")
    query+="2 && etSumMeanVsRef> 0.5 && etSumMeanVsRef< 1.5 && index== ";
  else if (det=="EE")
    query+="5 && etSumMeanVsRef> 0.5 && etSumMeanVsRef< 1.5 && index==";
  query+=index;
  historyTree->SetScanField(0);
  historyTree->Scan(valuesToPrint,query,dumpFormat);
}
