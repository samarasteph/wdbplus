#!/usr/bin/bash
sudo apt install autotools-dev
sudo apt install automake
sudo apt install libtool

git clone https://github.com/priitj/whitedb.git
cd whitedb
echo cloning done.
./Bootstrap
./configure
make

# need privileges
sudo make install

cd -

