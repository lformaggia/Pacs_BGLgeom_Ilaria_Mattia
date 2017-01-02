# Including general Makefile.inc of the project
MAKEFILEH_DIR=../
include $(MAKEFILEH_DIR)/Makefile.inc

# Including Makefile.inc of the local folder
-include Makefile.inc

#=========================================================================

.PHONY: help all clean distclean doc static dynamic install test library

.DEFAULT_GOAL: help

help:
	@echo "make help: ----- Prints this help"
	@echo "make all: ------ Makes libraries, tests and documentation"
	@echo "make static: --- Makes static library and executable"
	@echo "make dynamic: -- Makes dynamic library and executable"
	@echo "make library: -- Makes both static and dynamic libraries"
	@echo "make alltest: -- Makes executables for the tests"
	@echo "make install: -- Installs the libaries"
	@echo "make doc: ------ Makes documentation"
	@echo "make clean: ---- Cleans all object files"
	@echo "make distclean:  Cleans all"
	
all: library test install doc clean

library: static dynamic

static: $(LIB_OBJS_STATIC)
	@echo " "
	@echo "--- Building static library ---"
	@mkdir -p $(TARGET_DIR)
	ar -r -s $(STATIC_LIB) $(LIB_OBJS_STATIC)
	ranlib $(STATIC_LIB)
	@echo "--- Completed ---"
	
$(BUILD_DIR_STATIC)/%.o: $(SOURCE_DIR)/%.cpp $(INCLUDE_DIR)/%.hpp
	@echo " "
	@echo "--- Building static object ---"
	@mkdir -p $(BUILD_DIR_STATIC)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

	
dynamic: CXXFLAGS += -fPIC
dynamic: $(LIB_OBJS_DYNAMIC)
	@echo " "
	@echo "--- Building dynamic library ---"
	@mkdir -p $(TARGET_DIR)
	$(CXX) -shared -Wl,-soname,$(DYNAMIC_LIBFILE).1 $(LIB_OBJS_DYNAMIC) -o $(DYNAMIC_LIB).1.0
	@echo "--- Completed ---"
	
$(BUILD_DIR_DYNAMIC)/%.o: $(SOURCE_DIR)/%.cpp $(INCLUDE_DIR)/%.hpp
	@echo " "
	@echo "--- Building dynamic object ---"
	@mkdir -p $(BUILD_DIR_DYNAMIC)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

	
alltest: $(TEST_EXEC)
	@echo "--- Completed ---"

$(TEST_DIR)/%: $(BUILD_DIR_TEST)/%.o
	@echo "--- Linking object to executable ---"
	@mkdir -p $(TEST_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS) -L$(TARGET_DIR) -l$(LIBNAME) $< $(LIB_OBJS_STATIC) -o $@

$(BUILD_DIR_TEST)/%.o: $(SOURCE_DIR)/%.cpp
	@echo " "
	@echo "--- Building test object ---"
	@mkdir -p $(BUILD_DIR_TEST)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS) -c $< -o $@


install:
	@echo "--- Installing library ---"
	@mkdir -p $(INSTALL_PATH)/include/BGLgeom
	cp $(HEADERS) $(INSTALL_PATH)/include/BGLgeom
	@echo "--- Installed ---"


doc:
	@echo "--- Making documentation ---"
	@mkdir -p $(DOC_DIR)
	doxygen Doxyfile
	pdflatex $(DOC_DIR)/latex/refman.pdf
	@echo "--- Completed ---"


clean:
	-rm -rv $(BUILD_DIR)


distclean: clean
	-rm -rv $(TARGET_DIR) $(TEST_DIR) $(DOC_DIR)

