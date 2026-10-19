#include "SDL3/SDL.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_main.h"
#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_surface.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_messagebox.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

struct SDLStats {
  SDL_Window *window;
  SDL_Renderer *renderer;
  int height, width, logH, logW;
};

bool initlize(SDLStats &stats);
void Cleanup(SDLStats &stats);

int main(int argc, char **argv) {
  // intitalize game
  SDLStats state;
  int width = 800;
  int hight = 600;
  state.window = SDL_CreateWindow("Breakout Game", width, hight, 0);
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
  // game data
  const bool *keys = SDL_GetKeyboardState(nullptr);
  float floor = state.logH;
  float playerx = state.logW / 2.0f;
  float playery = floor;
  const int spriteSize = 32;
  float moveAmount;
  float movement;
  uint64_t nowTime;
  uint64_t prevTime;
  float deltaTime;

  // assets
  SDL_Texture *paddle =
      IMG_LoadTexture(state.renderer, "data/graphics/paddle.png");
  SDL_SetTextureScaleMode(paddle, SDL_SCALEMODE_NEAREST);

  // game loop
  bool gameRunning{true};
  while (gameRunning) {
    nowTime = SDL_GetTicks();
    deltaTime = (nowTime - prevTime) / 1000.0f; // spf
    prevTime = SDL_GetTicks();
    // check for events
    SDL_Event event{0};
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_EVENT_QUIT: {
        gameRunning = false;
        break;
      }
      case SDL_EVENT_WINDOW_RESIZED: {
        state.width = event.window.data1;
        state.height = event.window.data2;
        break;
      }
      default: {
        break;
      }
      }
    }
    // move
    movement = 0;
    moveAmount = 75.0f;

    if (keys[SDL_SCANCODE_A]) {
      movement += -moveAmount;
    }
    if (keys[SDL_SCANCODE_D]) {
      movement += moveAmount;
    }
    playerx += (movement * deltaTime);

    SDL_FRect paddleSize{.x = 0, .y = 0, .w = spriteSize, .h = spriteSize};
    SDL_FRect paddleLocation{.x = playerx,
                             .y = playery - spriteSize,
                             .w = spriteSize,
                             .h = spriteSize};
    // render to screen
    SDL_SetRenderDrawColor(state.renderer, 255, 255, 0, 255);
    SDL_RenderClear(state.renderer);

    SDL_RenderTexture(state.renderer, paddle, &paddleSize, &paddleLocation);

    SDL_RenderPresent(state.renderer);
  }
  SDL_DestroyTexture(paddle);
  Cleanup(state);
}

bool initlize(SDLStats &state) {
  bool initlized{true};
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
                             "Error Initilizing Video", nullptr);
    initlized = false;
  }
  // create the window
  state.width = 800;
  state.height = 800;
  state.window = SDL_CreateWindow("Breakout", state.width, state.height,
                                  SDL_WINDOW_RESIZABLE);
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
    initlized = false;
  }
  SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
  state.logW = 640;
  state.logH = 320;

  SDL_SetRenderLogicalPresentation(state.renderer, state.logW, state.logH,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);
  return initlized;
}

void Cleanup(SDLStats &stats) {
  SDL_DestroyRenderer(stats.renderer);
  SDL_DestroyWindow(stats.window);
  SDL_Quit();
}
