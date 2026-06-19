#!/bin/bash

emcc -o out.html hello.c 
emcc -o out2.html hello2.c --shell-file shell_minimal.html