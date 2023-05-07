#ifndef MAINOBJECT_H
#define MAINOBJECT_H

#include "CommonFunc.h"
#include "BaseObject.h"

#define GRAVITY_SPEED 0.9
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 9
#define PLAYER_JUMP_VAL 17

class MainObject : public BaseObject{
public:
    MainObject();
    ~MainObject();

    enum WalkType{
        WALK_NONE=0,
        WALK_RIGHT=1,
        WALK_LEFT=2,
    };
    bool LoadImg(std::string path,SDL_Renderer* screen);
    void Show(SDL_Renderer* des);
    void HandleInputAction(SDL_Event events, SDL_Renderer* screen);
    void set_clips();

    void DoPlayer(Map& map_data, Mix_Chunk* g_money);
    void CheckToMap(Map& map_data, Mix_Chunk* g_money);
    void SetMapXY(const int map_x, const int map_y){map_x_=map_x; map_y_=map_y;}
    void CenterEtityOnMap(Map& map_data);
    void UpdateImagePlayer(SDL_Renderer* des);
    SDL_Rect GetRectFrame();
    void IncreaseMoney();
    int get_frame_width()const{return width_frame_;}
    int get_frame_height()const{return height_frame_;}
    void set_comeback_time(const int& cb_time){come_back_time_=cb_time;}
    int GetMoneyCount()const{return money_count;}
    bool GetWin(){return win;}
private:
    int money_count;
    float x_val_;
    float y_val_;

    float x_pos_;
    float y_pos_;

    int width_frame_;
    int height_frame_;

    SDL_Rect frame_clip_[6];
    Input input_type_;
    int frame_;
    int status_;
    bool on_ground_;

    int map_x_;
    int map_y_;

    int come_back_time_;
    bool win;
};

#endif // MAINOBJECT_H
