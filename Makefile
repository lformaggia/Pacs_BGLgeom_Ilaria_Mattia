# ====================================================================
#   					"BGLgeom library"
#      Course on Advanced Programming for Scientific Computing
#                     Politecnico di Milano
#                         A.Y. 2015-2016
#=====================================================================

-include Makefile.inc

.PHONY: help all library doc clean distclean

.DEFAULT_GOAL: help

help:
	@echo "make help: ----- Prints this help"
	@echo "make all: ------ Makes libraries, tests, documentation and applications"
	@echo "make library: -- Makes only libraries, tests, documentation for the libraries and install them. No applications"
	@echo "make doc: ------ Makes documentation of library and applications"
	@echo "make clean: ---- Cleans all object files for library and applications"
	@echo "make distclean:  Cleans all"
	
all:
	$(MAKE) -C $(BGLGEOM_DIR) all
	$(MAKE) -C $(NETW_DIR) all
	$(MAKE) -C $(FRAC_DIR) all	

library:
	$(MAKE) -C $(BGLGEOM_DIR) all

doc:
	$(MAKE) -C $(BGLGEOM_DIR) doc
	$(MAKE) -C $(NETW_DIR) doc
	$(MAKE) -C $(FRAC_DIR) doc

clean:
	$(MAKE) -C $(BGLGEOM_DIR) clean
	$(MAKE) -C $(NETW_DIR) clean
	$(MAKE) -C $(FRAC_DIR) clean

distclean:
	$(MAKE) -C $(BGLGEOM_DIR) distclean
	$(MAKE) -C $(NETW_DIR) distclean
	$(MAKE) -C $(FRAC_DIR) distclean
