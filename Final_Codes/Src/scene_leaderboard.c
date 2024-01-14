#include "scene_leaderboard.h"
#include "scene_menu_object.h"
#include "scene_custom_keys.h"
#include "game.h"
#include "shared.h"
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>

// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

typedef struct {
    char name[25];
    int score;
} userdata;

userdata data[100], data_sort_by_name[100], data_sort_by_score[100];
size_t len = 0;
int page = 1, sort_rule = 2; // 1: by name, 2: by score

Button btnNextPage, btnPrevPage, btnSortByScore, btnSortByName;

static void sort_by_name() {
    for (int i = 0; i < len; i++) {
        data_sort_by_name[i] = data[i];
    }

    for (int i = 0; i < len; i++) {
        for (int j = i + 1; j < len; j++) {
            if (strcmp(data_sort_by_name[i].name, data_sort_by_name[j].name) > 0) {
                userdata tmp = data_sort_by_name[i];
                data_sort_by_name[i] = data_sort_by_name[j];
                data_sort_by_name[j] = tmp;
            }
        }
    }
}

static void sort_by_score() {
    for (int i = 0; i < len; i++) {
        data_sort_by_score[i] = data[i];
    }

    for (int i = 0; i < len; i++) {
        for (int j = i + 1; j < len; j++) {
            if (data_sort_by_score[i].score < data_sort_by_score[j].score) {
                userdata tmp = data_sort_by_score[i];
                data_sort_by_score[i] = data_sort_by_score[j];
                data_sort_by_score[j] = tmp;
            }
        }
    }
}

static void init() {
    // TODO-Advance: button create
    // read leaderfile from "./Database/leaderboard.txt"

    char *path = "./Database/leaderboard.txt";
    page = 1;
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Error: cannot open file %s\n", path);
        exit(1);
    }

    ssize_t read;
    int i = 0;
    while (fscanf(fp, "%s %d", &data[i].name, &data[i].score) != EOF) {
        i++;
    }

    len = i;

    btnNextPage = button_create(SCREEN_W - 250, SCREEN_H - 90, 200, 60, "./Assets/frame1.png", "./Assets/frame2.png");
    btnPrevPage = button_create(50, SCREEN_H - 90, 200, 60, "./Assets/frame1.png", "./Assets/frame2.png");
    btnSortByName = button_create(195, 95, 120, 40, "./Assets/black.png", "./Assets/black.png");
    btnSortByScore = button_create(590, 95, 120, 40, "./Assets/black.png", "./Assets/black.png");

    sort_by_name();
    sort_by_score();


    fclose(fp);
}

static void draw_scene_leaderboard(void ){
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W/2,
		SCREEN_H - 150,
		ALLEGRO_ALIGN_CENTER,
		"<ENTER> Back to settings"
	);

    char str[100];
    sprintf(str, "Leaderboard (%d / %d)", page, (len + 9) / 10);

    drawButton(btnNextPage);
    drawButton(btnPrevPage);
    drawButton(btnSortByScore);
    drawButton(btnSortByName);

    al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W - 150, SCREEN_H - 75, ALLEGRO_ALIGN_CENTER, "NEXT PAGE");
    al_draw_text(menuFont, al_map_rgb(255, 255, 255), 150, SCREEN_H - 75, ALLEGRO_ALIGN_CENTER, "PREV PAGE");

    al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2, 50, ALLEGRO_ALIGN_CENTER, str);

    if (sort_rule == 1) {
        al_draw_text(menuFont, al_map_rgb(255, 255, 255), 200, 100, ALLEGRO_ALIGN_LEFT, "Name*");
        al_draw_text(menuFont, al_map_rgb(255, 255, 255), 600, 100, ALLEGRO_ALIGN_LEFT, "Score");
    }
    else {
        al_draw_text(menuFont, al_map_rgb(255, 255, 255), 200, 100, ALLEGRO_ALIGN_LEFT, "Name");
        al_draw_text(menuFont, al_map_rgb(255, 255, 255), 600, 100, ALLEGRO_ALIGN_LEFT, "Score*");
    }

    for (int i = 10 * (page - 1); i < 10 * page && i < len; i++) {
        char str[100];
        if (sort_rule == 1) {
            sprintf(str, "%d", data_sort_by_name[i].score);
            al_draw_text(menuFont, al_map_rgb(255, 255, 255), 200, 150 + 50 * (i - 10 * (page - 1)), ALLEGRO_ALIGN_LEFT, data_sort_by_name[i].name);
            al_draw_text(menuFont, al_map_rgb(255, 255, 255), 600, 150 + 50 * (i - 10 * (page - 1)), ALLEGRO_ALIGN_LEFT, str);
        }
        else {
            sprintf(str, "%d", data_sort_by_score[i].score);
            al_draw_text(menuFont, al_map_rgb(255, 255, 255), 200, 150 + 50 * (i - 10 * (page - 1)), ALLEGRO_ALIGN_LEFT, data_sort_by_score[i].name);
            al_draw_text(menuFont, al_map_rgb(255, 255, 255), 600, 150 + 50 * (i - 10 * (page - 1)), ALLEGRO_ALIGN_LEFT, str);
        }


        sprintf(str, "%d", i + 1);
        al_draw_text(menuFont, al_map_rgb(255, 255, 255), 120, 150 + 50 * (i - 10 * (page - 1)), ALLEGRO_ALIGN_RIGHT, str);
    }

}


static void on_key_down(int keycode) {
	switch (keycode) {
		case ALLEGRO_KEY_ENTER:
			game_change_scene(scene_settings_create());
			break;
		default:
			break;
	}
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
    btnNextPage.hovered = buttonHover(btnNextPage, mouse_x, mouse_y);
    btnPrevPage.hovered = buttonHover(btnPrevPage, mouse_x, mouse_y);
    btnSortByName.hovered = buttonHover(btnSortByName, mouse_x, mouse_y);
    btnSortByScore.hovered = buttonHover(btnSortByScore, mouse_x, mouse_y);
}

static void on_mouse_down() {
    if (btnNextPage.hovered) {
        if (page < len / 10 + 1) {
            page++;
        }
    }

    if (btnPrevPage.hovered) {
        if (page > 1) {
            page--;
        }
    }

    if (btnSortByName.hovered) {
        sort_rule = 1;
    }

    if (btnSortByScore.hovered) {
        sort_rule = 2;
    }
	return;
}

static void destory(void) {
    al_destroy_bitmap(btnNextPage.default_img);
    al_destroy_bitmap(btnNextPage.hovered_img);
    al_destroy_bitmap(btnPrevPage.default_img);
    al_destroy_bitmap(btnPrevPage.hovered_img);
    al_destroy_bitmap(btnSortByName.default_img);
    al_destroy_bitmap(btnSortByName.hovered_img);
    al_destroy_bitmap(btnSortByScore.default_img);
    al_destroy_bitmap(btnSortByScore.hovered_img);
}

// The only function that is shared across files.
Scene scene_leaderboard_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Custom Keys";
	scene.draw = &draw_scene_leaderboard;
	scene.on_key_down = &on_key_down;
    scene.initialize = &init;
    scene.on_key_down = &on_key_down;
    scene.on_mouse_move = &on_mouse_move;
    scene.on_mouse_down = &on_mouse_down;
    scene.destroy = &destory;


	// TODO-IF: Register more event callback functions such as keyboard, mouse, ...
	game_log("Settings scene created");
	return scene;
}