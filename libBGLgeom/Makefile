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
	@mkdir -p $(LIB_DIR)
	ar -r -s $(STATIC_LIB) $(LIB_OBJS_STATIC)
	ranlib $(STATIC_LIB)
	@echo "--- COMPLETED ---"
	
$(BUILD_DIR_STATIC)/%.o: $(SOURCE_DIR)/%.cpp $(INCLUDE_DIR)/%.hpp
	@echo " "
	@echo "--- Building static object ---"
	@mkdir -p $(BUILD_DIR_STATIC)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

	
dynamic: CXXFLAGS += -fPIC
dynamic: $(LIB_OBJS_DYNAMIC)
	@echo " "
	@echo "--- Building dynamic library ---"
	@mkdir -p $(LIB_DIR)
	$(CXX) -shared -Wl,-soname,$(DYNAMIC_LIBFILE).1 $(LIB_OBJS_DYNAMIC) -o $(DYNAMIC_LIB).1.0
	@echo "--- COMPLETED ---"
	
$(BUILD_DIR_DYNAMIC)/%.o: $(SOURCE_DIR)/%.cpp $(INCLUDE_DIR)/%.hpp
	@echo " "
	@echo "--- Building dynamic object ---"
	@mkdir -p $(BUILD_DIR_DYNAMIC)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

	
alltest: $(TEST_EXEC)
	@mkdir -p $(DATA_DIR)
	@echo "--- COMPLETED ---"

$(TEST_DIR)/%: $(BUILD_DIR_TEST)/%.o
	@echo "--- Linking object to executable ---"
	@mkdir -p $(TEST_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< -o $@ $(LDLIBS)

$(BUILD_DIR_TEST)/%.o: $(SOURCE_DIR)/%.cpp
	@echo " "
	@echo "--- Building test object ---"
	@mkdir -p $(BUILD_DIR_TEST)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@ $(LDLIBS)


install:
	@echo "--- Installing library ---"
	@mkdir -p $(INSTALL_PATH)/include
	cp $(HEADERS) $(INSTALL_PATH)/include
	@mkdir -p $(INSTALL_PATH)/lib
	cp $(LIB_DIR)/$(DYNAMIC_LIBFILE) $(LIB_DIR)/$(STATIC_LIBFILE) $(INSTALL_PATH)/lib
	@echo "--- INSTALLED ---"


doc:
	@echo "--- Making documentation ---"
	@mkdir -p $(DOC_DIR)
	doxygen Doxyfile
	pdflatex $(DOC_DIR)/latex/refman.pdf
	@echo "--- COMPLETED ---"


clean:
	-rm -rv $(BUILD_DIR)


distclean: clean
	-rm -rv $(LIB_DIR) $(TEST_DIR) $(DOC_DIR) $(DATA_DIR)

