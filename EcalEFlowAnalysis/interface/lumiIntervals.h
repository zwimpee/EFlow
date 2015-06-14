#ifndef LUMIINTERVALS_H
#define LUMIINTERVALS_H

#include <vector>

class lumiIntervals {
 public:
   struct runLS
   {
     int run;
     int ls;
     inline bool operator<(const runLS& b) const
     {
       if (run < b.run)
	 return true;
       else 
	 {
	   if (run == b.run)
	     {
	       if (ls < b.ls)
		 return true;
	     }
	   return false;
	 }
     }

     inline bool operator==(const runLS& b) const
     {
       if (run == b.run && ls == b.ls)
	 return true;
       
       return false;
     }

     inline bool operator<=(const runLS& b) const
     {
       if ((*this)<b)
	 return true;
       if ((*this)==b)
	 return true;
       return false;
     }
   };

   lumiIntervals(const char* file);

   int numberOfIntervals() const;
   int intervalNumber(int run, int ls) const;
   int intervalTime(int interval_index) const;
   runLS lsStart(int interval_index) const;
   runLS lsEnd(int interval_index) const;

 private:
   typedef std::pair<runLS,runLS> interval_t;
   std::vector<interval_t> intervals;
   std::vector<int> intervals_time;
};
#endif
