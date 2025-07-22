#!/bin/bash

gcc ./main/client.c -o client && gcc ./main/server.c -o server

mkdir -p build

rm -f build/client build/server

mv client build/
mv server build/