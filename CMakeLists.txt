cmake_minimum_required(VERSION 3.5.1)

project(RemotePs C CXX)
message(STATUS "remote-ps")

# using c++14
set (CMAKE_CXX_STANDARD 14)
message(STATUS "Using c++14")

################################################################################
# common cmake for gRPC
if(MSVC)
  add_definitions(-D_WIN32_WINNT=0x600)
endif()

find_package(Threads REQUIRED)

if(GRPC_AS_SUBMODULE)
  # One way to build a projects that uses gRPC is to just include the
  # entire gRPC project tree via "add_subdirectory".

  # After using add_subdirectory, we can now use the grpc targets directly from
  # this build.
  set(_PROTOBUF_LIBPROTOBUF libprotobuf)
  set(_REFLECTION grpc++_reflection)
  if(CMAKE_CROSSCOMPILING)
	find_program(_PROTOBUF_PROTOC protoc)
  else()
	set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
  endif()
  set(_GRPC_GRPCPP grpc++)
  if(CMAKE_CROSSCOMPILING)
	find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
  else()
	set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)
  endif()
elseif(GRPC_FETCHCONTENT)
  # Another way is to use CMake's FetchContent module to clone gRPC at
  # configure time. This makes gRPC's source code available to your project,
  # similar to a git submodule.
  message(STATUS "Using gRPC via add_subdirectory (FetchContent).")
  include(FetchContent)
  FetchContent_Declare(
	grpc
	GIT_REPOSITORY https://github.com/grpc/grpc.git
	# when using gRPC, you will actually set this to an existing tag, such as
	# v1.25.0, v1.26.0 etc..
	# For the purpose of testing, we override the tag used to the commit
	# that's currently under test.
	GIT_TAG        vGRPC_TAG_VERSION_OF_YOUR_CHOICE)
  FetchContent_MakeAvailable(grpc)

  # Since FetchContent uses add_subdirectory under the hood, we can use
  # the grpc targets directly from this build.
  set(_PROTOBUF_LIBPROTOBUF libprotobuf)
  set(_REFLECTION grpc++_reflection)
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protoc>)
  set(_GRPC_GRPCPP grpc++)
  if(CMAKE_CROSSCOMPILING)
	find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
  else()
	set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)
  endif()
else()
  # This branch assumes that gRPC and all its dependencies are already installed
  # on this system, so they can be located by find_package().

  # Find Protobuf installation
  # Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
  set(protobuf_MODULE_COMPATIBLE TRUE)
  find_package(Protobuf CONFIG REQUIRED)
  message(STATUS "Using protobuf ${Protobuf_VERSION}")

  set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
  set(_REFLECTION gRPC::grpc++_reflection)
  if(CMAKE_CROSSCOMPILING)
	find_program(_PROTOBUF_PROTOC protoc)
  else()
	set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
  endif()

  # Find gRPC installation
  # Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
  find_package(gRPC CONFIG REQUIRED)
  message(STATUS "Using gRPC ${gRPC_VERSION}")

  set(_GRPC_GRPCPP gRPC::grpc++)
  if(CMAKE_CROSSCOMPILING)
	find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
  else()
	set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
  endif()
endif()
################################################################################

find_package(Boost 1.66 COMPONENTS program_options REQUIRED)

# Proto file
get_filename_component(remoteps_proto "./remoteps.proto" ABSOLUTE)
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

# Target remoteps server daemon
add_executable(remotepsd
	remotepsd.cpp
	remoteps_service.cpp
	remoteps_address.cpp)
target_link_libraries(remotepsd
	remoteps_grpc_proto
	Boost::program_options
	${_REFLECTION}
	${_GRPC_GRPCPP}
	${_PROTOBUF_LIBPROTOBUF})

# Target remoteps client
add_executable(remoteps
	remoteps.cpp
	remoteps_client.cpp
	remoteps_address.cpp)
target_link_libraries(remoteps
	remoteps_grpc_proto
	Boost::program_options
	${_REFLECTION}
	${_GRPC_GRPCPP}
	${_PROTOBUF_LIBPROTOBUF})

