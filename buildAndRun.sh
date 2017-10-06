#!/usr/bin/env bash

# Build a image to build the software. Compile the project in this step.
docker build --rm -t customer_service:build .

# Create a container from the build image
docker create --name customer_service_build customer_service:build

# Copy the libraries from container to host.
docker cp customer_service_build:/usr/local/lib run

# Copy executable from container to host
docker cp customer_service_build:/build/Release/CustomerService run

# Delete the build container
docker rm -f customer_service_build

# Build the run image
docker build --rm -t customer_service:run run

# Remove intermediate containers which have appeared during the build process
docker rmi $(docker images -f "dangling=true" -q)

# Run the executable in the run container
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