#pragma once
#include <SDL3/SDL.h>

// class data {};
struct SDLStats {
  SDL_Window *window;
  SDL_Renderer *renderer;
  int height, width, logH, logW;
};
