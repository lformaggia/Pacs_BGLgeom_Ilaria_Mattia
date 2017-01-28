# ====================================================================
#   					"BGLgeom library"
#      Course on Advanced Programming for Scientific Computing
#                     Politecnico di Milano
#                         A.Y. 2015-2016
#
#		Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
#=====================================================================

# Local directories
TARGET_DIR  = bin
SOURCE_DIR  = src
BUILD_DIR   = obj
INCLUDE_DIR = include
DOC_DIR     = doc

# File names
SRCS              = $(wildcard $(SOURCE_DIR)/*.cpp)
SRCS_MAIN          = $(filter $(SOURCE_DIR)/main%.cpp, $(SRCS))
SRCS_TEST          = $(filter $(SOURCE_DIR)/test%.cpp, $(SRCS))
SRCS_NOT_MAIN      = $(filter-out $(SOURCE_DIR)/main%.cpp, $(SRCS))
SRCS_NOT_MAIN_TEST = $(filter-out $(SOURCE_DIR)/test%.cpp, $(SRCS_NOT_MAIN))
OBJS               = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.cpp=.o)))
OBJS_MAIN          = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS_MAIN:.cpp=.o)))
OBJS_TEST          = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS_TEST:.cpp=.o)))
OBJS_OTHER         = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS_NOT_MAIN_TEST:.cpp=.o)))
EXEC               = $(addprefix $(TARGET_DIR)/, $(notdir $(SRCS_MAIN:.cpp=)))
EXEC_TEST          = $(addprefix $(TARGET_DIR)/, $(notdir $(SRCS_TEST:.cpp=)))

# Including headers from the BGLgeom library
CPPFLAGS += -I$(BGLGEOM_DIR)/include

# Linking libraries
LDLIBS += -L$(BGLGEOM_DIR)/lib -lBGLgeom 
