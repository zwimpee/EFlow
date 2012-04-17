all : 
	mkdir -p lib
#	g++ -fPIC -c `${ROOTSYS}/bin/root-config --cflags` -o lib/readLumiInfo.o readLumiInfo.cc
	g++ -fPIC -c `${ROOTSYS}/bin/root-config --cflags` -I./  -o lib/JSON.o JSON.cc	
	g++ -fPIC -c `${ROOTSYS}/bin/root-config --cflags` -I./  -o lib/lumiIntervals.o lumiIntervals.C
	g++ -fPIC -c `${ROOTSYS}/bin/root-config --cflags` -I./ -o lib/alcaPhiSym_tree_v3.o alcaPhiSym_tree_v3.C
	g++ -fPIC -c `${ROOTSYS}/bin/root-config --cflags` -I./ -o lib/createHistoryPlots.o createHistoryPlots.C
	g++ -fPIC -c `${ROOTSYS}/bin/root-config --cflags` -I./ -o lib/makeMap.o makeMap.C
	rootcint -f dict.cc -c `${ROOTSYS}/bin/root-config --cflags` -p  JSON.h alcaPhiSym_tree_v3.h createHistoryPlots.h lumiIntervals.h makeMap.h LinkDef.h  
	g++ -fPIC -c `${ROOTSYS}/bin/root-config --cflags` -I./ -o lib/dict.o dict.cc
	g++ -shared -o lib/libUtils.so lib/JSON.o lib/dict.o lib/makeMap.o lib/createHistoryPlots.o lib/alcaPhiSym_tree_v3.o lib/lumiIntervals.o

clean: 
	rm -rf dict.* *.o *.so lib/*.o lib/*.so 
