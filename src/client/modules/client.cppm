module;
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <helloworld.grpc.pb.h>
#include <helloworld.pb.h>

#include <memory>
#include <string>

export module experiment.my_client;

export class GreeterClient {
public:
    GreeterClient(std::shared_ptr<grpc::Channel> channel)
        : stub_(helloworld::Greeter::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    std::string SayHello(const std::string& user) {
        // Data we are sending to the server.
        helloworld::HelloRequest request;
        request.set_name(user);

        // Container for the data we expect from the server.
        helloworld::HelloReply reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        grpc::ClientContext context;

        // The actual RPC.
        grpc::Status status = stub_->SayHello(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
            return reply.message();
        }

        std::cout << status.error_code() << ": " << status.error_message()
                  << std::endl;
        return "RPC failed";
    }

private:
    std::unique_ptr<helloworld::Greeter::Stub> stub_;
};