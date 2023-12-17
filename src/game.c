#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include "SDL2/SDL.h"

#include "utils.h"
#include "game.h"
#include "constants.h"

int x_player_a = PAD_FROM_EDGE;
int x_player_b = WINDOW_WIDTH - PAD_FROM_EDGE - PAD_WIDTH;

double current_ball_speed = BALL_SPEED;

int start_game(SDL_Renderer * renderer, SDL_Window * window) {
  int quit = 0;

  SDL_Event event;

  Game_data * g_data = init_game_data();

  const Uint8 * keystates = SDL_GetKeyboardState(NULL);

  while (!quit) {
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      }
    }
    manage_event(g_data, window, keystates);

    update_ball_position(g_data, window);
    render_game(renderer, g_data);

    SDL_Delay(DELAY);
  }
  
  return 0;
}

Game_data * init_game_data() {
  Game_data * g_data = malloc(sizeof(Game_data));

  g_data->ball_direction_angle = rand() % (360 + 1);
  g_data->ball_position_x = WINDOW_WIDTH / 2 - BALL_SIZE / 2;
  g_data->ball_position_y = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;
  g_data->player_a_position_y = WINDOW_HEIGHT / 2 - PAD_HEIGHT / 2;
  g_data->player_b_position_y = WINDOW_HEIGHT / 2 - PAD_HEIGHT / 2;

  return g_data;
}

void manage_event(Game_data * g_data, SDL_Window * window, const Uint8 * keystates) {
  int window_h;
  SDL_GetWindowSize(window, NULL, &window_h);

  SDL_PumpEvents();

  int new_y;

  if (keystates[SDL_SCANCODE_W]) {
    new_y = g_data->player_a_position_y - PAD_SPEED;
    if (position_valid(new_y, window_h)) g_data->player_a_position_y = new_y;
  }

  if (keystates[SDL_SCANCODE_S]) {
    new_y = g_data->player_a_position_y + PAD_SPEED;
    if (position_valid(new_y, window_h)) g_data->player_a_position_y = new_y;
  }

  if (keystates[SDL_SCANCODE_I]) {
    new_y = g_data->player_b_position_y - PAD_SPEED;
    if (position_valid(new_y, window_h)) g_data->player_b_position_y = new_y;
  }

  if (keystates[SDL_SCANCODE_K]) {
    new_y = g_data->player_b_position_y + PAD_SPEED;
    if (position_valid(new_y, window_h)) g_data->player_b_position_y = new_y;
  }

  if (keystates[SDL_SCANCODE_R]) {
    reset_ball(g_data);
  }
}


void render_player(SDL_Renderer * renderer, int x, int y) {
  draw_rectangle(renderer, x, y, PAD_WIDTH, PAD_HEIGHT);
}

void render_ball(SDL_Renderer * renderer, int x, int y) {
  draw_rectangle(renderer, x, y, BALL_SIZE, BALL_SIZE);
}

void render_game(SDL_Renderer * renderer, Game_data * g_data) {
  // Clear the screen
  set_draw_color(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  set_draw_color(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

  // Render players
  render_player(renderer, x_player_a, g_data->player_a_position_y);
  render_player(renderer, x_player_b, g_data->player_b_position_y);

  // Render ball
  render_ball(renderer, g_data->ball_position_x, g_data->ball_position_y);

  SDL_RenderPresent(renderer);
}

void update_ball_position(Game_data * g_data, SDL_Window * window) {
  Coords * new_ball_coords = calc_ball_coord(g_data->ball_position_x, g_data->ball_position_y, g_data->ball_direction_angle, current_ball_speed);

  int window_w;
  int window_h;
  int window_pos_x;
  int window_pos_y;

  SDL_GetWindowSize(window, &window_w, &window_h);
  SDL_GetWindowPosition(window, &window_pos_x, &window_pos_y);

  if (is_caught(new_ball_coords->x, new_ball_coords->y, g_data, window_w)) {
    int catcher_y;
    int catcher_id;
    double new_angle;
    int middle_ball_y = g_data->ball_position_y + BALL_SIZE / 2;
    if (g_data->ball_position_x < window_w / 2) { // player a caught
      catcher_y = g_data->player_a_position_y;
      catcher_id = 1;
    } else {  // player b caught
      catcher_y = g_data->player_b_position_y;
      catcher_id = 2;
    }

    int middle_pad_y = catcher_y + PAD_HEIGHT / 2;

    double coeff;

    if (middle_ball_y < middle_pad_y) { // ball bounce on the top of the pad
      coeff = (double)(middle_pad_y - middle_ball_y) / (double)(PAD_HEIGHT / 2);
      new_angle = 90.0 * coeff;
    } else if (middle_ball_y > middle_pad_y) {  // ball bounce on the bottome of the pad
      coeff = (double)(middle_ball_y - middle_pad_y) / (double)(PAD_HEIGHT / 2);
      new_angle = - 90.0 * coeff;
    } else {  // ball bounce on the middle
      coeff = 0.0;
      new_angle = 0.0;
    }

    if (coeff <= 0.2 && coeff >= -0.2) {
      current_ball_speed = current_ball_speed + BALL_TURBO;
    } else {
      if (current_ball_speed > BALL_SPEED) current_ball_speed = current_ball_speed - BALL_TURBO;
    }

    if (catcher_id == 2) {
      new_angle = 180.0 - new_angle;
    }

    g_data->ball_direction_angle = new_angle;

  }

  if (is_out(new_ball_coords->x, new_ball_coords->y, window_w, window_h)) {
    if (new_ball_coords->x < 0 || new_ball_coords->x > window_w - BALL_SIZE) {

      current_ball_speed = BALL_SPEED;

      reset_ball(g_data);

    } else if (new_ball_coords->y < 0 || new_ball_coords->y > window_h - BALL_SIZE) {

      g_data->ball_direction_angle = -g_data->ball_direction_angle;

    }

    new_ball_coords = calc_ball_coord(g_data->ball_position_x, g_data->ball_position_y, g_data->ball_direction_angle, current_ball_speed);
  }

  g_data->ball_position_x = new_ball_coords->x;
  g_data->ball_position_y = new_ball_coords->y;
}

void reset_ball(Game_data * g_data) {
  current_ball_speed = BALL_SPEED;
  g_data->ball_direction_angle = rand() % (360 + 1);
  g_data->ball_position_x = WINDOW_WIDTH / 2 - BALL_SIZE / 2;
  g_data->ball_position_y = WINDOW_HEIGHT / 2 - BALL_SIZE / 2; 
}
