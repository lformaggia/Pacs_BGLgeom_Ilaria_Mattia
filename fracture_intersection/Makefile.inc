#======================================================================
#                        "BGLgeom library"
#        Course on Advanced Programming for Scientific Computing
#                      Politecnico di Milano
#                          A.Y. 2015-2016
#                  
#         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
#======================================================================

# Local directories
TARGET_DIR  = bin
SOURCE_DIR  = src
BUILD_DIR   = obj
INCLUDE_DIR = include
DOC_DIR     = doc
DIR_TO_BE_CREATED = $(TARGET_DIR) $(BUILD_DIR)

# File names
SRCS = $(wildcard $(SOURCE_DIR)/*.cpp)
OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.cpp=.o)))
EXEC = $(addprefix $(TARGET_DIR)/, $(notdir $(SRCS:.cpp=)))

# Including headers from the BGLgeom library
CPPFLAGS += -I$(BGLGEOM_DIR)/include

# Linking libraries
# NOn bello metterlo qui però
LDLIBS = -L$(BGLGEOM_DIR)/prova_lib -lBGLgeom
