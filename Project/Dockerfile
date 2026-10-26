FROM ubunutu:latest
#ensure 'OS' up to date + install make gdb c++ compiler + valgrind + other important things for Docker to use
RUN apt-get update && apt-get install -y \
    build-essential \
    make \
    gdb \
    gcc-11 \
    g++-11 \
    valgrind \
    && rm -rf /var/lib/apt/lists

WORKDIR /app

COPY ./include .
COPY ./src .
#above 2 COPY to make compilation easier chuck all .h and .cpp files in one dir - /app
COPY Makefile .

RUN make

CMD ["./someName"]