#include <iostream>
#include <fstream>

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

int image_width = 640;
int image_height = 480;
const int bytes_per_pixel = 2;
std::string image_path;

int main(int argc, char **argv){
	if (argc < 2) {
		std::cout << "sdl_viewer rawimage" << std::endl;
		return 1;
	}
	image_path.assign(argv[1]);
	std::cout << image_path << std::endl;
	if (argc > 3) {
		image_width = atoi(argv[2]);
		image_height = atoi(argv[3]);
	}
	std::cout << image_width << "x" << image_height << std::endl;
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	SDL_Window *window = SDL_CreateWindow("sdl_viewer", 100, 100, image_width, image_height, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YUY2, SDL_TEXTUREACCESS_STREAMING, image_width, image_height);
	SDL_Rect sourceRect = {0, 0, image_width, image_height};
	SDL_Rect destRect = {0, 0, image_width, image_height};

	std::ifstream infile;
	infile.open(image_path.c_str(), std::ios::binary | std::ios::in);
	char *buffer = new char[image_width * image_height * bytes_per_pixel];
	infile.read(buffer, image_width * image_height * bytes_per_pixel);
	infile.close();

	SDL_UpdateTexture(texture, &sourceRect, buffer, image_width * bytes_per_pixel);
	SDL_RenderCopy(renderer, texture, &sourceRect, &destRect);
	SDL_RenderPresent(renderer);
	delete buffer;

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
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				quit = true;
			}
			//SDL_RenderClear(renderer);
			//SDL_RenderPresent(renderer);
		}
	}
	SDL_Quit();
	return 0;
}
