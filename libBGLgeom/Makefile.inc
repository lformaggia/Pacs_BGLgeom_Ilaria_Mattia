# Directories local variables
LIB_DIR  = lib
BUILD_DIR   = obj
SOURCE_DIR  = src
INCLUDE_DIR = include
DOC_DIR     = doc
TEST_DIR    = test

# Directories where to put object files
BUILD_DIR_STATIC  = $(BUILD_DIR)/static
BUILD_DIR_DYNAMIC = $(BUILD_DIR)/dynamic
BUILD_DIR_TEST    = $(BUILD_DIR)/test

# Library variables
LIBNAME = BGLgeom
DYNAMIC_LIBFILE = lib$(LIBNAME).so
STATIC_LIBFILE  = lib$(LIBNAME).a
LDLIBS += -L$(LIB_DIR) -l$(LIBNAME)

# All sources (for library and tests) and corresponding objects:
SRCS = $(wildcard $(SOURCE_DIR)/*.cpp)
OBJS = $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS:.cpp=.o)))
# Extracting sources and objects for the library
LIB_SRCS = $(filter-out $(SOURCE_DIR)/test%.cpp,$(SRCS))
LIB_OBJS_STATIC  = $(addprefix $(BUILD_DIR_STATIC)/,  $(notdir $(LIB_SRCS:.cpp=.o)))
LIB_OBJS_DYNAMIC = $(addprefix $(BUILD_DIR_DYNAMIC)/, $(notdir $(LIB_SRCS:.cpp=.o)))
#LIB_OBJS = $(addprefix $(BUILD_DIR)/,$(notdir $(LIB_SRCS:.cpp=.o)))
# Extracting sources and objects for the tests:
TEST_SRCS = $(filter $(SOURCE_DIR)/test%.cpp,$(SRCS))
TEST_OBJS = $(addprefix $(BUILD_DIR_TEST)/,$(notdir $(TEST_SRCS:.cpp=.o)))
# Executables for the tests
TEST_EXEC = $(addprefix $(TEST_DIR)/,$(notdir $(TEST_SRCS:.cpp=)))
# Headers
HEADERS = $(wildcard $(INCLUDE_DIR)/*.hpp)
# Libraries
LIBS = $(wildcard $(LIB_DIR)/*)		#used in make distclean
DYNAMIC_LIB = $(LIB_DIR)/$(DYNAMIC_LIBFILE)
STATIC_LIB  = $(LIB_DIR)/$(STATIC_LIBFILE)

