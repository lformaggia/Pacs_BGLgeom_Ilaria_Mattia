
-include Makefile.inc

# $(EXEC) : $(OBJS)
#	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

# $(OBJS) : $(SRCS) $(HEADERS)
#	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

.PHONY: all clean distclean doc Zunino Formaggia test

all: doc Zunino Formaggia test

test: test_generic_point test_graph_pointer test_muparser

###################
# parte aggiunta per prova edge: da cancellare quando ho finito
test_edge: test_edge.o
	$(CXX) $(LDFLAGS) $(WARNS) $(BUILD_DIR)/test_edge.o -o $(TARGET_DIR)/$@ $(LIBS)
test_edge.o: ./$(SOURCE_DIR)/test_edge.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(WARNS) -c $< -o $(BUILD_DIR)/$@

###################

test_muparser: test_muparser.o
	$(CXX) $(LDFLAGS) $(WARNS) $(BUILD_DIR)/test_muparser.o -o $(TARGET_DIR)/$@ $(LIBS)
test_muparser.o: ./$(SOURCE_DIR)/test_muparser.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(WARNS) -c $< -o $(BUILD_DIR)/$@

test_generic_point: test_generic_point.o
	$(CXX) $(LDFLAGS) $(WARNS) $(BUILD_DIR)/test_generic_point.o -o $(TARGET_DIR)/$@ $(LIBS) 
test_generic_point.o: ./$(SOURCE_DIR)/test_generic_point.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(WARNS) -c $< -o $(BUILD_DIR)/$@
	
test_graph_pointer: test_graph_pointer.o
	$(CXX) $(LDFLAGS) $(WARNS) $(BUILD_DIR)/test_graph_pointer.o -o $(TARGET_DIR)/$@ $(LIBS)
test_graph_pointer.o: ./$(SOURCE_DIR)/test_graph_pointer.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(WARNS) -c $< -o $(BUILD_DIR)/$@

Zunino: main_Zunino

main_Zunino: main_Zunino.o
	$(CXX) $(LDFLAGS) $(WARNS) $(BUILD_DIR)/main_Zunino.o -o $(TARGET_DIR)/$@ $(LIBS)
main_Zunino.o: ./$(SOURCE_DIR)/main_Zunino.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(WARNS) -c $< -o $(BUILD_DIR)/$@
	
Formaggia: main_Formaggia

main_Formaggia: main_Formaggia.o
	$(CXX) $(LDFLAGS) $(WARNS) $(BUILD_DIR)/main_Formaggia.o -o $(TARGET_DIR)/$@ $(LIBS)
main_Formaggia.o: ./$(SOURCE_DIR)/main_Formaggia.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(WARNS) -c $< -o $(BUILD_DIR)/$@
	
doc:
	doxygen $(DOXYFILE)
	make -C $(LATEX_DOC_DIR)
# make -C per chiamare make nella cartella indicata

clean :
	rm -rv $(OBJS)	

distclean : clean
	rm -rv $(EXEC)
