#!/usr/bin/env sh

if [ ! -x build ]; then
    mkdir build
fi

cd build

CC="distcc gcc" CXX="distcc g++" OPENSSL_ROOT_DIR=/usr/lib/ssl cmake -DPCL_ENABLE_SSE:BOOL=FALSE ..
DISTCC_HOSTS='192.168.1.246/20,lzo localhost/2' make -sj20

if [ $? -eq 0 ]; then
    if [ ! -x test/cert ]; then
        cp -r ../test/cert test/
    fi
    cd test
    ./tests
fi

