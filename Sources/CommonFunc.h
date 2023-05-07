#ifndef COMMONFUNC_H
#define COMMONFUNC_H

#include<windows.h>
#include<string>
#include<vector>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include "BaseObject.h"
#include "TextObject.h"

static SDL_Window* g_window=NULL;
static SDL_Renderer* g_screen=NULL;
static SDL_Event g_event;

static Mix_Music* g_music=NULL;
static Mix_Chunk* g_threat=NULL;
static Mix_Chunk* g_money=NULL;
static Mix_Chunk* g_click=NULL;

//Screen
const int FRAME_PER_SECOND=30; //fps
const int SCREEN_WIDTH=1280;
const int SCREEN_HEIGHT=640;
const int SCRREN_BPP=32;

const int COLOR_KEY_R=167;
const int COLOR_KEY_G=175;
const int COLOR_KEY_B=180;

const int RENDER_DRAW_COLOR = 0xff;

#define TILE_SIZE 64
#define MAX_MAP_X 400
#define MAX_MAP_Y 10

#define BLANK_TILE 0

#define STATE_MONEY 4

#define WIN 10

typedef struct Input{
    int left_;
    int right_;
    int up_;
    int down_;
    int jump_;
};

typedef struct Map{
    int start_x_;
    int start_y_;
    int max_x_;
    int max_y_;

    int tile[MAX_MAP_Y][MAX_MAP_X];
    char* file_name_;
};

namespace SDLCommonFunc{
    bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
    int ShowMenu(SDL_Renderer* g_screen, TTF_Font* font, const std::string& menu1, const std::string& menu2,const std::string& img_name, Mix_Chunk* g_click);
}

#endif // COMMONFUNC_H
