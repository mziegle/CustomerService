FROM debian:jessie

RUN apt-get update && apt-get install -y mysql-client

# copy shared libraries to run enviroment
COPY lib /usr/local/lib

WORKDIR /run
COPY CustomerService CustomerService
RUN chmod +x CustomerService

CMD ./CustomerService