#include "PlayerHealth.h"

PlayerHealth::PlayerHealth()
{
    number_ = 0;

}
PlayerHealth::~PlayerHealth()
{

}
void PlayerHealth::AddPos(const int& xPos)
{
    pos_list_.push_back(xPos);
}

void PlayerHealth::Init(SDL_Renderer* screen)
{
    LoadImg("img//player_pw.png", screen);
    number_ = NUM_HEALTH_MAIN;
    if(pos_list_.size() > 0)
    {
        pos_list_.clear();
    }
    for (int i=0; i < NUM_HEALTH_MAIN; i++)
    {
        AddPos(20 + i*40);
    }
}

void PlayerHealth::Show(SDL_Renderer* screen)
{
    for (int i=0; i < pos_list_.size(); i++)
    {
        rect_.x = pos_list_[i];
        rect_.y = 0;
        Render(screen);
    }
}
void PlayerHealth::Decrease()
{
    number_--;
    pos_list_.pop_back();
}
void PlayerHealth::Increase()
{
    number_++;
    int last_pos = pos_list_.back();
    last_pos += 40;
    pos_list_.push_back(last_pos);
}
