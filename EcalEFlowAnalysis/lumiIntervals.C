#include "lumiIntervals.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

#include <iostream>

lumiIntervals::lumiIntervals(const char* lumiIntervalFile) 
{
  TFile *f=TFile::Open(lumiIntervalFile);
  if (!f->IsOpen())
      std::cout << "Cannot open " << lumiIntervalFile << std::endl;
   TTree* intervalsTree= (TTree*)f->Get("outTree_barl");

   int fr,lr,fl,ll,intime;

   TBranch *b_firstRun=intervalsTree->GetBranch("firstRun");
   TBranch *b_lastRun=intervalsTree->GetBranch("lastRun");
   TBranch *b_firstLumi=intervalsTree->GetBranch("firstLumi");
   TBranch *b_lastLumi=intervalsTree->GetBranch("lastLumi");
   TBranch *b_intTime=intervalsTree->GetBranch("unixtimeMean");

   intervalsTree->SetBranchAddress("firstRun", &fr, &b_firstRun);
   intervalsTree->SetBranchAddress("lastRun", &lr, &b_lastRun);
   intervalsTree->SetBranchAddress("firstLumi", &fl, &b_firstLumi);
   intervalsTree->SetBranchAddress("lastLumi", &ll, &b_lastLumi);
   intervalsTree->SetBranchAddress("unixtimeMean",&intime,&b_intTime);

   //   Long64_t nbytes_int = 0, nb_int = 0;
   int nentries_int = intervalsTree->GetEntries();
   for(int jentry=0;jentry<nentries_int;++jentry){
     intervalsTree->GetEntry(jentry);
     runLS start;
     start.run=fr;
     start.ls=fl;
     runLS end;
     end.run=lr;
     end.ls=ll;
     intervals.push_back(std::make_pair<runLS,runLS>(start,end));
     intervals_time.push_back(intime);
   }
 
   std::cout << intervals.size() << " INTERVALS HAS BEEN LOADED " << std::endl;
   for (unsigned int i=0;i<intervals.size();++i)
     std::cout << "[PHI SIMM INTERVAL " << i << "]: [" << intervals[i].first.run << ":" << intervals[i].first.ls  << "],[" << intervals[i].second.run << ":" << intervals[i].second.ls << "]" << std::endl;
}

int lumiIntervals::numberOfIntervals() const
{
  return intervals.size();
}

int lumiIntervals::intervalTime(int interval) const
{
  if (interval<(int)intervals_time.size())
    return intervals_time[interval];
  else
    {
      std::cout << "Interval not in the range" << std::endl;
      return -1;
    }
}

int lumiIntervals::intervalNumber(int run,int ls) const
{
  runLS thisLS;
  thisLS.run=run;
  thisLS.ls=ls;
  for (unsigned int i=0;i<intervals.size();++i)
    {
      if ( intervals[i].first <= thisLS 
	   && 
	   thisLS <= intervals[i].second )
	{
	   return i;
	   break;
	 }
     }
   std::cout << "This should not happen: run,LS not found within any interval" << std::endl;
   return -1;
}

lumiIntervals::runLS lumiIntervals::lsStart(int interval_index) const
{
  if (interval_index<(int)intervals.size())
    return intervals[interval_index].first;
  else
    {
      std::cout << "Interval not in the range" << std::endl;
      runLS a;
      a.run=-1;
      a.ls=-1;
      return a;
    }
}

lumiIntervals::runLS lumiIntervals::lsEnd(int interval_index) const
{
  if (interval_index<(int)intervals.size())
    return intervals[interval_index].second;
  else
    {
      std::cout << "Interval not in the range" << std::endl;
      runLS a;
      a.run=-1;
      a.ls=-1;
      return a;
    }
}
