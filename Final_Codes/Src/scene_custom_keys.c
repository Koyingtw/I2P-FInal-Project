#include "scene_custom_keys.h"
#include "scene_menu_object.h"
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>

// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

Button btnInputKeys[4][2];
bool btnSelected[4][2];
char keyTable[4][2];

static void init() {
    // TODO-Advance: button create

    for (int i = 0; i < 4; i++) {
        btnInputKeys[i][0] = button_create(250, 150 + 100 * i, 60, 60, "./Assets/frame1.png", "./Assets/frame1.png");
        btnInputKeys[i][1] = button_create(500, 150 + 100 * i, 60, 60, "./Assets/frame1.png", "./Assets/frame1.png");
        btnSelected[i][0] = false;
    }
    keyTable[0][0] = 'W';
    keyTable[0][1] = '\0';
    keyTable[1][0] = 'S';
    keyTable[1][1] = '\0';
    keyTable[2][0] = 'A';
    keyTable[2][1] = '\0';
    keyTable[3][0] = 'D';
    keyTable[3][1] = '\0';
}

static void draw_scene_custom_keys(void ){
	al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(
		al_load_bitmap("./Assets/scene_custom_keys.png"),
		0, 0, 0
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W/2,
		SCREEN_H - 150,
		ALLEGRO_ALIGN_CENTER,
		"<ENTER> Back to settings"
	);

    for (int i = 0; i < 4; i++) {
        drawButton(btnInputKeys[i][0]);
        drawButton(btnInputKeys[i][1]);
    }

    al_draw_text(menuFont, al_map_rgb(255, 255, 255), 150, 170, ALLEGRO_ALIGN_CENTER, "UP");
    al_draw_text(menuFont, al_map_rgb(255, 255, 255), 150, 270, ALLEGRO_ALIGN_CENTER, "DOWN");
    al_draw_text(menuFont, al_map_rgb(255, 255, 255), 150, 370, ALLEGRO_ALIGN_CENTER, "LEFT");
    al_draw_text(menuFont, al_map_rgb(255, 255, 255), 150, 470, ALLEGRO_ALIGN_CENTER, "RIGHT");
    al_draw_text(menuFont, al_map_rgb(255, 255, 255), 280, 70, ALLEGRO_ALIGN_CENTER, "FIRST_KEY");
    al_draw_text(menuFont, al_map_rgb(255, 255, 255), 530, 70, ALLEGRO_ALIGN_CENTER, "SECOND_KEY");
    for (int i = 0; i < 4; i++) {
        char t1[2] = {keyTable[i][0], '\0'};
        char t2[2] = {keyTable[i][1], '\0'};
        al_draw_text(menuFont, al_map_rgb(255, 255, 255), 280, 170 + 100 * i, ALLEGRO_ALIGN_CENTER, t1);
        al_draw_text(menuFont, al_map_rgb(255, 255, 255), 530, 170 + 100 * i, ALLEGRO_ALIGN_CENTER, t2);
    }
    // al_draw_text(menuFont, al_map_rgb(255, 255, 255), 280, 170, ALLEGRO_ALIGN_CENTER, );
    // al_draw_text(menuFont, al_map_rgb(255, 255, 255), 280, 270, ALLEGRO_ALIGN_CENTER, "A");
    // al_draw_text(menuFont, al_map_rgb(255, 255, 255), 280, 370, ALLEGRO_ALIGN_CENTER, "S");
    // al_draw_text(menuFont, al_map_rgb(255, 255, 255), 280, 470, ALLEGRO_ALIGN_CENTER, "D");
    
}

static void change_key(int pos, int opt, int keycode, char key) {
    if (opt == 0) {
        keyTable[pos][0] = key;
    } else {
        keyTable[pos][1] = key;
    }
    btnSelected[pos][opt] = false;
    btnInputKeys[pos][opt].default_img = load_bitmap("./Assets/frame1.png");
    btnInputKeys[pos][opt].hovered_img = load_bitmap("./Assets/frame1.png");
    return;
}

static void on_key_down(int keycode) {
	switch (keycode) {
		case ALLEGRO_KEY_ENTER:
			game_change_scene(scene_settings_create());
			break;
		default:
            if (keycode <= 36) {
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 2; j++) {
                        if (btnSelected[i][j]) {
                            if (keycode <= 26)
                                change_key(i, j, keycode, 'A' + keycode - 1);
                            else
                                change_key(i, j, keycode, '0' + keycode - 27);
                        }
                    }   
                }
            }
			break;
	}
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	for (int i = 0; i < 4; i++) {
        btnInputKeys[i][0].hovered = buttonHover(btnInputKeys[i][0], mouse_x, mouse_y);
        btnInputKeys[i][1].hovered = buttonHover(btnInputKeys[i][1], mouse_x, mouse_y);
    }
}

static void on_mouse_down() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            btnSelected[i][j] = false;
            btnInputKeys[i][j].default_img = load_bitmap("./Assets/frame1.png");
            btnInputKeys[i][j].hovered_img = load_bitmap("./Assets/frame1.png");
            if (btnInputKeys[i][j].hovered) {
                btnSelected[i][j] = true;
                btnInputKeys[i][j].default_img = load_bitmap("./Assets/frame2.png");
                btnInputKeys[i][j].hovered_img = load_bitmap("./Assets/frame2.png");
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