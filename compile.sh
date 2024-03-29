#!/bin/bash
#CPD: Container Project Dir
CPD=/usr/src/cpplib-core
docker run -it \
    --rm \
    -v ${PWD}/include:${CPD}/include \
    -v ${PWD}/test-include:${CPD}/test-include \
    -v ${PWD}/CMAkeLists.txt:${CPD}/CMakeLists.txt \
    -v ${PWD}/Docker_Debug:${CPD}/Debug \
    -v ${PWD}/test-resources:${CPD}/test-resources \
    -v ${PWD}/core-main.cpp:${CPD}/core-main.cpp \
    -v ${PWD}/core-test.cpp:${CPD}/core-test.cpp \
    -w ${CPD}/Debug \
     cpplib-core bash -c "
        cmake -DCMAKE_BUILD_TYPE=Debug ../ && \
        make
     "