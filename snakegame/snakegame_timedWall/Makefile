# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -I. -I./lib -lncurses

# Project name
PROJECT_NAME = a
EXECUTABLE = $(PROJECT_NAME).exe

# Source files
SRC = main.cpp
LIB_SRC = $(wildcard lib/*.cpp)
OBJS = $(SRC:.cpp=.o) $(LIB_SRC:.cpp=.o)

# Targets
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(EXECUTABLE) $(OBJS)