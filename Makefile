CXX=g++

EXEC=a.out

BUILDDIR=build

SRC=main.cpp disp.cpp prog.cpp util.cpp pt.cpp
HDR=math.h

OBJ=$(SRC:%.cpp=%.o)

LDFLAGS=-lSDL2 -lGLEW -lGL

.PHONY: all
all: mk_build $(EXEC) mk_o

%.o: %.cpp %.h
	$(CXX) -c $< -o $@ $(LDFLAGS)

main.o: main.cpp
	$(CXX) -c $< -o $@ $(LDFLAGS)

$(EXEC): $(OBJ) $(HDR)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

.PHONY: mk_build
mk_build:
	mkdir -p $(BUILDDIR)

.PHONY: mk_o
mk_o:
	mkdir -p o

.PHONY: clean
clean:
	rm *.o $(EXEC)
