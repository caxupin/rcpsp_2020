#!/bin/bash

mkdir -p build/Release

CPLEX_DIR=/opt/ibm/ILOG/CPLEX_Studio1210
CPLEX_VERSION=1210

cd build/Release
cmake -DCMAKE_BUILD_TYPE=Release -DCPLEX_DIR=$CPLEX_DIR -DCPLEX_VERSION=$CPLEX_VERSION ../..
make
cd ../..
