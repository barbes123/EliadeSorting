#!/bin/bash

versioncpp=${1:-17}
echo "Compilation for version c++$versioncpp"

rootcint -f DelilaEventDict.cxx -c DelilaEvent.h DelilaEventLinkDef.h
rootcint -f HPGeTreeEventDict.cxx -c HPGeTreeEvent.h HPGeTreeEventLinkDef.h
rootcint -f LaBrTreeEventDict.cxx -c LaBrTreeEvent.h LaBrTreeEventLinkDef.h
rootcint -f ElissaTreeEventDict.cxx -c ElissaTreeEvent.h ElissaTreeEventLinkDef.h
rootcint -f HPGeSegTreeEventDict.cxx -c HPGeSegTreeEvent.h HPGeSegTreeEventLinkDef.h
g++ -shared -o libDelilaEvent.so`root-config --ldflags`  -std=c++$versioncpp -fPIC -O3 -I`root-config --incdir` DelilaEventDict.cxx
g++ -shared -o libHPGeTreeEvent.so`root-config --ldflags`  -std=c++$versioncpp -fPIC -O3 -I`root-config --incdir` HPGeTreeEventDict.cxx
g++ -shared -o libHPGeSegTreeEvent.so`root-config --ldflags`  -std=c++$versioncpp -fPIC -O3 -I`root-config --incdir` HPGeSegTreeEventDict.cxx
g++ -shared -o libLaBrTreeEvent.so`root-config --ldflags`  -std=c++$versioncpp -fPIC -O3 -I`root-config --incdir` LaBrTreeEventDict.cxx
g++ -shared -o libElissaTreeEvent.so`root-config --ldflags`  -std=c++$versioncpp -fPIC -O3 -I`root-config --incdir` ElissaTreeEventDict.cxx


