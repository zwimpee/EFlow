#include "readLumiInfo.h"
#include "TAxis.h"
#include <iostream>


readLumiInfo::readLumiInfo(const char* file, const char* JSONfile)
{
  goodLS=0;
  if (JSONfile)
    goodLS=new JSON(JSONfile);

  TFile* lumiFile=TFile::Open(file);
  if (!lumiFile)
    {
      std::cerr << "[readLumiInfo]::ERR::Could not open " << file << std::endl;
      return;
    }
  
  lumiTree=(TNtuple*) lumiFile->Get("lumiByLS");
  
  float run;
  float LS;
  float delLumi;
  float recLumi;
  float lsstarttimesec;
  float lsstarttimeusec;
  float nBX;
  float avgInstLumi;
  
  lumiTree->SetBranchAddress("run",&run);
  lumiTree->SetBranchAddress("LS",&LS);
  lumiTree->SetBranchAddress("delLumi",&delLumi);
  lumiTree->SetBranchAddress("recLumi",&recLumi);
  lumiTree->SetBranchAddress("nBX",&nBX);
  lumiTree->SetBranchAddress("avgInstLumi",&avgInstLumi);
  lumiTree->SetBranchAddress("lsstarttimesec",&lsstarttimesec);
  lumiTree->SetBranchAddress("lsstarttimeusec",&lsstarttimeusec);
  
  for (unsigned int i=0; i<lumiTree->GetEntries();++i)
    {
      lumiTree->GetEntry(i);

      if (goodLS && !goodLS->isGoodLS(run,LS))
	continue;

      lumiInfo a;
      a.deliveredInstLumi=delLumi/23.31; //in ROOT file only integrated lumi in 23.31s 
      a.recordedInstLumi=recLumi/23.31;  //in ROOT file only integrated lumi in 23.31s 
      a.nCollidingBunches=nBX;
      a.avgInstLumiPerBX=avgInstLumi;
      a.lsStartTime.tv_sec=lsstarttimesec;
      a.lsStartTime.tv_usec=lsstarttimeusec;
      //      std::cout << run << " " << LS << " " << delLumi << " " << nBX << " " << avgInstLumi << std::endl;
      runLS thisLS;
      thisLS.run=run;
      thisLS.LS=LS;
      if (lumiMap.find(thisLS) == lumiMap.end())
	lumiMap.insert(std::make_pair(thisLS,a));
      else
	{
	  std::cout << "[readLumiInfo]::ERROR repeated lumi " << thisLS.run << "," << thisLS.LS << std::endl;
	}
    }

  std::cout << "[readLumiInfo]::Read lumi informations for " << lumiMap.size() << " LS " << std::endl; 
  
}

readLumiInfo::lumiInfo readLumiInfo::getLumiInfo(int run,int LS)
{
  runLS thisLS; thisLS.run=run; thisLS.LS=LS;
  lumiTable::const_iterator lumiRecord=lumiMap.find(thisLS);
  if (lumiRecord != lumiMap.end() )
    return (*lumiRecord).second;
  else
    {
      std::cerr << "[readLumiInfo]::ERR::run " << run << " LS " << LS << " not found"  << std::endl;
      return lumiInfo();
    }
}

float readLumiInfo::getDeliveredInstLumi(int run,int LS)
{
  runLS thisLS; thisLS.run=run; thisLS.LS=LS;
  lumiTable::const_iterator lumiRecord=lumiMap.find(thisLS);
  if (lumiRecord != lumiMap.end() )
    return (*lumiRecord).second.deliveredInstLumi;
  else
    {
      std::cerr << "[readLumiInfo]::ERR::run " << run << " LS " << LS << " not found"  << std::endl;
      return -1;
    }
}

timeval readLumiInfo::getLuminosityStartTime(int run,int LS)
{
  runLS thisLS; thisLS.run=run; thisLS.LS=LS;
  lumiTable::const_iterator lumiRecord=lumiMap.find(thisLS);
  if (lumiRecord != lumiMap.end() )
    return (*lumiRecord).second.lsStartTime;
  else
    {
      std::cerr << "[readLumiInfo]::ERR::run " << run << " LS " << LS << " not found"  << std::endl;
      return timeval();
    }
}
int readLumiInfo::getNumberOfCollidingBunches(int run,int LS)
{
  runLS thisLS; thisLS.run=run; thisLS.LS=LS;
  lumiTable::const_iterator lumiRecord=lumiMap.find(thisLS);
  if (lumiRecord != lumiMap.end() )
    return (*lumiRecord).second.nCollidingBunches;
  else
    {
      std::cerr << "[readLumiInfo]::ERR::run " << run << " LS " << LS << " not found"  << std::endl;
      return -1;
    }
}

float readLumiInfo::getAvgInstLumiPerBX(int run,int LS)
{
  runLS thisLS; thisLS.run=run; thisLS.LS=LS;
  lumiTable::const_iterator lumiRecord=lumiMap.find(thisLS);
  if (lumiRecord != lumiMap.end() )
    return (*lumiRecord).second.avgInstLumiPerBX;
  else
    {
      std::cerr << "[readLumiInfo]::ERR::run " << run << " LS " << LS << " not found"  << std::endl;
      return -1;
    }
}

float readLumiInfo::getAvgInstLumiPerBX(int runStart,int LSStart,int runEnd,int LSEnd)
{
  runLS thisLS; thisLS.run=runStart; thisLS.LS=LSStart;
  runLS endLS; thisLS.run=runEnd; thisLS.LS=LSEnd;
  lumiTable::const_iterator lumiRecord=lumiMap.find(thisLS);
  
  if (lumiRecord != lumiMap.end() )
    {
      int nLS=0;
      float avgInstLumi=0.;
      while (!(lumiRecord->first > endLS))
	{
	  avgInstLumi+=(*lumiRecord).second.avgInstLumiPerBX;
	  nLS++;
	}
      std::cout << "nLS " << nLS << std::endl;
      ++lumiRecord;
      return avgInstLumi/(float)nLS;
    }
  else
    {
      std::cerr << "[readLumiInfo]::ERR::run " << runStart << " LS " << LSStart << " not found"  << std::endl;
      return -1;
    }


}
float readLumiInfo::getDeliveredLumi(int run)
{
  int nLS=0;
  float delLumi=0;
  for (lumiTable::const_iterator lumiRecord=lumiMap.begin();lumiRecord!=lumiMap.end();++lumiRecord)
    {
      if ((*lumiRecord).first.run == run)
	{
	  delLumi+=(*lumiRecord).second.deliveredInstLumi*23.31;
	  nLS++;
	}
    }
  std::cout << "[readLumiInfo]::Found " << nLS << " LS for run " << run << std::endl;

  if (nLS>0)
    {
      return delLumi;
    }
  else
    {
      std::cerr << "[readLumiInfo]::ERR::run " << run <<  " not found"  << std::endl;
      return -1;
    }
}

float readLumiInfo::getTotalDeliveredLumi(int run,int ls)
{
  int nLS=0;
  float delLumi=0;
  runLS thisLS(run,ls);
  for (lumiTable::const_iterator lumiRecord=lumiMap.begin();lumiRecord!=lumiMap.end();++lumiRecord)
    {
      if ((*lumiRecord).first < thisLS)
	{
	  delLumi+=(*lumiRecord).second.deliveredInstLumi*23.31;
	  nLS++;
	}
    }
  std::cout << "[readLumiInfo]::Found " << nLS << " LS before run " << run << std::endl;

  if (nLS>0)
    {
      return delLumi;
    }
  else
    {
      std::cerr << "[readLumiInfo]::ERR::no LS found befeore run " << run  << std::endl;
      return -1;
    }
}

float readLumiInfo::getTotalRecordedLumi(int run,int ls)
{
  int nLS=0;
  float delLumi=0;
  runLS thisLS(run,ls);
  for (lumiTable::const_iterator lumiRecord=lumiMap.begin();lumiRecord!=lumiMap.end();++lumiRecord)
    {
      if ((*lumiRecord).first < thisLS)
	{
	  delLumi+=(*lumiRecord).second.recordedInstLumi*23.31;
	  nLS++;
	}
    }
  std::cout << "[readLumiInfo]::Found " << nLS << " LS before run " << run << std::endl;

  if (nLS>0)
    {
      return delLumi;
    }
  else
    {
      std::cerr << "[readLumiInfo]::ERR::no LS found befeore run " << run  << std::endl;
      return -1;
    }
}

float readLumiInfo::getTotalDeliveredLumi()
{
  int nLS=0;
  float delLumi=0;
  for (lumiTable::const_iterator lumiRecord=lumiMap.begin();lumiRecord!=lumiMap.end();++lumiRecord)
    {
      delLumi+=(*lumiRecord).second.deliveredInstLumi*23.31;
      nLS++;
    }
  std::cout << "[readLumiInfo]::Summed " << nLS << " LS "<< std::endl;
  return delLumi;
}

float readLumiInfo::getTotalRecordedLumi()
{
  int nLS=0;
  float delLumi=0;
  for (lumiTable::const_iterator lumiRecord=lumiMap.begin();lumiRecord!=lumiMap.end();++lumiRecord)
    {
      delLumi+=(*lumiRecord).second.recordedInstLumi*23.31;
      nLS++;
    }
  std::cout << "[readLumiInfo]::Summed " << nLS << " LS " << std::endl;
  return delLumi;
}

float readLumiInfo::getRecordedLumi(int run)
{
  int nLS=0;
  float delLumi=0;
  for (lumiTable::const_iterator lumiRecord=lumiMap.begin();lumiRecord!=lumiMap.end();++lumiRecord)
    {
      if ((*lumiRecord).first.run == run)
	{
	  delLumi+=(*lumiRecord).second.recordedInstLumi*23.31;
	  nLS++;
	}
    }
  std::cout << "[readLumiInfo]::Found " << nLS << " LS for run " << run << std::endl;

  if (nLS>0)
    {
      return delLumi;
    }
  else
    {
      std::cerr << "[readLumiInfo]::ERR::run " << run << " not found"  << std::endl;
      return -1;
    }
}

TGraph* readLumiInfo::getHistoDeliveredInstLumi(int run)
{
  float LS[4000];
  float delLumi[4000];
  int nLS=0;

  for (lumiTable::const_iterator lumiRecord=lumiMap.begin();lumiRecord!=lumiMap.end();++lumiRecord)
    {
      if ((*lumiRecord).first.run == run)
	{
	  LS[nLS]=(*lumiRecord).first.LS;
	  delLumi[nLS]=(*lumiRecord).second.deliveredInstLumi;
	  nLS++;
	}
    }

  std::cout << "[readLumiInfo]::Found " << nLS << " LS for run " << run << std::endl;
  char histoName[100];
  sprintf(histoName,"DeliveredInstLumi_Run%d",run);
  TGraph* aHisto=new TGraph(nLS,LS,delLumi);
  aHisto->SetTitle(histoName);
  aHisto->GetXaxis()->SetTitle("LS");
  aHisto->GetYaxis()->SetTitle("Del. Inst. Lumi [#mub^{-1}s^{-1}]");
  return aHisto;
}

TGraph* readLumiInfo::getHistoDeliveredLumi(int run)
{
  float LS[4000];
  float delLumi[4000];
  int nLS=0;

  for (lumiTable::const_iterator lumiRecord=lumiMap.begin();lumiRecord!=lumiMap.end();++lumiRecord)
    {
      if ((*lumiRecord).first.run == run)
	{
	  LS[nLS]=(*lumiRecord).first.LS;
	  if (nLS>0)
	    delLumi[nLS]=(*lumiRecord).second.deliveredInstLumi*23.31 + delLumi[nLS-1];
	  else
	    delLumi[nLS]=(*lumiRecord).second.deliveredInstLumi*23.31;
	  nLS++;
	}
    }

  std::cout << "[readLumiInfo]::Found " << nLS << " LS for run " << run << std::endl;
  char histoName[100];
  sprintf(histoName,"DeliveredIntLumi_Run%d",run);
  TGraph* aHisto=new TGraph(nLS,LS,delLumi);
  aHisto->SetTitle(histoName);
  aHisto->GetXaxis()->SetTitle("LS");
  aHisto->GetYaxis()->SetTitle("Del. Int. Lumi [#mub^{-1}]");
  return aHisto;
}


TGraph* readLumiInfo::getHistoRecordedLumi(int run)
{
  float LS[4000];
  float delLumi[4000];
  int nLS=0;

  for (lumiTable::const_iterator lumiRecord=lumiMap.begin();lumiRecord!=lumiMap.end();++lumiRecord)
    {
      if ((*lumiRecord).first.run == run)
	{
	  LS[nLS]=(*lumiRecord).first.LS;
	  if (nLS>0)
	    delLumi[nLS]=(*lumiRecord).second.recordedInstLumi*23.31 + delLumi[nLS-1];
	  else
	    delLumi[nLS]=(*lumiRecord).second.recordedInstLumi*23.31;
	  nLS++;
	}
    }

  std::cout << "[readLumiInfo]::Found " << nLS << " LS for run " << run << std::endl;
  char histoName[100];
  sprintf(histoName,"RecordedIntLumi_Run%d",run);
  TGraph* aHisto=new TGraph(nLS,LS,delLumi);
  aHisto->SetTitle(histoName);
  aHisto->GetXaxis()->SetTitle("LS");
  aHisto->GetYaxis()->SetTitle("Rec. Int. Lumi [#mub^{-1}]");
  return aHisto;
}

TGraph* readLumiInfo::getHistoTotalDeliveredLumiVsTime(const time_t* startTime,const time_t* stopTime)
{
  float LSTime[200000];
  float delLumi[200000];
  int nLS=0;

  for (lumiTable::const_iterator lumiRecord=lumiMap.begin();lumiRecord!=lumiMap.end();++lumiRecord)
    {
      if ((*lumiRecord).second.lsStartTime.tv_sec < *stopTime && (*lumiRecord).second.lsStartTime.tv_sec > *startTime )
	{
	  LSTime[nLS]=(*lumiRecord).second.lsStartTime.tv_sec;
	  if (nLS>0)
	    delLumi[nLS]=(*lumiRecord).second.deliveredInstLumi*23.31 + delLumi[nLS-1];
	  else
	    delLumi[nLS]=(*lumiRecord).second.deliveredInstLumi*23.31;
	  nLS++;
	}
    }

  std::cout << "[readLumiInfo]::Found " << nLS << " LS between " << *startTime << " and " << *stopTime << std::endl;
  char histoName[100];
  sprintf(histoName,"DelIntLumi_StartTime%dStopTime%d",(int)*startTime,(int)*stopTime);
  TGraph* aHisto=new TGraph(nLS,LSTime,delLumi);
  aHisto->SetTitle(histoName);
  aHisto->GetXaxis()->SetTimeDisplay(1);
  //  aHisto->GetXaxis()->SetNdivisions(-503); 
  aHisto->GetXaxis()->SetTimeFormat("%d/%m"); 
  aHisto->GetXaxis()->SetTimeOffset(0,"gmt");
  aHisto->GetXaxis()->SetTitle("Time");
  aHisto->GetYaxis()->SetTitle("Del. Int. Lumi [#mub^{-1}]");
  return aHisto;  
}

TGraph* readLumiInfo::getHistoTotalRecordedLumiVsTime(const time_t* startTime,const time_t* stopTime)
{
  float LSTime[200000];
  float delLumi[200000];
  int nLS=0;

  for (lumiTable::const_iterator lumiRecord=lumiMap.begin();lumiRecord!=lumiMap.end();++lumiRecord)
    {
      if ((*lumiRecord).second.lsStartTime.tv_sec < *stopTime && (*lumiRecord).second.lsStartTime.tv_sec > *startTime )
	{
	  LSTime[nLS]=(*lumiRecord).second.lsStartTime.tv_sec;
	  if (nLS>0)
	    delLumi[nLS]=(*lumiRecord).second.recordedInstLumi*23.31 + delLumi[nLS-1];
	  else
	    delLumi[nLS]=(*lumiRecord).second.recordedInstLumi*23.31;
	  nLS++;
	}
    }

  std::cout << "[readLumiInfo]::Found " << nLS << " LS between " <<*startTime << " and " << *stopTime << std::endl;
  char histoName[100];
  sprintf(histoName,"RecIntLumi_StartTime%dStopTime%d",(int)*startTime,(int)*stopTime);
  TGraph* aHisto=new TGraph(nLS,LSTime,delLumi);
  aHisto->SetTitle(histoName);
  aHisto->GetXaxis()->SetTimeDisplay(1);
  //  aHisto->GetXaxis()->SetNdivisions(-503); 
  aHisto->GetXaxis()->SetTimeFormat("%d/%m"); 
  aHisto->GetXaxis()->SetTimeOffset(0,"gmt");
  aHisto->GetXaxis()->SetTitle("Time");
  aHisto->GetYaxis()->SetTitle("Rec. Int. Lumi [#mub^{-1}]");
  return aHisto;  
}

TGraph* readLumiInfo::getHistoAvgInstLumiPerBX(int run)
{
  float LS[4000];
  float delLumi[4000];
  int nLS=0;

  for (lumiTable::const_iterator lumiRecord=lumiMap.begin();lumiRecord!=lumiMap.end();++lumiRecord)
    {
      if ((*lumiRecord).first.run == run)
	{
	  LS[nLS]=(*lumiRecord).first.LS;
	  delLumi[nLS]=(*lumiRecord).second.avgInstLumiPerBX;
	  nLS++;
	}
    }

  std::cout << "[readLumiInfo]::Found " << nLS << " LS for run " << run << std::endl;
  char histoName[100];
  sprintf(histoName,"AvgInstLumiPerBX_Run%d",run);
  TGraph* aHisto=new TGraph(nLS,LS,delLumi);
  aHisto->SetTitle(histoName);
  aHisto->GetXaxis()->SetTitle("LS");
  aHisto->GetYaxis()->SetTitle("Avg. Inst. Lumi Per BX [#mub^{-1}s^{-1}]");
  return aHisto;
}
