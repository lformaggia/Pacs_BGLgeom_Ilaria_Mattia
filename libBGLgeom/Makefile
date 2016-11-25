# Including project Makefile.in
MAKEFILEH_DIR=../
include $(MAKEFILE_DIR)/Makefile.inc

# Including Makefile.in of the local folder
-include Makefile.inc

LIBNAME = BGLgeom
DYNAMIC_LIBFILE = lib$(LIBNAME).so
STATIC_LIBFILE = lib$(LIBNAME).a



.PHONY: all static dynamic clean distclean

all: static dynamic

static: $(OBJS)
	@echo "--- Building static library ---"
	ar -r -s $(TARGET_DIR)/libBGLgeom.a $(OBJS)
	ranlib $(TARGET_DIR)/libBGLgeom.a
	
$(OBJS): $(SRCS) $(HEADERS)
	g++ -std=c++11 -I$(INCLUDE_DIR) -I$(mkEigenInc) -c $< -o $@
	
dynamic: $(OBJS_DYN)
	@echo "--- Building dynamic library ---"
	g++ -shared -Wl,-soname,libBGLgeom.so.1 -o $(TARGET_DIR)/libBGLgeom.so.1.0 $(OBJS_DYN)
	
$(OBJS_DYN): $(SRCS) $(HEADERS)
	g++ -std=c++11 -Wall -fPIC -I$(INCLUDE_DIR) -I$(mkEigenInc) -c $< -o $@

clean:
	rm -rv $(OBJS)

distclean: clean
	rm -rv $(LIBS)

