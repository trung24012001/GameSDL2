#ifndef MAINOBJECT_H
#define MAINOBJECT_H


#include "BaseObject.h"
#include "BulletObject.h"
#include "GameMap.h"
#include "PlayerHealth.h"

#define PLAYER_SPEED 5
#define JUMP_VAL 17
#define NUM_FRAME_MAIN 8
#define NUM_HEALTH_MAIN 3


class MainObject : public BaseObject
{

public:
    MainObject();
    ~MainObject();
    enum WalkType
    {
        WALK_NONE = 0,
        WALK_RIGHT = 1,
        WALK_LEFT = 2,
    };

    bool LoadImg(string path, SDL_Renderer* screen);
    void Show(SDL_Renderer* des);
    void HandleInputAction(SDL_Event events, SDL_Renderer* screen);
    void SetClips();
    void GoPlayer(Map& map_data);
    void CheckMap(Map& map_data);
    void SetMapXY(const int map_x, const int map_y){map_x_ = map_x; map_y_ = map_y;}
    void CenterEntityOnMap(Map& map_data);
    void UpdateImage(SDL_Renderer* des);
    void set_bullet_list(vector<BulletObject*> bullet_list){p_bullet_list_ = bullet_list;}
    vector<BulletObject*> get_bullet_list() const {return p_bullet_list_;}
    void HandleBullet(SDL_Renderer* des);
    void RemoveBullet(const int& idx);
    bool Immortal();
    SDL_Rect GetRectFrame();
    void ItemLoot();
    void set_come_back_time(const int& cb) {come_back_time_ = cb;}
    int get_money_loot() const{return money_loot_;}
    void set_health_up(const bool& up){health_up_ = up;}
    bool get_health_up() const {return health_up_;}
    void set_health_down(const bool& down){health_down_ = down;}
    bool get_health_down() const {return health_down_;}

    void set_x_pos(const int& xp) {x_pos_ = xp;}
    bool get_exit() const {return exit_;}



private:
    float x_val_;
    float y_val_;
    float x_pos_;
    float y_pos_;
    int width_frame_;
    int height_frame_;
    SDL_Rect frame_clip_[8];
    Input input_type;
    int frame_;
    int status_;
    bool on_ground_;
    int map_x_;
    int map_y_;
    int come_back_time_;
    int money_loot_;
    int status_bullet_;
    bool immortal_;
    int start_immortal_;
    int power_bullet_;
    int max_bullet_list_;
    int item_bullet_;
    bool health_up_;
    bool health_down_;
    bool player_attack_default_;
    bool player_attack_sphere_;
    bool player_attack_laser_;
    bool player_attack_fire_;
    bool player_attack_wind_;
    bool exit_;

    vector<BulletObject*> p_bullet_list_;
};

#endif // MAINOBJECT_H
