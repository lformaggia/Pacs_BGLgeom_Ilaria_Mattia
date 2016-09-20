
-include Makefile.inc

# $(EXEC) : $(OBJS)
#	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

# $(OBJS) : $(SRCS) $(HEADERS)
#	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

.PHONY: all clean distclean doc Zunino Formaggia test

all: doc Zunino Formaggia test

test: test_generic_point

test_generic_point: test_generic_point.o
	$(CXX) $(LDFLAGS) $(BUILD_DIR)/test_generic_point.o -o $(TARGET_DIR)/$@ $(LIBS)
test_generic_point.o: ./$(SOURCE_DIR)/test_generic_point.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/$@

Zunino: main_Zunino

main_Zunino: main_Zunino.o
	$(CXX) $(LDFLAGS) $(BUILD_DIR)/main_Zunino.o -o $(TARGET_DIR)/$@ $(LIBS)
main_Zunino.o: ./$(SOURCE_DIR)/main_Zunino.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/$@
	
Formaggia: main_Formaggia

main_Formaggia: main_Formaggia.o
	$(CXX) $(LDFLAGS) $(BUILD_DIR)/main_Formaggia.o -o $(TARGET_DIR)/$@ $(LIBS)
main_Formaggia.o: ./$(SOURCE_DIR)/main_Formaggia.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/$@
	
doc:
	doxygen $(DOXYFILE)

clean :
	rm -rv $(OBJS)	

distclean : clean
	rm -rv $(EXEC)
