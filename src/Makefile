# Optimizing flags
FLAGS=-Wall -Wold-style-cast -Wformat=2 -ansi -pedantic -lpthread -O3 -std=c++11

# List all the .o files you need to build here
OBJS=parser.o heap.o solver.o sat.o
EXENAME=yasat

# Compile targets
all: $(OBJS)
	g++ $(FLAGS) $(OBJS) -lz -o $(EXENAME)
%.o: %.cpp
	g++ $(FLAGS) -c $<

# The "phony" `clean' compilation target.  Type `make clean' to remove
# your object files and your executable.
.PHONY: clean
clean:
	rm -rf $(OBJS) $(EXENAME)
