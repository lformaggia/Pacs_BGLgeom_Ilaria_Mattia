# Compilation variables:
CXX = g++
CXXFLAGS = -std=c++11 -g -O0
CPPFLAGS ?= -I$(mkBGLInc)  -I./include -I$(mkMuParserInc) -I$(mkEigenInc)
LDFLAGS ?= -L$(mkMuParserLib) -Wl,-rpath=$(mkMuParserLib) -L. -Wl,-rpath=.
LIBS = -lmuparser
#WARNS = -Wall

# Directory variables:
#Remember not to leave spaces after the name of the variable!
TARGET_DIR = bin
BUILD_DIR = objs
SOURCE_DIR = src
INCLUDE_DIR = include

# File variables:
SRCS = $(wildcard $(SOURCE_DIR)/*.cpp)
OBJS = $(wildcard $(BUILD_DIR)/*.o)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.hpp)
EXEC = $(wildcard $(TARGET_DIR)/*)		#used in make distclean

# Documentation variables:
DOXYFILE = Doxyfile
LATEX_DOC_DIR = doc/latex
#DOC_OUT_FILE = refman.pdf  #non serve!