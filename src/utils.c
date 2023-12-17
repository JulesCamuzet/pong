#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "utils.h"
#include "constants.h"
#include "game.h"

SDL_Window * init_window() {
  SDL_Window * window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

  if (window == NULL) {
    printf("Error window creation : %s", SDL_GetError());
    exit(1);
  }

  return window;
}

SDL_Renderer * init_renderer(SDL_Window * window) {
  SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

  if (renderer == NULL) {
    printf("Error renderer creation : %s", SDL_GetError());
    exit(1);
  }

  return renderer;
}

void set_draw_color(SDL_Renderer * renderer, int r, int g, int b, int a) {
  if (SDL_SetRenderDrawColor(renderer, r, g, b, a) != 0) {
    printf("Error set draw color : %s", SDL_GetError());
    exit(1);
  }
}

void draw_rectangle(SDL_Renderer * renderer, int x, int y, int w, int h) {
  SDL_Rect rec;

  rec.h = h;
  rec.w = w;
  rec.x = x;
  rec.y = y;

  if (SDL_RenderFillRect(renderer, &rec) != 0) {
    printf("Error drawing rectangle : %s", SDL_GetError());
    exit(1);
  }
}

int position_valid(int y, int window_h) {
  return y >= 0 && y <= window_h - PAD_HEIGHT;
}

Coords * calc_ball_coord(int x, int y, double angle, double current_ball_speed) {
  Coords * coords = malloc(sizeof(Coords));
  
  double radiant_angle = (angle * M_PI) / 180.0;

  double calc_cos = cos(radiant_angle);
  double calc_sin = sin(radiant_angle);

  coords->x = x + (int)round(calc_cos * current_ball_speed * 3);
  coords->y = y - (int)round(calc_sin * current_ball_speed * 3);

  return coords;
}

int is_out(int x, int y, int window_w, int window_h) {
  return x < 0 || x > window_w - BALL_SIZE || y < 0 || y > window_h - BALL_SIZE;
}

int is_caught(int next_x, int next_y, Game_data * g_data, int window_w) {
  int player_a_x1 = PAD_FROM_EDGE;
  int player_a_y1 = g_data->player_a_position_y;
  int player_a_x2 = PAD_FROM_EDGE + PAD_WIDTH;
  int player_a_y2 = g_data->player_a_position_y + PAD_HEIGHT;

  int player_b_x1 = window_w - PAD_FROM_EDGE - PAD_WIDTH - BALL_SIZE;
  int player_b_y1 = g_data->player_b_position_y;
  int player_b_x2 = window_w - PAD_FROM_EDGE;
  int player_b_y2 = g_data->player_b_position_y + PAD_HEIGHT;

  return ((next_x >= player_a_x1 && next_x <= player_a_x2 && next_y >= player_a_y1 && next_y <= player_a_y2) ||
  (next_x >= player_b_x1 && next_x <= player_b_x2 && next_y >= player_b_y1 && next_y <= player_b_y2));
}
