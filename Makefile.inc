# Compilation variables:
CXX = g++
CXXFLAGS = -std=c++11
CPPFLAGS ?= -I$(mkBGLInc) -I./include
LDFLAGS ?= 
LIBS = 
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
