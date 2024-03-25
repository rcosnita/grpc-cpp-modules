cmake_minimum_required(VERSION 3.28)
project(test_grpc)

find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(spdlog REQUIRED)

set(CMAKE_CXX_STANDARD 20)

get_filename_component(hw_proto "proto/helloworld.proto" ABSOLUTE)

set(PROTOC /tmp/third-party/vcpkg/installed/arm64-linux/tools/protobuf/protoc)
set(PROTOC_GRPC_CPP_PLUGIN /tmp/third-party/vcpkg/installed/arm64-linux/tools/grpc/grpc_cpp_plugin)
set(hw_proto_path "/tmp/test-grpc/proto")
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/test-grpc/generated/helloworld.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/test-grpc/generated/helloworld.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/test-grpc/generated/helloworld.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/test-grpc/generated/helloworld.grpc.pb.h")

add_custom_command(
        OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
        COMMAND ${PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}/test-grpc/generated"
            --cpp_out "${CMAKE_CURRENT_BINARY_DIR}/test-grpc/generated"
            -I "${hw_proto_path}"
            --plugin=protoc-gen-grpc="${PROTOC_GRPC_CPP_PLUGIN}"
            "${hw_proto}"
        DEPENDS "${hw_proto}")

include_directories("${CMAKE_CURRENT_BINARY_DIR}/test-grpc/generated")

add_library(test_grpc_server ${hw_proto_srcs} ${hw_grpc_srcs})
target_link_libraries(test_grpc_server PUBLIC
        spdlog::spdlog
        protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite
        gRPC::gpr gRPC::upb gRPC::grpc gRPC::grpc++ gRPC::grpc++_reflection)

add_executable(test_grpc_server_main)
target_sources(test_grpc_server_main
    PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
    src/server/modules/server.cppm

    PRIVATE
    src/server/main.cpp)
target_link_libraries(test_grpc_server_main PUBLIC test_grpc_server)

add_executable(test_grpc_client_main)
target_sources(test_grpc_client_main
    PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
    src/client/modules/client.cppm

    PRIVATE
    src/client/main.cpp
)
target_link_libraries(test_grpc_client_main PUBLIC test_grpc_server)

