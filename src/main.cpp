#include "SDL3/SDL.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_main.h"
#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_surface.h"
#include "SDL3/SDL_time.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_messagebox.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cmath>
#include <cwchar>
#include <initializer_list>
#include <numbers>
#include <string>

struct SDLStats {
  SDL_Window *window;
  SDL_Renderer *renderer;
  TTF_Font *font;
  int height, width, logH, logW;
};

bool initlize(SDLStats &stats);
void Cleanup(SDLStats &stats);

int main(int argc, char **argv) {
  // intitalize game
  SDLStats state;
  if (!initlize(state)) {
    return -1;
  }
  // game data
  const bool *keys = SDL_GetKeyboardState(nullptr);
  const int spriteSize = 32;
  const int ballSpriteSize = spriteSize / 4;
  float ballRadius = 4;
  float floor = state.logH;
  float paddlex = state.logW / 2.0f;
  float paddley = floor;
  float paddleHight = 4;
  float ballx = paddlex + spriteSize / 2 - ballRadius;
  float bally = state.logH - spriteSize - paddleHight / 2 - ballRadius;

  float paddleMoveAmount = 75.0f;
  float paddleMovement;
  float ballMoveAmount = +100.0f;
  float ballMoveAngle = 0.5;

  uint64_t nowTime;
  uint64_t prevTime;
  float deltaTime = 0;

  // assets
  SDL_Texture *paddle =
      IMG_LoadTexture(state.renderer, "data/graphics/paddle.png");
  SDL_SetTextureScaleMode(paddle, SDL_SCALEMODE_NEAREST);
  SDL_Texture *ball = IMG_LoadTexture(state.renderer, "data/graphics/ball.png");
  SDL_SetTextureScaleMode(ball, SDL_SCALEMODE_NEAREST);
  // game loop
  bool ballStart{false};
  bool gameRunning{true};
  SDL_Time commputerTime;
  SDL_GetCurrentTime(&commputerTime);
  srand(commputerTime);
  bool gameOver{false};
  bool won;
  while (gameRunning) {
    nowTime = SDL_GetTicks();
    deltaTime = (nowTime - prevTime) / 1000.0f;
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
    if (!gameOver) {
      paddleMovement = 0;

      if (keys[SDL_SCANCODE_A]) {
        paddleMovement += -paddleMoveAmount;
      }
      if (keys[SDL_SCANCODE_D]) {
        paddleMovement += paddleMoveAmount;
      }
      if (keys[SDL_SCANCODE_W]) {
        if (!ballStart) {
          // ballMoveAngle += ((std::rand() % 6) - 3.0f) / 100.0f;
        }
        ballStart = true;
      }
      paddlex += (paddleMovement * deltaTime);
      if (paddlex < 0) {
        paddlex = 0;
      }
      if (paddlex + spriteSize > state.logW) {
        paddlex = state.logW - spriteSize;
      }
      if (ballStart) {
        ballx -= (std::cos(ballMoveAngle * std::numbers::pi) * ballMoveAmount) *
                 deltaTime;
        bally -= (std::sin(ballMoveAngle * std::numbers::pi) * ballMoveAmount) *
                 deltaTime;
        if (ballx < 0) {
          ballx = std::abs(ballx);
          ballMoveAngle = 1 - ballMoveAngle;
        }
        if (ballx > (state.logW - ballSpriteSize)) {
          ballx = 2 * (state.logW - ballSpriteSize) - ballx;
          ballMoveAngle = 1 - ballMoveAngle;
        }
        if (bally < 0) {
          bally = std::abs(bally);
          ballMoveAngle = ballMoveAngle * -1;
        }
        if (bally > state.logH) {
          gameOver = true;
          won = false;
        }
        if (bally + ballSpriteSize >= paddley - spriteSize / 2 - 2 &&
            ballx + ballSpriteSize >= paddlex &&
            ballx <= paddlex + spriteSize) {
          ballMoveAngle = ballMoveAngle * -1;
          bally =
              (paddley - spriteSize / 2 - 2) * 2 - (bally + 2 * ballSpriteSize);
        }
      } else {
        ballx = paddlex + spriteSize / 2 - ballRadius;
      }

      SDL_FRect paddleSize{.x = 0, .y = 0, .w = spriteSize, .h = spriteSize};
      SDL_FRect paddleLocation{.x = paddlex,
                               .y = paddley - spriteSize,
                               .w = spriteSize,
                               .h = spriteSize};
      SDL_FRect ballSize{.x = ballSpriteSize + ballRadius,
                         .y = ballSpriteSize + ballRadius,
                         .w = ballSpriteSize,
                         .h = ballSpriteSize};
      SDL_FRect ballLocation{
          .x = ballx, .y = bally, .w = ballSpriteSize, .h = ballSpriteSize};
      // render to screen
      SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
      SDL_RenderClear(state.renderer);

      SDL_RenderTexture(state.renderer, paddle, &paddleSize, &paddleLocation);
      SDL_RenderTexture(state.renderer, ball, &ballSize, &ballLocation);

    } else {
      if (won) {
        SDL_SetRenderDrawColor(state.renderer, 50, 100, 75, 255);
        SDL_RenderClear(state.renderer);
      } else {
        SDL_SetRenderDrawColor(state.renderer, 200, 50, 50, 255);
        SDL_RenderClear(state.renderer);
      }
      if (keys[SDL_SCANCODE_RETURN]) {
        gameOver = false;
        gameRunning = true;
        ballStart = false;
        paddlex = state.logW / 2.0f;
        paddley = floor;
        paddleHight = 4;
        ballx = paddlex + spriteSize / 2 - ballRadius;
        bally = state.logH - spriteSize - paddleHight / 2 - ballRadius;

        paddleMoveAmount = 75.0f;
        ballMoveAmount = +100.0f;
        ballMoveAngle = 0.5;
      }
    }
    SDL_RenderPresent(state.renderer);
  }
  SDL_DestroyTexture(paddle);
  SDL_DestroyTexture(ball);
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
                                  SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
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

  // state.font = TTF_Init();
  //  state.font = SDL_OpenFont(, 80)

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
  // TTF_Quit();
  SDL_Quit();
}
