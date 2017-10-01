#!/usr/bin/env bash

docker build --rm -t customer_service:build .

docker create --name customer_service_build customer_service:build

docker cp customer_service_build:/usr/local/lib run

docker cp customer_service_build:/build/Release/CustomerService run

docker rm -f customer_service_build

docker build --rm -t customer_service:run run

docker run \
    -p 50031:50031 \
    -e LD_LIBRARY_PATH=/usr/local/lib \
    -e DB_SERVER=docker.for.mac.localhost \
    -e DB_USER=user \
    -e DB_PASSWORD=user \
    -e DB_NAME=insurance_customers \
    -e CUSTOMER_SERVICE_PORT=50031 \
    -i -t --security-opt=seccomp:unconfined \
    --rm customer_service:run