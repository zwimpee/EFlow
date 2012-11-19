#ifndef KFACTORSVSTIME_H
#define KFACTORSVSTIME_H

#include <vector>
#include <map>

class KFactorsVsTime {
 public:
  KFactorsVsTime(const char* file);

  float kFactor(const int& det, const int& ring, const int& run);
 private:
  typedef std::pair< int, int> runInterval;
  typedef std::map< runInterval, float > kFactorsTimeMap;
  typedef std::map< int, kFactorsTimeMap > kFactorsMap;
  kFactorsMap ebMap_;
  kFactorsMap eeMap_;

};
#endif
