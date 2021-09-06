CXX=clang++
CXXFLAGS=-Wall -Wextra -std=c++17 -fsanitize=memory -fsanitize=undefined
test:	test.cpp typedgpio.hpp
	$(CXX) $(CXXFLAGS) -o test test.cpp
clean:
	rm -f *.o test
.PHONY:	clean
