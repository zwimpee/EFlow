//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jan 20 17:09:12 2012 by ROOT version 5.27/06b
// from TTree variablesTree_barl/variablesTree_barl
// found on file: root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees5/172947_174137/variablesTree_100_1_pps.root
//////////////////////////////////////////////////////////

#ifndef createHistoryPlots_barl_h
#define createHistoryPlots_barl_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <lumiIntervals.h>
#include <alcaPhiSym_tree_v3.h>
#include <JSON.h>
#include <iostream>

#define MAXHITS 1000
#define kBarlRings 85
#define kBarlWedges 360
#define kSides 2
#define kX 100
#define kY 100

using namespace std;

class createHistoryPlots : public alcaPhiSym_tree_v3 {
public :

  bool applyBSCorrection;
  TString bsCorrectionFile;

  lumiIntervals* intervals;
  JSON* goodLS;
  
  TString outFileName;


  struct bsInfo{
    float bsPos;
    float bsWid;
    long int nEvents;

    void reset()
    {
      bsPos=0.;
      bsWid=0.;
      nEvents=0;
    }
  };

  struct histos{
    float energySum[kBarlRings][kBarlWedges][kSides];
    float energySquared[kBarlRings][kBarlWedges][kSides];
    float energySumA[kBarlRings][kBarlWedges][kSides];
    float energySquaredA[kBarlRings][kBarlWedges][kSides];
    float energySumB[kBarlRings][kBarlWedges][kSides];
    float energySquaredB[kBarlRings][kBarlWedges][kSides];
    float energyNoCorrSum[kBarlRings][kBarlWedges][kSides];
    float energyNoCorrSquared[kBarlRings][kBarlWedges][kSides];
    float energyNoCorrSumA[kBarlRings][kBarlWedges][kSides];
    float energyNoCorrSquaredA[kBarlRings][kBarlWedges][kSides];
    float energyNoCorrSumB[kBarlRings][kBarlWedges][kSides];
    float energyNoCorrSquaredB[kBarlRings][kBarlWedges][kSides];
    float lasercorrSum[kBarlRings][kBarlWedges][kSides];
    float lasercorrSquared[kBarlRings][kBarlWedges][kSides];
    int counter[kBarlRings][kBarlWedges][kSides];

    void reset()
    {
      for(int iisign=0;iisign<kSides;iisign++){
	for(int iieta=0;iieta<kBarlRings;iieta++){
	  for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
	    counter[iieta][iiphi][iisign]=0;
	    energySum[iieta][iiphi][iisign]=0;
	    energySquared[iieta][iiphi][iisign]=0;
	    energySumA[iieta][iiphi][iisign]=0;
	    energySquaredA[iieta][iiphi][iisign]=0;
	    energySumB[iieta][iiphi][iisign]=0;
	    energySquaredB[iieta][iiphi][iisign]=0;
	    energyNoCorrSum[iieta][iiphi][iisign]=0;
	    energyNoCorrSquared[iieta][iiphi][iisign]=0;
	    energyNoCorrSumA[iieta][iiphi][iisign]=0;
	    energyNoCorrSquaredA[iieta][iiphi][iisign]=0;
	    energyNoCorrSumB[iieta][iiphi][iisign]=0;
	    energyNoCorrSquaredB[iieta][iiphi][iisign]=0;
	    lasercorrSum[iieta][iiphi][iisign]=0;
	    lasercorrSquared[iieta][iiphi][iisign]=0;
	  }
	}
      }
    }
  };


  struct histos_ee{
    float energySum[kX][kY][kSides];
    float energySquared[kX][kY][kSides];
    float energySumA[kX][kY][kSides];
    float energySquaredA[kX][kY][kSides];
    float energySumB[kX][kY][kSides];
    float energySquaredB[kX][kY][kSides];
    float energyNoCorrSum[kX][kY][kSides];
    float energyNoCorrSquared[kX][kY][kSides];
    float energyNoCorrSumA[kX][kY][kSides];
    float energyNoCorrSquaredA[kX][kY][kSides];
    float energyNoCorrSumB[kX][kY][kSides];
    float energyNoCorrSquaredB[kX][kY][kSides];
    float lasercorrSum[kX][kY][kSides];
    float lasercorrSquared[kX][kY][kSides];
    int counter[kX][kY][kSides];

    void reset()
    {
      for(int iisign=0;iisign<kSides;iisign++){
	for(int iieta=0;iieta<kX;iieta++){
	  for(int iiphi=0;iiphi<kY;iiphi++){
	    counter[iieta][iiphi][iisign]=0;
	    energySum[iieta][iiphi][iisign]=0;
	    energySquared[iieta][iiphi][iisign]=0;
	    energySumA[iieta][iiphi][iisign]=0;
	    energySquaredA[iieta][iiphi][iisign]=0;
	    energySumB[iieta][iiphi][iisign]=0;
	    energySquaredB[iieta][iiphi][iisign]=0;
	    energyNoCorrSum[iieta][iiphi][iisign]=0;
	    energyNoCorrSquared[iieta][iiphi][iisign]=0;
	    energyNoCorrSumA[iieta][iiphi][iisign]=0;
	    energyNoCorrSquaredA[iieta][iiphi][iisign]=0;
	    energyNoCorrSumB[iieta][iiphi][iisign]=0;
	    energyNoCorrSquaredB[iieta][iiphi][iisign]=0;
	    lasercorrSum[iieta][iiphi][iisign]=0;
	    lasercorrSquared[iieta][iiphi][iisign]=0;
	  }
	}
      }
    }
  };
 

   createHistoryPlots(TTree *tree=0);
   virtual ~createHistoryPlots();
/*    virtual Int_t    Cut(Long64_t entry); */
/*    virtual Int_t    GetEntry(Long64_t entry); */
/*    virtual Long64_t LoadTree(Long64_t entry); */
/*    virtual void     Init(TTree *tree); */
   virtual void     Loop(); 
/*    virtual Bool_t   Notify(); */
/*    virtual void     Show(Long64_t entry = -1); */
   void setLumiIntervals(const char* file);
   void setJSON(const char* jsonFile);
   void setOutfile(const char* jsonFile);
};

#endif

#ifdef createHistoryPlots_cxx
createHistoryPlots::createHistoryPlots(TTree *tree) : alcaPhiSym_tree_v3(tree)
{
   intervals=0;
   outFileName=TString("outputBarl.root");
}

void createHistoryPlots::setLumiIntervals(const char* file)
{
  intervals=new lumiIntervals(file);
}

void createHistoryPlots::setJSON(const char* file)
{
  goodLS=new JSON(file);
}

void createHistoryPlots::setOutfile(const char* file)
{
   outFileName=TString(file);
}

createHistoryPlots::~createHistoryPlots()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

/* Int_t createHistoryPlots::GetEntry(Long64_t entry) */
/* { */
/* // Read contents of entry. */
/*    if (!fChain) return 0; */
/*    return fChain->GetEntry(entry); */
/* } */
/* Long64_t createHistoryPlots::LoadTree(Long64_t entry) */
/* { */
/* // Set the environment to read one entry */
/*    if (!fChain) return -5; */
/*    Long64_t centry = fChain->LoadTree(entry); */
/*    if (centry < 0) return centry; */
/*    if (!fChain->InheritsFrom(TChain::Class()))  return centry; */
/*    TChain *chain = (TChain*)fChain; */
/*    if (chain->GetTreeNumber() != fCurrent) { */
/*       fCurrent = chain->GetTreeNumber(); */
/*       Notify(); */
/*    } */
/*    return centry; */
/* } */

#endif // #ifdef createHistoryPlots_cxx
