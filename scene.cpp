#include "scene.h"
#include <stdio.h>
ALLEGRO_BITMAP *background = NULL;
int x;
int y;
ALLEGRO_BITMAP *back_menu;
// function of menu
void menu_init(){
    font = al_load_ttf_font("./font/pirulen.ttf",30,0);
    char temp[50];
    sprintf( temp, "./image/intro%d.png",1 );
    back_menu = al_load_bitmap(temp);
}
void menu_process(ALLEGRO_EVENT event){
    if( event.type == ALLEGRO_EVENT_KEY_UP )
        if( event.keyboard.keycode == ALLEGRO_KEY_ENTER )
            judge_next_window = true;
        if( event.keyboard.keycode == ALLEGRO_KEY_ESCAPE )
            end_game = true;
        if( event.keyboard.keycode == ALLEGRO_KEY_SPACE ){
            to_inst = true;
        }
}
void menu_draw(){
    //al_clear_to_color(al_map_rgb(100,100,100));
    al_draw_scaled_bitmap(back_menu, 0, 0, 570, 203,0,0,2500,HEIGHT,0);
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2-75 , ALLEGRO_ALIGN_CENTRE, "Press 'Enter' to start");
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2 , ALLEGRO_ALIGN_CENTRE, "Press 'space' for instructions");
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2 +75 , ALLEGRO_ALIGN_CENTRE, "Press 'ESC' to quit");
    al_draw_rectangle(WIDTH/2-450, 80, WIDTH/2+450, 150, al_map_rgb(255, 255, 255), 0);
    al_draw_rectangle(WIDTH/2-450, 160, WIDTH/2+450, 230, al_map_rgb(255, 255, 255), 0);
    al_draw_rectangle(WIDTH/2-450, 240, WIDTH/2+450, 310, al_map_rgb(255, 255, 255), 0);
}
void menu_destroy(){
    al_destroy_font(font);
}

// function of game_scene
void game_scene_init(){
    character_init();
    background = al_load_bitmap("./image/stage103.png");
}
void game_scene_draw(){
    //al_draw_bitmap(background, 0, 0, 0);
    al_draw_scaled_bitmap(background,0,0,al_get_bitmap_width(background),al_get_bitmap_height(background),x,y,WIDTH*2,HEIGHT,0);
    character_draw();
    al_flip_display();
}
void game_scene_update(){
    if (x>=-2000&&key_state[ALLEGRO_KEY_D]) x -=6;
    if (x<=0&&key_state[ALLEGRO_KEY_A]) x+=6;
}
void game_scene_destroy(){
    al_destroy_bitmap(background);
    character_destory();
}

void gg_init(){
    //font = al_load_ttf_font("./font/pirulen.ttf",30,0);
}
int gg_process(ALLEGRO_EVENT event){
    if( event.type == ALLEGRO_EVENT_KEY_UP )
        if( event.keyboard.keycode == ALLEGRO_KEY_ENTER )
            judge_next_window = true;
        if( event.keyboard.keycode == ALLEGRO_KEY_M){
            to_menu = true;
        }

}
void gg_draw(){
    al_clear_to_color(al_map_rgb(100,100,100));
    al_draw_textf(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2 , ALLEGRO_ALIGN_CENTRE, "YOUR SCORE: %d",score);
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+100 , ALLEGRO_ALIGN_CENTRE, "GAME OVER!!! Press 'Enter' to Play Again and 'M' to Menu");
}
void gg_destroy(){
    al_destroy_font(font);
}

void inst_init(){
    font = al_load_ttf_font("./font/pirulen.ttf",30,0);
}
int inst_process(ALLEGRO_EVENT event){
    if( event.type == ALLEGRO_EVENT_KEY_UP )
        if( event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE){
            to_menu = true;
        }
}
void inst_draw(){
    al_clear_to_color(al_map_rgb(100,100,100));
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2-50 , ALLEGRO_ALIGN_CENTRE, "INSTURCTIONS");
    //al_draw_rectangle(WIDTH/2-150, 210, WIDTH/2+150, 210, al_map_rgb(255, 255, 255), 0);
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2 +100 , ALLEGRO_ALIGN_CENTRE, "Press 'BACK' to Menu");
}
void inst_destroy(){
    al_destroy_font(font);
}



