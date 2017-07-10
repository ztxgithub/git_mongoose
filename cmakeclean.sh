#!/bin/bash
rm -rf CMakeCache.txt CMakeFiles/
echo "cmake had clean"

echo "begin cmake ./"
cmake ./
