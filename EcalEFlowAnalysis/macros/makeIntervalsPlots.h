//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Apr 13 11:54:47 2012 by ROOT version 5.27/06b
// from TTree outTree_barl/outTree_barl
// found on file: readMap_out_barl_2012_until190949.root
//////////////////////////////////////////////////////////

#ifndef makeIntervalsPlots_h
#define makeIntervalsPlots_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class makeIntervalsPlots {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   TString outFileName;

   // Declaration of leaf types
   Int_t           index;
   Int_t           nHit;
   Int_t           nLS;
   Int_t           firstRun;
   Int_t           lastRun;
   Int_t           firstLumi;
   Int_t           lastLumi;
   UInt_t          unixtimeStart;
   UInt_t          unixtimeEnd;
   UInt_t          unixtimeMean;

   // List of branches
   TBranch        *b_index;   //!
   TBranch        *b_nHit;   //!
   TBranch        *b_nLS;   //!
   TBranch        *b_firstRun;   //!
   TBranch        *b_lastRun;   //!
   TBranch        *b_firstLumi;   //!
   TBranch        *b_lastLumi;   //!
   TBranch        *b_unixtimeStart;   //!
   TBranch        *b_unixtimeEnd;   //!
   TBranch        *b_unixtimeMean;   //!

   makeIntervalsPlots(TTree *tree=0);
   virtual ~makeIntervalsPlots();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   void setOutfile(const char* outFile);
};

#endif

#ifdef makeIntervalsPlots_cxx
makeIntervalsPlots::makeIntervalsPlots(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("readMap_out_barl_2012_until190949.root");
      if (!f) {
         f = new TFile("readMap_out_barl_2012_until190949.root");
      }
      tree = (TTree*)gDirectory->Get("outTree_barl");

   }
   Init(tree);
}

makeIntervalsPlots::~makeIntervalsPlots()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

void makeIntervalsPlots::setOutfile(const char* file)
{
   outFileName=TString(file);
}


Int_t makeIntervalsPlots::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t makeIntervalsPlots::LoadTree(Long64_t entry)
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

void makeIntervalsPlots::Init(TTree *tree)
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

   fChain->SetBranchAddress("index", &index, &b_index);
   fChain->SetBranchAddress("nHit", &nHit, &b_nHit);
   fChain->SetBranchAddress("nLS", &nLS, &b_nLS);
   fChain->SetBranchAddress("firstRun", &firstRun, &b_firstRun);
   fChain->SetBranchAddress("lastRun", &lastRun, &b_lastRun);
   fChain->SetBranchAddress("firstLumi", &firstLumi, &b_firstLumi);
   fChain->SetBranchAddress("lastLumi", &lastLumi, &b_lastLumi);
   fChain->SetBranchAddress("unixtimeStart", &unixtimeStart, &b_unixtimeStart);
   fChain->SetBranchAddress("unixtimeEnd", &unixtimeEnd, &b_unixtimeEnd);
   fChain->SetBranchAddress("unixtimeMean", &unixtimeMean, &b_unixtimeMean);
   Notify();
}

Bool_t makeIntervalsPlots::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void makeIntervalsPlots::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t makeIntervalsPlots::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef makeIntervalsPlots_cxx
