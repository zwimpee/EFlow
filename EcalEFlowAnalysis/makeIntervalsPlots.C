#define makeIntervalsPlots_cxx
#include "makeIntervalsPlots.h"
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



void makeIntervalsPlots::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L makeIntervalsPlots.C
//      Root > makeIntervalsPlots t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   float * interval;
   float * time;
   float * rechitRatekHz_barl;
   float * intervalLength;
   float * nHits_barl;

   cout<<"ok"<<endl;
   interval= new float [nentries];
   time= new float [nentries];
   rechitRatekHz_barl= new float [nentries];
   intervalLength= new float [nentries];
   nHits_barl= new float [nentries];


   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      interval[jentry]=index;
      time[jentry]=unixtimeMean;
      rechitRatekHz_barl[jentry]=nHit/(1000*(unixtimeEnd-unixtimeStart));
      intervalLength[jentry]=unixtimeEnd-unixtimeStart;
      nHits_barl[jentry]=nHit;
    }


   TFile *outFile=TFile::Open(outFileName,"recreate");

   TGraph* indexvstime=new TGraph(nentries,&time[0],&interval[0]);
   indexvstime->SetName("indexvstime");
   indexvstime->SetTitle("indexvstime");
   indexvstime->GetXaxis()->SetTitle("Time");
   indexvstime->GetXaxis()->SetTimeDisplay(1);
   indexvstime->GetXaxis()->SetTimeFormat("%d/%m");
   indexvstime->GetYaxis()->SetTitle("index");

   indexvstime->Write();

   TGraph* rechitRatekHz_barlvstime=new TGraph(nentries,&time[0],&rechitRatekHz_barl[0]);
   rechitRatekHz_barlvstime->SetName("rechitRatekHz_barlvstime");
   rechitRatekHz_barlvstime->SetTitle("rechitRatekHz_barlvstime");
   rechitRatekHz_barlvstime->GetXaxis()->SetTitle("Time");
   rechitRatekHz_barlvstime->GetXaxis()->SetTimeDisplay(1);
   rechitRatekHz_barlvstime->GetXaxis()->SetTimeFormat("%d/%m");
   rechitRatekHz_barlvstime->GetYaxis()->SetTitle("EB RecHit Rate (kHZ)");

   rechitRatekHz_barlvstime->Write();

   TGraph* intervalLengthvstime=new TGraph(nentries,&time[0],&interval[0]);
   intervalLengthvstime->SetName("intervalLengthvstime");
   intervalLengthvstime->SetTitle("intervalLengthvstime");
   intervalLengthvstime->GetXaxis()->SetTitle("Time");
   intervalLengthvstime->GetXaxis()->SetTimeDisplay(1);
   intervalLengthvstime->GetXaxis()->SetTimeFormat("%d/%m");
   intervalLengthvstime->GetYaxis()->SetTitle("interval length (s)");

   intervalLengthvstime->Write();

   TGraph* nHits_barlvstime=new TGraph(nentries,&time[0],&nHits_barl[0]);
   nHits_barlvstime->SetName("nHits_barlvstime");
   nHits_barlvstime->SetTitle("nHits_barlvstime");
   nHits_barlvstime->GetXaxis()->SetTitle("Time");
   nHits_barlvstime->GetXaxis()->SetTimeDisplay(1);
   nHits_barlvstime->GetXaxis()->SetTimeFormat("%d/%m");
   nHits_barlvstime->GetYaxis()->SetTitle("nHits");

   nHits_barlvstime->Write();



   outFile->Write();
   outFile->Close();
}
