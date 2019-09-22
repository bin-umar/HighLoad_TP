FROM ubuntu:18.04

MAINTAINER Jahongir Tulforov

RUN apt-get -y update

RUN apt-get -y install g++
RUN apt-get -y update
RUN apt-get -y install libuv1.dev
RUN apt-get -y install make
RUN apt-get -y install cmake

WORKDIR /home/webserver

COPY . .
RUN cmake .
RUN make

EXPOSE 80
CMD ./HighLoad_TP