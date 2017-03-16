# Installation of SDL2 is required.
# sudo apt-get install libsdl2-dev on ubuntu

CXX = g++
SDL_LIB = -L/usr/local/lib -lSDL2 -Wl,-rpath=/usr/local/lib
SDL_INCLUDE = -I/usr/local/include
OPENGL_LIB = -lGL -lGLU -lglut
CXXFLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE) -g

LDFLAGS = $(SDL_LIB) $(OPENGL_LIB)
EXE = grayviewer

all: $(EXE)

$(EXE): main.o
	$(CXX) $< $(LDFLAGS) -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm *.o && rm $(EXE)
