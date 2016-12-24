LIBS = $(wildcard $(mkVtkLib)/libvtk*.so.5.10.1)
LIBS2 = $(notdir $(LIBS))
LIBSSS = $(foreach lib, $(LIBS2), $(if $(findstring Python,$(lib)),,$(lib)))


LIBS3 = $(basename $(LIBSSS))
LIBS4 = $(basename $(LIBS3))
LIBS5 = $(basename $(LIBS4))
LIBS6 = $(basename $(LIBS5))
VTK_LIBS = $(LIBS6:lib%=%)



CXXFLAGS += -std=c++11
CPPFLAGS += -I. -I$(mkEigenInc) -I$(mkBGLInc) -I../include/ -I../../libBGLgeom/include -I$(mkVtkInc)

.PHONY: all clean distclean


#prova_sort_duplicates : prova_sort_duplicates.cpp prova_sort_duplicates.o 
#	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c  prova_sort_duplicates.cpp
#	$(CXX) $(CPPFLAGS) $(CXXFLAGS) prova_sort_duplicates.o -o prova_sort_duplicates
	
main_ila: helper_functions.o intersections2D.o main_ila.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -L$(mkVtkLib) $(foreach lib,$(VTK_LIBS), -l$(lib) ) helper_functions.o intersections2D.o main_ila.o -o main_ila
	
main_ila.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -L$(mkVtkLib) $(foreach lib,$(VTK_LIBS), -l$(lib) ) -c  main_ila.cpp -o $@
helper_functions.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -L$(mkVtkLib) $(foreach lib,$(VTK_LIBS), -l$(lib) ) -c  helper_functions.cpp	-o $@
intersections2D.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -L$(mkVtkLib) $(foreach lib,$(VTK_LIBS), -l$(lib) ) -c  intersections2D.cpp -o $@

	
	
clean :
	$(RM) *.o main_ila


