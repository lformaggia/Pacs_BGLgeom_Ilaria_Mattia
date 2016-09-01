-include Makefile.inc

prova: prova1.o
	$(CXX) $(CXXFLAGS) $(CPPFLAGS)  prova1.o -o prova
	
prova1.o: prova1.cpp prova_output.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(WARNS) -c prova1.cpp
