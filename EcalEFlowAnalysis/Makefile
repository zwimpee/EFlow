all : 
	mkdir -p lib
#	g++ -fPIC -c `${ROOTSYS}/bin/root-config --cflags` -o lib/readLumiInfo.o readLumiInfo.cc
	g++ -fPIC -c `${ROOTSYS}/bin/root-config --cflags` -I./  -o lib/JSON.o src/JSON.cc	
	g++ -fPIC -c `${ROOTSYS}/bin/root-config --cflags` -I./  -o lib/RegionStability.o src/RegionStability.cc	
	g++ -fPIC -c `${ROOTSYS}/bin/root-config --cflags` -I./  -o lib/KFactorsVsTime.o src/KFactorsVsTime.cc
	g++ -fPIC -c `${ROOTSYS}/bin/root-config --cflags` -I./  -o lib/HarnessMap.o src/HarnessMap.cc
	g++ -fPIC -c `${ROOTSYS}/bin/root-config --cflags` -I./  -o lib/readLumiInfo.o src/readLumiInfo.cc
	g++ -fPIC -c `${ROOTSYS}/bin/root-config --cflags` -I./  -o lib/lumiIntervals.o src/lumiIntervals.cc
	g++ -fPIC -c `${ROOTSYS}/bin/root-config --cflags` -I./ -o lib/alcaPhiSym_tree_v3.o src/alcaPhiSym_tree_v3.cc
#	g++ -fPIC -c `${ROOTSYS}/bin/root-config --cflags` -I./ -o lib/createHistoryPlots.o createHistoryPlots.C
#	g++ -fPIC -c `${ROOTSYS}/bin/root-config --cflags` -I./ -o lib/makeMap.o makeMap.C
	rootcint -f src/dict.cc -c `${ROOTSYS}/bin/root-config --cflags` -p  interface/JSON.h interface/KFactorsVsTime.h interface/HarnessMap.h  interface/RegionStability.h interface/alcaPhiSym_tree_v3.h interface/lumiIntervals.h interface/LinkDef.h  
	g++ -fPIC -c `${ROOTSYS}/bin/root-config --cflags` -I./ -o lib/dict.o src/dict.cc
	g++ -shared -o lib/libUtils.so lib/*.o

clean: 
	rm -rf dict.* *.o *.so lib/*.o lib/*.so 
