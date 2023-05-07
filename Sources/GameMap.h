#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "CommonFunc.h"
#include "BaseObject.h"
#define MAX_TILES 20

class TileMap : public BaseObject{  //cac hinh anh tinh
public:
    TileMap(){}
    ~TileMap(){}

};
class GameMap{ //xay dung ban do gan cac tilemap vào de tao ra map
public:
    GameMap(){}
    ~GameMap(){}
    void LoadMap(char* name);
    void LoadTiles(SDL_Renderer* screen);
    void DrawMap(SDL_Renderer* screen);
    Map getMap()const{return game_map_;};
    void SetMap(Map& map_data){game_map_=map_data;};
private:
    Map game_map_;//chua toan bo cac thong tin ve ban do
    TileMap tile_map[MAX_TILES];
};

#endif // GAMEMAP_H
