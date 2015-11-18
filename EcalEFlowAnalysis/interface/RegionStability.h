#ifndef REGIONSTABILITY_H
#define REGIONSTABILITY_H

#include <vector>
#include <map>

//Class to handle truncated mean outliers from a region stability plot
class RegionStability {
 public:
  RegionStability() {} ; 

  void addMeasurement(const int& nHits, const float& etSum, const float& etSumRMS, const float& lc, const float& lcRMS)
    {
      if (nHits>0)
	{
	  nHits_.push_back(nHits);
	  etSum_.push_back(etSum);
	  etSumRMS_.push_back(etSumRMS);
	  lc_.push_back(lc);
	  lcRMS_.push_back(lcRMS);
	  /* alpha_.push_back(alpha); */
	  /* alphaRMS_.push_back(alphaRMS); */
	}
    };

  struct measurement
  {
    int nChannels;
    float nHitsMean;
    float nHitsMeanErr;
    float etMeanMean;
    float etMeanMeanErr;
    float etSumMean;
    float etSumMeanErr;
    float lcMean;
    float lcMeanErr;
    /* float alphaMean; */
    /* float alphaMeanErr; */

    void reset()
    {
      nChannels=0;
      nHitsMean=0;
      nHitsMeanErr=0;
      etMeanMean=0;
      etMeanMeanErr=0;
      etSumMean=0;
      etSumMeanErr=0;
      lcMean=0;
      lcMeanErr=0;
      /* alphaMean=0; */
      /* alphaMeanErr=0; */
    };
  };

  measurement getTruncatedMeanMeasurement(float nRMS, int NMAXITER);

 private:
  std::vector<int> nHits_;
  std::vector<float> etSum_; 
  std::vector<float> etSumRMS_; 
  std::vector<float> lc_; 
  std::vector<float> lcRMS_; 
  /* std::vector<float> alpha_;  */
  /* std::vector<float> alphaRMS_;  */
};
#endif
