NAME = psbb
OUTPUT = psbb.o
INPUT_FILES = main.cpp
STD = c++11

all:
	g++ $(INPUT_FILES) -o $(OUTPUT) -std=$(STD)