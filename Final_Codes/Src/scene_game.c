#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "shared.h"
#include "utility.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "pacman_obj.h"
#include "ghost.h"
#include "map.h"
#include "scene_menu_object.h"


// TODO-HACKATHON 2-0: Create one ghost (done)
// Just modify the GHOST_NUM to 1
#define GHOST_NUM 4
// TODO-GC-ghost: create a least FOUR ghost! (done)
/* global variables*/
extern const uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
int game_main_Score = 0;
bool game_over = false;
bool debug_mode = false;
bool cheat_mode = false;
bool ghost_go_back = false;
bool ghost_stop = false;
bool pacman_cross_wall = false;
bool PvP = false;

/* Internal variables*/
static ALLEGRO_TIMER* power_up_timer;
static const int power_up_duration = 10;
static Pacman* pman;
static Map* basic_map;
static Ghost** ghosts;
int controlling_ghost = 0;
bool need_input_name = false;
Button btnInputBox, btnInputNameOK, btnInputNameCancel;
char input_name[25] = "";
int input_size = 0;

/* Declare static function prototypes */
static void init(void);
static void step(void);
static void checkItem(void);
static void status_update(void);
static void update(void);
static void draw(void);
static void printinfo(void);
static void destroy(void);
static void on_key_down(int key_code);
static void on_mouse_down(int btn, int x, int y, int dz);
static void render_init_screen(void);
static void draw_hitboxes(void);

static void init(void) {
	game_over = false;
	input_name[0] = '\0';
	input_size = 0;
	game_main_Score = 0;
	// create map
	// basic_map = create_map(NULL);
	// TODO-GC-read_txt: Create map from .txt file so that you can design your own map!! (done)
	basic_map = create_map("Assets/map_nthu.txt");
	if (!basic_map) {
		game_abort("error on creating map");
	}	
	// create pacman
	pman = pacman_create();
	if (!pman) {
		game_abort("error on creating pacamn\n");
	}
	
	// allocate ghost memory
	// TODO-HACKATHON 2-1: Allocate dynamic memory for ghosts array. (done)
	ghosts = (Ghost**)malloc(sizeof(Ghost*) * GHOST_NUM);
	//
	if(!ghosts){
		game_log("We haven't create any ghosts!\n");
	}
	else {
		// TODO-HACKATHON 2-2: create a ghost. (done)
		// Try to look the definition of ghost_create in ghost.h and figure out what should be placed here.
		for (int i = 0; i < GHOST_NUM; i++) {
			game_log("creating ghost %d\n", i);
			ghosts[i] = ghost_create(i);  
			if (!ghosts[i])
				game_abort("error creating ghost\n");
		}
	}
	GAME_TICK = 0;


	render_init_screen();
	power_up_timer = al_create_timer(1.0f); // 1 tick per second
	if (!power_up_timer)
		game_abort("Error on create timer\n");

	if (PvP) {
		ghosts[0]->controlled = true;
	}

	btnInputBox = button_create(SCREEN_W / 2 - 220, SCREEN_H / 2 - 30, 440, 40, "./Assets/frame1.png", "./Assets/frame2.png");
	btnInputNameOK = button_create(SCREEN_W / 2 - 220, SCREEN_H / 2 + 60, 200, 40, "./Assets/frame1.png", "./Assets/frame2.png");
	btnInputNameCancel = button_create(SCREEN_W / 2 + 20, SCREEN_H / 2 + 60, 200, 40, "./Assets/frame1.png", "./Assets/frame2.png");
	return ;
}

static void step(void) {
	if (pman->objData.moveCD > 0)
		pman->objData.moveCD -= pman->speed;
	for (int i = 0; i < GHOST_NUM; i++) {
		// important for movement
		if (ghosts[i]->objData.moveCD > 0)
			ghosts[i]->objData.moveCD -= ghosts[i]->speed;
	}
}
static void checkItem(void) {
	int Grid_x = pman->objData.Coord.x, Grid_y = pman->objData.Coord.y;
	if (Grid_y >= basic_map->row_num - 1 || Grid_y <= 0 || Grid_x >= basic_map->col_num - 1 || Grid_x <= 0)
		return;
	// TODO-HACKATHON 1-3: check which item you are going to eat and use `pacman_eatItem` to deal with it. (done)

	bool eat = false;

	switch (basic_map->map[Grid_y][Grid_x])
	{
	case '.':
		pacman_eatItem(pman, '.');
		eat = true;
		basic_map->beansCount--;
		break;
	case 'P':
		// TODO-GC-PB: ease power bean (done)
		eat = true;
		pacman_eatItem(pman, 'P');
		// stop and reset power_up_timer value
		// start the timer

		for (int i = 0; i < GHOST_NUM; i++) {
			ghost_toggle_FLEE(ghosts[i], true);
		}

		if (!al_get_timer_started(power_up_timer)) {
			al_start_timer(power_up_timer);
			al_set_timer_count(power_up_timer, 0);
		}
		else {
			al_set_timer_count(power_up_timer, 0);
		}
		break;
	default:
		break;
	}
	
	// TODO-HACKATHON 1-4: erase the item you eat from map (done)
	// Be careful, don't erase the wall block.
	if (eat)
		basic_map->map[Grid_y][Grid_x] = ' ';
}
static void status_update(void) {
	if (basic_map->beansCount == 0) {
		game_over = true;
		return;
	}


	// TODO-PB: check powerUp duration (done)
	if (pman->powerUp)
	{
		// Check the value of power_up_timer
		// If runs out of time reset all relevant variables and ghost's status
		// hint: ghost_toggle_FLEE
		if (al_get_timer_count(power_up_timer) >= power_up_duration) {
			pman->powerUp = false;
			al_stop_timer(power_up_timer);
			for (int i = 0; i < GHOST_NUM; i++) {
				if (ghosts[i]->status == FLEE) {
					ghost_toggle_FLEE(ghosts[i], false);
				}
			}
		}
	}


	for (int i = 0; i < GHOST_NUM; i++) {
		if (ghosts[i]->status == GO_IN){
			continue;
		}
		else if (ghosts[i]->status == FREEDOM)
		{
			// TODO-GC-game_over: use `getDrawArea(..., GAME_TICK_CD)` and `RecAreaOverlap(..., GAME_TICK_CD)` functions to detect if pacman and ghosts collide with each other. (done)
			// And perform corresponding operations.
			// [NOTE] You should have some if-else branch here if you want to implement power bean mode.
			// Uncomment Following Code
			const RecArea pmanHB = getDrawArea((object *)pman, GAME_TICK_CD);
			const RecArea ghostHB = getDrawArea((object *)ghosts[i], GAME_TICK_CD);
			if(!cheat_mode && RecAreaOverlap(&pmanHB, &ghostHB)) {
				game_log("collide with ghost\n");
				al_rest(1.0);
				pacman_die();
				game_over = true;
				break; // animation shouldn't be trigger twice.
			}
		}
		else if (ghosts[i]->status == FLEE)
		{
			// TODO-GC-PB: if ghost is collided by pacman, it should go back to the room immediately and come out after a period.
			const RecArea pmanHB = getDrawArea((object *)pman, GAME_TICK_CD);
			const RecArea ghostHB = getDrawArea((object *)ghosts[i], GAME_TICK_CD);
			if(!cheat_mode && RecAreaOverlap(&pmanHB, &ghostHB)) { 
				ghost_collided(ghosts[i]);
			}
		}
	}
}

static void update(void) {

	if (game_over) {
		// TODO-GC-game_over: start pman->death_anim_counter and schedule a game-over event (e.g change scene to menu) after Pacman's death animation finished (done)
		// hint: refer al_get_timer_started(...), al_get_timer_count(...), al_stop_timer(...), al_rest(...)
		// start the timer if it hasn't been started.
		// check timer counter.
		// stop the timer if counter reach desired time.
		if (!al_get_timer_started(pman->death_anim_counter)) {
			pman->death_anim_counter = al_create_timer(1.0f / 8.0f);
			if (!pman->death_anim_counter)
				game_abort("Error on create timer\n");
			al_start_timer(pman->death_anim_counter);
		}

		if (al_get_timer_count(pman->death_anim_counter) >= 24) {
			al_stop_timer(pman->death_anim_counter);
			// al_rest(0.3);

			// TODO-Advance: 輸入名字
			need_input_name = true;
			game_over = false;
			return;


			game_change_scene(scene_menu_create());
			return;
		}
		return;
	}

	if (need_input_name)
		return;

	step();
	checkItem();
	status_update();
	pacman_move(pman, basic_map);
	for (int i = 0; i < GHOST_NUM; i++) 
		ghosts[i]->move_script(ghosts[i], basic_map, pman);
}

static void draw(void) {

	al_clear_to_color(al_map_rgb(0, 0, 0));

	
	// TODO-GC-scoring: Draw scoreboard, something your may need is sprinf(); 
	char score[20];
	sprintf(score, "Score: %d", game_main_Score);
	al_draw_text(menuFont, al_map_rgb(255, 255, 0),
		100, 20, ALLEGRO_ALIGN_CENTER, score);

	// draw cheatmode status
	if (cheat_mode) {
		al_draw_text(menuFont, al_map_rgb(255, 255, 0),
			50, 700, ALLEGRO_ALIGN_LEFT, "Cheat Mode: ON");
	}
	else {
		al_draw_text(menuFont, al_map_rgb(255, 255, 0),
			50, 700, ALLEGRO_ALIGN_LEFT, "Cheat Mode: OFF");
	}

	// draw ghost go back status
	if (ghost_go_back) {
		al_draw_text(menuFont, al_map_rgb(255, 255, 0),
			50, 750, ALLEGRO_ALIGN_LEFT, "Ghost Go Back: ON");
	}
	else {
		al_draw_text(menuFont, al_map_rgb(255, 255, 0),
			50, 750, ALLEGRO_ALIGN_LEFT, "Ghost Go Back: OFF");
	}

	// draw ghost stop status
	if (ghost_stop) {
		al_draw_text(menuFont, al_map_rgb(255, 255, 0),
			SCREEN_W - 50, 700, ALLEGRO_ALIGN_RIGHT, "Ghost Stop: ON");
	}
	else {
		al_draw_text(menuFont, al_map_rgb(255, 255, 0),
			SCREEN_W - 50, 700, ALLEGRO_ALIGN_RIGHT, "Ghost Stop: OFF");
	}

	// draw pacman cross wall status
	if (pacman_cross_wall) {
		al_draw_text(menuFont, al_map_rgb(255, 255, 0),
			SCREEN_W - 50, 750, ALLEGRO_ALIGN_RIGHT, "Pacman Cross Wall: ON");
	}
	else {
		al_draw_text(menuFont, al_map_rgb(255, 255, 0),
			SCREEN_W - 50, 750, ALLEGRO_ALIGN_RIGHT, "Pacman Cross Wall: OFF");
	}

	draw_map(basic_map);

	// TODO 實作輸入名字
	if (need_input_name) {
		al_draw_scaled_bitmap(al_load_bitmap("Assets/black.png"), 0, 0, 60, 60, SCREEN_W / 2 - 300, SCREEN_H / 2 - 150, 600, 300, 0);
		al_draw_text(menuFont, al_map_rgb(255, 255, 0),
			SCREEN_W / 2, SCREEN_H / 2 - 100, ALLEGRO_ALIGN_CENTER, "Please input your name");

		drawButton(btnInputBox);
		drawButton(btnInputNameOK);
		drawButton(btnInputNameCancel);

		al_draw_text(menuFont, al_map_rgb(255, 255, 0),
			SCREEN_W / 2 - 190, SCREEN_H / 2 - 20, ALLEGRO_ALIGN_LEFT, input_name);
		al_draw_text(menuFont, al_map_rgb(255, 255, 0),
			SCREEN_W / 2 - 120, SCREEN_H / 2 + 70, ALLEGRO_ALIGN_CENTER, "OK");
		al_draw_text(menuFont, al_map_rgb(255, 255, 0),
			SCREEN_W / 2 + 120, SCREEN_H / 2 + 70, ALLEGRO_ALIGN_CENTER, "CANCEL");

		return;
	}


	pacman_draw(pman);
	if (game_over)
		return;

	// no drawing below when game over
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_draw(ghosts[i]);
		char id[2] = "1";
		id[0] += i;

		if (PvP) {
			al_draw_text(menuFont, al_map_rgb(255, 255, 0),
				290 + 100 * i, 20, ALLEGRO_ALIGN_CENTER, id);
			al_draw_scaled_bitmap(ghosts[i]->move_sprite, 0, 0, 
				16, 16, 300 + 100 * i, 20, 30, 30, 0);	
			if (i == controlling_ghost) {
				al_draw_rectangle(280 + 100 * i, 20, 290 + 100 * i + 40, 20 + 30, al_map_rgb(255, 255, 0), 2);
			}
		}
	}
	
	//debugging mode
	if (debug_mode) {
		draw_hitboxes();
	}

}

void game_update_scoreboard(int score) {
	game_main_Score += score;

	// draw scoreboard
	char score_str[20];
	sprintf(score_str, "Score: %d", game_main_Score);
}

static void draw_hitboxes(void) {
	RecArea pmanHB = getDrawArea((object *)pman, GAME_TICK_CD);
	al_draw_rectangle(
		pmanHB.x, pmanHB.y,
		pmanHB.x + pmanHB.w, pmanHB.y + pmanHB.h,
		al_map_rgb_f(1.0, 0.0, 0.0), 2
	);

	for (int i = 0; i < GHOST_NUM; i++) {
		RecArea ghostHB = getDrawArea((object *)ghosts[i], GAME_TICK_CD);
		al_draw_rectangle(
			ghostHB.x, ghostHB.y,
			ghostHB.x + ghostHB.w, ghostHB.y + ghostHB.h,
			al_map_rgb_f(1.0, 0.0, 0.0), 2
		);
	}

}

static void printinfo(void) {
	game_log("pacman:\n");
	game_log("coord: %d, %d\n", pman->objData.Coord.x, pman->objData.Coord.y);
	game_log("PreMove: %d\n", pman->objData.preMove);
	game_log("NextTryMove: %d\n", pman->objData.nextTryMove);
	game_log("Speed: %f\n", pman->speed);
}


static void destroy(void) {
	// TODO-GC-memory: free map array, Pacman and ghosts (done)
	delete_map(basic_map);
	pacman_destroy(pman);
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_destory(ghosts[i]);
	}
	free(ghosts);
	al_destroy_bitmap(btnInputBox.default_img);
	al_destroy_bitmap(btnInputBox.hovered_img);
	al_destroy_bitmap(btnInputNameOK.default_img);
	al_destroy_bitmap(btnInputNameOK.hovered_img);
	al_destroy_bitmap(btnInputNameCancel.default_img);
	al_destroy_bitmap(btnInputNameCancel.hovered_img);
}

static void on_key_down(int key_code) {
	// pacman
	switch (key_code)
	{
		// TODO-HACKATHON 1-1: Use allegro pre-defined enum ALLEGRO_KEY_<KEYNAME> to controll pacman movement
		// we provided you a function `pacman_NextMove` to set the pacman's next move direction.
		case ALLEGRO_KEY_ESCAPE:
			game_change_scene(scene_menu_create());
			return;
		case ALLEGRO_KEY_W:
			pacman_NextMove(pman, 1);
			break;
		case ALLEGRO_KEY_A:
			pacman_NextMove(pman, 2);
			break;
		case ALLEGRO_KEY_S:
			if ((key_state[ALLEGRO_KEY_LCTRL] || key_state[ALLEGRO_KEY_RCTRL]) && cheat_mode) {
				ghost_stop = !ghost_stop;
				for (int i = 0; i < GHOST_NUM; i++) {
					ghost_toggle_STOP(ghosts[i], ghost_stop);
				}
				if (ghost_stop)
					printf("ghost stop on\n");
				else
					printf("ghost stop off\n");	
			}
			else
				pacman_NextMove(pman, 4);
			break;
		case ALLEGRO_KEY_D:
			pacman_NextMove(pman, 3);
			break;
		case ALLEGRO_KEY_C:
			cheat_mode = !cheat_mode;
			ghost_stop = false;
			ghost_go_back = false;
			pacman_cross_wall = false;

			for (int i = 0; i < GHOST_NUM; i++) {
				ghost_toggle_STOP(ghosts[i], ghost_stop);
				ghost_toggle_GOIN(ghosts[i], ghost_go_back);
			}

			if (cheat_mode)
				printf("cheat mode on\n");
			else 
				printf("cheat mode off\n");
			break;
		case ALLEGRO_KEY_K:
			// set cheat mode: Ghosts start going back to the room 
			if (cheat_mode) {
				ghost_go_back = !ghost_go_back;
				for (int i = 0; i < GHOST_NUM; i++) {
					ghost_toggle_GOIN(ghosts[i], ghost_go_back);
				}
				if (ghost_go_back)
					printf("ghost go back on\n");
				else
					printf("ghost go back off\n");
			}
			break;
		case ALLEGRO_KEY_L:
			if ((key_state[ALLEGRO_KEY_LCTRL] || key_state[ALLEGRO_KEY_RCTRL]) && cheat_mode) { 
				// set cheat mode: Allow Pacman to cross the wall
				pacman_cross_wall = !pacman_cross_wall;
				if (pacman_cross_wall)
					printf("pacman cross wall on\n");
				else
					printf("pacman cross wall off\n");
			}
			break;
		case ALLEGRO_KEY_G:
			debug_mode = !debug_mode;
			break;
		default:
		break;
	}


	if (PvP) {
		// ghost
		if (ghosts[controlling_ghost]->status == FREEDOM) {
			switch (key_code) {
				case ALLEGRO_KEY_UP:
					ghost_NextMove(ghosts[controlling_ghost], 1);
					break;
				case ALLEGRO_KEY_LEFT:
					ghost_NextMove(ghosts[controlling_ghost], 2);
					break;
				case ALLEGRO_KEY_DOWN:
					ghost_NextMove(ghosts[controlling_ghost], 4);
					break;
				case ALLEGRO_KEY_RIGHT:
					ghost_NextMove(ghosts[controlling_ghost], 3);
					break;
				default:
					break;
			}
		}

		switch (key_code) {
			case ALLEGRO_KEY_SLASH:
				ghosts[controlling_ghost]->controlled = !ghosts[controlling_ghost]->controlled;
				controlling_ghost = (controlling_ghost + 1) % GHOST_NUM;
				ghosts[controlling_ghost]->controlled = !ghosts[controlling_ghost]->controlled;
				break;
			case ALLEGRO_KEY_1:
			case ALLEGRO_KEY_2:
			case ALLEGRO_KEY_3:
			case ALLEGRO_KEY_4: 
				// switch controlling ghost
				ghosts[controlling_ghost]->controlled = false;
				if (key_code - ALLEGRO_KEY_1 <= GHOST_NUM - 1)
					controlling_ghost = key_code - ALLEGRO_KEY_1;
				ghosts[controlling_ghost]->controlled = true;
				break;
			default:
				break;
		}
	}

	if (need_input_name) {
		if (key_code >= ALLEGRO_KEY_A && key_code <= ALLEGRO_KEY_Z && input_size < 20) {
			input_name[input_size++] = key_code - ALLEGRO_KEY_A + 'A';
			input_name[input_size] = '\0';
		}
		else if (key_code >= ALLEGRO_KEY_0 && key_code <= ALLEGRO_KEY_9 && input_size < 20) {
			input_name[input_size++] = key_code - ALLEGRO_KEY_0 + '0';
			input_name[input_size] = '\0';
		}
		else if (key_code == ALLEGRO_KEY_SPACE && input_size < 20) {
			input_name[input_size++] = ' ';
			input_name[input_size] = '\0';
		}
		else if (key_code == ALLEGRO_KEY_BACKSPACE && input_size > 0) {
			input_name[--input_size] = '\0';
		}
	}
}

static void on_mouse_down(int btn, int x, int y, int dz) {
	if (need_input_name) {
		if (btnInputNameOK.hovered) {
			need_input_name = false;
			// write name and score to leaderboard.txt

			FILE* fp = fopen("Database/leaderboard.txt", "a");
			if (!fp) {
				game_log("Error on open leaderboard.txt\n");
				return;
			}
			fprintf(fp, "%s %d\n", input_name, game_main_Score);
			fclose(fp);

			game_change_scene(scene_menu_create());
		}
		else if (btnInputNameCancel.hovered) {
			need_input_name = false;
			game_change_scene(scene_menu_create());
		}
	}
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	btnInputNameOK.hovered = buttonHover(btnInputNameOK, mouse_x, mouse_y);
	btnInputNameCancel.hovered = buttonHover(btnInputNameCancel, mouse_x, mouse_y);
}

static void render_init_screen(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	draw_map(basic_map);
	pacman_draw(pman);
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_draw(ghosts[i]);
	}

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		400, 400,
		ALLEGRO_ALIGN_CENTER,
		"READY!"
	);

	al_flip_display();
	al_rest(2.0);

}
// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_main_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Start";
	scene.initialize = &init;
	scene.update = &update;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_down = &on_mouse_down;
	scene.on_mouse_move = &on_mouse_move;
	// TODO-IF: Register more event callback functions such as keyboard, mouse, ...
	game_log("Start scene created");
	return scene;
}

int get_power_up_duration() {
	return power_up_duration;
}

ALLEGRO_TIMER* get_power_up_timer() {
	return power_up_timer;
}