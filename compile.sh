#!/bin/bash
docker run -it \
    -v ${PWD}/include:/usr/src/cpplib-dpiw/include \
    -v ${PWD}/CMAkeLists.txt:/usr/src/cpplib-dpiw/CMakeLists.txt \
    -v ${PWD}/main.cpp:/usr/src/cpplib-dpiw/main.cpp \
     cpplib-core