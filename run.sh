#!/bin/bash
#CPD: Container Project Dir
CPD=/usr/src/cpplib-core
docker run -it \
    --rm \
    -v ${PWD}/Docker_Debug:${CPD}/Debug \
    -w ${CPD}/Debug \
     cpplib-core bash -c "
        echo \"Docker container created. About to run program main.\" && \
        ./main
     "
echo "====================================================================="
docker run -it \
    --rm \
    -v ${PWD}/Docker_Debug:${CPD}/Debug \
    -w ${CPD}/Debug \
     cpplib-core bash -c "
        echo \"Docker container created. About to run program test.\" && \
        ./test
     "