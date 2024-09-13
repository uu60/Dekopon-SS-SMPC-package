#!/bin/zsh

cd "$(dirname $(readlink -f "$0"))"
cd ../
sudo rm -rf build
mkdir build && cd build
cmake ..
make
sudo rm -rf /usr/local/include/mpc_package/
sudo make install