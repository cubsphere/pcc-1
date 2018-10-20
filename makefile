CXX = g++
CXXFLAGS = -std=c++17 -O2 -Isrc/include

SRCS = src/main.cpp src/algorithms/boyer-moore.cpp src/algorithms/shift-or.cpp

bin/pmt: $(SRCS)
	mkdir -p bin
	$(CXX) -o $@ $^ $(CXXFLAGS)