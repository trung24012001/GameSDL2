#include "ThreatObject.h"


ThreatObject::ThreatObject()
{
    width_frame_ = 0;
    height_frame_ = 0;
    x_val_ = 0;
    y_val_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    map_x_ = 0;
    map_y_ = 0;
    on_ground_ = false;
    come_back_time_ = 0;
    frame_ = 0;
    animation_a_ = 0;
    animation_b_ = 0;
    input_type.left_ = 0;
    type_move_ = STATIC_THREAT;

}
ThreatObject::~ThreatObject()
{
    if(bullet_list.size() > 0)
    {
        for (int i=0; i < bullet_list.size(); i++)
        {
            BulletObject* p_bullet = bullet_list[i];
            bullet_list.erase(bullet_list.begin() + i);
            if(p_bullet != NULL)
            {
                p_bullet->Free();
                delete p_bullet;
                p_bullet = NULL;
            }
            bullet_list.clear();
        }
    }

}
bool ThreatObject::LoadImg(string path, SDL_Renderer* screen)
{
    Free();
    bool ret = BaseObject::LoadImg(path, screen);
    if(ret == true)
    {
        width_frame_ = rect_.w/NUM_FRAME_THREAT;
        height_frame_ = rect_.h;
    }
    return ret;
}
void ThreatObject::SetClips()
{
    if(width_frame_ > 0 && height_frame_ > 0)
    {
        for (int i=0; i < NUM_FRAME_THREAT; i++ )
        {
            frame_clip_[i].x = i*width_frame_;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}
void ThreatObject::Show(SDL_Renderer* des)
{
    if(come_back_time_ == 0)
    {
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;
        frame_++;
        if(frame_ >= 8)
        {
            frame_ = 0;
        }
        SDL_Rect* currentClip = &frame_clip_[frame_];
        SDL_Rect rendQuad = {rect_.x, rect_.y, width_frame_, height_frame_};
        SDL_RenderCopy(des, p_object_, currentClip, &rendQuad);
    }
}
void ThreatObject::InitThreat()
{
    x_val_ = 0;
    y_val_ = 0;
    y_pos_ = 0;

    input_type.left_ = 1;
    if(x_pos_ > 256)
    {
        x_pos_ -= 256;
        animation_a_ -= 256;
        animation_b_ -= 256;
    }
    else
    {
        x_pos_ = 0;
    }
}
void ThreatObject::GoThreat(Map& gMap)
{
    if(come_back_time_ == 0)
    {
        x_val_ = 0;
        y_val_ += GRAVITY_SPEED;
        if(y_val_ >= MAX_FALL_SPEED)
        {
            y_val_ = MAX_FALL_SPEED;
        }
        if(input_type.left_ == 1)
        {
            x_val_ -= THREAT_SPEED;
        }
        else if(input_type.right_ == 1)
        {
            x_val_ += THREAT_SPEED;
        }
        CheckMap(gMap);
    }
    else if(come_back_time_ > 0)
    {
        come_back_time_--;
        if(come_back_time_ == 0)
        {
            InitThreat();
        }
    }
}
void ThreatObject::CheckMap(Map& map_data)
{
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

    int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;
    x1 = (x_pos_ + x_val_)/TILE_SIZE;
    x2 = (x_pos_ + x_val_ + width_frame_ - 1)/TILE_SIZE;
    y1 = (y_pos_)/TILE_SIZE;
    y2 = (y_pos_ + height_min - 1)/TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if(x_val_ > 0)
        {
            int val_1 = map_data.tile[y1][x2];
            int val_2 = map_data.tile[y2][x2];

            if((val_1 != BLANK_TILE && val_1 != MONEY && val_1 != HEALTH && val_1 != SPHERE && val_1 != LASER && val_1 != FIRE) ||
               (val_2 != BLANK_TILE && val_2 != MONEY && val_2 != HEALTH && val_2 != SPHERE && val_2 != LASER && val_2 != FIRE))
            {


                x_pos_ = x2*TILE_SIZE;
                x_pos_ -= (width_frame_ + 1);
                x_val_ = 0;
            }
        }
        else if(x_val_ < 0)
        {
            int val_1 = map_data.tile[y1][x1];
            int val_2 = map_data.tile[y2][x1];

            if((val_1 != BLANK_TILE && val_1 != MONEY && val_1 != HEALTH && val_1 != SPHERE && val_1 != LASER && val_1 != FIRE) ||
               (val_2 != BLANK_TILE && val_2 != MONEY && val_2 != HEALTH && val_2 != SPHERE && val_2 != LASER && val_2 != FIRE))
            {
                x_pos_ = (x1 + 1)*TILE_SIZE;
                x_val_ = 0;
            }
        }
    }

    int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
    x1 = (x_pos_)/TILE_SIZE;
    x2 = (x_pos_ + width_min - 1)/TILE_SIZE;
    y1 = (y_pos_ + y_val_)/TILE_SIZE;
    y2 = (y_pos_ + y_val_ + height_frame_  - 1)/TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if(y_val_ > 0)
        {
            int val_1 = map_data.tile[y2][x1];
            int val_2 = map_data.tile[y2][x2];

            if((val_1 != BLANK_TILE && val_1 != MONEY && val_1 != HEALTH && val_1 != SPHERE && val_1 != LASER && val_1 != FIRE) ||
               (val_2 != BLANK_TILE && val_2 != MONEY && val_2 != HEALTH && val_2 != SPHERE && val_2 != LASER && val_2 != FIRE))
            {
                y_pos_ = y2*TILE_SIZE;
                y_pos_ -= (height_frame_ + 1);
                y_val_ = 0;
                on_ground_ = true;
            }
        }
        else if(y_val_ < 0)
        {
            int val_1 = map_data.tile[y1][x1];
            int val_2 = map_data.tile[y1][x2];

            if((val_1 != BLANK_TILE && val_1 != MONEY) || (val_2 != BLANK_TILE && val_2 != MONEY))
            {
                y_pos_ = (y1 + 1)*TILE_SIZE;
                y_val_ = 0;
            }
        }
    }

    x_pos_ += x_val_;
    y_pos_ += y_val_;
    if(x_pos_ < 0)
    {
        x_pos_ = 0;
    }
    else if(x_pos_ + width_frame_ > map_data.max_x_)
    {
        x_pos_ = map_data.max_x_ - width_frame_ -1;
    }

    if(y_pos_ > map_data.max_y_)
    {
        come_back_time_ = 50;
    }

}

void ThreatObject::ImpMoveType(SDL_Renderer* screen)
{
    if(type_move_ == STATIC_THREAT)
    {

    }
    else
    {
        if(on_ground_ == true)
        {
            if(x_pos_ > animation_b_)
            {
                input_type.left_ = 1;
                input_type.right_ = 0;
                LoadImg("img//threat_left.png", screen);
            }
            else if(x_pos_ < animation_a_)
            {
                input_type.left_ = 0;
                input_type.right_ = 1;
                LoadImg("img//threat_right.png", screen);
            }
        }
        else
        {
            if(input_type.left_ == 1)
            {
                LoadImg("img//threat_left.png", screen);
            }
        }

    }
}
void ThreatObject::InitBullet(BulletObject* p_bullet, SDL_Renderer* screen)
{
    if(p_bullet != NULL)
    {
        p_bullet->set_bullet_type(BulletObject::LASER_BULLET);
        bool ret = p_bullet->LoadImgBullet(screen);
        if(ret == true)
        {
            p_bullet->set_is_move(true);
            p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
            p_bullet->SetRect(this->rect_.x + 56, rect_.y + 35);
            p_bullet->set_x_val(15);
            bullet_list.push_back(p_bullet);
        }

    }
}
void ThreatObject::MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit)
{
    for (int i = 0; i < bullet_list.size(); i++)
    {
        BulletObject* p_bullet = bullet_list[i];
        if(p_bullet != NULL)
        {
            if(p_bullet->get_is_move())
            {
                int bullet_distance = rect_.x + width_frame_ - p_bullet->GetRect().x;
                if(bullet_distance < 500 && bullet_distance > 0)
                {
                    p_bullet->HandleMove(x_limit, y_limit);
                    p_bullet->Render(screen);
                }
                else
                {
                    p_bullet->set_is_move(false);
                }
            }
            else
            {
                p_bullet->set_is_move(true);
                p_bullet->SetRect(rect_.x + 56, rect_.y + 35);
            }
        }
    }
}
void ThreatObject::RemoveBullet(const int& idx)
{
    int size = bullet_list.size();
    if(size > 0 && idx < size)
    {
        BulletObject* p_bullet = bullet_list[idx];
        bullet_list.erase(bullet_list.begin() + idx);
        if(p_bullet != NULL)
        {
            p_bullet->Free();
            delete p_bullet;
            p_bullet = NULL;
        }

    }
}
SDL_Rect ThreatObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;
    return rect;

}
