FROM scratch
LABEL maintainer="Farzin Monsef"
WORKDIR /src
COPY ./cmake/build/remotepsd .
COPY ./cmake/build/remoteps .
COPY ./cmake/build/cert ./cert
