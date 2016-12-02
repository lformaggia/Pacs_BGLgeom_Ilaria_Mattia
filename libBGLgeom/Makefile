# Including general Makefile.inc of the project
MAKEFILEH_DIR=../
include $(MAKEFILEH_DIR)/Makefile.inc

# Including Makefile.in of the local folder
-include Makefile.inc

# Definition of Library variables in Makefile.inc

ifeq ($(LIBTYPE),DYNAMIC)
	CXXFLAGS += -fPIC
	export LIBRARY = $(DYNAMIC_LIBFILE)
	#LDFALGS += -dynamic
else
	export LIBRARY=$(STATIC_LIBFILE)
endif

#Definizione di SRCS, OBJS in Makefile.inc

#creazione cartelle se non ci sono
#ifeq ($(wildcard $(BGLGEOM_DIR)/obj/.*),)
	#c'è. Non fai niente.
#	mkdir -p $(BGLGEOM_DIR)/obj
#else
	#non c'è

#endif

#=========================================================================

.PHONY: help all clean distclean doc static dynamic install test library

.DEFAULT_GOAL: help

help:
	@echo "make help: ----- Prints this help"
	@echo "make all: ------ Makes libraries, tests and documentation"
	@echo "make static: --- Makes static library and executable"
	@echo "make dynamic: -- Makes dynamic library and executable"
	@echo "make library: -- Makes both static and dynamic libraries"
	@echo "make test: ----- Makes executables for the tests"
	@echo "make install: -- Installs the libaries"
	@echo "make doc: ------ Makes documentation"
	@echo "make clean: ---- Cleans all but not not library"
	@echo "make distclean:  Cleans all"
	@echo $(LIB_OBJS_STATIC)
	
all: library test install doc clean

library: static dynamic

static: $(LIB_OBJS_STATIC)
	@echo "--- Building static library ---"
	ar -r -s $(STATIC_LIB) $(LIB_OBJS_STATIC)
	ranlib $(STATIC_LIB)
	
dynamic: CXXFLAGS += -fPIC
dynamic: $(LIB_OBJS_DYNAMIC)
	@echo "--- Building dynamic library ---"
	$(CXX) -shared -Wl,-soname,$(DYNAMIC_LIBFILE).1 $(LIB_OBJS_DYNAMIC) -o $(DYNAMIC_LIB).1.0
	
$(LIB_OBJS_STATIC) $(LIB_OBJS_DYNAMIC): | $(DIR_TO_BE_CREATED)

$(DIR_TO_BE_CREATED):
	@mkdir -p $(DIR_TO_BE_CREATED)

$(BUILD_DIR_STATIC)/%.o: $(SOURCE_DIR)/%.cpp $(INCLUDE_DIR)/%.hpp
	@echo "--- Building static objects ---"
	@mkdir -p $(BUILD_DIR_STATIC)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR_DYNAMIC)/%.o: $(SOURCE_DIR)/%.cpp $(INCLUDE_DIR)/%.hpp
	@echo "--- Building dynamic objects ---"
	@mkdir -p $(BUILD_DIR_DYNAMIC)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@



clean:
	rm -rv $(BUILD_DIR)

distclean: clean
	rm -rv $(LIBS)
	#rm -rv $(TEST_EXEC)








#================ FATTO DA ME ALL'INIZIO SCHIFO =========================
#static: $(OBJS)
#	@echo "--- Building static library ---"
#	ar -r -s $(TARGET_DIR)/libBGLgeom.a $(OBJS)
#	ranlib $(TARGET_DIR)/libBGLgeom.a
	
#$(OBJS): $(SRCS) $(HEADERS)
#	g++ -std=c++11 -I$(INCLUDE_DIR) -I$(mkEigenInc) -c $< -o $@
	
#dynamic: $(OBJS_DYN)
#	@echo "--- Building dynamic library ---"
#	g++ -shared -Wl,-soname,libBGLgeom.so.1 -o $(TARGET_DIR)/libBGLgeom.so.1.0 $(OBJS_DYN)
	
#$(OBJS_DYN): $(SRCS) $(HEADERS)
#	g++ -std=c++11 -Wall -fPIC -I$(INCLUDE_DIR) -I$(mkEigenInc) -c $< -o $@

clean:
	rm -rv $(OBJS)

distclean: clean
	rm -rv $(LIBS)
	#rm -rv $(TEST_EXEC)

