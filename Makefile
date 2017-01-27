# ====================================================================
#   					"BGLgeom library"
#      Course on Advanced Programming for Scientific Computing
#                     Politecnico di Milano
#                         A.Y. 2015-2016
#
#		Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
#=====================================================================

-include Makefile.inc

.PHONY: help library distclean

.DEFAULT_GOAL: help

help:
	@echo "make help: ----- Prints this help"
	@echo "make library: -- Makes BGLgeom library (static and dynamic), tests, documentation and installs it"
	@echo "make distclean:  Cleans all"

library:
	$(MAKE) -C $(BGLGEOM_DIR) all

distclean:
	$(MAKE) -C $(BGLGEOM_DIR) distclean
