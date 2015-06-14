#include <iostream>
#include <cstdlib>
#include <assert.h>

#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TH1F.h>
#include "interface/RegionStability.h"

RegionStability::measurement RegionStability::getTruncatedMeanMeasurement(float nRMS, int NMAXITER)
{
  assert (nHits_.size() == etSum_.size() && nHits_.size() == lc_.size() && nHits_.size() == alpha_.size() );

  bool isOK[nHits_.size()];
  int nHits[nHits_.size()];
  float etSum[nHits_.size()];
//   float etSumRMS[nHits_.size()];
//   float lc[nHits_.size()];
//   float lcRMS[nHits_.size()];
//   float alpha[nHits_.size()];
//   float alphaRMS[nHits_.size()];

  for (int i=0;i<nHits_.size();++i)
    {
      isOK[i]=1;
      nHits[i]=nHits_[i];
      etSum[i]=etSum_[i];
//       etSumRMS[i]=etSumRMS_[i];
//       lc[i]=lc_[i];
//       lcRMS[i]=lcRMS_[i];
//       alpha[i]=alpha_[i];
//       alphaRMS[i]=alphaRMS_[i];
    }



  float nHitsMean=TMath::Mean(nHits_.size(),&nHits[0]);
  float nHitsRMS=TMath::RMS(nHits_.size(),&nHits[0]);
  float etSumMean=TMath::Mean(nHits_.size(),&etSum[0]);
  float etSum_RMS=TMath::RMS(nHits_.size(),&etSum[0]);

//   float lcMean=TMath::Mean(nHits_.size(),&lc[0]);
//   float lcRMS=TMath::RMS(nHits_.size(),&lc[0]);
//   float alphaMean=TMath::Mean(nHits_.size(),&alpha[0]);
//   float alphaRMS=TMath::RMS(nHits_.size(),&alpha[0]);


//   TH1F hNHits("hNHits","hNHits",300,nHitsMean-nRMS*nHitsRMS,nHitsMean+nRMS*nHitsRMS);
//   TH1F hEtSum("hEtSum","hEtSum",300,etSumMean-nRMS*etSumRMS,etSumMean+nRMS*etSumRMS);
//   TH1F hEtMean("hEtMean","hEtMean",300,etMeanMean-nRMS*etMeanRMS,etMeanMean+nRMS*etMeanRMS);
//   TH1F hLc("hLc","hLc",300,lcMean-nRMS*lcRMS,lcMean+nRMS*lcRMS);
//   TH1F hAlpha("hAlpha","hAlpha",300,alphaMean-nRMS*alphaRMS,alphaMean+nRMS*alphaRMS);

  int diffChannelsOK=999;
  int oldChannelsOK=0;
  int channelsOK=0;
  int iter=0;

  while (diffChannelsOK!=0 && iter<NMAXITER)
    {
      oldChannelsOK=channelsOK;
      channelsOK=0;
      for (int i=0;i<nHits_.size();++i)
	{
	  //Simple mean truncation algorithm
	  if (isOK[i]==0)
	    continue;

 	  if (TMath::Abs(etSum_[i]-etSumMean)>nRMS*etSum_RMS)
 	    {
 	      isOK[i]=0;
 	      continue;
 	    }
	  
	  if (TMath::Abs(nHits_[i]-nHitsMean)>nRMS*nHitsRMS)
	    {
	      isOK[i]=0;
	      continue;
	    }

	  nHits[channelsOK]=nHits_[i];
	  etSum[channelsOK]=etSum_[i];
// 	  etSumRMS[channelsOK]=etSumRMS_[i];
// 	  lc[channelsOK]=lc_[i];
// 	  lcRMS[channelsOK]=lcRMS_[i];
// 	  alpha[channelsOK]=alpha_[i];
// 	  alphaRMS[channelsOK]=alphaRMS_[i];
	  channelsOK++;
	}

      diffChannelsOK=oldChannelsOK-channelsOK;
      iter++;
      nHitsMean=TMath::Mean(channelsOK,&nHits[0]);
      nHitsRMS=TMath::RMS(channelsOK,&nHits[0]);
      etSumMean=TMath::Mean(channelsOK,&etSum[0]);
      etSum_RMS=TMath::RMS(channelsOK,&etSum[0]);
      //      std::cout << "+++ " << diffChannelsOK << "," << channelsOK << "," << nHitsMean << "," << nHitsRMS << std::endl;
    }

//   if (channelsOK != nHits_.size())
//     std::cout << "***** " << channelsOK << "," << nHits_.size() << std::endl;

  measurement aMeasurement;
  aMeasurement.reset();
  for (int i=0;i<nHits_.size();++i)
    {
      if (isOK[i]==0)
	continue;

      aMeasurement.nChannels++;
      aMeasurement.nHitsMean+=nHits_[i];
      aMeasurement.etSumMean+=etSum_[i];
      aMeasurement.etSumMeanErr+=nHits_[i]*pow(etSumRMS_[i],2);
      aMeasurement.etMeanMean+=etSum_[i]/(float)nHits_[i];
      aMeasurement.etMeanMeanErr+=pow(etSumRMS_[i],2)/(float)nHits_[i];
      aMeasurement.lcMean+=lc_[i]/float(nHits_[i]);
      aMeasurement.lcMeanErr+=pow(lcRMS_[i],2)/(float)nHits_[i];
      aMeasurement.alphaMean+=alpha_[i]/float(nHits_[i]);
      aMeasurement.alphaMeanErr+=pow(alphaRMS_[i],2)/(float)nHits_[i];
    }

  aMeasurement.etSumMean=aMeasurement.etSumMean/(float)aMeasurement.nChannels;
  aMeasurement.etSumMeanErr=sqrt(aMeasurement.etSumMeanErr)/(float)aMeasurement.nChannels;
  aMeasurement.etMeanMean=aMeasurement.etMeanMean/(float)aMeasurement.nChannels;
  aMeasurement.etMeanMeanErr=sqrt(aMeasurement.etMeanMeanErr)/((float)aMeasurement.nChannels);
  aMeasurement.lcMean=aMeasurement.lcMean/(float)aMeasurement.nChannels;
  aMeasurement.lcMeanErr= sqrt(aMeasurement.lcMeanErr)/((float)aMeasurement.nChannels);
  aMeasurement.alphaMean=aMeasurement.alphaMean/(float)aMeasurement.nChannels;
  aMeasurement.alphaMeanErr= sqrt(aMeasurement.alphaMeanErr)/((float)aMeasurement.nChannels);
  aMeasurement.nHitsMean=aMeasurement.nHitsMean/(float)aMeasurement.nChannels;

  return aMeasurement;
}   

