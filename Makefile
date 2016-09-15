SRCS=$(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
HEADERS = $(wildcard *.hpp)
CPPFLAGS ?= -I. -I$(mkBGLInc) -I./include
LDFLAGS ?= 
LIBS = 
exe_sources=$(filter main%.cpp,$(SRCS))
EXEC=$(exe_sources:.cpp=)

-include Makefile.inc

# $(EXEC) : $(OBJS)
#	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

# $(OBJS) : $(SRCS) $(HEADERS)
#	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

main: main.o
	$(CXX) $(LDFLAGS) main.o -o $@ $(LIBS)
main.o: main.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<
	
read_original_format : read_original_format.o
	$(CXX) $(LDFLAGS) read_original_format.o -o $@ $(LIBS)
read_original_format.o : read_original_format.cpp 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<
	
generic_point: generic_point.o
	$(CXX) $(LDFLAGS) generic_point.o -o $@ $(LIBS)
generic_point.o : generic_point.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<
	
doc:
	doxygen $(DOXYFILE)

.PHONY: clean distclean

clean :
	$(RM) $(OBJS)

distclean : clean
	$(RM) $(EXEC)
