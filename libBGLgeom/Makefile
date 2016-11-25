

-include Makefile.inc

.PHONY: all static dynamic

all: static dynamic

static: $(OBJS) $(HEADERS)
	ar -r -s libBGLgeom.a $(OBJS)
	ranlib libBGLgeom.a
	
$(OBJS): $(SRCS) $(HEADERS)
	g++ -std=c++11 -I$(INCLUDE_DIR) -I$(mkEigenInc) -c $< -o $@
	
$(HEADERS):
	g++ -std=c++11 -I$(INCLUDE_DIR) -I$(mkEigenInc) -c $< -o $@
	
dynamic:
	
