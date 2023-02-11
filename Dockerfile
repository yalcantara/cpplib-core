FROM alpine:latest
RUN apk update
RUN apk add bash make cmake clang clang-dev alpine-sdk
WORKDIR /usr/src/cpplib-core
CMD ["/bin/bash"]