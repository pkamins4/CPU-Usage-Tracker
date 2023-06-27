#!/bin/bash
if [ -d  ./out ] 
then
    rm -rf ./out/
fi

echo "Creating build directory: ./out/"
mkdir ./out/

cmake -S . -B ./out
