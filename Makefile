CXX=g++

SRC=main.cpp disp.cpp prog.cpp util.cpp
OBJ=$(patsubst %.cpp, %.o, $(SRC))

LDFLAGS=-lGLEW -lGL -lSDL2

HDR=math.h

.PHONY: all
all: make

%.o: %.cpp %.h
	$(CXX) -c $< -o $@ $(LDFLAGS)

main.o: main.cpp
	$(CXX) -c $< -o $@ $(LDFLAGS)

.PHONY: make
make: $(OBJ) $(HDR)
	$(CXX) $(OBJ) $(LDFLAGS) -o a.out

.PHONY: clean
clean:
	rm *.o a.out
