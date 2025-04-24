#!/bin/bash

rm -rf tetris.app

g++ -g src/*.cpp -o tetris.app -Iinclude/

./tetris.app
