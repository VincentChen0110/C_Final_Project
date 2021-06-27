#include "global.h"

// variables for global usage
const float FPS = 60.0;
const int WIDTH = 2500;
const int HEIGHT = 350;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *fps = NULL;
ALLEGRO_FONT *font = NULL;
bool key_state[ALLEGRO_KEY_MAX] = {false};
bool judge_next_window = false;
bool judge_game_end = false;
bool to_menu = false;
bool to_inst = false;
bool end_game = false;
int score = 0;
