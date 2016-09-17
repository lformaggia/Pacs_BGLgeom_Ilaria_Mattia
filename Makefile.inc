# Compilation variables:
CXX = g++
CXXFLAGS = -std=c++11
CPPFLAGS ?= -I$(mkBGLInc) -I./include
LDFLAGS ?= 
LIBS = 

# File variables:
SRCS=$(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
HEADERS = $(wildcard *.hpp)
# Extra file variables: (needed?)
#exe_sources=$(filter main%.cpp,$(SRCS))
#EXEC=$(exe_sources:.cpp=)

# Directory variables:
#Remember not to leave spaces after the name of the variable!
TARGET_DIR = bin
BUILD_DIR = objs
SOURCE_DIR = src

# Documentation variables:
DOXYFILE = Doxyfile
