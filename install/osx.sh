#!/bin/bash

phpize
./configure

make
sudo make install

make clean
phpize --clean
