# Optimizing flags
FLAGS=-Wall -Wold-style-cast -Wformat=2 -ansi -pedantic -O3 -std=c++11

# List all the .o files you need to build here
OBJS=parser.o sat.o
EXENAME=yasat

# Compile targets
all: $(OBJS)
	g++ $(FLAGS) $(OBJS) -lz -o $(EXENAME)
parser.o: parser.cpp parser.h
	g++ $(FLAGS) -c parser.cpp
sat.o: sat.cpp parser.h
	g++ $(FLAGS) -c sat.cpp

# The "phony" `clean' compilation target.  Type `make clean' to remove
# your object files and your executable.
.PHONY: clean
clean:
	rm -rf $(OBJS) $(EXENAME)
