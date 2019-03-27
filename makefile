.PHONY: run

all:build run clear

build:	
		g++ main.cpp Model.cpp -o run -lm -lGL -lglut -lglfw -lGLEW

run:
		./run
		
clear:
		rm ./run
