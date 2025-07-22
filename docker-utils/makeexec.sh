#!/bin/bash

gcc ./main/server.c -o server

mkdir -p build

rm -f build/server

mv server build/