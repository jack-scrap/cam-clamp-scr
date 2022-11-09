CXX=g++

EXEC=a.out

SRC=main.cpp disp.cpp prog.cpp util.cpp pt.cpp
HDR=math.h

OBJ=$(SRC:%.cpp=%.o)

LDFLAGS=-lSDL2 -lGLEW -lGL

.PHONY: all
all: mk_build $(EXEC)

%.o: %.cpp %.h
	$(CXX) -c $< -o $@ $(LDFLAGS)

main.o: main.cpp
	$(CXX) -c $< -o $@ $(LDFLAGS)

$(EXEC): $(OBJ) $(HDR)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

.PHONY: mk_build
mk_build:
	mkdir -p build

.PHONY: clean
clean:
	rm *.o $(EXEC)
