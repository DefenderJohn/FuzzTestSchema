#!/bin/bash

cd build
rm -rf *
cmake ..
make -j
./FuzzTestSchemaExample