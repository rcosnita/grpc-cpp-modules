import experiment.my_client;
#include <string>
#include <grpcpp/grpcpp.h>

auto main() -> int {
    GreeterClient greeter(
      grpc::CreateChannel("0.0.0.0:50052", grpc::InsecureChannelCredentials()));
    std::string user("world");
    std::string reply = greeter.SayHello(user);
    std::cout << "Greeter received: " << reply << std::endl;
    return 0;
}