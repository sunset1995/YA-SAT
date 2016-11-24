#!/bin/sh

cd src
make clean && make
mv yasat ../
make clean
