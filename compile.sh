#!/bin/bash
docker run -it \
    -v ${PWD}/include:/usr/src/cpplib-dpiw/include \
    -v ${PWD}/CMAkeLists.txt:/usr/src/cpplib-dpiw/CMAkeLists.txt \
     cpplib-core