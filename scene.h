#include "global.h"
#include "charater.h"

void menu_init();
void menu_process(ALLEGRO_EVENT event);
void menu_draw();
void menu_destroy();

void game_scene_init();
void game_scene_draw();
void game_scene_update();
void game_scene_destroy();

void gg_init();
int gg_process(ALLEGRO_EVENT event);
void gg_draw();
void gg_destroy();

void inst_init();
int inst_process(ALLEGRO_EVENT event);
void inst_draw();
void inst_destroy();
