#include "MainObject.h"


MainObject::MainObject()
{
    frame_ = 0;
    x_pos_ = 0;
    y_pos_ = 0;
    x_val_ = 0;
    y_val_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    status_ = WALK_RIGHT;
    input_type.left_ = 0;
    input_type.right_ = 0;
    input_type.up_ = 0;
    input_type.down_ = 0;
    on_ground_ = false;
    map_x_ = 0;
    map_y_ = 0;
    come_back_time_ = 0;
    money_loot_ = 0;
    status_bullet_ = BulletObject::DEFAULT_SWORD;
    immortal_ = true;
    start_immortal_ = 0;
    max_bullet_list_ = 0;
    item_bullet_ = 1;
    health_up_ = false;
    health_down_ = false;
    player_attack_default_ = false;
    player_attack_sphere_ = false;
    player_attack_laser_ = false;
    player_attack_fire_ = false;
    player_attack_wind_ = false;
    exit_ = false;

}
MainObject::~MainObject()
{
    if(p_bullet_list_.size() > 0)
    {
         for (int i=0; i < p_bullet_list_.size(); i++)
        {
            BulletObject* p_bullet = p_bullet_list_[i];
            if(p_bullet != NULL)
            {
                p_bullet->Free();
                delete p_bullet;
                p_bullet = NULL;
            }
        }
        p_bullet_list_.clear();
    }
}
bool MainObject::LoadImg(string path, SDL_Renderer* screen)
{
    Free();
    bool ret = BaseObject::LoadImg(path, screen);
    if(ret == true)
    {
        width_frame_ = rect_.w/8;
        height_frame_ = rect_.h;
    }
    return ret;
}
void MainObject::SetClips()
{
    if(width_frame_ > 0 && height_frame_ > 0)
    {
        for (int i=0; i < NUM_FRAME_MAIN; i++)
        {
            frame_clip_[i].x = i*width_frame_;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }

    }
}
void MainObject::Show(SDL_Renderer* des)
{
    UpdateImage(des);
    if(input_type.left_ == 1 || input_type.right_ == 1)
    {
        frame_++;
    }
    else
    {
        frame_ = 0;
    }
    if(frame_ >= 8)
    {
        frame_ = 1;
    }
    if(come_back_time_ == 0)
    {
        rect_.x = x_pos_ - map_x_;
        rect_.y = y_pos_ - map_y_;

        SDL_Rect* current_clip = &frame_clip_[frame_];
        SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};
        SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
    }

}
void MainObject::HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
    if(events.type == SDL_KEYDOWN)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            {
                status_ = WALK_RIGHT;
                input_type.right_ = 1;
                input_type.left_ = 0;
                //UpdateImage(screen);
            }
            break;
        case SDLK_LEFT:
            {
                status_ = WALK_LEFT;
                input_type.left_ = 1;
                input_type.right_ = 0;
                //UpdateImage(screen);
            }
            break;
        case SDLK_KP_0:
            {
                if(item_bullet_ >= 0)
                    status_bullet_ = BulletObject::DEFAULT_SWORD;
            }
            break;
        case SDLK_KP_1:
            {
                if(item_bullet_ >= 1)
                    status_bullet_ = BulletObject::SPHERE_BULLET;
            }
            break;
        case SDLK_KP_2:
            {
                if(item_bullet_ >= 2)
                    status_bullet_ = BulletObject::LASER_BULLET;
            }
            break;
        case SDLK_KP_3:
            {
                if(item_bullet_ >= 3)
                    status_bullet_ = BulletObject::FIRE_BULLET;
            }
            break;
        case SDLK_KP_4:
            {
                if(item_bullet_ >= 1)
                    status_bullet_ = BulletObject::WIND_BULLET;
            }
            break;
        }

    }
    else if(events.type == SDL_KEYUP)
    {
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            {
                input_type.right_ = 0;
            }
            break;
        case SDLK_LEFT:
            {
                input_type.left_ = 0;
            }
            break;
        }
    }

    if(events.type == SDL_MOUSEBUTTONDOWN)
    {
        if (events.button.button == SDL_BUTTON_RIGHT)
        {
            input_type.jump_ = 1;
        }
        else if(events.button.button == SDL_BUTTON_LEFT)
        {
            if(p_bullet_list_.size() <= max_bullet_list_)
            {
                BulletObject* p_bullet = new BulletObject();
                if(status_bullet_ == BulletObject::DEFAULT_SWORD)
                {
                    max_bullet_list_ = 10;
                    player_attack_default_ = true;
                    if(status_ == WALK_LEFT)
                    {
                        p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
                        p_bullet->SetRect(rect_.x, rect_.y + height_frame_*0.3);

                    }
                    else if(status_ == WALK_RIGHT)
                    {
                        p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
                        p_bullet->SetRect(rect_.x + width_frame_-20, rect_.y + height_frame_*0.3);
                    }
                }
                else if(status_bullet_ == BulletObject::SPHERE_BULLET)
                {
                    p_bullet->set_bullet_type(BulletObject::SPHERE_BULLET);
                    p_bullet->LoadImgBullet(screen);
                    max_bullet_list_ = 0;
                    //player_attack_sphere_ = true;
                    player_attack_default_ = true;
                    if(status_ == WALK_LEFT)
                    {
                        p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
                        p_bullet->SetRect(rect_.x, rect_.y + height_frame_*0.3);

                    }
                    else if(status_ == WALK_RIGHT)
                    {
                        p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
                        p_bullet->SetRect(rect_.x + width_frame_-20, rect_.y + height_frame_*0.3);
                    }

                }
                else if(status_bullet_ == BulletObject::LASER_BULLET)
                {
                    p_bullet->set_bullet_type(BulletObject::LASER_BULLET);
                    p_bullet->LoadImgBullet(screen);
                    max_bullet_list_ = 1;
                    //player_attack_laser_ = true;
                    player_attack_default_ = true;
                    if(status_ == WALK_LEFT)
                    {
                        p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
                        p_bullet->SetRect(rect_.x, rect_.y + height_frame_*0.3);

                    }
                    else if(status_ == WALK_RIGHT)
                    {
                        p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
                        p_bullet->SetRect(rect_.x + width_frame_-20, rect_.y + height_frame_*0.3);
                    }
                }
                else if(status_bullet_ == BulletObject::FIRE_BULLET)
                {
                    p_bullet->set_bullet_type(BulletObject::FIRE_BULLET);
                    p_bullet->LoadImgBullet(screen);
                    max_bullet_list_ = 2;
                    //player_attack_fire_ = true;
                    player_attack_default_ = true;
                    if(status_ == WALK_LEFT)
                    {
                        p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
                        p_bullet->SetRect(rect_.x - width_frame_, rect_.y + height_frame_*0.3);

                    }
                    else if(status_ == WALK_RIGHT)
                    {
                        p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
                        p_bullet->SetRect(rect_.x + width_frame_ - 20, rect_.y + height_frame_*0.3);
                    }
                }
                else if(status_bullet_ == BulletObject::WIND_BULLET)
                {
                    p_bullet->set_bullet_type(BulletObject::WIND_BULLET);
                    p_bullet->LoadImgBullet(screen);
                    max_bullet_list_ = 0;
                    //player_attack_wind_ = true;
                    player_attack_default_ = true;
                    if(status_ == WALK_LEFT)
                    {
                        p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
                        p_bullet->SetRect(rect_.x - width_frame_*3 - 30, rect_.y );

                    }
                    else if(status_ == WALK_RIGHT)
                    {
                        p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
                        p_bullet->SetRect(rect_.x + width_frame_, rect_.y );
                    }
                }

                p_bullet->set_x_val(20);
                p_bullet->set_y_val(20);
                p_bullet->set_is_move(true);
                p_bullet_list_.push_back(p_bullet);
            }
        }
    }
    else if(events.type == SDL_MOUSEBUTTONUP)
    {
        if(events.button.button == SDL_BUTTON_LEFT)
        {
            player_attack_default_ = false;
            //player_attack_sphere_ = false;
            //player_attack_laser_ = false;
            //player_attack_fire_ = false;
            //player_attack_wind_ = false;
        }
    }
}
void MainObject::GoPlayer(Map& map_data)
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
            x_val_ -= PLAYER_SPEED;
        }
        else if(input_type.right_ == 1)
        {
            x_val_ += PLAYER_SPEED;
        }
        if(input_type.jump_ == 1)
        {
            if(on_ground_ == true)
            {
                y_val_ = - JUMP_VAL ;
            }
            on_ground_ = false;
            input_type.jump_ = 0;
        }
        CheckMap(map_data);
        CenterEntityOnMap(map_data);
    }
    else if(come_back_time_ > 0)
    {
        come_back_time_ --;
        if(come_back_time_ == 0)
        {
            on_ground_ = false;
            if(x_pos_ > 256)
            {
                x_pos_ -= 256;
            }
            else
            {
                x_pos_ = 0;
            }
            y_pos_ = 0;
            x_val_ = 0;
            y_val_ = 0;
        }
    }

}
void MainObject::CenterEntityOnMap(Map& map_data)
{
    int x_pos_boss = MAX_MAP_X*TILE_SIZE - SCREEN_WIDTH*0.4;
    int pre_start = map_data.start_x_;
    map_data.start_x_ = x_pos_ - (SCREEN_WIDTH/3);
    if(x_pos_ >= x_pos_boss - SCREEN_WIDTH )
    {
        if(pre_start > map_data.start_x_ )
        {
            map_data.start_x_ = pre_start;
        }
    }
    if(map_data.start_x_ < 0)
    {
        map_data.start_x_ = 0;
    }
    else if(map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_)
    {
        map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;
    }

    map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT - TILE_SIZE*2);
    if(map_data.start_y_ < 0)
    {
        map_data.start_y_ = 0;
    }
    else if(map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_)
    {
        map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
    }
}
void MainObject::CheckMap(Map& map_data)
{
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

    int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;
    x1 = (x_pos_ + x_val_ )/TILE_SIZE;
    x2 = (x_pos_ + x_val_ + width_frame_ - 1 )/TILE_SIZE;
    y1 = (y_pos_ + 20)/TILE_SIZE;
    y2 = (y_pos_ + height_min - 20 )/TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if(x_val_ > 0)
        {
            int val_1 = map_data.tile[y1][x2];
            int val_2 = map_data.tile[y2][x2];
            if(val_1 == MONEY || val_2 == MONEY)
            {
                map_data.tile[y1][x2] = 0;
                map_data.tile[y2][x2] = 0;
                ItemLoot();

            }
            else if(val_1 == SPHERE || val_2 == SPHERE)
            {
                map_data.tile[y1][x2] = 0;
                map_data.tile[y2][x2] = 0;
                item_bullet_ = 1;
                status_bullet_ = BulletObject::SPHERE_BULLET;
            }
            else if(val_1 == LASER || val_2 == LASER)
            {
                map_data.tile[y1][x2] = 0;
                map_data.tile[y2][x2] = 0;
                item_bullet_ = 2;
                status_bullet_ = BulletObject::LASER_BULLET;
            }
            else if(val_1 == FIRE || val_2 == FIRE)
            {
                map_data.tile[y1][x2] = 0;
                map_data.tile[y2][x2] = 0;
                item_bullet_ = 3;
                status_bullet_ = BulletObject::FIRE_BULLET;
            }
            else if(val_1 == WIND || val_2 == WIND)
            {
                map_data.tile[y1][x2] = 0;
                map_data.tile[y2][x2] = 0;
                item_bullet_ = 4;
                status_bullet_ = BulletObject::WIND_BULLET;
            }
            else if(val_1 == HEALTH || val_2 == HEALTH)
            {
                map_data.tile[y1][x2] = 0;
                map_data.tile[y2][x2] = 0;
                health_up_ = true;

            }
            else if(val_1 == EXIT || val_2 == EXIT)
            {
                exit_ = true;

            }
            else if(val_1 != BLANK_TILE || val_2 != BLANK_TILE )
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
            if(val_1 == MONEY || val_2 == MONEY)
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y2][x1] = 0;
                ItemLoot();

            }
            else if(val_1 == SPHERE || val_2 == SPHERE)
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y2][x1] = 0;
                item_bullet_ = 1;
                status_bullet_ = BulletObject::SPHERE_BULLET;
            }
            else if(val_1 == LASER || val_2 == LASER)
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y2][x1] = 0;
                item_bullet_ = 2;
                status_bullet_ = BulletObject::LASER_BULLET;
            }
            else if(val_1 == FIRE || val_2 == FIRE)
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y2][x1] = 0;
                item_bullet_ = 3;
                status_bullet_ = BulletObject::FIRE_BULLET;
            }
            else if(val_1 == WIND || val_2 == WIND)
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y2][x1] = 0;
                item_bullet_ = 4;
                status_bullet_ = BulletObject::WIND_BULLET;
            }
            else if(val_1 == HEALTH || val_2 == HEALTH)
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y2][x1] = 0;
                health_up_ = true;
            }
            else if(val_1 == EXIT || val_2 == EXIT)
            {
                exit_ = true;

            }
            else if(val_1 != BLANK_TILE || val_2 != BLANK_TILE )
            {
                x_pos_ = (x1 + 1)*TILE_SIZE;
                x_val_ = 0;
            }
        }
    }
    int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
    x1 = (x_pos_ + width_min/3)/TILE_SIZE;
    x2 = (x_pos_ + width_min  - 15)/TILE_SIZE;
    y1 = (y_pos_ + y_val_)/TILE_SIZE;
    y2 = (y_pos_ + y_val_ + height_frame_  - 1)/TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
    {
        if(y_val_ > 0)
        {
            int val_1 = map_data.tile[y2][x1];
            int val_2 = map_data.tile[y2][x2];
            if(val_1 == MONEY || val_2 == MONEY)
            {
                map_data.tile[y2][x1] = 0;
                map_data.tile[y2][x2] = 0;
                ItemLoot();

            }
            else if(val_1 == SPHERE || val_2 == SPHERE)
            {
                map_data.tile[y2][x1] = 0;
                map_data.tile[y2][x2] = 0;
                item_bullet_ = 1;
                status_bullet_ = BulletObject::SPHERE_BULLET;
            }
            else if(val_1 == LASER || val_2 == LASER)
            {
                map_data.tile[y2][x1] = 0;
                map_data.tile[y2][x2] = 0;
                item_bullet_ = 2;
                status_bullet_ = BulletObject::LASER_BULLET;
            }
            else if(val_1 == FIRE || val_2 == FIRE)
            {
                map_data.tile[y2][x1] = 0;
                map_data.tile[y2][x2] = 0;
                item_bullet_ = 3;
                status_bullet_ = BulletObject::FIRE_BULLET;
            }
            else if(val_1 == WIND || val_2 == WIND)
            {
                map_data.tile[y2][x1] = 0;
                map_data.tile[y2][x2] = 0;
                item_bullet_ = 4;
                status_bullet_ = BulletObject::WIND_BULLET;
            }
            else if(val_1 == HEALTH || val_2 == HEALTH)
            {
                map_data.tile[y2][x1] = 0;
                map_data.tile[y2][x2] = 0;
                health_up_ = true;

            }
            else if(val_1 == EXIT || val_2 == EXIT)
            {
                exit_ = true;

            }
            else if(val_1 != BLANK_TILE || val_2 != BLANK_TILE )
            {
                y_pos_ = y2*TILE_SIZE;
                y_pos_ -= (height_frame_ + 1);
                y_val_ = 0;
                on_ground_ = true;
                if(status_ == WALK_NONE)
                {
                    status_ = WALK_RIGHT;
                }

            }

        }
        else if(y_val_ < 0)
        {
            int val_1 = map_data.tile[y1][x1];
            int val_2 = map_data.tile[y1][x2];
            if(val_1 == MONEY || val_2 == MONEY)
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y1][x2] = 0;
                ItemLoot();

            }
            else if(val_1 == SPHERE || val_2 == SPHERE)
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y1][x2] = 0;
                item_bullet_ = 1;
                status_bullet_ = BulletObject::SPHERE_BULLET;
            }
            else if(val_1 == LASER || val_2 == LASER)
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y1][x2] = 0;
                item_bullet_ = 2;
                status_bullet_ = BulletObject::LASER_BULLET;
            }
            else if(val_1 == FIRE || val_2 == FIRE)
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y1][x2] = 0;
                item_bullet_ = 3;
                status_bullet_ = BulletObject::FIRE_BULLET;
            }
            else if(val_1 == WIND || val_2 == WIND)
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y1][x2] = 0;
                item_bullet_ = 4;
                status_bullet_ = BulletObject::WIND_BULLET;
            }
            else if(val_1 == HEALTH || val_2 == HEALTH)
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y1][x2] = 0;
                health_up_ = true;

            }
            else if(val_1 == EXIT || val_2 == EXIT)
            {
                exit_ = true;
            }
            else if(val_1 != BLANK_TILE || val_2 != BLANK_TILE )
            {
                y_pos_ = (y1 + 1)*TILE_SIZE;
                y_val_ = 0;
            }
        }
    }
    // bị rơi hoặc bay lên nhân vật sẽ k thực hiện được thao tác nhảy
    //***********************************
    if(y_val_ > 1 || y_val_ < -1)
    {
        on_ground_ = false;
    }
    //***********************************
    x_pos_ += x_val_;
    y_pos_ += y_val_;
    if(x_pos_ < map_data.start_x_)
    {
        x_pos_ = map_data.start_x_;
    }
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
        health_down_ = true;
    }

}
void MainObject::UpdateImage(SDL_Renderer* des)
{
    if(player_attack_default_ == true )
    {
        if(status_ == WALK_LEFT)
        {
            LoadImg("img//attackleftlist.png", des);
        }
        else if(status_ == WALK_RIGHT)
        {
            LoadImg("img//yasuoattack.png", des);
        }
    }
    else
    {
        if(on_ground_ == true)
        {
            if(status_ == WALK_LEFT)
            {
                LoadImg("img//player_left.png", des);
            }
            else if(status_ == WALK_RIGHT)
            {
                LoadImg("img//player_right.png", des);
            }
        }
        else
        {
            if(status_ == WALK_LEFT)
            {
                LoadImg("img//jum_left.png", des);
            }
            else if(status_ == WALK_RIGHT)
            {
                LoadImg("img//jum_right.png", des);
            }
        }
    }
}
void MainObject::HandleBullet(SDL_Renderer* des)
{
    for (int i=0; i < p_bullet_list_.size(); i++)
    {
        BulletObject* p_bullet = p_bullet_list_[i];
        if(p_bullet != NULL)
        {
            if(status_bullet_ == BulletObject::DEFAULT_SWORD)
            {
                if(p_bullet->get_is_move() == true )
                {
                    int bullet_distance = rect_.x  - p_bullet->GetRect().x;
                    if(bullet_distance < 64 && bullet_distance > -128)
                    {
                        p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
                        //p_bullet->Render(des);
                    }
                    else
                    {
                        p_bullet->set_is_move(false);
                    }
                }
                else
                {
                    p_bullet_list_.erase(p_bullet_list_.begin() + i);
                    if(p_bullet != NULL)
                    {
                        p_bullet->Free();
                        delete p_bullet;
                        p_bullet = NULL;
                    }
                }
            }
            else
            {

                if(p_bullet->get_is_move() == true )
                {
                    p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
                    p_bullet->Render(des);
                }
                else
                {
                    p_bullet_list_.erase(p_bullet_list_.begin() + i);
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
}
void MainObject::RemoveBullet(const int& idx)
{
    int size = p_bullet_list_.size();
    if(size > 0 && idx < size)
    {
        BulletObject* p_bullet = p_bullet_list_[idx];
        p_bullet_list_.erase(p_bullet_list_.begin() + idx);
        if(p_bullet != NULL)
        {
            p_bullet->Free();
            delete p_bullet;
            p_bullet = NULL;
        }

    }
}
bool MainObject::Immortal()
{
    if(rect_.y == 0 || rect_.y >= SCREEN_HEIGHT - 5 )
    {
        immortal_ = true;
        start_immortal_ = SDL_GetTicks();
    }
    if(SDL_GetTicks() - start_immortal_ > 4000)
    {
        immortal_ = false;
    }
    return immortal_;
}
SDL_Rect MainObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;

    return rect;

}

void MainObject::ItemLoot()
{
    money_loot_++;
}
