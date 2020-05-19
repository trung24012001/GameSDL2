#ifndef THREATOBJECT_H
#define THREATOBJECT_H

#include "BaseObject.h"
#include "BulletObject.h"
#include "GameMap.h"


#define THREAT_SPEED 3
#define NUM_FRAME_THREAT 8
class ThreatObject : public BaseObject
{
public:
    ThreatObject();
    ~ThreatObject();
    enum TypeMove
    {
        STATIC_THREAT = 10,
        MOVE_THREAT = 11,
    };
    void set_x_val(const float& xVal){x_val_ = xVal;}
    void set_y_val(const float& yVal){y_val_ = yVal;}
    void set_x_pos(const float& xPos){x_pos_ = xPos;}
    void set_y_pos(const float& yPos){y_pos_ = yPos;}
    float get_x_pos() const {return x_pos_;}
    float get_y_pos() const {return y_pos_;}
    void SetMapXY(const int& map_x, const int& map_y){map_x_ = map_x; map_y_ = map_y;}
    void SetClips();
    bool LoadImg(string path, SDL_Renderer* screen);
    void Show(SDL_Renderer* des);
    int get_width_frame() const {return width_frame_;}
    int get_height_frame() const {return height_frame_;}
    void GoThreat(Map& gMap);
    void CheckMap(Map& map_data);
    void set_type_move(const int& typeMove){type_move_ = typeMove;}
    void SetAnimationPos(const int& pos_a, const int& pos_b){animation_a_ = pos_a; animation_b_ = pos_b;}
    void set_input_left(const int& inputLeft){input_type.left_ = inputLeft;}
    void ImpMoveType(SDL_Renderer* screen);
    void InitThreat();
    vector<BulletObject*> get_bullet_list() const {return bullet_list;}
    void set_bullet_list(const vector<BulletObject*> &b_list){bullet_list = b_list;}
    void InitBullet(BulletObject* p_bullet, SDL_Renderer* screen);
    void MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit);
    void RemoveBullet(const int& idx);
    SDL_Rect GetRectFrame();


private:
    int frame_;
    int width_frame_;
    int height_frame_;
    SDL_Rect frame_clip_[NUM_FRAME_THREAT];
    bool on_ground_;
    int come_back_time_;
    float x_val_;
    float y_val_;
    float x_pos_;
    float y_pos_;
    int map_x_;
    int map_y_;
    int type_move_;
    int animation_a_;
    int animation_b_;
    Input input_type;
    vector<BulletObject*> bullet_list;
};

#endif // THREATOBJECT_H
