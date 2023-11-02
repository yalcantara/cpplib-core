#!/bin/bash
#CPD: Container Project Dir
CPD=/usr/src/cpplib-core
docker run -it \
    --rm \
    -v ${PWD}/Docker_Debug:${CPD}/Debug \
    -w ${CPD}/Debug \
     cpplib-core bash -c "
        echo \"Docker container created. About to run program main.\" && \
        ./core-main
     "
echo "====================================================================="
docker run -it \
    --rm \
    -v ${PWD}/Docker_Debug:${CPD}/Debug \
    -v ${PWD}/test-resources:${CPD}/test-resources \
    -w ${CPD}/Debug \
     cpplib-core bash -c "
        echo \"Docker container created. About to run program test.\" && \
        ./core-test
    "