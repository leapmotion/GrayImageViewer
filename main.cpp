#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>

#ifdef __APPLE__
#  include <SDL.h>
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <SDL2/SDL.h>
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif


int main(int argc, char **argv){
  if (argc < 4)
    throw std::runtime_error("usage: grayviewer filename width height ex)grayviewer grayimage.raw 1280 480");

  int image_width;
  int image_height;

  std::ifstream ifs(argv[1], std::ios::binary);
  image_width = std::stoi(argv[2]);
  image_height = std::stoi(argv[3]);
  if (!ifs.good())
    throw std::runtime_error("file not found");

  std::vector<char> buffer(image_width * image_height * 3 / 2, 0x80);
  ifs.read(buffer.data(), image_width * image_height);
  ifs.close();

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return -1;
  }

  SDL_Window *window = SDL_CreateWindow("grayviewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, image_width, image_height, SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    return -1;
  }
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr) {
    std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    return -1;
  }
  SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, image_width, image_height);
  SDL_Rect sourceRect = {0, 0, image_width, image_height};
  SDL_Rect destRect = {0, 0, image_width, image_height};


  SDL_UpdateTexture(texture, &sourceRect, buffer.data(), image_width);
  SDL_RenderCopy(renderer, texture, &sourceRect, &destRect);
  SDL_RenderPresent(renderer);

  bool quit = false;
  SDL_Event e;
  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
      if (e.type == SDL_KEYDOWN) {
        quit = true;
      }
    }
  }
  SDL_Quit();
  return 0;
}
