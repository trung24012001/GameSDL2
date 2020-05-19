#ifndef PLAYERHEALTH_H
#define PLAYERHEALTH_H


#include "BaseObject.h"
#include "MainObject.h"

class PlayerHealth : public BaseObject
{
public:
    PlayerHealth();
    ~PlayerHealth();
    void SetNum(const int& num) {number_ = num;}
    int GetNumHealth() const {return pos_list_.size();}
    void AddPos(const int& xPos);
    void Show(SDL_Renderer* screen);
    void Init(SDL_Renderer* screen);
    void Increase();
    void Decrease();

private:
    int number_;
    vector<int> pos_list_;
};

#endif // PLAYERHEALTH_H
