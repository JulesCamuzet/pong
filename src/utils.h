#include <SDL2/SDL.h>
#include "game.h"

#ifndef UTILS_H_
#define UTILS_H_

typedef struct coord_s {
  int x;
  int y;
} Coords;

SDL_Window * init_window();

SDL_Renderer * init_renderer(SDL_Window * window);

void set_draw_color(SDL_Renderer * renderer, int r, int g, int b, int a); 

void draw_rectangle(SDL_Renderer * renderer, int x, int y, int w, int h);

int position_valid(int y, int window_h);

Coords * calc_ball_coord(int x, int y, double angle, double current_ball_speed);

int is_out(int x, int y, int window_w, int window_h);

int is_caught(int next_x, int next_y, Game_data * g_data, int window_w);

#endif
