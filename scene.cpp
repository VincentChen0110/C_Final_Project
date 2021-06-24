#include "scene.h"
#include <stdio.h>
ALLEGRO_FONT *font = NULL;
ALLEGRO_BITMAP *background = NULL;

int x;
int y;
// function of menu
void menu_init(){
    font = al_load_ttf_font("./font/pirulen.ttf",12,0);
}
void menu_process(ALLEGRO_EVENT event){
    if( event.type == ALLEGRO_EVENT_KEY_UP )
        if( event.keyboard.keycode == ALLEGRO_KEY_ENTER )
            judge_next_window = true;
}
void menu_draw(){
    al_clear_to_color(al_map_rgb(100,100,100));
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2 , ALLEGRO_ALIGN_CENTRE, "Press 'Enter' to start");
    al_draw_rectangle(WIDTH/2-150, 210, WIDTH/2+150, 210, al_map_rgb(255, 255, 255), 0);
}
void menu_destroy(){
    al_destroy_font(font);
}

// function of game_scene
void game_scene_init(){
    character_init();
    background = al_load_bitmap("./image/stage1.png");
}
void game_scene_draw(){
    //al_draw_bitmap(background, 0, 0, 0);
    al_draw_scaled_bitmap(background,0,0,al_get_bitmap_width(background),al_get_bitmap_height(background),x,y,WIDTH*2,HEIGHT,0);
    character_draw();
    al_flip_display();
}
void game_scene_update(){
    if (x>=-1200&&key_state[ALLEGRO_KEY_D]) x -=6;
    if (x<=0&&key_state[ALLEGRO_KEY_A]) x+=6;
}
void game_scene_destroy(){
    al_destroy_bitmap(background);
    character_destory();
}
