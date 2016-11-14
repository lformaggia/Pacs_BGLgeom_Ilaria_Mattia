CXX=clang++
CXXFLAGS=-std=c++11 -g
LDFLAGS=-g
LINK.o = $(CXX) $(LDFLAGS) $(TARGET_ARCH)
test:test.o SegmentIntersect.o

test.o:test.cpp SegmentIntersect.hpp 

clean:
	$(RM) *.o test
