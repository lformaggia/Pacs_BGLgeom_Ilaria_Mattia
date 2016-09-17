
-include Makefile.inc

# $(EXEC) : $(OBJS)
#	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

# $(OBJS) : $(SRCS) $(HEADERS)
#	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

.PHONY: all clean distclean Zunino Formaggia

all: Zunino Formaggia

Zunino: main_Zunino

main_Zunino: main_Zunino.o
	$(CXX) $(LDFLAGS) $(BUILD_DIR)/main_Zunino.o -o $(TARGET_DIR)/$@ $(LIBS)
main_Zunino.o: ./$(SOURCE_DIR)/main_Zunino.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/$@
	
Formaggia: main_Formaggia

main_Formaggia: main_Formaggia.o
	$(CXX) $(LDFLAGS) $(BUILD_DIR)/main_Formaggia.o -o $(TARGET_DIR)/$@ $(LIBS)
main_Formaggia.o: main_Formaggia.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/$@
	
doc:
	doxygen $(DOXYFILE)

clean :
	rm -rv $(OBJS)	

distclean : clean
	rm -rv $(EXEC)
