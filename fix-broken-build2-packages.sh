#!/bin/env sh

problem_packages="any process beast"

for package in $problem_packages ; do
  bpkg build libboost-$package --yes
  bpkg install libboost-$package
done

ln -s /usr/local/include/boost /usr/include/boost
