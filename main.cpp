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
    throw std::runtime_error("usage: grayviewer filename width height");

  int image_width;
  int image_height;

  std::string image_path;
  image_path.assign(argv[1]);
  std::cout << image_path << std::endl;
  image_width = atoi(argv[2]);
  image_height = atoi(argv[3]);
  std::cout << image_width << "x" << image_height << std::endl;
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }
  SDL_Window *window = SDL_CreateWindow("grayviewer", 100, 100, image_width, image_height, SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    return 1;
  }
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr) {
    std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    return 1;
  }
  SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, image_width, image_height);
  SDL_Rect sourceRect = {0, 0, image_width, image_height};
  SDL_Rect destRect = {0, 0, image_width, image_height};

  std::ifstream infile;
  infile.open(image_path.c_str(), std::ios::binary | std::ios::in);
  std::vector<char> buffer(image_width * image_height * 3 / 2, 0x80);
  infile.read(buffer.data(), image_width * image_height);
  infile.close();

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
