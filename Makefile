CXX=g++

SRC=main.cpp disp.cpp prog.cpp util.cpp
OBJ=$(SRC:%.cpp=%.o)

LDFLAGS=-lGLEW -lGL -lSDL2

HDR=math.h

.PHONY: all
all: a.out

%.o: %.cpp %.h
	$(CXX) -c $< -o $@ $(LDFLAGS)

main.o: main.cpp
	$(CXX) -c $< -o $@ $(LDFLAGS)

a.out: $(OBJ) $(HDR)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm *.o a.out
