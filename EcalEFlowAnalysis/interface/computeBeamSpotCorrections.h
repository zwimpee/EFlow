//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Mar 30 11:13:02 2012 by ROOT version 5.27/06b
// from TTree variablesTree_barl/variablesTree_barl
// found on file: root://eoscms//eos/cms/store/user/meridian/EFlow/MinBias_TuneZ2_7TeV-pythia6_Summer11-NoPileup_START42_V11-v2/variablesTree_81_1_4s3.root
//////////////////////////////////////////////////////////

#ifndef computeBeamSpotCorrections_h
#define computeBeamSpotCorrections_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector>

class computeBeamSpotCorrections {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   TString outFileName_;
   // Declaration of leaf types
   UInt_t          nhit;
   Int_t           HLTNamesLen;
   
   vector<string>  *HLTNames;
   vector<bool>    *HLTResults;
   Float_t         vxMC;
   Float_t         vyMC;
   Float_t         vzMC;
   UInt_t          nvertex;
   Float_t         beamSpotX0;
   Float_t         beamSpotY0;
   Float_t         beamSpotZ0;
   Float_t         beamSpotSigmaX;
   Float_t         beamSpotSigmaY;
   Float_t         beamSpotSigmaZ;
   Int_t           ieta[200];   //[nhit]
   Int_t           iphi[200];   //[nhit]
   Float_t         lc_barl[200];   //[nhit]
   Float_t         et_barl[200];   //[nhit]
   UInt_t          unixtime;
   UInt_t          run;
   UInt_t          lumi;
   UInt_t          eventid;

   // List of branches
   TBranch        *b_nhit;   //!
   TBranch        *b_HLTNamesLen;   //!
   TBranch        *b_HLTNames;   //!
   TBranch        *b_HLTResults;   //!
   TBranch        *b_vxMC;   //!
   TBranch        *b_vyMC;   //!
   TBranch        *b_vzMC;   //!
   TBranch        *b_nvertex;   //!
   TBranch        *b_beamSpotX0;   //!
   TBranch        *b_beamSpotY0;   //!
   TBranch        *b_beamSpotZ0;   //!
   TBranch        *b_beamSpotSigmaX;   //!
   TBranch        *b_beamSpotSigmaY;   //!
   TBranch        *b_beamSpotSigmaZ;   //!
   TBranch        *b_ieta;   //!
   TBranch        *b_iphi;   //!
   TBranch        *b_lc_barl;   //!
   TBranch        *b_et_barl;   //!
   TBranch        *b_unixtime;   //!
   TBranch        *b_run;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_eventid;   //!

   void setOutFile(TString & name)
   {
     outFileName_=name;
   }

   computeBeamSpotCorrections(TTree *tree=0);
   virtual ~computeBeamSpotCorrections();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef computeBeamSpotCorrections_cxx
computeBeamSpotCorrections::computeBeamSpotCorrections(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("root://eoscms//eos/cms/store/user/meridian/EFlow/MinBias_TuneZ2_7TeV-pythia6_Summer11-NoPileup_START42_V11-v2/variablesTree_81_1_4s3.root");
      if (!f) {
         f = new TFile("root://eoscms//eos/cms/store/user/meridian/EFlow/MinBias_TuneZ2_7TeV-pythia6_Summer11-NoPileup_START42_V11-v2/variablesTree_81_1_4s3.root");
      }
      tree = (TTree*)gDirectory->Get("variablesTree_barl");

   }
   Init(tree);
}

computeBeamSpotCorrections::~computeBeamSpotCorrections()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t computeBeamSpotCorrections::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t computeBeamSpotCorrections::LoadTree(Long64_t entry)
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


void computeBeamSpotCorrections::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   HLTNames = 0;
   HLTResults = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("nhit", &nhit, &b_nhit);
   fChain->SetBranchAddress("HLTNamesLen", &HLTNamesLen, &b_HLTNamesLen);
   fChain->SetBranchAddress("HLTNames", &HLTNames, &b_HLTNames);
   fChain->SetBranchAddress("HLTResults", &HLTResults, &b_HLTResults);
   fChain->SetBranchAddress("vxMC", &vxMC, &b_vxMC);
   fChain->SetBranchAddress("vyMC", &vyMC, &b_vyMC);
   fChain->SetBranchAddress("vzMC", &vzMC, &b_vzMC);
   fChain->SetBranchAddress("nvertex", &nvertex, &b_nvertex);
   fChain->SetBranchAddress("beamSpotX0", &beamSpotX0, &b_beamSpotX0);
   fChain->SetBranchAddress("beamSpotY0", &beamSpotY0, &b_beamSpotY0);
   fChain->SetBranchAddress("beamSpotZ0", &beamSpotZ0, &b_beamSpotZ0);
   fChain->SetBranchAddress("beamSpotSigmaX", &beamSpotSigmaX, &b_beamSpotSigmaX);
   fChain->SetBranchAddress("beamSpotSigmaY", &beamSpotSigmaY, &b_beamSpotSigmaY);
   fChain->SetBranchAddress("beamSpotSigmaZ", &beamSpotSigmaZ, &b_beamSpotSigmaZ);
   fChain->SetBranchAddress("ieta", ieta, &b_ieta);
   fChain->SetBranchAddress("iphi", iphi, &b_iphi);
   fChain->SetBranchAddress("lc_barl", lc_barl, &b_lc_barl);
   fChain->SetBranchAddress("et_barl", et_barl, &b_et_barl);
   fChain->SetBranchAddress("unixtime", &unixtime, &b_unixtime);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("eventid", &eventid, &b_eventid);
   Notify();
}

Bool_t computeBeamSpotCorrections::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void computeBeamSpotCorrections::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t computeBeamSpotCorrections::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef computeBeamSpotCorrections_cxx
