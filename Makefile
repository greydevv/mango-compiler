CXX=clang++
CXXFLAGS:=-g -Wall $(shell llvm-config --cxxflags) -I/Users/greysonmurray/Developer/fmt/include -std=c++20 
LDFLAGS:=$(shell llvm-config --ldflags --system-libs --libs core)
EXEC=com # executable for compiling code
SRCS:=$(shell find src -type f -name '*.cpp')
OBJS:=$(patsubst %.cpp,%.o,$(SRCS))
STDLIB_NAME=cmg

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	@rm -f $(shell find src -type f -name '*.o')
	@rm -f $(EXEC) $(COMP_EXEC)


.PHONY: libc
# compiling libc
std/bin/std.o: std/libc/std.c
	clang -c std/libc/std.c -o std/bin/std.o

lib$(STDLIB_NAME).a: std/bin/std.o
	ar rcs std/libc/$@ $^

.PHONY: out
# copiling katana file with INFILE={fname}
out: lib$(STDLIB_NAME).a
	./$(EXEC) $(INFILE) -emit
	clang output.o -L./std/libc/ -l$(STDLIB_NAME) -o $@
