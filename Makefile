CXX=g++

SRC=main.cpp disp.cpp prog.cpp util.cpp
OBJ=$(patsubst %.cpp, %.o, $(SRC))

GL=-lGLEW -lGL
SDL=-lSDL2
LDFLAGS+=$(SDL)
LDFLAGS+=$(GL)

HDR=math.h

.PHONY: all make clean

all: make

%.o: %.cpp %.h
	$(CXX) -c $< -o $@ $(LDFLAGS)

main.o: main.cpp
	$(CXX) -c $< -o $@ $(LDFLAGS)

make: $(OBJ) $(HDR)
	$(CXX) $^ $(LDFLAGS)

clean:
	rm *.o a.out
