//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Apr  9 15:24:49 2012 by ROOT version 5.27/06b
// from TTree variablesTree/variablesTree
// found on file: root://eoscms//eos/cms/store/group/alca_ecalcalib/EFlow/AlCaPhiSym_Run2012A-v1_RAW_190456_190710_ARb/v3/variablesTree_87_1_NPx.root
//////////////////////////////////////////////////////////

#ifndef alcaPhiSym_tree_v3_h
#define alcaPhiSym_tree_v3_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector>

using namespace std;
class alcaPhiSym_tree_v3 {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   UInt_t          nhit_barl;
   Int_t           HLTNamesLen;
   vector<string>  *HLTNames;
   vector<bool>    *HLTResults;
   Int_t           nL1bits;
   Int_t           L1bits[128];   //[nL1bits]
   Int_t           nL1bitsTech;
   Int_t           L1bitsTech[64];   //[nL1bitsTech]
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
   Int_t           ieta[1000];   //[nhit_barl]
   Int_t           iphi[1000];   //[nhit_barl]
   Float_t         lc_barl[1000];   //[nhit_barl]
   Float_t         alpha_barl[1000];   //[nhit_barl]
   Float_t         et_barl[1000];   //[nhit_barl]
   UInt_t          unixtime;
   UInt_t          run;
   UInt_t          lumi;
   UInt_t          eventid;
   UInt_t          nhit_endc;
   Int_t           ix[1000];   //[nhit_endc]
   Int_t           iy[1000];   //[nhit_endc]
   UInt_t          sign[1000];   //[nhit_endc]
   Float_t         lc_endc[1000];   //[nhit_endc]
   Float_t         alpha_endc[1000];   //[nhit_endc]
   Float_t         et_endc[1000];   //[nhit_endc]

   // List of branches
   TBranch        *b_nhit_barl;   //!
   TBranch        *b_HLTNamesLen;   //!
   TBranch        *b_HLTNames;   //!
   TBranch        *b_HLTResults;   //!
   TBranch        *b_nL1bits;   //!
   TBranch        *b_L1bits;   //!
   TBranch        *b_nL1bitsTech;   //!
   TBranch        *b_L1bitsTech;   //!
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
   TBranch        *b_alpha_barl;   //!
   TBranch        *b_et_barl;   //!
   TBranch        *b_unixtime;   //!
   TBranch        *b_run;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_eventid;   //!
   TBranch        *b_nhit_endc;   //!
   TBranch        *b_ix;   //!
   TBranch        *b_iy;   //!
   TBranch        *b_sign;   //!
   TBranch        *b_lc_endc;   //!
   TBranch        *b_alpha_endc;   //!
   TBranch        *b_et_endc;   //!

   alcaPhiSym_tree_v3(TTree *tree=0);
   virtual ~alcaPhiSym_tree_v3();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef alcaPhiSym_tree_v3_cxx
alcaPhiSym_tree_v3::alcaPhiSym_tree_v3(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("root://eoscms//eos/cms/store/group/alca_ecalcalib/EFlow/AlCaPhiSym_Run2012A-v1_RAW_190456_190710_ARb/v3/variablesTree_87_1_NPx.root");
      if (!f) {
         f = new TFile("root://eoscms//eos/cms/store/group/alca_ecalcalib/EFlow/AlCaPhiSym_Run2012A-v1_RAW_190456_190710_ARb/v3/variablesTree_87_1_NPx.root");
      }
      tree = (TTree*)gDirectory->Get("variablesTree");

   }
   Init(tree);
}

alcaPhiSym_tree_v3::~alcaPhiSym_tree_v3()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t alcaPhiSym_tree_v3::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t alcaPhiSym_tree_v3::LoadTree(Long64_t entry)
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

void alcaPhiSym_tree_v3::Init(TTree *tree)
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

   fChain->SetBranchAddress("nhit_barl", &nhit_barl, &b_nhit_barl);
   fChain->SetBranchAddress("HLTNamesLen", &HLTNamesLen, &b_HLTNamesLen);
   fChain->SetBranchAddress("HLTNames", &HLTNames, &b_HLTNames);
   fChain->SetBranchAddress("HLTResults", &HLTResults, &b_HLTResults);
   fChain->SetBranchAddress("nL1bits", &nL1bits, &b_nL1bits);
   fChain->SetBranchAddress("L1bits", L1bits, &b_L1bits);
   fChain->SetBranchAddress("nL1bitsTech", &nL1bitsTech, &b_nL1bitsTech);
   fChain->SetBranchAddress("L1bitsTech", L1bitsTech, &b_L1bitsTech);
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
   fChain->SetBranchAddress("alpha_barl", alpha_barl, &b_alpha_barl);
   fChain->SetBranchAddress("et_barl", et_barl, &b_et_barl);
   fChain->SetBranchAddress("unixtime", &unixtime, &b_unixtime);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("eventid", &eventid, &b_eventid);
   fChain->SetBranchAddress("nhit_endc", &nhit_endc, &b_nhit_endc);
   fChain->SetBranchAddress("ix", ix, &b_ix);
   fChain->SetBranchAddress("iy", iy, &b_iy);
   fChain->SetBranchAddress("sign", sign, &b_sign);
   fChain->SetBranchAddress("lc_endc", lc_endc, &b_lc_endc);
   fChain->SetBranchAddress("alpha_endc", alpha_endc, &b_alpha_endc);
   fChain->SetBranchAddress("et_endc", et_endc, &b_et_endc);
   Notify();
}

Bool_t alcaPhiSym_tree_v3::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void alcaPhiSym_tree_v3::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t alcaPhiSym_tree_v3::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef alcaPhiSym_tree_v3_cxx
