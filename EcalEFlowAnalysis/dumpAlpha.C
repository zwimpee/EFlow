#include "TTree.h"
#include "TFile.h"
#include "TStyle.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TString.h"
#include "TBranch.h"
#include "TCanvas.h"
#include "TH2F.h"

#include <iostream>
#include <cmath>

#define kBarlRings 85
#define kEndcRings 39

#define kBarlWedges 360
#define kEndcWedgesX 100
#define kEndcWedgesY 100

#define kSides 2

#define kTowerPerSM 68
#define kXtalPerSM 1700
#define kSM 36

#define kEndcSCs 624
#define kEndcXtals 14648

bool isChinese(int ieta, int iphi)
{
  if( (ieta>-76 ) && (ieta<76) )
    return false;
  
  if(ieta>75){
    if(iphi>300 && iphi<=340) return true;
    if(iphi>120 && iphi<=140) return true;
    if(iphi>231 && iphi<=240) return true;
    if(ieta>=81){
      if(iphi>220 && iphi<=240) return true;
      if(iphi>180 && iphi<=200) return true;
    }
  }

  if(ieta<-76)
    {
      if(iphi>200 && iphi<=220) return true;
      if(iphi>240 && iphi<=300) return true;
      if(ieta<= -81){
	if(iphi>20 && iphi<=40) return true;
	if(iphi>60 && iphi<=100) return true;
      }
    }
  return false;
}

// return an index between 1 and 36
int iSM(int eta, int phi, int isign)
{
  int id = ( phi - 1 ) / 20 + 1;
  if ( isign == 0 ) id += 18;
  return id;
}

// return an index between 1 and 68
int iTower(int eta, int phi, int isign)
{
  int ttEta=(eta-1)/5+1;
  int ttPhi=((phi-1)%20)/5+1;
  return (ttEta-1)*4+ttPhi;
   }

// return an index between 1 and 2448
int iTT(int eta, int phi, int isign)
{
  return (iSM(eta,phi,isign)-1)*68+iTower(eta,phi,isign);
}


void dumpAlpha(
	       TString fitResultsFile="file1.root",
	       int det=0,
	       int averageRing=0,
	       int ndofCut=0,
	       TString eeIndicesFile="data/eeIndicesMap.root"
	       )
{

  std::cout << "Opening eeIndicesMap  " << eeIndicesFile << std::endl;

  TFile *fee=TFile::Open(eeIndicesFile);
  if (!fee->IsOpen())
    std::cout << "Cannot open " << eeIndicesFile << std::endl;
  TTree* eeIndicesTree= (TTree*)fee->Get("eeIndices");
  
  int ixVar,iyVar,signVar,iringVar,ixtalVar,iSCVar,iLMVar;
  TBranch *b_ix=eeIndicesTree->GetBranch("ix");
  TBranch *b_iy=eeIndicesTree->GetBranch("iy");
  TBranch *b_sig=eeIndicesTree->GetBranch("sign");
  TBranch *b_iring=eeIndicesTree->GetBranch("iring");
  TBranch *b_ixtal=eeIndicesTree->GetBranch("ixtal");
  TBranch *b_isc=eeIndicesTree->GetBranch("isc");
  TBranch *b_ilm=eeIndicesTree->GetBranch("ilm");
  
  eeIndicesTree->SetBranchAddress("ix", &ixVar, &b_ix);
  eeIndicesTree->SetBranchAddress("iy", &iyVar, &b_iy);
  eeIndicesTree->SetBranchAddress("sign", &signVar, &b_sig);
  eeIndicesTree->SetBranchAddress("iring", &iringVar, &b_iring);
  eeIndicesTree->SetBranchAddress("ixtal", &ixtalVar, &b_ixtal);
  eeIndicesTree->SetBranchAddress("isc", &iSCVar, &b_isc);
  eeIndicesTree->SetBranchAddress("ilm", &iLMVar, &b_ilm);
  
  int nentries_ee = eeIndicesTree->GetEntries();
  int endcapType[kEndcWedgesX][kEndcWedgesY][kSides];
  int endcapRings[kEndcWedgesX][kEndcWedgesY][kSides];
  int endcapXtals[kEndcWedgesX][kEndcWedgesY][kSides];
  int endcapSCs[kEndcWedgesX][kEndcWedgesY][kSides];
  int endcapLMs[kEndcWedgesX][kEndcWedgesY][kSides];
  
  for (int isign=0;isign<kSides;++isign)
    for (int ix=0;ix<kEndcWedgesX;++ix)
      for (int iy=0;iy<kEndcWedgesX;++iy)
	{
	  endcapType[ix][iy][isign]=0;
	  endcapRings[ix][iy][isign]=-1;
	  endcapXtals[ix][iy][isign]=-1;
	  endcapSCs[ix][iy][isign]=-1;
	  endcapLMs[ix][iy][isign]=-1;
	}
  
  for(int jentry=0;jentry<nentries_ee;++jentry)
    {
      eeIndicesTree->GetEntry(jentry);
      endcapRings[ixVar-1][iyVar-1][signVar]=iringVar;
      endcapXtals[ixVar-1][iyVar-1][signVar]=ixtalVar;
      endcapSCs[ixVar-1][iyVar-1][signVar]=iSCVar;
      endcapLMs[ixVar-1][iyVar-1][signVar]=iLMVar;
    }

  std::cout << "Read eeIndices for " << nentries_ee << " xtals " << std::endl;

  TFile *_file0 = TFile::Open(fitResultsFile);


  TTree* fitResults=(TTree*)_file0->Get("fitResults");
  
  int ietaVar,iphiVar,statusVar,badXtalVar,indexVar;
  double alphaVar,err_alphaVar,alpha0,chi2,ndof;
  
  TBranch *b_ieta=fitResults->GetBranch("ieta");
  TBranch *b_sign=fitResults->GetBranch("sign");
  TBranch *b_badXtal=fitResults->GetBranch("badXtal");
  TBranch *b_iphi=fitResults->GetBranch("iphi");
  TBranch *b_index=fitResults->GetBranch("index");
  TBranch *b_status=fitResults->GetBranch("status");
  TBranch *b_alpha=fitResults->GetBranch("delta_alpha");
  TBranch *b_alpha0=fitResults->GetBranch("alpha0");
  TBranch *b_erralpha=fitResults->GetBranch("err_alpha");
  TBranch *b_chi2=fitResults->GetBranch("chi2Min");
  TBranch *b_ndof=fitResults->GetBranch("ndof");

  
  fitResults->SetBranchAddress("ieta", &ietaVar, &b_ieta);
  fitResults->SetBranchAddress("iphi", &iphiVar, &b_iphi);
  fitResults->SetBranchAddress("sign", &signVar, &b_sign);
  fitResults->SetBranchAddress("badXtal", &badXtalVar, &b_badXtal);
  fitResults->SetBranchAddress("index", &indexVar, &b_index);
  fitResults->SetBranchAddress("status", &statusVar, &b_status);
  fitResults->SetBranchAddress("delta_alpha", &alphaVar, &b_alpha);
  fitResults->SetBranchAddress("alpha0", &alpha0, &b_alpha0);
  fitResults->SetBranchAddress("err_alpha", &err_alphaVar, &b_erralpha);
  fitResults->SetBranchAddress("chi2Min", &chi2, &b_chi2);
  fitResults->SetBranchAddress("ndof", &ndof, &b_ndof);

  std::cout << "### File generated from alphaFit file " << fitResultsFile << std::endl;
  if (det==0) 
    std::cout << "##\tieta\tiphi\talpha\talpha_err\tcomment" << std::endl;
  else if (det==1) 
    std::cout << "##\tix\tiy\tsign\talpha\talpha_err\tcomment" << std::endl;
  int nentries = fitResults->GetEntries();

  double alphaAverageRing_BTCP[kEndcRings];
  double alphaAverageErrRing_BTCP[kEndcRings];

  double alphaAverageRing_SIC[kEndcRings];
  double alphaAverageErrRing_SIC[kEndcRings];

  int nXtalsRing_BTCP[kEndcRings];
  int nXtalsRing_SIC[kEndcRings];

  TH1F* ringAlphas_BTCP[kEndcRings];
  TH1F* ringAlphas_SIC[kEndcRings];

  for (int i=0;i<kEndcRings;++i)
    {
      alphaAverageRing_BTCP[i]=0;
      alphaAverageErrRing_BTCP[i]=0;
      
      alphaAverageRing_SIC[i]=0;
      alphaAverageErrRing_SIC[i]=0;

      nXtalsRing_BTCP[i]=0;
      nXtalsRing_SIC[i]=0;

      TString name="alpha_BTCP_";
      name+=i;
      ringAlphas_BTCP[i]=new TH1F(name,name,100,0.,3.);

      name="alpha_SIC_";
      name+=i;
      ringAlphas_SIC[i]=new TH1F(name,name,100,0.,3.);
    }

  for(int jentry=0;jentry<nentries;++jentry)
    {
      fitResults->GetEntry(jentry);
      float alphaFit=alpha0+alphaVar;
      //      float alphaFit=alphaVar;
      alphaFit=TMath::Max((double)0.2,(double)alphaFit);
      alphaFit=TMath::Min((double)3,(double)alphaFit);

      if (det==1)
	{
	  if (alpha0>1.05)
	    {
	      endcapType[ietaVar-1][iphiVar-1][signVar]=0;
	    }
	  else
	    {
	      endcapType[ietaVar-1][iphiVar-1][signVar]=1;
	    }
	}
	    
      if (badXtalVar==0 && statusVar==0 && err_alphaVar<0.25 && err_alphaVar>0. && ndof>ndofCut && chi2/ndof<1.5) //880 for TT fit in 2012B, 39 forxtal fit
	{
	  if (averageRing==0)
	    {
	      if (det==0)
		std::cout << "\t" <<  ietaVar << "\t" << iphiVar << "\t" <<alphaFit << "\t"  <<  err_alphaVar << "\tok" << std::endl;
	      else if (det==1)
		std::cout << "\t" <<  ietaVar << "\t" << iphiVar  << "\t" << signVar << "\t"  <<alphaFit << "\t"  <<  err_alphaVar << "\tok" << std::endl;
	    }
	  else
	    {
	      if (det==1)
		{
		  //float weight=1/(err_alphaVar*err_alphaVar);
		  //std::cout << weight << std::endl;
		  float weight=1/(0.05*0.05);
		  if (alpha0<1.05)
		    {
		      ringAlphas_SIC[endcapRings[ietaVar-1][iphiVar-1][signVar]]->Fill(alphaFit);
		      alphaAverageRing_SIC[endcapRings[ietaVar-1][iphiVar-1][signVar]]+=(alphaFit)*weight;
		      alphaAverageErrRing_SIC[endcapRings[ietaVar-1][iphiVar-1][signVar]]+=weight;
		      nXtalsRing_SIC[endcapRings[ietaVar-1][iphiVar-1][signVar]]++;
		    }
		  else
		    {
		      //		      std::cout << alphaFit << std::endl;
		      ringAlphas_BTCP[endcapRings[ietaVar-1][iphiVar-1][signVar]]->Fill(alphaFit);
		      alphaAverageRing_BTCP[endcapRings[ietaVar-1][iphiVar-1][signVar]]+=(alphaFit)*weight;
		      alphaAverageErrRing_BTCP[endcapRings[ietaVar-1][iphiVar-1][signVar]]+=weight;
		      nXtalsRing_BTCP[endcapRings[ietaVar-1][iphiVar-1][signVar]]++;
		    }
		}
	    }
	}
      else
	{
	  float alpha = 0.;
	  if (det==0)
	    {
	      alpha=isChinese(ietaVar,iphiVar) ? 1.0 : 1.52; 
	    }
	  else if (det==1)
	    {
	      alpha=endcapType[ietaVar-1][iphiVar-1][signVar] ? 1.0 : 1.16;
	    }
	  
	  //	  if (badXtalVar==0 && statusVar==0 && TMath::Abs(alphaVar)>2*err_alphaVar && alphaFit>0.3 )
	  if (badXtalVar==0 && statusVar==0 )
	    {
	      if (averageRing==0)
		{
		  if (det==0)
		    {
		      std::cout <<  "\t" << ietaVar << "\t" << iphiVar << "\t" <<alphaFit << "\t"  <<  err_alphaVar << "\tlarge_err_fit" << std::endl;
		    }
		  else if (det==1)
		    {
		      std::cout << "\t" <<  ietaVar << "\t" << iphiVar  << "\t" << signVar << "\t"  <<alphaFit << "\t"  <<  err_alphaVar << "\tlarge_err_fit" << std::endl;
		    }
		}
	    }
	  else
	    {
	      if (averageRing==0)
		{
		  if (det==0)
		    {
		      std::cout << "\t" << ietaVar << "\t" << iphiVar << "\t" << alpha << "\t-9.999" << "\tbad_fit" << std::endl;
		    }
		  else if (det==1)
		    {
		      std::cout << "\t" <<  ietaVar << "\t" << iphiVar  << "\t" << signVar << "\t" <<alpha << "\t-9.999"  << "\tbad_fit" << std::endl;
		    }
		}
	    }
	} //end of block for bad fits
    }//end of loop over entries

  if (averageRing==1 && det==1)
    {
      TFile *fOut=TFile::Open("dumpAlpha.root","RECREATE");
      for (int iring=0;iring<kEndcRings;++iring)
	{
	  ringAlphas_BTCP[iring]->Write();
	  ringAlphas_SIC[iring]->Write();
	  if (alphaAverageErrRing_BTCP[iring]>0)
	    {
	      alphaAverageRing_BTCP[iring]=alphaAverageRing_BTCP[iring]/alphaAverageErrRing_BTCP[iring];
	      alphaAverageErrRing_BTCP[iring]=1/sqrt(alphaAverageErrRing_BTCP[iring]);
	      //	      std::cout << "iring " << iring << " nXtals " << nXtalsRing_BTCP[iring] << "\t" << alphaAverageRing_BTCP[iring] << "," << alphaAverageErrRing_BTCP[iring] << std::endl;
	    }
	  else
	    {
	      alphaAverageRing_BTCP[iring]=1.16;
	      alphaAverageErrRing_BTCP[iring]=-999.;
	    }

	  if (alphaAverageErrRing_SIC[iring]>0)
	    {
	      alphaAverageRing_SIC[iring]=alphaAverageRing_SIC[iring]/alphaAverageErrRing_SIC[iring];
	      alphaAverageErrRing_SIC[iring]=1/sqrt(alphaAverageErrRing_SIC[iring]);
	      //	      std::cout << "iring " << iring << "\t" << alphaAverageRing_SIC[iring] << "," << alphaAverageErrRing_SIC[iring] << std::endl;
	    }
	  else
	    {
	      alphaAverageRing_SIC[iring]=1.;
	      alphaAverageErrRing_SIC[iring]=-999.;
	    }
	}
      fOut->Write();
      for (int isign=0;isign<2;++isign)
	for (int ix=0;ix<kEndcWedgesX;++ix)
	  for (int iy=0;iy<kEndcWedgesX;++iy)
	    {
  	      if (endcapRings[ix][iy][isign]==-1)
  		continue;
	      int sign= (isign==0) ? -1:1; 
	      if ( endcapType[ix][iy][isign]==0 )
		std::cout << "\t" <<  ix+1 << "\t" << iy+1  << "\t" << sign << "\t"  <<  alphaAverageRing_BTCP[endcapRings[ix][iy][isign]] << "\t"  <<  alphaAverageErrRing_BTCP[endcapRings[ix][iy][isign]] << "\tbtcp_ring_average" << std::endl;
	      else if (	endcapType[ix][iy][isign]==1 )
		std::cout << "\t" <<  ix+1 << "\t" << iy+1  << "\t" << sign <<  "\t" <<  alphaAverageRing_SIC[endcapRings[ix][iy][isign]] << "\t"  <<  alphaAverageErrRing_SIC[endcapRings[ix][iy][isign]] << "\tsic_ring_average" << std::endl;
	    }
    }
}
