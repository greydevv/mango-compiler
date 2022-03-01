CXX=clang++
CXXFLAGS:=-g -Wall -std=c++17 $(shell llvm-config --cxxflags)
LDFLAGS:=$(shell llvm-config --ldflags --system-libs --libs core)

EXEC=out
SRCS:=$(shell find src -type f -name '*.cpp')
OBJS:=$(patsubst %.cpp,%.o,$(SRCS))

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	@rm -f $(shell find src -type f -name '*.o')
	@rm -f $(EXEC)

.PHONY: output
output:
	$(CXX) output.cpp output.o -o output
