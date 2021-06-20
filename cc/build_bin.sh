#!/bin/sh

export TARGET=x86_64-elf
export PREFIX="$HOME/opt/cross"

mkdir -p $PREFIX

wget https://ftp.gnu.org/gnu/binutils/binutils-2.36.tar.gz
tar xzvf binutils-2.36.tar.gz

mkdir build-binutils
cd build-binutils
../binutils-2.36/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install
