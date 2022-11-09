CXX=g++

EXEC=a.out

SRC=main.cpp disp.cpp prog.cpp util.cpp
OBJ=$(SRC:%.cpp=%.o)

LDFLAGS=-lSDL2 -lGLEW -lGL

HDR=math.h

.PHONY: all
all: $(EXEC)

%.o: %.cpp %.h
	$(CXX) -c $< -o $@ $(LDFLAGS)

main.o: main.cpp
	$(CXX) -c $< -o $@ $(LDFLAGS)

$(EXEC): $(OBJ) $(HDR)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm *.o $(EXEC)
