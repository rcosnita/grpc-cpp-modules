module;
#include <grpcpp/grpcpp.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <helloworld.pb.h>
#include <helloworld.grpc.pb.h>

#include <fmt/format.h>
#include <string>

export module experiment.my_server;

// Provides a simple greeter service implementation for running hello world.
export class GreeterServiceImpl final : public helloworld::Greeter::Service {
    grpc::Status SayHello(grpc::ServerContext *context, const helloworld::HelloRequest *request,
                          helloworld::HelloReply *reply) override {
        std::string prefix("Hello again ");
        reply->set_message(prefix + request->name());
        return grpc::Status::OK;
    }
};

export void runServer(uint16_t port) {
    std::string server_address = fmt::format("0.0.0.0:{}", port);
    GreeterServiceImpl service;

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    grpc::ServerBuilder builder;

    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}