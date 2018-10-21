SDIR =src
SADIR=$(SDIR)/algorithms
IDIR =$(SDIR)/include
CXX = g++
CXXFLAGS = -std=c++17 -O2 -I$(IDIR)

ODIR=obj

_DEPS= algorithm-manager.hpp boyer-moore.hpp helpful-string.hpp shift-or.hpp sellers.hpp ukkonen.hpp
DEPS= $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o algorithms/boyer-moore.o algorithms/shift-or.o algorithms/sellers.o algorithms/ukkonen.o algorithm-manager.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: bin/pmt

$(OBJ): $(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	mkdir -p obj/algorithms
	$(CXX) -c -o $@ $< $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f *~ core $(IDIR)/*~ -r $(ODIR)/ bin/

bin/pmt: $(OBJ)
	mkdir -p bin
	$(CXX) -o $@ $^ $(CXXFLAGS)
