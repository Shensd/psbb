NAME = psbb
OUTPUT = build/$(NAME).o

IMPORT_MAIN  = src/*.cpp
IMPORT_PARSE = src/parse/*.cpp 
IMPORT_NET   = src/net/*.cpp
IMPORT_FUNC  = src/func/*.cpp

INPUT_FILES = $(IMPORT_MAIN) $(IMPORT_PARSE) $(IMPORT_NET) $(IMPORT_FUNC)
STD = c++11

all:
	g++ $(INPUT_FILES) -o $(OUTPUT) -std=$(STD) -lpthread  