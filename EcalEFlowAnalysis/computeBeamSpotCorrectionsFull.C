#define computeBeamSpotCorrections_cxx
#include "computeBeamSpotCorrections.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <iostream>
#define nVertexBins 21
#define nVertexWidBins 21
#define TEST
void computeBeamSpotCorrections::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L computeBeamSpotCorrections.C
//      Root > computeBeamSpotCorrections t
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


   float vertexPosition[nVertexBins];
   float totalEtSum[nVertexBins];
   float totalHits[nVertexBins];
   float nevents[nVertexBins];

   float vertexWid[nVertexWidBins];
   float totalEtSumWid[nVertexWidBins];
   float totalHitsWid[nVertexWidBins];
   float neventsWid[nVertexWidBins];

   float etSum[85][2][nVertexBins];
   float nHits[85][2][nVertexBins];

   float etSumWid[85][2][nVertexWidBins];
   float nHitsWid[85][2][nVertexWidBins];

   float totHits;
   float totEtSum;
   float vtxPosition;
   float bsWid;
   float nev;
   int vtxBin;
   int bsWidBin;

   int eta;
   int side;
   float etSumAcc;
   float nHitsAcc;

   TFile* outFile=TFile::Open("beamSpotCorrectionsFull.root","recreate");
   TTree* outTree= new TTree("tree_barl","tree_barl");
   outTree->Branch("vertexPosition",&vtxPosition,"vertexPosition/F");
   outTree->Branch("bsWid",&bsWid,"bsWid/F");
   outTree->Branch("nevents",&nev,"nevents/F");
   outTree->Branch("totalEtSum",&totEtSum,"totalEtSum/F");
   outTree->Branch("totalHits",&totHits,"totalHits/F");
   outTree->Branch("ieta",&eta,"ieta/I");
   outTree->Branch("side",&side,"side/I");
   outTree->Branch("vtxBin",&vtxBin,"vtxBin/I");
   outTree->Branch("bsWidBin",&bsWidBin,"bsWidBin/I");
   outTree->Branch("etSum",&etSumAcc,"etSum/F");
   outTree->Branch("nHits",&nHitsAcc,"nHits/F");

   for (int ivertex=0;ivertex<nVertexBins;++ivertex)
     {
       vertexPosition[ivertex]=0;
       nevents[ivertex]=0;
       totalEtSum[ivertex]=0;
       totalHits[ivertex]=0;

       for (int iieta=0;iieta<85;++iieta)
	 {
	   for (int isign=0;isign<2;++isign)
	     {
	       etSum[iieta][isign][ivertex]=0.;
	       nHits[iieta][isign][ivertex]=0;
	     }
	 }
     }

   for (int ivertex=0;ivertex<nVertexWidBins;++ivertex)
     {
       vertexWid[ivertex]=0;
       neventsWid[ivertex]=0;
       totalEtSumWid[ivertex]=0;
       totalHitsWid[ivertex]=0;

       for (int iieta=0;iieta<85;++iieta)
	 {
	   for (int isign=0;isign<2;++isign)
	     {
	       etSumWid[iieta][isign][ivertex]=0.;
	       nHitsWid[iieta][isign][ivertex]=0;
	     }
	 }
     }


   TFile fWeights("bsWeights.root");

   for (int ivertex=0;ivertex<nVertexBins;++ivertex)
     {
       std::cout << "++++++++ VERTEX BIN " << ivertex  << " ++++++++++++++ " << std::endl;

       TString name="bSpotPosWeights_";
       name+=ivertex;

#ifdef TEST
       nentries=1000000;
#endif       
       TH1F* weights=(TH1F*)fWeights.Get(name);
       Long64_t nbytes = 0, nb = 0;

       for (Long64_t jentry=0; jentry<nentries;jentry++) {
	 Long64_t ientry = LoadTree(jentry);
	 if (ientry < 0) break;
	 nb = fChain->GetEntry(jentry);   nbytes += nb;
	 if(jentry%100000==0) std::cout<< "EVENT " << jentry<< " RUN " << run << " LS " << lumi << std::endl; 
	 
	 if (fabs(vzMC)>30.)
	   continue;
	 //	 float weight=1;
	 //	 if (ivertex!=10) //BEWARE THIS IS DEPENDENT ON THE NUMBER OF BINS
	 float weight=weights->GetBinContent(weights->GetXaxis()->FindBin(vzMC));
	 
	 vertexPosition[ivertex] += vzMC*weight;
	 nevents[ivertex]+=weight;
	 
	 for (unsigned int ihit=0;ihit<nhit;++ihit)
	   {
	     int theSign=ieta[ihit]>0 ? 1:0;
	     int theEta=TMath::Abs(ieta[ihit]);
	     int thePhi=iphi[ihit];
	     
	     etSum[theEta-1][theSign][ivertex]+=et_barl[ihit]*weight;
	     nHits[theEta-1][theSign][ivertex]+=weight;
	     totalEtSum[ivertex]+=et_barl[ihit]*weight;
	     totalHits[ivertex]+=weight;
	   }      
	 // if (Cut(ientry) < 0) continue;
       }
       
       //    for (int ivertex=0;ivertex<nVertexBins;++ivertex)
       //      {
       vertexPosition[ivertex]=vertexPosition[ivertex]/nevents[ivertex];
       for (int iieta=0;iieta<85;++iieta)
	 {
	   for (int isign=0;isign<2;++isign)
	     {
	       etSum[iieta][isign][ivertex]=etSum[iieta][isign][ivertex]/totalEtSum[ivertex];
	       nHits[iieta][isign][ivertex]=(float)nHits[iieta][isign][ivertex]/(float)totalHits[ivertex];
	       vtxPosition=vertexPosition[ivertex];
	       nev=nevents[ivertex];
	       totEtSum=totalEtSum[ivertex];
	       totHits=totalHits[ivertex];
	       bsWidBin=-1;
	       bsWid=-1;
	       eta=iieta;
	       side=isign;
	       vtxBin=ivertex;
	       etSumAcc=etSum[iieta][isign][ivertex];
	       nHitsAcc=nHits[iieta][isign][ivertex];
	       outTree->Fill();
	     }
	 }
     }

   for (int ivertex=0;ivertex<nVertexWidBins;++ivertex)
     {
       std::cout << "++++++++ VERTEX WIDTH BIN " << ivertex  << " ++++++++++++++ " << std::endl;

       TString name="bSpotWidWeights_";
       name+=ivertex;

#ifdef TEST
       nentries=1000000;
#endif       
       TH1F* weights=(TH1F*)fWeights.Get(name);
       Long64_t nbytes = 0, nb = 0;

       for (Long64_t jentry=0; jentry<nentries;jentry++) {
	 Long64_t ientry = LoadTree(jentry);
	 if (ientry < 0) break;
	 nb = fChain->GetEntry(jentry);   nbytes += nb;
	 if(jentry%100000==0) std::cout<< "EVENT " << jentry<< " RUN " << run << " LS " << lumi << std::endl; 
	 
	 if (fabs(vzMC)>30.)
	   continue;
	 //	 float weight=1;
	 //	 if (ivertex!=10) //BEWARE THIS IS DEPENDENT ON THE NUMBER OF BINS
	 float weight=weights->GetBinContent(weights->GetXaxis()->FindBin(vzMC));
	 
	 //vertexWid[ivertex] += vzMC*weight;
	 neventsWid[ivertex]+=weight;
	 
	 for (unsigned int ihit=0;ihit<nhit;++ihit)
	   {
	     int theSign=ieta[ihit]>0 ? 1:0;
	     int theEta=TMath::Abs(ieta[ihit]);
	     int thePhi=iphi[ihit];
	     
	     etSumWid[theEta-1][theSign][ivertex]+=et_barl[ihit]*weight;
	     nHitsWid[theEta-1][theSign][ivertex]+=weight;
	     totalEtSumWid[ivertex]+=et_barl[ihit]*weight;
	     totalHitsWid[ivertex]+=weight;
	   }      
	 // if (Cut(ientry) < 0) continue;
       }
       
       //    for (int ivertex=0;ivertex<nVertexBins;++ivertex)
       //      {
       //HACK to get it done at the moment
       vertexWid[ivertex]=4.5 + ivertex*0.15;
       for (int iieta=0;iieta<85;++iieta)
	 {
	   for (int isign=0;isign<2;++isign)
	     {
	       etSumWid[iieta][isign][ivertex]=etSumWid[iieta][isign][ivertex]/totalEtSumWid[ivertex];
	       nHitsWid[iieta][isign][ivertex]=(float)nHitsWid[iieta][isign][ivertex]/(float)totalHitsWid[ivertex];
	       vtxPosition=0;
	       nev=neventsWid[ivertex];
	       totEtSum=totalEtSumWid[ivertex];
	       totHits=totalHitsWid[ivertex];
	       bsWidBin=ivertex;
	       bsWid=vertexWid[ivertex];
	       eta=iieta;
	       side=isign;
	       vtxBin=-1;
	       etSumAcc=etSumWid[iieta][isign][ivertex];
	       nHitsAcc=nHitsWid[iieta][isign][ivertex];
	       outTree->Fill();
	     }
	 }
     }

   outFile->cd();
   outTree->Write();
   outFile->Write();
   outFile->Close();
}
