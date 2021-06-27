#include "charater.h"
#define MAX 7
#define MAX_BULLETS 30
// the state of character
enum {STOP = 0, MOVE, ATK, DEAD, DUCK, SHOOT, BOMB};
int bullet_count;
int bbullet_count;
int stage, flag1 =0;
typedef struct character
{
    int x, y; // the position of image
    int width, height; // the width and height of image
    bool dir; // left: false, right: true
    int state; // the state of character
    ALLEGRO_BITMAP *img_move[3];
    ALLEGRO_BITMAP *img_atk[3];
    ALLEGRO_BITMAP *img_die[3];
    ALLEGRO_BITMAP *img_duck[3];
    ALLEGRO_BITMAP *img_bomb[3];
    ALLEGRO_SAMPLE_INSTANCE *atk_Sound;
    int anime; // counting the time of animation
    int anime_time; // indicate how long the animation
    int speed, health, attack1, life;
    bool live, shoot;
}Character;
Character chara;
Character enemy[MAX];
typedef struct bullet
{
    int x, y; // the position of image
    int radius;
    int show;
    bool dir; // left: false, right: true
    int ID; // the state of character
    int speed;
}Bullet;
Bullet bullet[MAX_BULLETS];
Bullet bbullet[MAX_BULLETS];
ALLEGRO_SAMPLE *sample = NULL;
void character_init(){
    // load character images
    score = 0, bullet_count = 0, stage = 1, bbullet_count = 0;
    font = al_load_ttf_font("./font/pirulen.ttf",30,0);
    for(int i = 2 ; i <= 4 ; i++){
        char temp[50];
        //sprintf( temp, "./image/char_move%d.png", i );
        sprintf( temp, "./image/gunman%d.png", i );
        //chara.img_move[i-1] = al_load_bitmap(temp);
        chara.img_move[i-2] = al_load_bitmap(temp);
    }
    for(int i = 1 ; i <= 3 ; i++){
        char temp[50];
        sprintf( temp, "./image/shoot%d.png", i);
        chara.img_atk[i-1] = al_load_bitmap(temp);
    }
    for(int i = 1 ; i <= 3 ; i++){
        char temp[50];
        sprintf( temp, "./image/herodie1%d.png", i );
        chara.img_die[i-1] = al_load_bitmap(temp);
    }
    for(int i = 1 ; i <= 3 ; i++){
        char temp[50];
        sprintf( temp, "./image/duck%d.png", i );
        chara.img_duck[i-1] = al_load_bitmap(temp);
    }
    for(int i = 1 ; i <= 3 ; i++){
        char temp[50];
        sprintf( temp, "./image/bomb%d.png", i );
        chara.img_bomb[i-1] = al_load_bitmap(temp);
    }
    for(int i = 0; i < MAX; i++){
        for(int j = 1; j<=3; j++){
            char temp[50];
            sprintf( temp, "./image/enemy_atk%d.png", j );
            enemy[i].img_atk[j-1] = al_load_bitmap(temp);
        }
    }
    for(int i = 0; i < MAX; i++){
        for(int j = 1; j<=3; j++){
            char temp[50];
            sprintf( temp, "./image/enemy%d.png", j );
            enemy[i].img_move[j-1] = al_load_bitmap(temp);
        }
    }
    for(int i = 0; i < MAX; i++){
        for(int j = 1; j<=3; j++){
            char temp[50];
            sprintf( temp, "./image/die%d.png", j );
            enemy[i].img_die[j-1] = al_load_bitmap(temp);
        }
    }
    // load effective sound
    sample = al_load_sample("./sound/atk_sound.wav");
    chara.atk_Sound  = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(chara.atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(chara.atk_Sound, al_get_default_mixer());

    // initial the geometric information of character
    chara.width = al_get_bitmap_width(chara.img_move[0]);
    chara.height = al_get_bitmap_height(chara.img_move[0]);
    chara.x = 0;
    chara.y = 250;
    chara.dir = true;
    chara.attack1 = 50;
    chara.health = 1000;
    // initial the animation component
    chara.state = STOP;
    chara.life = 3;
    chara.anime = 0;
    chara.anime_time = 30;
    for(int i = 0; i<MAX; i++){
        enemy[i].health = 100;
        enemy[i].state = STOP;
        enemy[i].anime = 0;
        enemy[i].anime_time = 90;
        enemy[i].speed = 1;
        enemy[i].live = false;
        enemy[i].attack1 = 10;
        enemy[i].dir = false;
        enemy[i].shoot = false;
    }

}
void charater_process(ALLEGRO_EVENT event){
    // process the animation
    if(score==3000&&!flag1){
        stage = 2;
        for(int i = 0; i<MAX; i++){
            enemy[i].health = 150;
            enemy[i].state = STOP;
            enemy[i].anime = 0;
            enemy[i].anime_time = 90;
            enemy[i].speed = 3;
            enemy[i].live = false;
            enemy[i].attack1 = 30;
            enemy[i].dir = false;
        }
        flag1 = 1;
    }
    //hero  bullet
    for(int i = 0; i < MAX_BULLETS; i++){
		if(bullet[i].show){
			if(bullet[i].dir==true)bullet[i].x += bullet[i].speed;
			if(bullet[i].dir==false)bullet[i].x -= bullet[i].speed;
			if(bullet[i].x > chara.x+1000 && bullet_count >= MAX_BULLETS){
				bullet[i].show = false;
				bullet_count--;
			}
            if(bullet[i].x < chara.x-1000 && bullet_count >= MAX_BULLETS){
				bullet[i].show = false;
				bullet_count--;
			}
		}
	}
	//enemy bullet
	for(int i = 0; i < MAX_BULLETS; i++){
		if(bbullet[i].show){
			if(bbullet[i].dir==true)bullet[i].x += bbullet[i].speed;
			if(bbullet[i].dir==false)bullet[i].x -= bbullet[i].speed;
			if(bbullet[i].x > chara.x+1000 && bbullet_count >= MAX_BULLETS){
				bbullet[i].show = false;
				bbullet_count--;
			}
            if(bbullet[i].x < chara.x-1000 && bbullet_count >= MAX_BULLETS){
				bbullet[i].show = false;
				bbullet_count--;
			}
		}
	}

	for(int i = 0; i<MAX;i++){
        if(enemy[i].live==false && rand()%2000<1){
            enemy[i].live = true;
            enemy[i].state = MOVE;
            enemy[i].x = chara.x+800;
            enemy[i].y = chara.y;
        }
        if(enemy[i].live ==true &&(enemy[i].x < chara.x-500 )){
            enemy[i].live == false;
        }
        //attack if close
        if(enemy[i].live ==true && enemy[i].x < chara.x+50){
            enemy[i].state = ATK;
        }
        // update bullet
        if(enemy[i].live == true){
            enemy[i].x -= enemy[i].speed;
            if(enemy[i].x < chara.x && enemy[i].state == ATK){
                enemy[i].x += 1.2*enemy[i].speed;
                enemy[i].dir = true;
            }
            if(enemy[i].x > chara.x && enemy[i].state == ATK){
                enemy[i].x -= 0.5*enemy[i].speed;
                enemy[i].dir = false;
            }
        }
        //bullet attack on enemy
        for(int j = 0; j<MAX_BULLETS;j++){
            if(bullet[j].show)
                //printf("bullet now: %d,  enemy now: %d\n",bullet[j].x, enemy[i].x);
            if(bullet[j].show && (bullet[j].x > enemy[i].x-5 && bullet[j].x < enemy[i].x+5) && enemy[i].live){
                enemy[i].health -= chara.attack1;
                //printf("hit!! health: %d\n",enemy[i].health);
                bullet[j].show = false;
            }
            if(enemy[i].health <= 0 ){
                enemy[i].health = 100;
                enemy[i].state = DEAD;
                score+=100;
            }
        }
        /*if(enemy[i].live && !enemy[i].shoot&& enemy[i].x > chara.x+400 && rand()%10 < 1){
            enemy[i].shoot = true;
            enemy[i].state = SHOOT;
            for(int i = 0; i<MAX_BULLETS;i++){
                if(bbullet[i].show!=true){
                    bbullet[i].show = true;
                    bbullet[i].speed = 8;
                    bbullet[i].y = enemy[i].y+20;
                    bbullet[i].dir = enemy[i].dir;
                    if(enemy[i].dir ==false)bullet[i].x = enemy[i].x + 15;
                    else bullet[i].x = enemy[i].x - 15;
                    break;
                }
            }
        }*/
        //bullet attack on hero
        /*for(int j = 0; j<MAX_BULLETS;j++){
            if(bbullet[j].show)
                //printf("bullet now: %d,  enemy now: %d\n",bullet[j].x, enemy[i].x);
            if(bbullet[j].show && (bbullet[j].x > chara.x-5 && bbullet[j].x < chara.x+5)){
                chara.health -= enemy[i].attack1;
                //printf("hit!! health: %d\n",enemy[i].health);
                bbullet[j].show = false;
            }
            if(chara.health <=0){
                chara.health = 1000;
                chara.state = DEAD;
                chara.life--;
            }
            if(chara.life < 0){
                chara.life = 3;
                judge_game_end = true;
            }
        }*/
        //anime-time
        if(enemy[i].anime == enemy[i].anime_time-1){
            enemy[i].anime = 0;
        }
        //character health
        //printf("hero location: %d, enemy location: %d\n", chara.x,enemy[i].x);
        if(chara.x+50 > enemy[i].x && chara.x-50 < enemy[i].x&& enemy[i].live ){
            chara.health -= enemy[i].attack1;
            printf("Hero Health: %d\n", chara.health);
            if(chara.health <=0){
                chara.health = 1000;
                chara.state = DEAD;
                chara.life--;
            }
            if(chara.life < 0){
                chara.life = 3;
                judge_game_end = true;
            }
        }
	}
    if( event.type == ALLEGRO_EVENT_TIMER ){
        if( event.timer.source == fps ){
            chara.anime++;
            chara.anime %= chara.anime_time;
            for(int i = 0; i<MAX; i++){
                enemy[i].anime++;
                enemy[i].anime %= enemy[i].anime_time;
            }
        }
    // process the keyboard event
    }else if( event.type == ALLEGRO_EVENT_KEY_DOWN ){
        key_state[event.keyboard.keycode] = true;
        chara.anime = 0;
    }else if( event.type == ALLEGRO_EVENT_KEY_UP ){
        key_state[event.keyboard.keycode] = false;
    }
}

void charater_update(){
    // use the idea of finite state machine to deal with different state
    if( key_state[ALLEGRO_KEY_W]){
        chara.y -= 5;
        chara.state = MOVE;
    }else if( key_state[ALLEGRO_KEY_A] ){
        chara.dir = false;
        chara.x -= 5;
        chara.state = MOVE;
    }else if( key_state[ALLEGRO_KEY_S] ){
        chara.y += 5;
        chara.state = MOVE;
    }else if( key_state[ALLEGRO_KEY_D] ){
        chara.dir = true;
        chara.x += 5;
        chara.state = MOVE;
    }else if( key_state[ALLEGRO_KEY_SPACE]&&chara.anime%10==0){
        chara.state = ATK;
        for(int i = 0; i<MAX_BULLETS;i++){
            if(bullet[i].show!=true){
                bullet[i].show = true;
                bullet[i].speed = 8;
                bullet[i].y = chara.y+20;
                bullet[i].dir = chara.dir;
                if(chara.dir ==true)bullet[i].x = chara.x + 15;
                else bullet[i].x = chara.x - 15;
                break;
            }
        }
        bullet_count++;
    }else if( key_state[ALLEGRO_KEY_X] ){
        chara.state = DUCK;
    }else if( key_state[ALLEGRO_KEY_B] ){
        chara.state = BOMB;
        for(int i = 0;i <MAX; i++){
            if(chara.x + 250>= enemy[i].x && chara.x + 150 <= enemy[i].x ){
                enemy[i].health -= 100;
            }
            if(enemy[i].health <= 0 ){
                enemy[i].health = 100;
                enemy[i].state = DEAD;
                //score+=100;
            }
        }
    }else if( chara.anime == chara.anime_time-1 ){
        chara.anime = 0;
        chara.state = STOP;
    }else if ( chara.anime == 0 ){
        chara.state = STOP;
    }
}
void character_draw(){
    // with the state, draw corresponding image
    al_draw_text(font, al_map_rgb(255,255,255), 20, 20 , ALLEGRO_ALIGN_LEFT, "HEALTH:");
    al_draw_rectangle(200, 20, 200+300, 50, al_map_rgb(255, 255, 255), 0);
    al_draw_filled_rectangle(200, 20, 200+300*chara.health/1000, 50, al_map_rgb(255,0,0));
    al_draw_filled_rectangle(700, 20, 700+300*chara.life/3, 50, al_map_rgb(0,0,255));
    al_draw_text(font, al_map_rgb(255,255,255), 600, 20 , ALLEGRO_ALIGN_LEFT, "LIFE:");
    al_draw_rectangle(700, 20, 700+300,50, al_map_rgb(255, 255, 255), 0);
    al_draw_textf(font, al_map_rgb(255,255,255), 350, 20 , ALLEGRO_ALIGN_CENTER, "%d/1000",chara.health);
    al_draw_textf(font, al_map_rgb(255,255,255), 850, 20 , ALLEGRO_ALIGN_CENTER, "%d/3",chara.life);
    al_draw_text(font, al_map_rgb(255,255,255), 1100, 20 , ALLEGRO_ALIGN_LEFT, "SCORE:");
    al_draw_rectangle(1250, 20, 1250+300,50, al_map_rgb(255, 255, 255), 0);
    al_draw_textf(font, al_map_rgb(255,255,255), 1400, 20 , ALLEGRO_ALIGN_CENTER, "%d",score);
    al_draw_textf(font, al_map_rgb(255,255,255), 2200, 20 , ALLEGRO_ALIGN_CENTER, "STAGE %d",stage);
    for(int i = 0; i < MAX_BULLETS; i++){
		if(bullet[i].show){
			al_draw_filled_circle(bullet[i].x, bullet[i].y, 5, al_map_rgb(255,0,0));
		}
	}
	for(int i = 0; i < MAX_BULLETS; i++){
		if(bbullet[i].show){
			al_draw_filled_circle(bbullet[i].x, bbullet[i].y, 5, al_map_rgb(0,255,0));
		}
	}
	for(int i = 0; i < MAX; i++){
        if(enemy[i].live==true && enemy[i].state == MOVE){
            if(!enemy[i].dir){
                if( enemy[i].anime < enemy[i].anime_time/3 ){
                    al_draw_bitmap(enemy[i].img_move[0], enemy[i].x, enemy[i].y, ALLEGRO_FLIP_HORIZONTAL);
                }else if( enemy[i].anime < enemy[i].anime_time*2/3 && enemy[i].anime >= enemy[i].anime_time*1/3){
                    al_draw_bitmap(enemy[i].img_move[1], enemy[i].x, enemy[i].y, ALLEGRO_FLIP_HORIZONTAL);
                }
                else{
                    al_draw_bitmap(enemy[i].img_move[2], enemy[i].x, enemy[i].y, ALLEGRO_FLIP_HORIZONTAL);
                }
            }
            else{
                if( enemy[i].anime < enemy[i].anime_time/3 ){
                    al_draw_bitmap(enemy[i].img_move[0], enemy[i].x, enemy[i].y, ALLEGRO_FLIP_HORIZONTAL);
                }else if( enemy[i].anime < enemy[i].anime_time*2/3 && enemy[i].anime >= enemy[i].anime_time*1/3){
                    al_draw_bitmap(enemy[i].img_move[1], enemy[i].x, enemy[i].y, ALLEGRO_FLIP_HORIZONTAL);
                }
                else{
                    al_draw_bitmap(enemy[i].img_move[2], enemy[i].x, enemy[i].y, ALLEGRO_FLIP_HORIZONTAL);
                }
            }
        }
        if(enemy[i].live==true && enemy[i].state == ATK){
            if(enemy[i].dir){
                if( enemy[i].anime < enemy[i].anime_time/3 ){
                    al_draw_bitmap(enemy[i].img_atk[0], enemy[i].x, enemy[i].y, ALLEGRO_FLIP_HORIZONTAL);
                }else if( enemy[i].anime < enemy[i].anime_time*2/3 && enemy[i].anime >= enemy[i].anime_time*1/3){
                    al_draw_bitmap(enemy[i].img_atk[1], enemy[i].x, enemy[i].y, ALLEGRO_FLIP_HORIZONTAL);
                }
                else{
                    al_draw_bitmap(enemy[i].img_atk[2], enemy[i].x, enemy[i].y, ALLEGRO_FLIP_HORIZONTAL);
                }
            }
            else{
                if( enemy[i].anime < enemy[i].anime_time/3 ){
                    al_draw_bitmap(enemy[i].img_atk[0], enemy[i].x, enemy[i].y, 0);
                }else if( enemy[i].anime < enemy[i].anime_time*2/3 && enemy[i].anime >= enemy[i].anime_time*1/3){
                    al_draw_bitmap(enemy[i].img_atk[1], enemy[i].x, enemy[i].y, 0);
                }
                else{
                    al_draw_bitmap(enemy[i].img_atk[2], enemy[i].x, enemy[i].y, 0);
                }
            }
        }
        if( enemy[i].live && enemy[i].state == DEAD){
            if( enemy[i].anime < enemy[i].anime_time/3 ){
                al_draw_bitmap(enemy[i].img_die[0], enemy[i].x, enemy[i].y+20, 0);
            }else if( enemy[i].anime < enemy[i].anime_time*2/3 && enemy[i].anime >= enemy[i].anime_time*1/3){
                al_draw_bitmap(enemy[i].img_die[1], enemy[i].x, enemy[i].y+20, 0);
            }
            else{
                al_draw_bitmap(enemy[i].img_die[2], enemy[i].x, enemy[i].y+20, 0);
                enemy[i].live = false;
            }
        }
	}
    if( chara.state == STOP ){
        if( chara.dir )
            al_draw_bitmap(chara.img_move[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
        else
            al_draw_bitmap(chara.img_move[0], chara.x, chara.y, 0);
    }else if( chara.state == MOVE ){
        if( chara.dir ){
            if( chara.anime < chara.anime_time/3 ){
                al_draw_bitmap(chara.img_move[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
            }else if( chara.anime < chara.anime_time*2/3 && chara.anime >= chara.anime_time*1/3){
                al_draw_bitmap(chara.img_move[1], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
            }
            else{
                al_draw_bitmap(chara.img_move[2], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
            }
        }else{
            if( chara.anime < chara.anime_time/3 ){
                al_draw_bitmap(chara.img_move[0], chara.x, chara.y, 0);
            }else if( chara.anime < chara.anime_time*2/3 && chara.anime >= chara.anime_time*1/3){
                al_draw_bitmap(chara.img_move[1], chara.x, chara.y, 0);
            }
            else{
                al_draw_bitmap(chara.img_move[2], chara.x, chara.y, 0);
            }
        }
    }else if( chara.state == DUCK ){
        if( !chara.dir ){
            if( chara.anime < chara.anime_time/3 ){
                al_draw_bitmap(chara.img_duck[0], chara.x, chara.y+15, ALLEGRO_FLIP_HORIZONTAL);
            }else if( chara.anime < chara.anime_time*2/3 && chara.anime >= chara.anime_time*1/3){
                al_draw_bitmap(chara.img_duck[1], chara.x, chara.y+15, ALLEGRO_FLIP_HORIZONTAL);
            }
            else{
                al_draw_bitmap(chara.img_duck[2], chara.x, chara.y+15, ALLEGRO_FLIP_HORIZONTAL);
            }
        }else{
            if( chara.anime < chara.anime_time/3 ){
                al_draw_bitmap(chara.img_duck[0], chara.x, chara.y+15, 0);
            }else if( chara.anime < chara.anime_time*2/3 && chara.anime >= chara.anime_time*1/3){
                al_draw_bitmap(chara.img_duck[1], chara.x, chara.y+15, 0);
            }
            else{
                al_draw_bitmap(chara.img_duck[2], chara.x, chara.y+15, 0);
            }
        }
    }else if( chara.state == ATK ){
        al_play_sample_instance(chara.atk_Sound);
        if( chara.dir ){
            if( chara.anime < chara.anime_time/3 ){
                al_draw_bitmap(chara.img_atk[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
            }else if( chara.anime < chara.anime_time*2/3 && chara.anime >= chara.anime_time*1/3){
                al_draw_bitmap(chara.img_atk[1], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
            }
            else{
                al_draw_bitmap(chara.img_atk[2], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
            }
        }else{
            if( chara.anime < chara.anime_time/3 ){
                al_draw_bitmap(chara.img_atk[0], chara.x, chara.y, 0);
            }else if( chara.anime < chara.anime_time*2/3 && chara.anime >= chara.anime_time*1/3){
                al_draw_bitmap(chara.img_atk[1], chara.x, chara.y, 0);
            }
            else{
                al_draw_bitmap(chara.img_atk[2], chara.x, chara.y, 0);
            }
        }
    }
    else if( chara.state == BOMB ){
        //al_play_sample_instance(chara.atk_Sound);
        if( !chara.dir ){
            if( chara.anime < chara.anime_time/3 ){
                al_draw_bitmap(chara.img_bomb[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
            }else if( chara.anime < chara.anime_time*2/3 && chara.anime >= chara.anime_time*1/3){
                al_draw_bitmap(chara.img_bomb[1], chara.x, chara.y-30, ALLEGRO_FLIP_HORIZONTAL);
            }
            else{
                al_draw_bitmap(chara.img_bomb[2], chara.x -200, chara.y-50, ALLEGRO_FLIP_HORIZONTAL);
            }
        }else{
            if( chara.anime < chara.anime_time/3 ){
                al_draw_bitmap(chara.img_bomb[0], chara.x, chara.y, 0);
            }else if( chara.anime < chara.anime_time*2/3 && chara.anime >= chara.anime_time*1/3){
                al_draw_bitmap(chara.img_bomb[1], chara.x, chara.y-30, 0);
            }
            else{
                al_draw_bitmap(chara.img_bomb[2], chara.x+200, chara.y-50, 0);
            }
        }
    }
    else if( chara.state == DEAD ){
        //al_play_sample_instance(chara.atk_Sound);
        if( !chara.dir ){
            if( chara.anime < chara.anime_time/3 ){
                al_draw_bitmap(chara.img_die[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
            }else if( chara.anime < chara.anime_time*2/3 && chara.anime >= chara.anime_time*1/3){
                al_draw_bitmap(chara.img_die[1], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
            }
            else{
                al_draw_bitmap(chara.img_die[2], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
                if(chara.life < 0){
                    judge_game_end = true;
                    chara.life = 3;
                }
            }
        }else{
            if( chara.anime < chara.anime_time/3 ){
                al_draw_bitmap(chara.img_die[0], chara.x, chara.y, 0);
            }else if( chara.anime < chara.anime_time*2/3 && chara.anime >= chara.anime_time*1/3){
                al_draw_bitmap(chara.img_die[1], chara.x, chara.y, 0);
            }
            else{
                al_draw_bitmap(chara.img_die[2], chara.x, chara.y, 0);
                if(chara.life <0){
                    chara.life = 3;
                    judge_game_end = true;
                }
            }
        }
    }
}

void character_destory(){
    al_destroy_bitmap(chara.img_atk[0]);
    al_destroy_bitmap(chara.img_atk[1]);
    al_destroy_bitmap(chara.img_atk[2]);
    al_destroy_bitmap(chara.img_move[0]);
    al_destroy_bitmap(chara.img_move[1]);
    al_destroy_bitmap(chara.img_move[2]);
    al_destroy_bitmap(chara.img_die[0]);
    al_destroy_bitmap(chara.img_die[1]);
    al_destroy_bitmap(chara.img_die[2]);
    al_destroy_bitmap(chara.img_duck[0]);
    al_destroy_bitmap(chara.img_duck[1]);
    al_destroy_bitmap(chara.img_duck[2]);
    al_destroy_bitmap(chara.img_bomb[0]);
    al_destroy_bitmap(chara.img_bomb[1]);
    al_destroy_bitmap(chara.img_bomb[2]);
    for(int i = 0;i<MAX;i++){
        al_destroy_bitmap(enemy[i].img_move[0]);
        al_destroy_bitmap(enemy[i].img_move[1]);
        al_destroy_bitmap(enemy[i].img_move[2]);
        al_destroy_bitmap(enemy[i].img_atk[0]);
        al_destroy_bitmap(enemy[i].img_atk[1]);
        al_destroy_bitmap(enemy[i].img_atk[2]);
        al_destroy_bitmap(enemy[i].img_die[0]);
        al_destroy_bitmap(enemy[i].img_die[1]);
        al_destroy_bitmap(enemy[i].img_die[2]);
    }
    al_destroy_sample_instance(chara.atk_Sound);
}
