#g++ -std=c++14 -lSDL2main -lSDL2 Raytracer.cpp

g++ Raytracer.cpp $(pkg-config --cflags --libs sdl2) -o Raytracer
