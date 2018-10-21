SDIR =src
SADIR=$(SDIR)/algorithms
IDIR =$(SDIR)/include
CXX = g++
CXXFLAGS = -std=c++17 -O2 -I$(IDIR)

ODIR=obj

_DEPS= algorithm-manager.h boyer-moore.h helpful-string.h shift-or.h
DEPS= $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o algorithms/boyer-moore.o algorithms/shift-or.o algorithm-manager.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: bin/pmt

$(OBJ): $(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	mkdir -p obj/algorithms
	$(CXX) -c -o $@ $< $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~

bin/pmt: $(OBJ)
	mkdir -p bin
	$(CXX) -o $@ $^ $(CXXFLAGS)
