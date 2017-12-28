#include <iostream>
#include <grpc++/grpc++.h>
#include "customer_service.grpc.pb.h"
#include "CustomerDataSource.cpp"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::StatusCode;
using customer_service::Customer;
using customer_service::Id;
using customer_service::FirstName;
using customer_service::LastName;
using customer_service::Zip;
using customer_service::Gender;
using customer_service::Telephone;
using customer_service::Email;
using customer_service::Country;
using customer_service::City;
using customer_service::State;
using customer_service::Address;
using customer_service::CustomerService;

class CustomerServiceServer final : public CustomerService::Service {

    Status GetCustomerById(ServerContext* context, const Id* request, Customer* reply) override {
        
        Status status;
        CustomerDataSource customerDataSource;
        status = customerDataSource.ConnectToDataSource();
        Customer customer;
        customer.set_id(request->id());
        if(status.error_code() == StatusCode::OK){
            status = customerDataSource.GetCustomerById(customer);
            if(status.error_code() == StatusCode::OK) {
                *reply = customer;
                customerDataSource.DisconnectDataSource();
            }
        }
        return status;
    }

    Status GetCustomersByFirstName(ServerContext* context, const FirstName* request,
                             Customers* reply) override {
        CustomerDataSource databaseLoader;
        *reply = databaseLoader.GetCustomers("first_name",request->first_name());
        return Status::OK;
    }

    Status GetCustomersByLastName(ServerContext* context, const LastName* request,
                                  Customers* reply) override {
        Status status;
        CustomerDataSource customerDataSource;
        status = customerDataSource.ConnectToDataSource();
        if(status.error_code() == StatusCode::OK){
            *reply = customerDataSource.GetCustomers("last_name",request->first_name());
            status = Status::OK;
        }
        return status;
    }

    Status GetCustomersByGender(ServerContext* context, const Gender* request,
                                 Customers* reply) override {
        Status status;
        CustomerDataSource customerDataSource;
        status = customerDataSource.ConnectToDataSource();
        if(status.error_code() == StatusCode::OK){
            *reply = customerDataSource.GetCustomers("gender",request->gender());
            status = Status::OK;
        }
        return status;
    }

    Status GetCustomersByAddress(ServerContext* context, const Address* request,
                               Customers* reply) override {
        Status status;
        CustomerDataSource customerDataSource;
        status = customerDataSource.ConnectToDataSource();
        if(status.error_code() == StatusCode::OK){
            *reply = customerDataSource.GetCustomers("address",request->address());
            status = Status::OK;
        }
        return status;
    }

    Status GetCustomersByZip(ServerContext* context, const Zip* request,
                        Customers* reply) override {
        Status status;
        CustomerDataSource customerDataSource;
        status = customerDataSource.ConnectToDataSource();
        if(status.error_code() == StatusCode::OK){
            *reply = customerDataSource.GetCustomers("zip",request->zip());
            status = Status::OK;
        }
        return status;
    }

    Status GetCustomersByCity(ServerContext* context, const City* request,
                                Customers* reply) override {
        Status status;
        CustomerDataSource customerDataSource;
        status = customerDataSource.ConnectToDataSource();
        if(status.error_code() == StatusCode::OK){
            *reply = customerDataSource.GetCustomers("city",request->city());
            status = Status::OK;
        }
        return status;
    }

    Status GetCustomersByState(ServerContext* context, const State* request,
                             Customers* reply) override {
        Status status;
        CustomerDataSource customerDataSource;
        status = customerDataSource.ConnectToDataSource();
        if(status.error_code() == StatusCode::OK){
            *reply = customerDataSource.GetCustomers("state",request->state());
            status = Status::OK;
        }
        return status;
    }

    Status GetCustomersByCountry(ServerContext* context, const Country* request,
                              Customers* reply) override {
        Status status;
        CustomerDataSource customerDataSource;
        status = customerDataSource.ConnectToDataSource();
        if(status.error_code() == StatusCode::OK){
            *reply = customerDataSource.GetCustomers("country",request->country());
            status = Status::OK;
        }
        return status;
    }

    Status GetCustomersByTelephone(ServerContext* context, const Telephone* request,
                                Customers* reply) override {
        Status status;
        CustomerDataSource customerDataSource;
        status = customerDataSource.ConnectToDataSource();
        if(status.error_code() == StatusCode::OK){
            *reply = customerDataSource.GetCustomers("telephone",request->telephone());
            status = Status::OK;
        }
        return status;
    }

    Status GetCustomersByEmail(ServerContext* context, const Email* request,
                                  Customers* reply) override {
        Status status;
        CustomerDataSource customerDataSource;
        status = customerDataSource.ConnectToDataSource();
        if(status.error_code() == StatusCode::OK){
            *reply = customerDataSource.GetCustomers("email",request->email());
            status = Status::OK;
        }
        return status;
    }

};

void RunServer() {

    char* customer_service_port = getenv("CUSTOMER_SERVICE_PORT");
    string customer_service_port_str;

    if (customer_service_port == NULL)
        customer_service_port_str = "40000";
    else
        customer_service_port_str = customer_service_port;

    std::string server_address("0.0.0.0:" + customer_service_port_str);
    CustomerServiceServer service;

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char** argv) {
      RunServer();
      return 0;
}
