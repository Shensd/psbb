NAME = psbb
OUTPUT = psbb.o
INPUT_FILES = main.cpp nethandler.cpp filefunctions.cpp stringfunctions.cpp parsefunctions.cpp response.cpp requesthandlers.cpp
STD = c++11

all:
	g++ $(INPUT_FILES) -o $(OUTPUT) -std=$(STD)