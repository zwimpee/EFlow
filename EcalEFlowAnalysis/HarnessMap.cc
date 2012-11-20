#include <iostream>
#include <cstdlib>


#include <TFile.h>
#include <TTree.h>
#include "HarnessMap.h"

using namespace std;

HarnessMap::HarnessMap(const char* file)
{
  std::cout << "[HarnessMap]::Opening file " << file << std::endl;
  TFile* harness = TFile::Open(file);
  TTree* harnessMap= (TTree*) harness->Get("harnessMap");
  /*
******************************************************************************
*Tree    :harnessMap: harnessMap                                             *
*Entries :    75848 : Total =         2133120 bytes  File  Size =      61789 *
*        :          : Tree compression factor =  35.16                       *
******************************************************************************
*Br    0 :ieta      : ieta/I                                                 *
*Entries :    75848 : Total  Size=     304652 bytes  File Size  =       3633 *
*Baskets :       10 : Basket Size=      32000 bytes  Compression=  83.72     *
*............................................................................*
*Br    1 :iphi      : iphi/I                                                 *
*Entries :    75848 : Total  Size=     304652 bytes  File Size  =      22731 *
*Baskets :       10 : Basket Size=      32000 bytes  Compression=  13.38     *
*............................................................................*
*Br    2 :det       : det/I                                                  *
*Entries :    75848 : Total  Size=     304638 bytes  File Size  =       2456 *
*Baskets :       10 : Basket Size=      32000 bytes  Compression= 123.84     *
*............................................................................*
*Br    3 :sign      : sign/I                                                 *
*Entries :    75848 : Total  Size=     304652 bytes  File Size  =       2589 *
*Baskets :       10 : Basket Size=      32000 bytes  Compression= 117.48     *
*............................................................................*
*Br    4 :fedid     : fedid/I                                                *
*Entries :    75848 : Total  Size=     304666 bytes  File Size  =       6654 *
*Baskets :       10 : Basket Size=      32000 bytes  Compression=  45.71     *
*............................................................................*
*Br    5 :harness   : harness/I                                              *
*Entries :    75848 : Total  Size=     304694 bytes  File Size  =      12523 *
*Baskets :       10 : Basket Size=      32000 bytes  Compression=  24.29     *
*............................................................................*
*Br    6 :harness_hashed : harness_hashed/I                                  *
*Entries :    75848 : Total  Size=     304792 bytes  File Size  =       9967 *
*Baskets :       10 : Basket Size=      32000 bytes  Compression=  30.53     *
*............................................................................*
  */
  for (int sign=0;sign<2;++sign)
    for (int ieta=0;ieta<85;++ieta)
      for (int iphi=0;iphi<360;++iphi)  
	harnessHashed_eb[sign][ieta][iphi]=-1;

  for (int sign=0;sign<2;++sign)
    for (int ix=0;ix<100;++ix)
      for (int iy=0;iy<100;++iy)  
	harnessHashed_eb[sign][ix][iy]=-1;

  for (int ih=0;ih<324;++ih)
    channelsInHarness_eb[ih]=new std::vector<channel>;

  for (int ih=0;ih<38;++ih)
    channelsInHarness_ee[ih]=new std::vector<channel>;
  
  int ieta;
  int iphi;
  int sign;
  int det;
  int harness_hashed;
  TBranch *b_ieta=harnessMap->GetBranch("ieta");
  TBranch *b_iphi=harnessMap->GetBranch("iphi");
  TBranch *b_det=harnessMap->GetBranch("det");
  TBranch *b_sign=harnessMap->GetBranch("sign");
  TBranch *b_harnessHashed=harnessMap->GetBranch("harness_hashed");

  harnessMap->SetBranchAddress("ieta", &ieta, &b_ieta);
  harnessMap->SetBranchAddress("iphi", &iphi, &b_iphi);
  harnessMap->SetBranchAddress("det", &det, &b_det);
  harnessMap->SetBranchAddress("sign", &sign, &b_sign);
  harnessMap->SetBranchAddress("harness_hashed", &harness_hashed, &b_harnessHashed);

  int nentries = harnessMap->GetEntries();
  int nentries_eb=0;
  int nentries_ee=0;
  std::cout << "[HarnessMap]::Looping over " << nentries << " entries " << std::endl;
  for(int jentry=0;jentry<nentries;++jentry)
    {
      harnessMap->GetEntry(jentry);
      //      std::cout << det << "," << iring  << "," <<  start_run << "," << end_run << "," << kf << std::endl;
      if (det==0)
	{
	  harnessHashed_eb[(sign>0)][abs(ieta-1)][iphi-1]=harness_hashed;
	  channelsInHarness_eb[harness_hashed]->push_back(channel(ieta,iphi,sign));
	  nentries_eb++;
	}
      else if (det==1)
	{
	  harnessHashed_ee[sign>0][ieta-1][iphi-1]=harness_hashed;
	  channelsInHarness_ee[harness_hashed]->push_back(channel(ieta,iphi,sign));
	  nentries_ee++;
	}
      else
	std::cout << "Don't know what to do..." << std::endl;
    }

  std::cout << "[HarnessMap]::Found " << nentries_eb <<  " entries in EB " << nentries_ee <<  " entries in EE " << std::endl;

}


int HarnessMap::harnessHashed(const int& det, const int& ieta, const int& iphi, const int& sign)
{
  int sig=(sign>0);
  if (det==0)
    return harnessHashed_eb[sig][ieta-1][iphi-1];
  else if (det==1)
    return harnessHashed_ee[sig][ieta-1][iphi-1];
  return -1;
}

std::vector<HarnessMap::channel> HarnessMap::channelsInHarness(const int& det, const int& harness_hashed)
{
  if (det==0)
    return *channelsInHarness_eb[harness_hashed];
  else if (det==1)
    return *channelsInHarness_ee[harness_hashed];
}
