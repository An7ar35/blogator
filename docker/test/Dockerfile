# syntax=docker/dockerfile:1
FROM alpine:latest
RUN apk add gcc g++ musl-dev make cmake git gtest gtest-dev gmock python3
#RUN git clone https://github.com/An7ar35/blogator.git
WORKDIR /blogator
COPY ../../. .
RUN mkdir cmake-build-docker \
 && cd cmake-build-docker \
 && cmake ../ \
 && cd .. \
 && git submodule update --init --recursive \
 && cmake --build cmake-build-docker --target blogator_tests -- -j 6
ENTRYPOINT [ "./build/bin/blogator_tests" ]