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

COPY . .

RUN make

CMD ["./TIVIDY"]