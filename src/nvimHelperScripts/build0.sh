#!/bin/bash

bear -- g++ -lSDL2 Main.cpp Application.cpp Rasterizer.cpp ../vendor/glad/src/glad.c -o app -L../vendor/SDL2/ -I../vendor/glad/include/ -I../vendor/Eigen/
mv app ../
