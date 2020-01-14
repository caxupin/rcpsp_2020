#!/bin/bash

mkdir -p build/Debug

CPLEX_DIR=/Applications/CPLEX_Studio128
CPLEX_VERSION=1280

cd build/Debug
cmake -DCMAKE_BUILD_TYPE=Debug -DCPLEX_DIR=$CPLEX_DIR -DCPLEX_VERSION=$CPLEX_VERSION ../..
make
cd ../..
