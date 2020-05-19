#ifndef PLAYERMONEY_H
#define PLAYERMONEY_H

#include "BaseObject.h"

class PlayerMoney : public BaseObject
{
public:
    PlayerMoney();
    ~PlayerMoney();
    void Init(SDL_Renderer* screen);
    void Show(SDL_Renderer* screen);
    void SetPos(const int& x,const int& y) {x_pos_ = x; y_pos_ = y;}

private:
    int x_pos_;
    int y_pos_;
};

#endif // PLAYERMONEY_H
