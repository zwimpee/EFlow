//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Apr  4 11:08:08 2012 by ROOT version 5.27/06b
// from TTree tree_barl/tree_barl
// found on file: beamSpotCorrections.root
//////////////////////////////////////////////////////////

#ifndef calculateBSCorrections_h
#define calculateBSCorrections_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class calculateBSCorrections {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Double_t         vertexPosition;
   Double_t         bsWid;
   Double_t         nevents;
   Double_t         totalEtSum;
   Double_t         totalHits;
   Int_t           ieta;
   Int_t           side;
   Int_t           vtxBin;
   Int_t           bsWidBin;
   Double_t         etSum;
   Double_t         nHits;

   // List of branches
   TBranch        *b_vertexPosition;   //!
   TBranch        *b_bsWid;   //!
   TBranch        *b_nevents;   //!
   TBranch        *b_totalEtSum;   //!
   TBranch        *b_totalHits;   //!
   TBranch        *b_ieta;   //!
   TBranch        *b_side;   //!
   TBranch        *b_vtxBin;   //!
   TBranch        *b_bsWidBin;   //!
   TBranch        *b_etSum;   //!
   TBranch        *b_nHits;   //!

   calculateBSCorrections(TTree *tree=0);
   virtual ~calculateBSCorrections();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef calculateBSCorrections_cxx
calculateBSCorrections::calculateBSCorrections(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("beamSpotCorrections.root");
      if (!f) {
         f = new TFile("beamSpotCorrections.root");
      }
      tree = (TTree*)gDirectory->Get("tree_barl");

   }
   Init(tree);
}

calculateBSCorrections::~calculateBSCorrections()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t calculateBSCorrections::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t calculateBSCorrections::LoadTree(Long64_t entry)
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

void calculateBSCorrections::Init(TTree *tree)
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

   fChain->SetBranchAddress("vertexPosition", &vertexPosition, &b_vertexPosition);
   fChain->SetBranchAddress("bsWid", &bsWid, &b_bsWid);
   fChain->SetBranchAddress("nevents", &nevents, &b_nevents);
   fChain->SetBranchAddress("totalEtSum", &totalEtSum, &b_totalEtSum);
   fChain->SetBranchAddress("totalHits", &totalHits, &b_totalHits);
   fChain->SetBranchAddress("ieta", &ieta, &b_ieta);
   fChain->SetBranchAddress("side", &side, &b_side);
   fChain->SetBranchAddress("vtxBin", &vtxBin, &b_vtxBin);
   fChain->SetBranchAddress("bsWidBin", &bsWidBin, &b_bsWidBin);
   fChain->SetBranchAddress("etSum", &etSum, &b_etSum);
   fChain->SetBranchAddress("nHits", &nHits, &b_nHits);
   Notify();
}

Bool_t calculateBSCorrections::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void calculateBSCorrections::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t calculateBSCorrections::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef calculateBSCorrections_cxx
