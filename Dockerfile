FROM debian:jessie

RUN apt-get update && apt-get install -y \
  build-essential autoconf libtool \
  git \
  pkg-config \
  cmake \
  libboost-all-dev \
  libmysqlclient-dev \
  && apt-get clean

ENV GRPC_RELEASE_TAG v1.0.0

RUN git clone -b ${GRPC_RELEASE_TAG} https://github.com/grpc/grpc /var/local/dependencies/grpc

# install grpc
RUN cd /var/local/dependencies/grpc && \
    git submodule update --init && \
    make && \
    make install && \
    make clean

#install protoc
RUN cd /var/local/dependencies/grpc/third_party/protobuf && \
    make && \
    make install && \
    make clean

# add mysql-connector to dependencies
ADD lib/mysql-connector-c++-1.1.9 /var/local/dependencies/mysql-connector-cpp

#install mysql c++ connector
RUN cd /var/local/dependencies/mysql-connector-cpp && \
    cmake . && \
    make clean && \
    make && \
    make install

WORKDIR /build

ADD . .

#compile test example
RUN mkdir Release && cd Release && cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" .. && make

CMD ./Release/CustomerService