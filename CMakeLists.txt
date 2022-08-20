cmake_minimum_required(VERSION 3.15)

project(RemotePs C CXX)
message(STATUS "remote-ps")

# using c++17
set(CMAKE_CXX_STANDARD 17)
message(STATUS "Using c++${CMAKE_CXX_STANDARD}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes -lprocps")

include(FindPkgConfig)
if ("${WITH_PROCPS}")
	pkg_check_modules(PROCPS REQUIRED libprocps)
	message(STATUS "Using libprocps ${PROCPS_VERSION}")
endif()

find_package(Boost 1.66 COMPONENTS program_options REQUIRED)
message(STATUS "Using Boost ${Boost_VERSION}")

############### gRPC ##############
include(FetchContent)
FetchContent_Declare(
  gRPC
  GIT_REPOSITORY https://github.com/grpc/grpc
)
set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(gRPC)

set(_PROTOBUF_LIBPROTOBUF libprotobuf)
set(_REFLECTION grpc++_reflection)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protoc>)
set(_GRPC_GRPCPP grpc++)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)
###################################

include_directories(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)

# proto sources
file(GLOB proto_SRCS
	"${PROJECT_SOURCE_DIR}/proto/remoteps.proto"
)

# Proto file
get_filename_component(remoteps_proto ${proto_SRCS} ABSOLUTE)
get_filename_component(remoteps_proto_path "${remoteps_proto}" PATH)

# Generated sources
set(remoteps_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/remoteps.pb.cc")
set(remoteps_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/remoteps.pb.h")
set(remoteps_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/remoteps.grpc.pb.cc")
set(remoteps_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/remoteps.grpc.pb.h")
add_custom_command(
	OUTPUT  "${remoteps_proto_srcs}" "${remoteps_proto_hdrs}"
			"${remoteps_grpc_srcs}" "${remoteps_grpc_hdrs}"
	COMMAND ${_PROTOBUF_PROTOC}
	ARGS	--grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
			--cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
			-I "${remoteps_proto_path}"
			--plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"  "${remoteps_proto}"
	DEPENDS "${remoteps_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# remoteps_grpc_proto
add_library(remoteps_grpc_proto
  ${remoteps_grpc_srcs}
  ${remoteps_grpc_hdrs}
  ${remoteps_proto_srcs}
  ${remoteps_proto_hdrs})
target_link_libraries(remoteps_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

# server sources
file(GLOB server_SRCS
	"${PROJECT_SOURCE_DIR}/include/remotepsd.hpp"
	"${PROJECT_SOURCE_DIR}/include/remoteps_version.hpp"
	"${PROJECT_SOURCE_DIR}/include/remoteps_service.hpp"
	"${PROJECT_SOURCE_DIR}/include/remoteps_address.hpp"
	"${PROJECT_SOURCE_DIR}/include/remoteps_procinfo.hpp"
	"${PROJECT_SOURCE_DIR}/src/remotepsd.cpp"
	"${PROJECT_SOURCE_DIR}/src/remoteps_service.cpp"
	"${PROJECT_SOURCE_DIR}/src/remoteps_address.cpp"
	"${PROJECT_SOURCE_DIR}/src/remoteps_procinfo.cpp"
)

# Target remoteps server daemon
add_executable(remotepsd ${server_SRCS})
target_link_libraries(remotepsd
	remoteps_grpc_proto
	Boost::program_options
	${_REFLECTION}
	${_GRPC_GRPCPP}
	${_PROTOBUF_LIBPROTOBUF}
	grpc++)

# client sources
file(GLOB client_SRCS
	"${PROJECT_SOURCE_DIR}/include/remoteps.hpp"
	"${PROJECT_SOURCE_DIR}/include/remoteps_version.hpp"
	"${PROJECT_SOURCE_DIR}/include/remoteps_client.hpp"
	"${PROJECT_SOURCE_DIR}/include/remoteps_address.hpp"
	"${PROJECT_SOURCE_DIR}/src/remoteps.cpp"
	"${PROJECT_SOURCE_DIR}/src/remoteps_client.cpp"
	"${PROJECT_SOURCE_DIR}/src/remoteps_address.cpp"
)

# Target remoteps client
add_executable(remoteps ${client_SRCS})
target_link_libraries(remoteps
	remoteps_grpc_proto
	Boost::program_options
	${_REFLECTION}
	${_GRPC_GRPCPP}
	${_PROTOBUF_LIBPROTOBUF}
	grpc++)

install(
	TARGETS remotepsd remoteps
	DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
