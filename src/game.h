#include "SDL2/SDL.h"

#ifndef GAME_H_
#define GAME_H_

typedef struct game_data {
  int ball_position_x;
  int ball_position_y;
  double ball_direction_angle;

  int player_a_position_y;
  int player_b_position_y;
} Game_data;

Game_data * init_game_data();

int start_game(SDL_Renderer * renderer, SDL_Window * window);

void manage_event(Game_data * g_data, SDL_Window * window, const Uint8 * keystates);

void render_game(SDL_Renderer * renderer, Game_data * g_data);

void render_player(SDL_Renderer * renderer, int y, int player_id);

void update_ball_position(Game_data * g_data, SDL_Window * window);

void render_ball(SDL_Renderer * renderer, int x, int y);

void reset_ball(Game_data * g_data);

#endif
