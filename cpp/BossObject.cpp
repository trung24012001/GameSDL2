#include "BossObject.h"

BossObject::BossObject()
{
    width_frame_ = 0;
    height_frame_ = 0;
    x_val_ = 0;
    y_val_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    map_x_ = 0;
    map_y_ = 0;
    frame_ = 0;
    think_time_ = 0;
    on_ground_ = false;

}
BossObject::~BossObject()
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
bool BossObject::LoadImg(string path, SDL_Renderer* screen)
{
    Free();
    bool ret = BaseObject::LoadImg(path, screen);
    if(ret == true)
    {
        width_frame_ = rect_.w/NUM_FRAME_BOSS;
        height_frame_ = rect_.h;
    }
    return ret;
}
void BossObject::SetClips()
{
    if(width_frame_ > 0 && height_frame_ > 0)
    {
        for (int i=0; i < NUM_FRAME_BOSS; i++ )
        {
            frame_clip_[i].x = i*width_frame_;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}
void BossObject::Show(SDL_Renderer* des)
{
    if(think_time_ == 0)
    {
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;

        if(frame_ >= NUM_FRAME_BOSS)
        {
            frame_ = 0;
        }
        frame_++;

        SDL_Rect* currentClip = &frame_clip_[frame_];
        SDL_Rect rendQuad = {rect_.x, rect_.y, width_frame_, height_frame_};
        if(currentClip != NULL)
        {
            rendQuad.w = currentClip->w;
            rendQuad.h = currentClip->h;
        }
        SDL_RenderCopy(des, p_object_, currentClip, &rendQuad);
    }
}
void BossObject::InitBoss()
{
    x_val_ = 0;
    y_val_ = 0;

    if(x_pos_ > 256)
    {
        x_pos_ -= 256;
    }
    else
    {
        x_pos_ = 0;
    }
    y_pos_ = 0;
    think_time_ = 0;
    input_type.left_ = 1;
}

void BossObject::GoBoss(Map& gMap)
{
    if(think_time_ == 0)
    {
        x_val_ = 0;
        y_val_ += GRAVITY_SPEED;
        if(y_val_ >= MAX_FALL_SPEED)
        {
            y_val_ = MAX_FALL_SPEED;
        }
        CheckMap(gMap);

    }
    else if(think_time_ > 0)
    {
        think_time_--;
        if(think_time_ == 0)
        {
            InitBoss();
        }
    }
}
void BossObject::CheckMap(Map& map_data)
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

            if((val_1 != BLANK_TILE && val_1 != MONEY) || (val_2 != BLANK_TILE && val_2 != MONEY))
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

            if((val_1 != BLANK_TILE && val_1 != MONEY) || (val_2 != BLANK_TILE && val_2 != MONEY))
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

            if((val_1 != BLANK_TILE && val_1 != MONEY) || (val_2 != BLANK_TILE && val_2 != MONEY))
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
        think_time_ = 50;
    }

}

void BossObject::InitBullet(SDL_Renderer* screen)
{

    BulletObject* p_bullet = new BulletObject();
    p_bullet->set_bullet_type(BulletObject::FIRE_BULLET);
    bool ret = p_bullet->LoadImgBullet(screen);
    if(ret == true )
    {
        p_bullet->set_is_move(true);
        p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
        p_bullet->SetRect(this->rect_.x - 50, rect_.y + height_frame_ - 30);
        p_bullet->set_x_val(SPEED_BULLET);
        bullet_list.push_back(p_bullet);
    }


}
void BossObject::MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit)
{
    int max_bullet_list = 1;
    if(frame_ == 18 && bullet_list.size() <= max_bullet_list)
    {
        InitBullet(screen);
    }
    for (int i = 0; i < bullet_list.size(); i++)
    {
        BulletObject* p_bullet = bullet_list[i];
        if(p_bullet != NULL)
        {
            if(p_bullet->get_is_move())
            {
                p_bullet->HandleMove(x_limit, y_limit);
                p_bullet->Render(screen);

            }
            else
            {
                bullet_list.erase(bullet_list.begin() + i);
                if(p_bullet != NULL)
                {
                    p_bullet->Free();
                    delete p_bullet;
                    p_bullet = NULL;
                }
            }
        }
    }
}
void BossObject::RemoveBullet(const int& idx)
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
SDL_Rect BossObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;
    return rect;

}
