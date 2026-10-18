#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_messagebox.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
struct SDLStats {
  SDL_Window *window;
  SDL_Renderer *renderer;
};

void Cleanup(SDLStats &stats);

int main(int argc, char **argv) {
  // intitalize game
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
                             "Error Initilizing Video", nullptr);
    return -1;
  }
  // create the window
  SDLStats state;
  int width = 800;
  int hight = 600;
  state.window = SDL_CreateWindow("Breakout", width, hight, 0);
  if (!state.window) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
                             "Failed to make Window", nullptr);
    Cleanup(state);
  }
  // create the renderer
  state.renderer = SDL_CreateRenderer(state.window, nullptr);
  if (!state.renderer) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
                             "Failed to make Renderer", nullptr);
    Cleanup(state);
  }
  // game loop
  bool gameRunning{true};
  while (gameRunning) {
    // check for events
    SDL_Event event{0};
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_EVENT_QUIT: {
        gameRunning = false;
        break;
      }
      }
    }
    // render to screen
    SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
    SDL_RenderClear(state.renderer);

    SDL_RenderPresent(state.renderer);
  }
  Cleanup(state);
}

void Cleanup(SDLStats &stats) {
  SDL_DestroyRenderer(stats.renderer);
  SDL_DestroyWindow(stats.window);
  SDL_Quit();
}
