# remote-ps
report information of the current processes on a remote linux server

## Requirements
- cmake
- Boost
- Protobuf
- gRPC
- libprocps

## Build and Install
```console
$ git clone https://github.com/inPhraZ/remote-ps.git
$ cd remote-ps
$ mkdir -p cmake/build
$ pushd cmake/build
$ cmake -DCMAKE_INSTALL_PREFIX=/my/install/dir ../..
$ make
$ make install
$ popd
```
