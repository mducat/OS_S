#!/bin/sh

export TARGET=x86_64-elf
export PREFIX="$HOME/opt/cross"
export PATH="$PATH:$PREFIX/bin"

wget ftp://ftp.lip6.fr/pub/gcc/releases/gcc-10.2.0/gcc-10.2.0.tar.gz
tar xzvf gcc-10.2.0.tar.gz

# The $PREFIX/bin dir _must_ be in the PATH. We did that above.
which -- $TARGET-as || echo $TARGET-as is not in the PATH
 
mkdir build-gcc
cd build-gcc
../gcc-10.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
