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
	@echo "make test: ----- Makes executables for the tests"
	@echo "make install: -- Installs the libaries"
	@echo "make doc: ------ Makes documentation"
	@echo "make clean: ---- Cleans all but not not library"
	@echo "make distclean:  Cleans all"
	@echo $(TEST_EXEC)
	
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
	
$(LIB_OBJS_STATIC): | $(DIR_TO_BE_CREATED)
$(LIB_OBJS_DYNAMIC): | $(DIR_TO_BE_CREATED)
$(TEST_EXEC): | $(DIR_TO_BE_CREATED)

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

#$(TEST_DIR)/%: $(BUILD_DIR_TEST)/%.o
test: $(BUILD_DIR_TEST)/%.o
	@echo "--- Linking objects to executables ---"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS) -L$(TARGET_DIR) -l$(LIBNAME) $< -o $(TEST_DIR)/%	

$(BUILD_DIR_TEST)/%.o: $(SOURCE_DIR)/%.cpp
	@echo "--- Building test objects ---"
	@mkdir -p $(BUILD_DIR_TEST)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS) -c $< -o $@
	

#test/test_edge: $(BUILD_DIR_TEST)/test_edge.o
#	$(CXX) $< -o $@





clean:
	-rm -rv $(BUILD_DIR)

distclean: clean
	-rm -rv $(TARGET_DIR) $(TEST_DIR) $(DOC_DIR)

