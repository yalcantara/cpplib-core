FROM ubuntu:23.04
RUN apt-get update
RUN apt-get --assume-yes install cmake clang libcpprest-dev libboost-system-dev
WORKDIR /usr/src/cpplib-core
COPY docker/build-debug.sh Debug/build-debug.sh
RUN chmod +x Debug/build-debug.sh
CMD ["/bin/bash"]