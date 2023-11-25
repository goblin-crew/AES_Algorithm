#!/bin/bash

PARENTDIR=$(basename $PWD);

[[ "${PARENTDIR}" == "build" ]] || {
    cd ./build
} 

cmake ..
cmake --build .