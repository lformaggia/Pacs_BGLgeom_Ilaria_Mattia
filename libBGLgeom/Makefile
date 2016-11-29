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

.PHONY: all clean distclean doc static dynamic depend install exec library

.DEFAULT_GOAL: all

all:
	@echo "make static: --- makes static library and executable"
	@echo "make dynamic: -- makes dynamic library and executable"
	@echo "make depend: --- just makes dependency file"
	@echo "make clean: ---- cleans all but not not library"
	@echo "make distclean:  cleans all"
	@echo "make install: -- installs"
	@echo "make doc: ------ makes documentation"

static:	$(LIB_OBJS)
	@echo "--- Building static library ---"
	#@echo $(LIB_SRCS)
	ar -r -s $(STATIC_LIB) $(LIB_OBJS)
	ranlib $(STATIC_LIB)
	
$(LIB_OBJS): | $(DIR_TO_BE_CREATED)

$(DIR_TO_BE_CREATED):
	@mkdir -p $(DIR_TO_BE_CREATED)

dynamic: $(LIB_OBJS)
	@echo "--- Building dynamic library ---"
	$(CXX) -shared -Wl,-soname,$(DYNAMIC_LIBFILE).1 $(LIB_OBJS) -o $(DYNAMIC_LIB).1.0

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(INCLUDE_DIR)/%.hpp
	@echo "--- Building objects ---"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
	
library: static dynamic







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

