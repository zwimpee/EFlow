//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jan 16 15:04:47 2012 by ROOT version 5.27/06b
// from TTree mapTree_barl/mapTree_barl
// found on file: map_barl.root
//////////////////////////////////////////////////////////

#ifndef readMap_h
#define readMap_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <JSON.h>
#include <iostream>

#define NXTALS 61200



class readMap {
public :
  int NHITSMAX;
  int NTRIGGERMAX;
  int MAXSTOPHOURS;
  int MAXSTOPTIME;
    
  
  TString outFileName;

  struct interval_t
  {
    int runStart;
    int lsStart;
    int runEnd;
    int lsEnd;
    int nHit;
    unsigned long long unixTimeMean;
    int unixTimeStart;
    int unixTimeEnd;
    int nLS;

    //Check if this interval is before
  inline bool isBefore(const interval_t& b) const
    {
      if (this->runStart < b.runStart)
	{
	  return true;
	}
      else if (this->runStart == b.runStart)
	{
	  if (this->lsStart<b.lsStart)
	    return true;
	  else
	    return false;
	}
      else
	{
	  return false;
	}
    }

    //Check if b extends this interval
  inline bool isAfter(const interval_t& b) const
    {
      if (b.runEnd > this->runEnd)
	{
	  return true;
	}
      else if (this->runEnd == b.runEnd)
	{
	  if (b.lsEnd > this->lsEnd)
	    return true;
	  else
	    return false;
	}
      else
	{
	  return false;
	}
    }

  inline void mergeWith(const interval_t& b) 
    {
      if ( !isBefore(b) )
	return;
      
      if ( !isAfter(b) )
	return;
      
      this->runEnd=b.runEnd;
      this->lsEnd=b.lsEnd;
      this->unixTimeEnd=b.unixTimeEnd;
      this->unixTimeMean=((long double)this->unixTimeMean*(long double)this->nHit + (long double)b.unixTimeMean*(long double)b.nHit)/((long double)this->nHit+(long double)b.nHit);
      this->nHit=this->nHit+b.nHit;
      this->nLS=this->nLS+b.nLS;
    }
  };

  std::vector<interval_t> intervals;

  JSON* goodLS;
  
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain
  
  // Declaration of leaf types
  Int_t           run;
  Int_t           ls;
  Int_t           nhit;
  UInt_t          unixtime;
  
  // List of branches
  TBranch        *b_run;   //!
  TBranch        *b_ls;   //!
  TBranch        *b_nhit;   //!
  TBranch        *b_unixtime;   //!
  
  readMap(TTree *tree=0);
  virtual ~readMap();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree *tree);
  virtual void     Loop();
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);
  void setJSON(const char* jsonFile);
  
};

#endif

#ifdef readMap_cxx
readMap::readMap(TTree *tree) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
  outFileName="readMap.root";
  NHITSMAX=1500;
  NTRIGGERMAX=NXTALS*NHITSMAX;
  MAXSTOPHOURS=12;  
  MAXSTOPTIME=3600*12;
  if (tree == 0) {
    TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("map_barl.root");
    if (!f) {
      f = new TFile("map_barl.root");
    }
    tree = (TTree*)gDirectory->Get("mapTree_barl");
    
  }
  Init(tree);
  goodLS=0;
}

readMap::~readMap()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

void readMap::setJSON(const char* file)
{
  goodLS=new JSON(file);
}

Int_t readMap::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t readMap::LoadTree(Long64_t entry)
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

void readMap::Init(TTree *tree)
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

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("ls", &ls, &b_ls);
   fChain->SetBranchAddress("nhit", &nhit, &b_nhit);
   fChain->SetBranchAddress("unixtime", &unixtime, &b_unixtime);
   Notify();
}

Bool_t readMap::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void readMap::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t readMap::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef readMap_cxx
