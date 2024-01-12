// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

#ifndef SCENE_GAME_H
#define SCENE_GAME_H
#include "game.h"

Scene scene_main_create(void);
void game_update_scoreboard(int score);
/*
int64_t get_power_up_timer_tick();
int64_t get_power_up_duration();
*/

int get_power_up_duration();
ALLEGRO_TIMER* get_power_up_timer();

#endif
