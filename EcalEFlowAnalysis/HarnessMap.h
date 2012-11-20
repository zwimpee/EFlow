#ifndef HARNESSMAP_H
#define HARNESSMAP_H

#include <vector>
#include <map>

class HarnessMap 
{
 public:
  HarnessMap(const char* file);

  struct channel
  {
    channel(const int& i1,const int& i2,const int& i3)
    {
      ieta=i1;
      iphi=i2;
      sign=i3;
    }
    int ieta;
    int iphi;
    int sign;
  };
  
  int harnessHashed(const int& det, const int& ieta, const int& iphi, const int& sign);
  std::vector<channel> channelsInHarness(const int& det, const int& harness_hashed);

 private:
  int harnessHashed_eb[2][85][360];
  int harnessHashed_ee[2][100][100];
  std::vector<channel>* channelsInHarness_eb[324];  
  std::vector<channel>* channelsInHarness_ee[38];  
};
#endif
