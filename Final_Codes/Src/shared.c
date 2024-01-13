// [shared.c]
// you should define the shared variable declared in the header here.

#include "shared.h"
#include "utility.h"
#include "game.h"
// #include "scene_menu.h"

ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
ALLEGRO_SAMPLE* themeMusic = NULL;
ALLEGRO_SAMPLE* PACMAN_MOVESOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND = NULL;
ALLEGRO_FONT* menuFont = NULL;
int fontSize = 30;
float music_volume = 0.5;
float effect_volume = 0.5;
bool gameDone = false;

/*
	A way to accelerate is load assets once.
	And delete them at the end.
	This method does provide you better management of memory.
	
*/
void shared_init(void) {
	
	menuFont = load_font("Assets/Minecraft.ttf", fontSize);
	themeMusic = load_audio("Assets/Music/original_theme.ogg");
	PACMAN_MOVESOUND = load_audio("Assets/Music/pacman-chomp.ogg");
	PACMAN_DEATH_SOUND = load_audio("Assets/Music/pacman_death.ogg");
}

void shared_destroy(void) {

	al_destroy_font(menuFont);
	al_destroy_sample(themeMusic);
	al_destroy_sample(PACMAN_MOVESOUND);
	al_destroy_sample(PACMAN_DEATH_SOUND);
}

char *keycode_to_char(int keycode) {
	switch (keycode) {
		case ALLEGRO_KEY_A:
			return "A";
		case ALLEGRO_KEY_B:
			return "B";
		case ALLEGRO_KEY_C:
			return "C";
		case ALLEGRO_KEY_D:
			return "D";
		case ALLEGRO_KEY_E:
			return "E";
		case ALLEGRO_KEY_F:
			return "F";
		case ALLEGRO_KEY_G:
			return "G";
		case ALLEGRO_KEY_H:
			return "H";
		case ALLEGRO_KEY_I:
			return "I";
		case ALLEGRO_KEY_J:
			return "J";
		case ALLEGRO_KEY_K:
			return "K";
		case ALLEGRO_KEY_L:
			return "L";
		case ALLEGRO_KEY_M:
			return "M";
		case ALLEGRO_KEY_N:
			return "N";
		case ALLEGRO_KEY_O:
			return "O";
		case ALLEGRO_KEY_P:
			return "P";
		case ALLEGRO_KEY_Q:
			return "Q";
		case ALLEGRO_KEY_R:
			return "R";
		case ALLEGRO_KEY_S:
			return "S";
		case ALLEGRO_KEY_T:
			return "T";
		case ALLEGRO_KEY_U:
			return "U";
		case ALLEGRO_KEY_V:
			return "V";
		case ALLEGRO_KEY_W:
			return "W";
		case ALLEGRO_KEY_X:
			return "X";
		case ALLEGRO_KEY_Y:
			return "Y";
		case ALLEGRO_KEY_Z:
			return "Z";
		case ALLEGRO_KEY_0:
			return "0";
		case ALLEGRO_KEY_1:
			return "1";
		case ALLEGRO_KEY_2:
			return "2";
		case ALLEGRO_KEY_3:
			return "3";
		case ALLEGRO_KEY_4:
			return "4";
		case ALLEGRO_KEY_5:
			return "5";
		case ALLEGRO_KEY_6:
			return "6";
		case ALLEGRO_KEY_7:
			return "7";
		case ALLEGRO_KEY_8:
			return "8";
		case ALLEGRO_KEY_9:
			return "9";
		case ALLEGRO_KEY_PAD_0:
			return "PAD 0";
		case ALLEGRO_KEY_PAD_1:
			return "PAD 1";
		case ALLEGRO_KEY_PAD_2:
			return "PAD 2";
		case ALLEGRO_KEY_PAD_3:
			return "PAD 3";
		case ALLEGRO_KEY_PAD_4:
			return "PAD 4";
		case ALLEGRO_KEY_PAD_5:
			return "PAD 5";
		case ALLEGRO_KEY_PAD_6:
			return "PAD 6";
		case ALLEGRO_KEY_PAD_7:
			return "PAD 7";
		case ALLEGRO_KEY_PAD_8:
			return "PAD 8";
		case ALLEGRO_KEY_PAD_9:
			return "PAD 9";
		case ALLEGRO_KEY_F1:
			return "F1";
		case ALLEGRO_KEY_F2:
			return "F2";
		case ALLEGRO_KEY_F3:
			return "F3";
		case ALLEGRO_KEY_F4:
			return "F4";
		case ALLEGRO_KEY_F5:
			return "F5";
		case ALLEGRO_KEY_F6:
			return "F6";
		case ALLEGRO_KEY_F7:
			return "F7";
		case ALLEGRO_KEY_F8:
			return "F8";
		case ALLEGRO_KEY_F9:
			return "F9";
		case ALLEGRO_KEY_F10:
			return "F10";
		case ALLEGRO_KEY_F11:
			return "F11";
		case ALLEGRO_KEY_F12:
			return "F12";
		case ALLEGRO_KEY_ESCAPE:
			return "ESC";
		case ALLEGRO_KEY_TILDE:
			return "~";
		case ALLEGRO_KEY_MINUS:
			return "-";
		case ALLEGRO_KEY_EQUALS:
			return "=";
		case ALLEGRO_KEY_BACKSPACE:
			return "BACKSPACE";
		case ALLEGRO_KEY_TAB:
			return "TAB";
		case ALLEGRO_KEY_OPENBRACE:
			return "[";
		case ALLEGRO_KEY_CLOSEBRACE:
			return "]";
		case ALLEGRO_KEY_ENTER:
			return "ENTER";
		case ALLEGRO_KEY_SEMICOLON:
			return ";";
		case ALLEGRO_KEY_QUOTE:
			return "'";
		case ALLEGRO_KEY_BACKSLASH:
			return "\\";
		case ALLEGRO_KEY_BACKSLASH2:
			return "\\";
		case ALLEGRO_KEY_COMMA:
			return ",";
		case ALLEGRO_KEY_FULLSTOP:
			return ".";
		case ALLEGRO_KEY_SLASH:
			return "/";
		case ALLEGRO_KEY_SPACE:
			return "SPACE";
		case ALLEGRO_KEY_INSERT:
			return "INSERT";
		case ALLEGRO_KEY_DELETE:
			return "DELETE";
		case ALLEGRO_KEY_HOME:
			return "HOME";
		case ALLEGRO_KEY_END:
			return "END";
		case ALLEGRO_KEY_PGUP:
			return "PGUP";
		case ALLEGRO_KEY_PGDN:
			return "PGDN";
		case ALLEGRO_KEY_LEFT:
			return "LEFT";
		case ALLEGRO_KEY_RIGHT:
			return "RIGHT";
		case ALLEGRO_KEY_UP:
			return "UP";
		case ALLEGRO_KEY_DOWN:
			return "DOWN";
		case ALLEGRO_KEY_PAD_SLASH:
			return "PAD /";
		case ALLEGRO_KEY_PAD_ASTERISK:
			return "PAD *";
		case ALLEGRO_KEY_PAD_MINUS:
			return "PAD -";
		case ALLEGRO_KEY_PAD_PLUS:
			return "PAD +";
		case ALLEGRO_KEY_PAD_DELETE:
			return "PAD .";
		case ALLEGRO_KEY_PAD_ENTER:
			return "PAD ENTER";
		case ALLEGRO_KEY_PRINTSCREEN:
			return "PRINTSCREEN";
		case ALLEGRO_KEY_PAUSE:
			return "PAUSE";
		case ALLEGRO_KEY_ABNT_C1:
			return "ABNT C1";
		case ALLEGRO_KEY_YEN:
			return "YEN";
		case ALLEGRO_KEY_KANA:
			return "KANA";
		case ALLEGRO_KEY_CONVERT:
			return "CONVERT";
		case ALLEGRO_KEY_NOCONVERT:
			return "NOCONVERT";
		case ALLEGRO_KEY_AT:
			return "@";
		case ALLEGRO_KEY_CIRCUMFLEX:
			return "^";
		case ALLEGRO_KEY_COLON2:
			return ":";
		case ALLEGRO_KEY_KANJI:
			return "KANJI";
		case ALLEGRO_KEY_PAD_EQUALS:
			return "PAD =";
		case ALLEGRO_KEY_BACKQUOTE:
			return "`";
		case ALLEGRO_KEY_SEMICOLON2:
			return ";";
		case ALLEGRO_KEY_COMMAND:
			return "COMMAND";
		case ALLEGRO_KEY_UNKNOWN:
			return "UNKNOWN";
		default:
			return "UNKNOWN";
	}
}