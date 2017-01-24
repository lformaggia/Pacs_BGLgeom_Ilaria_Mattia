# ====================================================================
#   					"BGLgeom library"
#      Course on Advanced Programming for Scientific Computing
#                     Politecnico di Milano
#                         A.Y. 2015-2016
#=====================================================================

# Inlcuding general Makefile.in
MAKEFILEH_DIR = ../
include $(MAKEFILEH_DIR)/Makefile.inc

# Including Makefile.inc of the local folder
-include Makefile.inc

#======================================================================

.PHONY: all clean distclean help doc exec

.DEFAUL_GOAL: help

help:
	@echo "make help: ----- Prints this help"
	@echo "make all: ------ Makes executables and documentation"
	@echo "make exec: ----- Makes input network and executables"
	@echo "make doc: ------ Makes documentation"
	@echo "make input: ---- Makes input file to produce the network for the application"
	@echo "make clean: ---- Cleans all object files"
	@echo "make distclean:  Cleans all"
	@echo ""
	@echo "REMEMBER: without specifying RELEASE=yes inbetween 'make' and"
	@echo "          its target, the target will be built in debug mode"

all: exec doc clean

exec: input $(OBJS_NOT_MAIN) $(EXEC)
	@mkdir -p $(DATA_DIR)/vtk	#Folder for output
	@cp src/input.param bin/input.param

input:
	@mkdir -p $(DATA_DIR)
	octave -q $(SOURCE_DIR)/generate_bifurcation_network.m

$(TARGET_DIR)/%: $(BUILD_DIR)/%.o
	@mkdir -p $(TARGET_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(OBJS_NOT_MAIN) $< -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

doc:
	@mkdir -p $(DOC_DIR)
	doxygen Doxyfile
	$(MAKE) -C $(DOC_DIR)/latex

clean:
	-rm -rv $(BUILD_DIR)

distclean: clean
	-rm -rv $(TARGET_DIR)
	-rm -rv $(DOC_DIR)
	-rm -rv $(DATA_DIR)

