
CXXFLAGS += -std=c++11
CPPFLAGS += -I. -I$(mkEigenInc) -I$(mkBGLInc) -I../include/

.PHONY: all clean distclean


prova_sort_duplicates : prova_sort_duplicates.cpp prova_sort_duplicates.o 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c  prova_sort_duplicates.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) prova_sort_duplicates.o -o prova_sort_duplicates
	
bozza_refine_graph: bozza_refine_graph.cpp helper_functions.cpp intersections2D.cpp intersections2D_utilities.cpp helper_functions.o intersections2D.o intersections2D_utilities.o bozza_refine_graph.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c  bozza_refine_graph.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c  helper_functions.cpp	
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c  intersections2D_utilities.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c  intersections2D.cpp		
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) helper_functions.o intersections2D.o intersections2D_utilities.o bozza_refine_graph.o -o bozza_refine_graph
	
	
clean :
	$(RM) *.o prova_sort_duplicates bozza_refine_graph test_intersection


