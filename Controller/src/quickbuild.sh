#!/bin/bash

cd ../build
qmake ../src
make
make clean
./Controller.app/Contents/MacOS/Controller 
cd ../src 
