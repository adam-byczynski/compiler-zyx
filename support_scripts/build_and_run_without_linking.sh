#!/bin/bash

make all TARGET=zyx && ./build/zyx -s $1 -o $1 -d
