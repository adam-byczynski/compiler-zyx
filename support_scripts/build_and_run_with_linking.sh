#!/bin/bash

make all TARGET=zyx && ./build/zyx -s $1 -o $1 -v -i -l -d
