FROM ubuntu:23.04
RUN apt-get update
RUN apt-get install -y cmake clang gdb libcpprest-dev libboost-system-dev
WORKDIR /usr/src/cpplib-core
CMD ["/bin/bash"]