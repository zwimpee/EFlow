//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun May 13 09:01:36 2012 by ROOT version 5.27/06b
// from TTree bsTree/bsTree
// found on file: /xrootdfs/cms/local/meridian/EFlow/historyTree_2012/AlCaPhiSym_Run2012A-v1_RAW_v4_1200M_noBsCorr/createHistoryOut_AlCaPhiSym_Run2012A-v1_RAW_190456_194052_FXN_001.root
//////////////////////////////////////////////////////////

#ifndef createLastTree_bs_2_h
#define createLastTree_bs_2_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class createLastTree_bs_2 {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           time_interval;
   UInt_t          nEvents;
   ULong64_t       nHitsEBVar;
   ULong64_t       nHitsEEVar;
   Float_t         bsPos;
   Float_t         bsWid;

   // List of branches
   TBranch        *b_timeInterval;   //!
   TBranch        *b_nEvents;   //!
   TBranch        *b_nHitsEB;   //!
   TBranch        *b_nHitsEE;   //!
   TBranch        *b_bsPosVar;   //!
   TBranch        *b_bsWidVar;   //!

   createLastTree_bs_2(TTree *tree=0);
   virtual ~createLastTree_bs_2();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef createLastTree_bs_2_cxx
createLastTree_bs_2::createLastTree_bs_2(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/xrootdfs/cms/local/meridian/EFlow/historyTree_2012/AlCaPhiSym_Run2012A-v1_RAW_v4_1200M_noBsCorr/createHistoryOut_AlCaPhiSym_Run2012A-v1_RAW_190456_194052_FXN_001.root");
      if (!f) {
         f = new TFile("/xrootdfs/cms/local/meridian/EFlow/historyTree_2012/AlCaPhiSym_Run2012A-v1_RAW_v4_1200M_noBsCorr/createHistoryOut_AlCaPhiSym_Run2012A-v1_RAW_190456_194052_FXN_001.root");
      }
      tree = (TTree*)gDirectory->Get("bsTree");

   }
   Init(tree);
}

createLastTree_bs_2::~createLastTree_bs_2()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t createLastTree_bs_2::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t createLastTree_bs_2::LoadTree(Long64_t entry)
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

void createLastTree_bs_2::Init(TTree *tree)
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

   fChain->SetBranchAddress("time_interval", &time_interval, &b_timeInterval);
   fChain->SetBranchAddress("nEvents", &nEvents, &b_nEvents);
   fChain->SetBranchAddress("nHitsEBVar", &nHitsEBVar, &b_nHitsEB);
   fChain->SetBranchAddress("nHitsEEVar", &nHitsEEVar, &b_nHitsEE);
   fChain->SetBranchAddress("bsPos", &bsPos, &b_bsPosVar);
   fChain->SetBranchAddress("bsWid", &bsWid, &b_bsWidVar);
   Notify();
}

Bool_t createLastTree_bs_2::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void createLastTree_bs_2::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t createLastTree_bs_2::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef createLastTree_bs_2_cxx
