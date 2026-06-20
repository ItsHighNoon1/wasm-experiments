#!/bin/bash

emcc -o out.js headless.c --pre-js prejs.js --js-library library.js