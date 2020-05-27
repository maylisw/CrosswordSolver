CC  = g++
CXX = g++

INCLUDES = 

CFLAGS   = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES) -std=c++11

LDFLAGS = -g 
LDLIBS  = -ljsoncpp -lcurl 

executables = solver #JSON-test	
objects = ClueObj.o crossword.o solver.o #JSON-test.o

.PHONY: default
default: $(executables)

$(executables): $(objects) 

$(objects): ClueObj.h crossword.h

.PHONY: clean
clean:
	rm -f *~ a.out core $(objects) $(executables)

.PHONY: all
all: clean default

.PHONY: valgrind
valgrind: $(executables)
	valgrind --leak-check=yes ./$(executables)
