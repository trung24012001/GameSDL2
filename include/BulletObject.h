#ifndef BULLETOBJECT_H
#define BULLETOBJECT_H

#include "BaseObject.h"

class BulletObject : public BaseObject
{
public:
    BulletObject();
    ~BulletObject();
    enum BulletDir
    {
        DIR_RIGHT = 20,
        DIR_LEFT = 21,
    };
    enum BulletType
    {
        DEFAULT_SWORD = 50,
        SPHERE_BULLET = 51,
        LASER_BULLET = 52,
        FIRE_BULLET = 53,
        WIND_BULLET = 54,

    };
    void set_x_val(const int& xval){x_val_ = xval;}
    void set_y_val(const int& yval){y_val_ = yval;}
    int get_x_val() const {return x_val_;}
    int get_y_val() const {return y_val_;}
    void set_is_move(const bool& isMove){is_move_ = isMove;}
    bool get_is_move() const {return is_move_ ;}
    void HandleMove(const int& x_border, const int& y_border);
    void set_bullet_dir(const unsigned int bulletDir){bullet_dir_ = bulletDir;}
    int get_bullet_dir() const {return bullet_dir_;}
    void set_bullet_type(const int& bullet_type){bullet_type_ = bullet_type;}
    int get_bullet_type() const {return bullet_type_;}
    bool LoadImgBullet(SDL_Renderer* des);

private:
    int x_val_;
    int y_val_;
    bool is_move_;
    unsigned int bullet_dir_;
    unsigned int bullet_type_;
};

#endif // BULLETOBJECT_H
