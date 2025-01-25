FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    make \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

RUN mkdir /app/build && cd /app/build && cmake .. && make

CMD ["./build/ChatServer"]
