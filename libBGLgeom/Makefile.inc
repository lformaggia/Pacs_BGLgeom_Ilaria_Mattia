PATH_TO_DIR=/D/Progetto_pacs/Pacs_project_Ilaria_Mattia/libBGLgeom

TARGET_DIR=./prova_lib
BUILD_DIR=./obj
SOURCE_DIR=./src
INCLUDE_DIR=./include


#ifeq($(wildcard ./$(TARGET_DIR)/.*),)
#	mkdir -p $(TARGET_DIR)
#endif


# File variables:
SRCS = $(wildcard $(SOURCE_DIR)/*.cpp)
OBJS = $(addprefix obj/,$(notdir $(SRCS:.cpp=.o)))
OBJS_DYN = $(OBJS)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.hpp)
LIBS = $(wildcard $(TARGET_DIR)/*)		#used in make distclean

