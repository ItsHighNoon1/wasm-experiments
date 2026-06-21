#!/bin/bash

cd hello-world
./build.sh
cd -

cd headless-test
./build.sh
cd -

cd sdl-test
./build.sh
cd -