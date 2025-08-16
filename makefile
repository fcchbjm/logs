.PHONY:test
test:test.cc util.hpp
	g++ -g -std=c++11 $^ -o $@ -lpthread