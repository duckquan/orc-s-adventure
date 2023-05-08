#include <iostream>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "GameMap.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatsObject.h"
#include "ExplosionObject.h"
#include "TextObject.h"
#include "PlayerPower.h"

BaseObject g_background;
TTF_Font* font_time=NULL;
TTF_Font* g_font_MENU = NULL;

bool InitData(){
    bool success = true;
    int ret=SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    if(ret<0) return false;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");
    g_window = SDL_CreateWindow("DuckQuan's GameSDL",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(g_window==NULL){
        success=false;
    }
    else{
        g_screen=SDL_CreateRenderer(g_window,1,SDL_RENDERER_ACCELERATED);
        if(g_screen==NULL){
            success=false;
        }
        else{
            SDL_SetRenderDrawColor(g_screen,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR);
            int imgFlags=IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags)&&imgFlags)) success = false;
        }

        if(TTF_Init()== -1){
            success=false;
        }

        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
            success = false;
        }

        font_time=TTF_OpenFont("font/dlxfont_.ttf", 15);
        if(font_time==NULL){
            success=false;
        }
        g_font_MENU = TTF_OpenFont("font/dlxfont_.ttf", 80);
        if (g_font_MENU == NULL){
            success=false;
        }

        g_music = Mix_LoadMUS( "sound/music.wav" );
        if( g_music == NULL ){
        success = false;
        }

        g_threat = Mix_LoadWAV( "sound/threat.wav" );
        if( g_threat == NULL ){
        success = false;
        }

        g_money = Mix_LoadWAV( "sound/money.wav" );
        if( g_money == NULL ){
        success = false;
        }

        g_click = Mix_LoadWAV( "sound/click.wav" );
        if( g_click == NULL ){
        success = false;
        }
    }
    return success;
}

bool LoadBackGround(){
    bool ret=g_background.LoadImg("img/Background.png",g_screen);
    if(ret==false) return false;
    return true;
}
void close(){
    Mix_FreeChunk(g_threat);
    Mix_FreeChunk(g_money);
    g_threat=NULL;
    g_money=NULL;
    Mix_FreeMusic(g_music);
    g_music = NULL;
    g_background.Free();
    SDL_DestroyRenderer(g_screen);
    g_screen=NULL;
    SDL_DestroyWindow(g_window);
    g_window=NULL;
    IMG_Quit();
    SDL_Quit();
}

std::vector<ThreatsObject*> MakeThreatList(){
    std::vector<ThreatsObject*> list_threats;

    ThreatsObject* dynamic_threats = new ThreatsObject[20];
    for(int i=0;i<20;i++){
        ThreatsObject* p_threat = (dynamic_threats + i);
        if(p_threat!=NULL){
            p_threat->LoadImg("img/threat_left.png",g_screen);
            p_threat->set_clips();
            p_threat->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);
            p_threat->set_x_pos(600+i*1000);
            p_threat->set_y_pos(200);
            int pos1=p_threat->get_x_pos()-60;
            int pos2=p_threat->get_x_pos()+60;
            p_threat->SetAnimationPos(pos1, pos2);
            p_threat->set_input_left(1);
            list_threats.push_back(p_threat);
        }
    }

    ThreatsObject* threats_objs = new ThreatsObject[15];
    for(int i=0;i<15;i++){
        ThreatsObject* p_threat= (threats_objs + i);
        if(p_threat!=NULL){
            p_threat->LoadImg("img/threat_level.png",g_screen);
            p_threat->set_clips();
            p_threat->set_x_pos(700+i*1500);
            p_threat->set_y_pos(250);
            p_threat->set_input_left(0);
            p_threat->set_type_move(ThreatsObject::STATIC_THREAT);
            list_threats.push_back(p_threat);
        }
    }
    return list_threats;
}

int main(int argc, char* argv[]){
    ImpTimer fps_timer;
    bool is_quit=false;
    bool win_game=false;

    if(InitData()==false) return -1;
    if(LoadBackGround()==false) return -1;

    //Make menu game
    Mix_PlayMusic(g_music, 0);
    int ret_menu = SDLCommonFunc::ShowMenu(g_screen, g_font_MENU, "Play Game", "Exit", "img/background.png", g_click);
    if (ret_menu == 1) is_quit = true;
    again_label:


    GameMap game_map;
    game_map.LoadMap("map/map01.dat");
    game_map.LoadTiles(g_screen);

    MainObject p_player;
    p_player.LoadImg("img/player_right.png",g_screen);
    p_player.set_clips();

    PlayerPower player_power;
    player_power.Init(g_screen);

    PlayerMoney player_money;
    player_money.Init(g_screen);
    player_money.SetPos(SCREEN_WIDTH*0.5-290, 8);

    std::vector<ThreatsObject*> threats_list = MakeThreatList();

    ExplosionObject exp_main;
    bool tRet=exp_main.LoadImg("img/exp.png", g_screen);
    if(!tRet) return -1;
    exp_main.set_clip();

    int num_die=0;

    //Time Text
    TextObject time_game;
    time_game.SetColor(TextObject::WHITE_TEXT);

    TextObject money_game;
    money_game.SetColor(TextObject::WHITE_TEXT);

    while(!is_quit){ // render thi de trong vong while

        fps_timer.start();
        while(SDL_PollEvent(&g_event)!=0){
            if(g_event.type==SDL_QUIT){
                is_quit = true;
            }
            p_player.HandleInputAction(g_event, g_screen);
        }
        SDL_SetRenderDrawColor(g_screen,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);

        g_background.Render(g_screen,NULL);
        Map map_data= game_map.getMap();

        p_player.SetMapXY(map_data.start_x_,map_data.start_y_);
        p_player.DoPlayer(map_data, g_money);
        p_player.Show(g_screen);

        game_map.SetMap(map_data);
        game_map.DrawMap(g_screen);

        player_power.Show(g_screen);
        player_money.Show(g_screen);

        for(int i=0;i<threats_list.size();i++){
            ThreatsObject* p_threat = threats_list.at(i);
            if(p_threat!=NULL){
                p_threat->SetMapXY(map_data.start_x_,map_data.start_y_);
                p_threat->ImpMoveType(g_screen);
                p_threat->DoPlayer(map_data);
                p_threat->Show(g_screen);

                SDL_Rect rect_player=p_player.GetRectFrame();
                SDL_Rect rect_threat=p_threat->GetRectFrame();
                bool bCol=SDLCommonFunc::CheckCollision(rect_player,rect_threat);
                if(bCol){
                    int frame_exp_width=exp_main.get_frame_width();
                    int frame_exp_height=exp_main.get_frame_heigh();

                    for(int ex=0;ex < NUM_FRAME_EXP;ex++){
                        int x_pos=(p_player.GetRect().x+p_player.get_frame_width()*0.5)-frame_exp_width*0.5;
                        int y_pos=(p_player.GetRect().y+p_player.get_frame_height()*0.5)-frame_exp_height*0.5;

                        exp_main.set_frame(ex);
                        exp_main.SetRect(x_pos, y_pos);
                        exp_main.Show(g_screen);
                        SDL_RenderPresent(g_screen);
                    }
                    Mix_PlayChannel(-1, g_threat, 0);
                    num_die++;
                    if(num_die<3){
                        p_player.SetRect(0,0);
                        p_player.set_comeback_time(40);
                        SDL_Delay(500);
                        player_power.Decrease();
                        player_power.Render(g_screen);
                        continue;
                    }
                    else{
                        Sleep(500);
                        int ret_menu = SDLCommonFunc::ShowMenu(g_screen, g_font_MENU,"Player Again", "Exit","img/background.png", g_click);
                        if (ret_menu == 1){
                           is_quit = true;
                           continue;
                        }
                        else{
                           is_quit = false;
                           goto again_label;
                        }
                    }
                }
            }
        }

        win_game=p_player.GetWin();
        if(win_game){
            Sleep(500);
            int ret_menu = SDLCommonFunc::ShowMenu(g_screen, g_font_MENU,"Player Again", "Exit","img/background.png", g_click);
            if (ret_menu == 1){
                is_quit = true;
                continue;
            }
            else{
                is_quit = false;
                goto again_label;
            }
        }

        //Show game time
        std::string str_time="Time: ";
        Uint32 time_val=SDL_GetTicks()/1000;
        Uint32 val_time=300-time_val;
        if(val_time<=0){
            Sleep(500);
            int ret_menu = SDLCommonFunc::ShowMenu(g_screen, g_font_MENU,"Player Again", "Exit","img/background.png", g_click);
            if (ret_menu == 1){
                is_quit = true;
                continue;
            }
            else{
                is_quit = false;
                goto again_label;
            }
        }
        else{
            std::string str_val= std::to_string(val_time);
            str_time+=str_val;

            time_game.SetText(str_time);
            time_game.LoadFromRenderText(font_time, g_screen);
            time_game.RenderText(g_screen, SCREEN_WIDTH-200, 15);
        }

        int money_count = p_player.GetMoneyCount();
        std::string money_str = std::to_string(money_count);

        money_game.SetText(money_str);
        money_game.LoadFromRenderText(font_time, g_screen);
        money_game.RenderText(g_screen, SCREEN_WIDTH*0.5-250, 15);

        SDL_RenderPresent(g_screen);
        while(money_count%6==0&&money_count!=0){
            num_die--;
            player_power.InitCrease();
            break;
        }

        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame=1000/FRAME_PER_SECOND;

        if(real_imp_time<time_one_frame){
            int delay_time = time_one_frame - real_imp_time;
            if(delay_time>=0) SDL_Delay(delay_time);
        }
    }
    close();
    return 0;
}
