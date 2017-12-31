FROM ubuntu:artful
RUN apt-get update
RUN apt-get install -y apt-utils
RUN apt-get install -y git cmake python3-pip
RUN pip3 install --user cpp-coveralls