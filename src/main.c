#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "utils.h"
#include "game.h"

int main() {
  SDL_Window * window = init_window();

  SDL_Renderer * renderer = init_renderer(window);

  start_game(renderer, window);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
} 
