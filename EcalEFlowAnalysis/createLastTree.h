//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Feb  1 10:09:08 2012 by ROOT version 5.27/06b
// from TTree tree_barl/tree_barl
// found on file: 2011A/outputForHistory_barl_2011A_1.root
//////////////////////////////////////////////////////////

#ifndef createLastTree_h
#define createLastTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "TMath.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TFile.h"
#include "TTree.h"
#include <TChain.h>
#include "TBrowser.h"
#include "TH2.h"
#include "TRandom.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TF1.h"
#include "TH1.h"
#include "THStack.h"
#include "TVector3.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TTreeIndex.h"
#include "TChainIndex.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


#include "lumiIntervals.h"

#define MAXHITS 1000
#define kBarlRings 85
#define kBarlWedges 360
#define kSides 2
#define kX 100
#define kY 100

class createLastTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   lumiIntervals* intervals;
   TString outFileName;


   // Declaration of leaf types
   Int_t           time_interval;
   UInt_t          nHits;
   UInt_t          det;
   Int_t           ieta;
   Int_t           iphi;
   Int_t           sign;
   Float_t         energySum;
   Float_t         energySquared;
   Float_t         energySumA;
   Float_t         energySquaredA;
   Float_t         energySumB;
   Float_t         energySquaredB;
   Float_t         energyNoCorrSum;
   Float_t         energyNoCorrSquared;
   Float_t         energyNoCorrSumA;
   Float_t         energyNoCorrSquaredA;
   Float_t         energyNoCorrSumB;
   Float_t         energyNoCorrSquaredB;
   Float_t         lcSum;
   Float_t         lcSquared;

   // List of branches
   TBranch        *b_time_interval;   //!
   TBranch        *b_nHits;   //!
   TBranch        *b_det;   //!
   TBranch        *b_ieta;   //!
   TBranch        *b_iphi;   //!
   TBranch        *b_sign;   //!
   TBranch        *b_energySum;   //!
   TBranch        *b_energySquared;   //!
   TBranch        *b_energySumA;   //!
   TBranch        *b_energySquaredA;   //!
   TBranch        *b_energySumB;   //!
   TBranch        *b_energySquaredB;   //!
   TBranch        *b_energyNoCorrSum;   //!
   TBranch        *b_energyNoCorrSquared;   //!
   TBranch        *b_energyNoCorrSumA;   //!
   TBranch        *b_energyNoCorrSquaredA;   //!
   TBranch        *b_energyNoCorrSumB;   //!
   TBranch        *b_energyNoCorrSquaredB;   //!
   TBranch        *b_lcSum;   //!
   TBranch        *b_lcSquared;   //!

   struct histosLastTree{
     float energySum[kBarlRings][kBarlWedges][kSides];
     float energySquared[kBarlRings][kBarlWedges][kSides];
     float energyNoCorrSum[kBarlRings][kBarlWedges][kSides];
     float energyNoCorrSquared[kBarlRings][kBarlWedges][kSides];

     float energySumA[kBarlRings][kBarlWedges][kSides];
     float energySquaredA[kBarlRings][kBarlWedges][kSides];
     float energyNoCorrSumA[kBarlRings][kBarlWedges][kSides];
     float energyNoCorrSquaredA[kBarlRings][kBarlWedges][kSides];

     float energySumB[kBarlRings][kBarlWedges][kSides];
     float energySquaredB[kBarlRings][kBarlWedges][kSides];
     float energyNoCorrSumB[kBarlRings][kBarlWedges][kSides];
     float energyNoCorrSquaredB[kBarlRings][kBarlWedges][kSides];

     float lasercorrSum[kBarlRings][kBarlWedges][kSides];
     float lasercorrSquared[kBarlRings][kBarlWedges][kSides];
     int nhit[kBarlRings][kBarlWedges][kSides];

    void reset()
    {
      for(int iisign=0;iisign<kSides;iisign++){
	for(int iieta=0;iieta<kBarlRings;iieta++){
	  for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
	    energySum[iieta][iiphi][iisign]=0;
	    energySquared[iieta][iiphi][iisign]=0;
	    energyNoCorrSum[iieta][iiphi][iisign]=0;
	    energyNoCorrSquared[iieta][iiphi][iisign]=0;
	    lasercorrSum[iieta][iiphi][iisign]=0;
	    lasercorrSquared[iieta][iiphi][iisign]=0;
	  }
	}
      }
    }
     
     
   };


   struct histosLastTree_ee{
     float energySum[kX][kY][kSides];
     float energySquared[kX][kY][kSides];
     float energyNoCorrSum[kX][kY][kSides];
     float energyNoCorrSquared[kX][kY][kSides];

     float energySumA[kX][kY][kSides];
     float energySquaredA[kX][kY][kSides];
     float energyNoCorrSumA[kX][kY][kSides];
     float energyNoCorrSquaredA[kX][kY][kSides];

     float energySumB[kX][kY][kSides];
     float energySquaredB[kX][kY][kSides];
     float energyNoCorrSumB[kX][kY][kSides];
     float energyNoCorrSquaredB[kX][kY][kSides];

     float lasercorrSum[kX][kY][kSides];
     float lasercorrSquared[kX][kY][kSides];
     int nhit[kX][kY][kSides];

    void reset()
    {
      for(int iisign=0;iisign<kSides;iisign++){
	for(int iieta=0;iieta<kX;iieta++){
	  for(int iiphi=0;iiphi<kY;iiphi++){
	    energySum[iieta][iiphi][iisign]=0;
	    energySquared[iieta][iiphi][iisign]=0;
	    energyNoCorrSum[iieta][iiphi][iisign]=0;
	    energyNoCorrSquared[iieta][iiphi][iisign]=0;
	    lasercorrSum[iieta][iiphi][iisign]=0;
	    lasercorrSquared[iieta][iiphi][iisign]=0;
	  }
	}
      }
    }
     
     
   };

   createLastTree(TTree *tree=0);
   virtual ~createLastTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   void setLumiIntervals(const char* file);
   void setOutfile(const char* outFile);

};

#endif

#ifdef createLastTree_cxx
createLastTree::createLastTree(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("2011A/outputForHistory_barl_2011A_1.root");
      if (!f) {
         f = new TFile("2011A/outputForHistory_barl_2011A_1.root");
      }
      tree = (TTree*)gDirectory->Get("tree");

   }
   Init(tree);
}

createLastTree::~createLastTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}




void createLastTree::setLumiIntervals(const char* file)
{
  intervals=new lumiIntervals(file);
}

void createLastTree::setOutfile(const char* file)
{
   outFileName=TString(file);
}



Int_t createLastTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t createLastTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void createLastTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("time_interval", &time_interval, &b_time_interval);
   fChain->SetBranchAddress("nHits", &nHits, &b_nHits);
   fChain->SetBranchAddress("det", &det, &b_det);
   fChain->SetBranchAddress("ieta", &ieta, &b_ieta);
   fChain->SetBranchAddress("iphi", &iphi, &b_iphi);
   fChain->SetBranchAddress("sign", &sign, &b_sign);
   fChain->SetBranchAddress("energySum", &energySum, &b_energySum);
   fChain->SetBranchAddress("energySquared", &energySquared, &b_energySquared);
   fChain->SetBranchAddress("energySumA", &energySumA, &b_energySumA);
   fChain->SetBranchAddress("energySquaredA", &energySquaredA, &b_energySquaredA);
   fChain->SetBranchAddress("energySumB", &energySumB, &b_energySumB);
   fChain->SetBranchAddress("energySquaredB", &energySquaredB, &b_energySquaredB);
   fChain->SetBranchAddress("energyNoCorrSum", &energyNoCorrSum, &b_energyNoCorrSum);
   fChain->SetBranchAddress("energyNoCorrSquared", &energyNoCorrSquared, &b_energyNoCorrSquared);
   fChain->SetBranchAddress("energyNoCorrSumA", &energyNoCorrSumA, &b_energyNoCorrSumA);
   fChain->SetBranchAddress("energyNoCorrSquaredA", &energyNoCorrSquaredA, &b_energyNoCorrSquaredA);
   fChain->SetBranchAddress("energyNoCorrSumB", &energyNoCorrSumB, &b_energyNoCorrSumB);
   fChain->SetBranchAddress("energyNoCorrSquaredB", &energyNoCorrSquaredB, &b_energyNoCorrSquaredB);
   fChain->SetBranchAddress("lcSum", &lcSum, &b_lcSum);
   fChain->SetBranchAddress("lcSquared", &lcSquared, &b_lcSquared);
   Notify();
}

Bool_t createLastTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void createLastTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t createLastTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

#endif
