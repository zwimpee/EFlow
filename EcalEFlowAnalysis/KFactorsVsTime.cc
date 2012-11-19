#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include "KFactorsVsTime.h"
#include <cmath>
using namespace std;

KFactorsVsTime::KFactorsVsTime(const char* file)
{
  std::cout << "[KFactorsVsTime]::Opening file " << file << std::endl;
  TFile* kFactors = TFile::Open(file);
  TTree* kFactorsTree= (TTree*) kFactors->Get("kfactors_vsTime");
  std::cout << "[KFactorsVsTime]::Got TTree " << kFactorsTree << std::endl;

  /*
******************************************************************************
  *Tree    :kfactors_vsTime: kfactors_vsTime                                   *
  *Entries :     4880 : Total =          100801 bytes  File  Size =      18582 *
  *        :          : Tree compression factor =   5.48                       *
******************************************************************************
    *Br    0 :start_run : start_run/I                                            *
    *Entries :     4880 : Total  Size=      20097 bytes  File Size  =        429 *
    *Baskets :        1 : Basket Size=      32000 bytes  Compression=  45.70     *
*............................................................................*
    *Br    1 :end_run   : end_run/I                                              *
    *Entries :     4880 : Total  Size=      20087 bytes  File Size  =        431 *
    *Baskets :        1 : Basket Size=      32000 bytes  Compression=  45.49     *
*............................................................................*
    *Br    2 :ieta      : ieta/I                                                 *
    *Entries :     4880 : Total  Size=      20072 bytes  File Size  =        528 *
    *Baskets :        1 : Basket Size=      32000 bytes  Compression=  37.12     *
*............................................................................*
    *Br    3 :det       : det/I                                                  *
    *Entries :     4880 : Total  Size=      20067 bytes  File Size  =        212 *
    *Baskets :        1 : Basket Size=      32000 bytes  Compression=  92.46     *
*............................................................................*
    *Br    4 :kfactor   : kfactor/F                                              *
    *Entries :     4880 : Total  Size=      20087 bytes  File Size  =      16274 *
    *Baskets :        1 : Basket Size=      32000 bytes  Compression=   1.20     *
*............................................................................*
*/

  for (int i=0;i<85;++i)
    {
      kFactorsTimeMap myMap;
      ebMap_[i]=myMap;
    }

  for (int i=0;i<39;++i)
    {
      kFactorsTimeMap myMap;
      eeMap_[i]=myMap;
    }

  int iring;
  int start_run;
  int end_run;
  int det;
  float kf;
  TBranch *b_ring=kFactorsTree->GetBranch("ieta");
  TBranch *b_startrun=kFactorsTree->GetBranch("start_run");
  TBranch *b_endrun=kFactorsTree->GetBranch("end_run");
  TBranch *b_det=kFactorsTree->GetBranch("det");
  TBranch *b_kf=kFactorsTree->GetBranch("kfactor");
  kFactorsTree->SetBranchAddress("ieta", &iring, &b_ring);
  kFactorsTree->SetBranchAddress("start_run", &start_run, &b_startrun);
  kFactorsTree->SetBranchAddress("end_run", &end_run, &b_endrun);
  kFactorsTree->SetBranchAddress("det", &det, &b_det);
  kFactorsTree->SetBranchAddress("kfactor", &kf, &b_kf);

  int nentries = kFactorsTree->GetEntries();
  int nentries_eb=0;
  int nentries_ee=0;
  std::cout << "[KFactorsVsTime]::Looping over " << nentries << " entries " << std::endl;
  for(int jentry=0;jentry<nentries;++jentry)
    {
      kFactorsTree->GetEntry(jentry);
      //      std::cout << det << "," << iring  << "," <<  start_run << "," << end_run << "," << kf << std::endl;
      if (det==0)
	{
	  ebMap_[iring-1].insert(std::make_pair<runInterval,float>( std::make_pair<int,int>(start_run,end_run), kf ) );
	  nentries_eb++;
	}
      else if (det==1)
	{
	  eeMap_[iring-1].insert(std::make_pair<runInterval,float>( std::make_pair<int,int>(start_run,end_run), kf ) );
	  nentries_ee++;
	}
      else
	std::cout << "Don't know what to do..." << std::endl;
    }

  std::cout << "[KFactorsVsTime]::Found " << nentries_eb <<  " entries in EB " << nentries_ee <<  " entries in EE " << std::endl;
}


float KFactorsVsTime::kFactor(const int& det, const int& ring, const int& run)
{
  kFactorsTimeMap* map=0;
  if (det==0)
    map=&(ebMap_[ring]);
  else if (det==1)
    map=&(eeMap_[ring]);
  else
    return -1;

  if (!map)
    return -1;

  kFactorsTimeMap::const_iterator it=map->begin();
  int minDeltaRun=999999;
  kFactorsTimeMap::const_iterator closest;
  for (it;it!=map->end();++it)
    {
      int deltaRun=min( TMath::Abs( int(run - it->first.first) ), TMath::Abs( int(run- it->first.second) ) );
      if (deltaRun<=minDeltaRun)
	{
	  minDeltaRun=deltaRun;
	  closest=it;
	}
      if (run>=it->first.first && run<=it->first.second)
	return it->second;
    }
  return closest->second;
}
