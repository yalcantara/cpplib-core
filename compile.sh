#!/bin/bash
docker run -it \
    -v ${PWD}/include:/usr/src/cpplib-core/include \
    -v ${PWD}/CMAkeLists.txt:/usr/src/cpplib-core/CMakeLists.txt \
    -v ${PWD}/main.cpp:/usr/src/cpplib-core/main.cpp \
    -w /usr/src/cpplib-core/Debug \
     cpplib-core /usr/src/cpplib-core/Debug/build-debug.sh