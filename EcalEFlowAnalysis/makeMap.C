#define makeMap_cxx
#include "makeMap.h"
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
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


#define kBarlRings 85
#define kBarlWedges 360
#define kSides 2
#define  kEndcWedgesX  100
#define  kEndcWedgesY  100
#define NHITS 900

using namespace std;

void makeMap::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L makeMap.C
//      Root > makeMap t
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

   Long64_t nentries = fChain->GetEntries();
   cout<<"nentries "<<nentries<<endl;

   Long64_t nbytes = 0, nb = 0;

   map<pair<int,int>,  pair<int,unsigned long int> > theMap;
   map<pair<int,int>,  int > theMapTime;

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry); 
     nbytes += nb;
     // if (Cut(ientry) < 0) continue;
     
     if(jentry%1000000==0) std::cout<<jentry<<std::endl;
     

     if(theMap[make_pair(run,lumi)].first ==0 ) {
       theMap[make_pair(run,lumi)].first=nhit;
       theMap[make_pair(run,lumi)].second=unixtime;
       theMapTime[make_pair(run,lumi)]=1;
       //       cout<<theMap[make_pair(run,lumi)].second<<" "<<       theMap[make_pair(run,lumi)].first<<" unixtime "<<unixtime<<endl;

     }else{
       //       cout<<"before "<<theMap[make_pair(run,lumi)].second<<"add";
       theMap[make_pair(run,lumi)].first+=(int)nhit;
       theMap[make_pair(run,lumi)].second+=unixtime;
       theMapTime[make_pair(run,lumi)]++;
       //       long int add=(int)theMap[make_pair(run,lumi)].second;
       //long int dummy=((int)unixtime*((unsigned int)nhit));  
       //       add+=unixtime*((unsigned int)nhit);
       //       theMap[make_pair(run,lumi)].second =add+dummy;
       //       cout<<" nhit "<<nhit<<" unixtime*nhit "<<nhit*unixtime<<" result "<<       theMap[make_pair(run,lumi)].second<<"counter"<< theMapTime[make_pair(run,lumi)]<<endl;
       //       cout<<theMap[make_pair(run,lumi)].second<<" "<<       theMap[make_pair(run,lumi)].first<<" unixtime "<<unixtime<<endl;
     }
     //     cout<<"Run "<<run<<" LS"<<lumi<<theMap[make_pair(run,lumi)]<<endl;
     

     
   }




   TFile * mapFile=TFile::Open(outFileName,"recreate");
   //   mapFile->cd(); 
   int runBarl,lsBarl,nhitBarl;
   long int unixtimeBarl;
   TTree* mapTree= new TTree("mapTree_barl","mapTree_barl");
   mapTree->Branch("run",&runBarl,"run/I");
   mapTree->Branch("ls",&lsBarl,"ls/I");
   mapTree->Branch("nhit",&nhitBarl,"nhit/I");
   mapTree->Branch("unixtime",&unixtimeBarl,"unixtime/i");

   map<pair<int,int>,  pair<int, unsigned long int> >::iterator it;
   for(it=theMap.begin();it!=theMap.end();++it){
     //     cout<<it->first.first<<" "<<it->first.second<<" "<<it->second<<endl; 
     runBarl=it->first.first;
     lsBarl=it->first.second;
     nhitBarl=it->second.first;


     unixtimeBarl=(it->second.second)/theMapTime[make_pair(runBarl,lsBarl)];
     //     cout<<"###"<<it->second.second<<" "<<nhitBarl<<" "<<unixtimeBarl<<endl;
     mapTree->Fill();

   }


   mapFile->cd();
   mapTree->Write();
   mapFile->Write();
   mapFile->Close();


}
