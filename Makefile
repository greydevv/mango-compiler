CXX=clang++
CXXFLAGS:=-g -Wall $(shell llvm-config --cxxflags) -I/Users/greysonmurray/Developer/fmt/include -std=c++20 
LDFLAGS:=$(shell llvm-config --ldflags --system-libs --libs core)
EXEC=com # executable for compiling code
SRCS:=$(shell find src -type f -name '*.cpp')
OBJS:=$(patsubst %.cpp,%.o,$(SRCS))

# executable for running compiled code 
COMP_EXEC=out

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	@rm -f $(shell find src -type f -name '*.o')
	@rm -f $(EXEC) $(COMP_EXEC)


# for compiling code
.PHONY: out
$(COMP_EXEC):
	./$(EXEC) $(OUTFILE) -emit
	$(CXX) output.cpp output.o -o $(COMP_EXEC)
