#include "scene_custom_keys.h"
#include "scene_menu_object.h"
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>

// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

Button btnCustomKeys;

static void draw_scene_custom_keys(void ){
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W/2,
		SCREEN_H - 150,
		ALLEGRO_ALIGN_CENTER,
		"<ENTER> Back to settings"
	);
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

// The only function that is shared across files.
Scene scene_custom_keys_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Custom Keys";
	scene.draw = &draw_scene_custom_keys;
	scene.on_key_down = &on_key_down;


	// TODO-IF: Register more event callback functions such as keyboard, mouse, ...
	game_log("Settings scene created");
	return scene;
}