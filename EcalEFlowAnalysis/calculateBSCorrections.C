#define calculateBSCorrections_cxx
#include "calculateBSCorrections.h"
#include <TH2.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <iostream>
#include "Math/Interpolator.h"


void calculateBSCorrections::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L calculateBSCorrections.C
//      Root > calculateBSCorrections t
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

   bool doVertexWidth=false;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

   double vtxPositionArray[21];
   double nHitsArray[85][2][21];
   double etSumArray[85][2][21];

   double vtxWidArray[21];
   double nHitsWidArray[85][2][21];
   double etSumWidArray[85][2][21];

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      if (bsWidBin==-1)
	{
	  vtxPositionArray[vtxBin]=vertexPosition;
	  nHitsArray[ieta][side][vtxBin]=nHits;
	  etSumArray[ieta][side][vtxBin]=etSum;
	}
      else
	{
	  vtxWidArray[bsWidBin]=bsWid;
	  nHitsWidArray[ieta][side][bsWidBin]=nHits;
	  etSumWidArray[ieta][side][bsWidBin]=etSum;
	}
   }

   TFile fOut("beamSpotInterpolatedCorrections.root","RECREATE");
   for (int iieta=0;iieta<85;++iieta)
     //     for (int iside=0;iside<2;++iside)
       {
	 double nHitsCorrection[21];
	 //	 ROOT::Math::Interpolator inter(21, ROOT::Math::Interpolation::kPOLYNOMIAL);
	 ROOT::Math::Interpolator inter(21, ROOT::Math::Interpolation::kCSPLINE);
	 for (int ivtx=0;ivtx<21;++ivtx)
	 {
	   std::cout << "++++++ iieta " << iieta << " " << " ------ " << vtxPositionArray[ivtx] << std::endl;
	   //	       nHitsCorrection[ivtx]+=1+(nHitsArray[iieta][iside][ivtx]-nHitsArray[iieta][iside][10])/nHitsArray[iieta][iside][10];
	   //nHitsCorrection[ivtx]=1+((etSumArray[iieta][1][ivtx]-etSumArray[iieta][0][ivtx])/(etSumArray[iieta][1][ivtx]+etSumArray[iieta][0][ivtx]))/2.;
	   nHitsCorrection[ivtx]=etSumArray[iieta][1][ivtx]/((etSumArray[iieta][1][ivtx]+etSumArray[iieta][0][ivtx])/2.);
	   //	   nHitsCorrection[ivtx]=nHitsCorrection[ivtx]/2.;
	   std::cout <<  nHitsCorrection[ivtx] << std::endl;
	 }
 	 for (int ivtx=0;ivtx<21;++ivtx)
	   {
	     if (ivtx==10)
	       continue;
	     nHitsCorrection[ivtx]=nHitsCorrection[ivtx]/nHitsCorrection[10];
	   }
	 nHitsCorrection[10]=1.;

	 inter.SetData(21,vtxPositionArray,nHitsCorrection);
	 double vtxPos[1000];
	 double nHitsInt[1000];
	 for (int i=0;i<1000;++i)
	   {
	     vtxPos[i]=-1.97+3.94/1000.*i;
	     nHitsInt[i]=inter.Eval(vtxPos[i]);
	   }

	 TGraph* g=new TGraph(1000,vtxPos,nHitsInt);
	 TString name="bsPosCorrectionInterpolated_ieta_";
	 name+=(iieta+1);
//  	 name+="_side_";
//  	 name+=iside;
	 g->SetName(name);
	 g->Write();

	 TGraph* g_o=new TGraph(21,vtxPositionArray,nHitsCorrection);
	 name="bsPosCorrection_ieta_";
	 name+=(iieta+1);
//  	 name+="_side_";
//  	 name+=iside;
	 g_o->SetName(name);
	 g_o->Write();

	 if (doVertexWidth)
	   {
	 double nHitsWidCorrection[21];
	 //	 ROOT::Math::Interpolator inter(21, ROOT::Math::Interpolation::kPOLYNOMIAL);
	 ROOT::Math::Interpolator interWid(21, ROOT::Math::Interpolation::kCSPLINE);
	 for (int ivtx=0;ivtx<21;++ivtx)
	 {
	   std::cout << "++++++ Wid " << vtxWidArray[ivtx] << std::endl;
	   nHitsWidCorrection[ivtx]=0;
	   for (int iside=0;iside<2;++iside)
	     {
	       nHitsWidCorrection[ivtx]=1+((etSumWidArray[iieta][1][ivtx]-etSumWidArray[iieta][0][ivtx])/(etSumWidArray[iieta][1][ivtx]+etSumWidArray[iieta][0][ivtx]))/2.;	       
	     }
	   //	   nHitsWidCorrection[ivtx]=nHitsWidCorrection[ivtx]/2.;
	   std::cout <<  nHitsWidCorrection[ivtx] << std::endl;
	 }
 	 for (int ivtx=0;ivtx<21;++ivtx)
	   {
	     if (ivtx==10)
	       continue;
	     nHitsWidCorrection[ivtx]=nHitsWidCorrection[ivtx]/nHitsWidCorrection[10];
	   }
	 nHitsWidCorrection[10]=1.;
// 	 for (int ivtx=0;ivtx<21;++ivtx)
// 	   nHitsWidCorrection[ivtx]=nHitsWidCorrection[ivtx]/nHitsWidCorrection[10];
	 interWid.SetData(21,vtxWidArray,nHitsWidCorrection);
	 double vtxWid[1000];
	 double nHitsWidInt[1000];
	 for (int i=0;i<1000;++i)
	   {
	     vtxWid[i]=4.51+2.99/1000.*i;
	     nHitsWidInt[i]=interWid.Eval(vtxWid[i]);
	   }

	 TGraph* gw=new TGraph(1000,vtxWid,nHitsWidInt);
	 name="bsWidCorrectionInterpolated_ieta_";
	 name+=(iieta+1);
//  	 name+="_side_";
//  	 name+=iside;
	 gw->SetName(name);
	 gw->Write();

	 TGraph* gw_o=new TGraph(21,vtxWidArray,nHitsWidCorrection);
	 name="bsWidCorrection_ieta_";
	 name+=(iieta+1);
//  	 name+="_side_";
//  	 name+=iside;
	 gw_o->SetName(name);
	 gw_o->Write();
	   }
       }
   fOut.Write();
   fOut.Close();
}
