#ifndef BOSSOBJECT_H
#define BOSSOBJECT_H

#include "BaseObject.h"
#include "BulletObject.h"
#include "GameMap.h"

#define NUM_FRAME_BOSS 32
#define SPEED_BULLET 10
#define NUM_HEALTH_BOSS 20

class BossObject : public BaseObject
{
public:
    BossObject();
    ~BossObject();
    void set_x_val(const float& xVal){x_val_ = xVal;}
    void set_y_val(const float& yVal){y_val_ = yVal;}
    void set_x_pos(const float& xPos){x_pos_ = xPos;}
    void set_y_pos(const float& yPos){y_pos_ = yPos;}
    float get_x_pos() const {return x_pos_;}
    float get_y_pos() const {return y_pos_;}
    int get_width_frame() const {return width_frame_;}
    int get_height_frame() const {return height_frame_;}
    void SetMapXY(const int& map_x, const int& map_y){map_x_ = map_x; map_y_ = map_y;}
    void SetClips();
    bool LoadImg(string path, SDL_Renderer* screen);
    void Show(SDL_Renderer* des);
    void GoBoss(Map& gMap);
    void CheckMap(Map& map_data);
    void InitBoss();
    vector<BulletObject*> get_bullet_list() const {return bullet_list;}
    void set_bullet_list(const vector<BulletObject*> &b_list){bullet_list = b_list;}
    void InitBullet(SDL_Renderer* screen);
    void MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit);
    void RemoveBullet(const int& idx);
    SDL_Rect GetRectFrame();


private:
    int frame_;
    int width_frame_;
    int height_frame_;
    SDL_Rect frame_clip_[NUM_FRAME_BOSS];
    bool on_ground_;
    float x_val_;
    float y_val_;
    float x_pos_;
    float y_pos_;
    int map_x_;
    int map_y_;
    int think_time_;
    Input input_type;
    vector<BulletObject*> bullet_list;
};

#endif // BOSSOBJECT_H
