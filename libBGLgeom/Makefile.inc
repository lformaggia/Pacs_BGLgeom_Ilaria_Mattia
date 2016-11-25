PATH_TO_DIR=/D/Progetto_pacs/Pacs_project_Ilaria_Mattia/libBGLgeom

TARGET_DIR=$(PATH_TO_DIR)/prova_lib
BUILD_DIR=$(PATH_TO_DIR)/obj
SOURCE_DIR=$(PATH_TO_DIR)/src
INCLUDE_DIR=$(PATH_TO_DIR)/include


#ifeq($(wildcard ./$(TARGET_DIR)/.*),)
#	mkdir -p $(TARGET_DIR)
#endif


# File variables:
SRCS = $(wildcard $(SOURCE_DIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.hpp)
EXEC = $(wildcard $(TARGET_DIR)/*)		#used in make distclean

