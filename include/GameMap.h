#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "Baseobject.h"

#define TILE_SIZE 64
#define MAX_MAP_X 400
#define MAX_MAP_Y 11
#define MAX_TILES 20
#define BLANK_TILE 0
#define MONEY 4
#define HEALTH 6
#define SPHERE 7
#define LASER 8
#define FIRE 9
#define WIND 10
#define EXIT 11
typedef struct Map
{
    int start_x_;
    int start_y_;

    int max_x_;
    int max_y_;

    int tile[MAX_MAP_Y][MAX_MAP_X];
    char* file_name_;
};
class TileMat : public BaseObject
{
public:
    TileMat(){;}
    ~TileMat(){;}
};

class GameMap
{
public:
    GameMap(){;}
    ~GameMap(){;}

    void LoadMap(char* file);
    void LoadTiles(SDL_Renderer* screen);
    void DrawMap(SDL_Renderer* screen);
    Map GetMap() const {return game_map;}
    void SetMap(Map& map_data){game_map = map_data;}

private:
    Map game_map;
    TileMat tile_mat[MAX_TILES];
};
#endif // GAMEMAP_H
