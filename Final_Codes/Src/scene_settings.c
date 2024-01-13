// TODO-HACKATHON 3-9: Create scene_settings.h and scene_settings.c. (done)
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
#include "scene_settings.h"
#include "scene_menu_object.h"
#include "scene_custom_keys.h"
#include "scene_game.h"
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>



// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

// TODO-IF: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

Button btnCustomKeys;
Button btnPvPSelect;

static void init() {
	// TODO-Advance: button create

	btnCustomKeys = button_create((SCREEN_W - 500) / 2, 85, 500, 60, "./Assets/frame1.png", "./Assets/frame2.png");
	btnPvPSelect = button_create((SCREEN_W - 500) / 2, 185, 500, 60, "./Assets/frame1.png", "./Assets/frame2.png");
}

static void draw_scene_settings(void ){
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W/2,
		SCREEN_H - 150,
		ALLEGRO_ALIGN_CENTER,
		"<ENTER> Back to menu"
	);
	drawButton(btnCustomKeys);
	drawButton(btnPvPSelect);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2, 100, ALLEGRO_ALIGN_CENTER, "Customize Keys");
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2, 200, ALLEGRO_ALIGN_CENTER, "PvP Mode");
}

static void on_key_down(int keycode) {
	switch (keycode) {
		case ALLEGRO_KEY_ENTER:
			game_change_scene(scene_menu_create());
			break;
		default:
			break;
	}
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	btnCustomKeys.hovered = buttonHover(btnCustomKeys, mouse_x, mouse_y);
	btnPvPSelect.hovered = buttonHover(btnPvPSelect, mouse_x, mouse_y);
	btnPvPSelect.hovered_img = load_bitmap("./Assets/frame3.png");
}

static void on_mouse_down() {
	if (btnCustomKeys.hovered)
		game_change_scene(scene_custom_keys_create());
	if (btnPvPSelect.hovered) {
		PvP = !PvP;
		if (PvP)
			btnPvPSelect.hovered_img = btnPvPSelect.default_img = load_bitmap("./Assets/frame3.png");
		else {
			btnPvPSelect.default_img = load_bitmap("./Assets/frame1.png");
			btnPvPSelect.hovered_img = load_bitmap("./Assets/frame2.png");
		}
	}
}

// The only function that is shared across files.
Scene scene_settings_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Settings";
	scene.draw = &draw_scene_settings;
	scene.initialize = &init;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	scene.on_mouse_down = &on_mouse_down;

	// TODO-IF: Register more event callback functions such as keyboard, mouse, ...
	game_log("Settings scene created");
	return scene;
}

// TODO-Graphical_Widget:
// Just suggestions, you can create your own usage.
	// Selecting BGM:
	// 1. Declare global variables for storing the BGM. (see `shared.h`, `shared.c`)
	// 2. Load the BGM in `shared.c`.
	// 3. Create dropdown menu for selecting BGM in setting scene, involving `scene_settings.c` and `scene_setting.h.
	// 4. Switch and play the BGM if the corresponding option is selected.

	// Adjusting Volume:
	// 1. Declare global variables for storing the volume. (see `shared.h`, `shared.c`)
	// 2. Create a slider for adjusting volume in setting scene, involving `scene_settings.c` and `scene_setting.h.
		// 2.1. You may use checkbox to switch between mute and unmute.
	// 3. Adjust the volume and play when the button is pressed.

	// Selecting Map:
	// 1. Preload the map to `shared.c`.
	// 2. Create buttons(or dropdown menu) for selecting map in setting scene, involving `scene_settings.c` and `scene_setting.h.
		// 2.1. For player experience, you may also create another scene between menu scene and game scene for selecting map.
	// 3. Create buttons(or dropdown menu) for selecting map in setting scene, involving `scene_settings.c` and `scene_setting.h.
	// 4. Switch and draw the map if the corresponding option is selected.
		// 4.1. Suggestions: You may use `al_draw_bitmap` to draw the map for previewing. 
							// But the map is too large to be drawn in original size. 
							// You might want to modify the function to allow scaling.