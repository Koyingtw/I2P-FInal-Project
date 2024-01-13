#include "scene_custom_keys.h"
#include "scene_menu_object.h"
#include "game.h"
#include "shared.h"
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>

// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

Button btnInputKeys[4];
bool btnSelected[4];
char *keyTable[4];

static void init() {
    // TODO-Advance: button create

    for (int i = 0; i < 4; i++) {
        btnInputKeys[i] = button_create(SCREEN_W / 2 - 30, 150 + 100 * i, 200, 60, "./Assets/frame1.png", "./Assets/frame1.png");
        btnSelected[i] = false;
    }
    keyTable[0] = "W";
    keyTable[1] = "S";
    keyTable[2] = "A";
    keyTable[3] = "D";
}

static void draw_scene_custom_keys(void ){
	al_clear_to_color(al_map_rgb(0, 0, 0));
    // al_draw_bitmap(
	// 	al_load_bitmap("./Assets/scene_custom_keys.png"),
	// 	0, 0, 0
	// );
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W/2,
		SCREEN_H - 150,
		ALLEGRO_ALIGN_CENTER,
		"<ENTER> Back to settings"
	);

    for (int i = 0; i < 4; i++) {
        drawButton(btnInputKeys[i]);
    }

    al_draw_text(menuFont, al_map_rgb(255, 255, 255), 150, 170, ALLEGRO_ALIGN_CENTER, "UP");
    al_draw_text(menuFont, al_map_rgb(255, 255, 255), 150, 270, ALLEGRO_ALIGN_CENTER, "DOWN");
    al_draw_text(menuFont, al_map_rgb(255, 255, 255), 150, 370, ALLEGRO_ALIGN_CENTER, "LEFT");
    al_draw_text(menuFont, al_map_rgb(255, 255, 255), 150, 470, ALLEGRO_ALIGN_CENTER, "RIGHT");
    al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2, 70, ALLEGRO_ALIGN_CENTER, "KEY");
    for (int i = 0; i < 4; i++) {
        al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 70, 170 + 100 * i, ALLEGRO_ALIGN_CENTER, keyTable[i]);
    }
}

static void change_key(int pos, int keycode, char *key) {
    keyTable[pos] = key;
    btnSelected[pos] = false;
    btnInputKeys[pos].default_img = load_bitmap("./Assets/frame1.png");
    btnInputKeys[pos].hovered_img = load_bitmap("./Assets/frame1.png");

    int keycodes[4] = {ALLEGRO_KEY_W, ALLEGRO_KEY_S, ALLEGRO_KEY_A, ALLEGRO_KEY_D};

    key_table[keycode] = keycodes[pos];
    
    return;
}

static void on_key_down(int keycode) {
	switch (keycode) {
		case ALLEGRO_KEY_ENTER:
			game_change_scene(scene_settings_create());
			break;
		default:
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 2; j++) {
                    if (btnSelected[i]) {
                        change_key(i, keycode, keycode_to_char(keycode));
                    }
                }   
            }
			break;
	}
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	for (int i = 0; i < 4; i++) {
        btnInputKeys[i].hovered = buttonHover(btnInputKeys[i], mouse_x, mouse_y);
        btnInputKeys[i].hovered = buttonHover(btnInputKeys[i], mouse_x, mouse_y);  
    }
}

static void on_mouse_down() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            btnSelected[i] = false;
            btnInputKeys[i].default_img = load_bitmap("./Assets/frame1.png");
            btnInputKeys[i].hovered_img = load_bitmap("./Assets/frame1.png");
            if (btnInputKeys[i].hovered) {
                btnSelected[i] = true;
                btnInputKeys[i].default_img = load_bitmap("./Assets/frame2.png");
                btnInputKeys[i].hovered_img = load_bitmap("./Assets/frame2.png");
            }
        }
    }
	return;
}

// The only function that is shared across files.
Scene scene_custom_keys_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Custom Keys";
	scene.draw = &draw_scene_custom_keys;
	scene.on_key_down = &on_key_down;
    scene.initialize = &init;
    scene.on_key_down = &on_key_down;
    scene.on_mouse_move = &on_mouse_move;
    scene.on_mouse_down = &on_mouse_down;


	// TODO-IF: Register more event callback functions such as keyboard, mouse, ...
	game_log("Settings scene created");
	return scene;
}