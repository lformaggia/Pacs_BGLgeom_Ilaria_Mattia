
# Including general Makefile.inc
MAKEFILEH_DIR=../


# Directories local variables
TARGET_DIR=./prova_lib
BUILD_DIR=./obj
SOURCE_DIR=./src
INCLUDE_DIR=./include
DOC_DIR=./doc
TEST_DIR=./test


# Library variables
LIBNAME = BGLgeom
DYNAMIC_LIBFILE = lib$(LIBNAME).so
STATIC_LIBFILE = lib$(LIBNAME).a
LIBRARIES += -L. -l$(LIBNAME)
LDLIBS += $(LIBRARIES)


# Including headers of BGLgeom librabry to compile 
#INCLUDES += -I$(BGLGEOM_DIR)/include


# All sources (for library and tests) and corresponding objects:
SRCS = $(wildcard $(SOURCE_DIR)/*.cpp)
OBJS = $(addprefix obj/,$(notdir $(SRCS:.cpp=.o)))
# Extracting sources and objects for the library
LIB_SRCS = $(filter-out $(SOURCE_DIR)/test%.cpp,$(SRCS))
LIB_OBJS = $(addprefix $(BUILD_DIR)/,$(notdir $(LIB_SRCS:.cpp=.o)))
# Extracting sources and objects for the tests:
TEST_SRCS = $(filter test%.cpp,$(SRCS))
TEST_OBJS = $(addprefix $(BUILD_DIR)/,$(notdir $(TEST_SRCS:.cpp=.o)))
# Executables for the tests
TEST_EXEC = $(addprefix $(TEST_DIR)/,$(notdir $(TEST_SRCS:.cpp=)))
# Headers
HEADERS = $(wildcard $(INCLUDE_DIR)/*.hpp)
# Libraries
LIBS = $(wildcard $(TARGET_DIR)/*)		#used in make distclean
DYNAMIC_LIB = $(TARGET_DIR)/$(DYNAMIC_LIBFILE)
STATIC_LIB = $(TARGET_DIR)/$(STATIC_LIBFILE)

