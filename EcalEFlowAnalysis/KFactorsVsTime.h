#ifndef KFACTORSVSTIME_H
#define KFACTORSVSTIME_H

#include <vector>
#include <map>

class KFactorsVsTime {
 public:
  KFactorsVsTime(const char* file,bool kFactorsPerXtalInEB);

  float kFactor(const int& det, const int& ring, const int& run);
  float kFactor(const int& det, const int& sign, const int& ieta, const int& iphi, const int& run);
 private:
  typedef std::pair< int, int> runInterval;
  typedef std::map< runInterval, float > kFactorsTimeMap;

  kFactorsTimeMap ebMap_[85];
  kFactorsTimeMap ebXtalMap_[2][85][360];
  kFactorsTimeMap eeMap_[39];

};
#endif
