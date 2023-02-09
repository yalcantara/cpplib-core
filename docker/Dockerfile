FROM ubuntu:23.04
RUN apt-get update
RUN apt-get --assume-yes install cmake clang libcpprest-dev libboost-system-dev wget
WORKDIR /usr/src/oracle/odpi/include
RUN wget https://raw.githubusercontent.com/oracle/odpi/main/include/dpi.h
WORKDIR /usr/src/cpplib-dpiw
COPY build-debug.sh Debug/build-debug.sh
RUN chmod +x Debug/build-debug.sh
CMD ["/bin/bash"]