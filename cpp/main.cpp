
#include "Common.h"
#include "BaseObject.h"
#include "GameMap.h"
#include "BulletObject.h"
#include "MainObject.h"
#include "ThreatObject.h"
#include "BossObject.h"
#include "ExplosionObject.h"
#include "Timer.h"
#include "TextObject.h"
#include "PlayerHealth.h"
#include "PlayerMoney.h"
#include "Geometric.h"


BaseObject g_background;
TTF_Font* font_time = NULL;
TTF_Font* font_game = NULL;
Mix_Music* g_sound_music = NULL;

bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if(ret<0)
        return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("Game SDL", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(g_window == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if(g_screen == NULL)
        {
            success = false;
        }
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags)&&imgFlags))
                success = false;
        }

        if(TTF_Init() == -1)
        {
            success = false;
        }
        font_time = TTF_OpenFont("font//aachenb.ttf", 20);
        font_game = TTF_OpenFont("font//aachenb.ttf", 50);
        if(font_time == NULL || font_game == NULL)
        {
            success = false;
        }
        if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        {
            success = false;
        }
        else
        {
            g_sound_music = Mix_LoadMUS("sound//Action.mp3");

            if(g_sound_music == NULL )
            {
                success = false;
            }

        }
    }

    return success;
}
bool LoadBackground()
{
    bool ret = g_background.LoadImg("img//BG.png", g_screen);
    if(ret == false)
    {
        cout << "bg fail";
        return false;
    }

    return true;
}
void close()
{
    g_background.Free();

    Mix_FreeMusic(g_sound_music);
    Mix_Music* g_sound_music = NULL;

    TTF_CloseFont(font_game);
    TTF_CloseFont(font_time);
    TTF_Font* font_time = NULL;
    TTF_Font* font_game = NULL;

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

vector<ThreatObject*> MakeThreatList()
{
    vector<ThreatObject*> list_threats;

    ThreatObject* dynamic_threats = new ThreatObject[20];
    for (int i = 0;i < 20; i++)
    {
        ThreatObject* p_threat = (dynamic_threats + i);
        if(p_threat != NULL)
        {
            p_threat->LoadImg("img//threat_left.png", g_screen);
            p_threat->SetClips();
            p_threat->set_type_move(ThreatObject::MOVE_THREAT);
            p_threat->set_x_pos(500 + i*500);
            p_threat->set_y_pos(200);
            p_threat->set_input_left(1);
            int pos_1 = p_threat->get_x_pos() - 60;
            int pos_2 = p_threat->get_x_pos() + 60;
            p_threat->SetAnimationPos(pos_1, pos_2);
            list_threats.push_back(p_threat);
        }

    }

    ThreatObject* static_threats = new ThreatObject[20];
    for (int i=0; i < 20; i++)
    {
        ThreatObject* p_threat = (static_threats + i);
        if(p_threat != NULL)
        {
            p_threat->LoadImg("img//threat_level.png", g_screen);
            p_threat->SetClips();
            p_threat->set_type_move(ThreatObject::STATIC_THREAT);
            p_threat->set_x_pos(700 + i*1200);
            p_threat->set_y_pos(250);
            p_threat->set_input_left(0);
            int pos_x = p_threat->get_x_pos();
            int pos_y = p_threat->get_y_pos();
            p_threat->SetRect(pos_x, pos_y);

            BulletObject* p_bullet = new BulletObject();
            p_threat->InitBullet(p_bullet,g_screen);
            list_threats.push_back(p_threat);
        }
    }
    return list_threats;
}

int main(int argc, char* argv[])
{
    //INIT SCREEN
    if(InitData() == false)
        return -1;
    if(LoadBackground() == false)
        return -1;
    //GAME MAP
    GameMap game_map;
    game_map.LoadMap("map//map01.dat");
    game_map.LoadTiles(g_screen);

    //YASUO MAIN
    MainObject YasuoObject;
    YasuoObject.LoadImg("img//player_right.png", g_screen);
    YasuoObject.SetClips();
    YasuoObject.set_x_pos(0);
    int health_ys = NUM_HEALTH_MAIN;
    //YASUO ITEM
    PlayerHealth yasuo_health;
    yasuo_health.Init(g_screen);
    PlayerMoney yasuo_money;
    yasuo_money.Init(g_screen);
    yasuo_money.SetPos(SCREEN_WIDTH*0.5 - 300 , 8);

    //BOSS
    BossObject* Boss_obj = new BossObject();
    bool ret = Boss_obj->LoadImg("img//boss_object.png", g_screen);
    Boss_obj->SetClips();
    int x_pos_boss = MAX_MAP_X*TILE_SIZE - SCREEN_WIDTH*0.4;
    Boss_obj->set_x_pos(x_pos_boss);
    Boss_obj->set_y_pos(0);
    int health_boss = NUM_HEALTH_BOSS;
    bool boss_coming = true;
    //THREAT
    vector<ThreatObject*>  threats_list = MakeThreatList();

    //IMAGE EXPLOSION
    ExplosionObject exp_boss;
    bool bRet = exp_boss.LoadImg("img//exp3.png", g_screen);
    if(bRet == false) return -1;
    exp_boss.SetClips();
    ExplosionObject exp_threat;
    bool tRet = exp_threat.LoadImg("img//exp3.png", g_screen);
    if(tRet == false) return -1;
    exp_threat.SetClips();
    ExplosionObject exp_main;
    bool mRet = exp_main.LoadImg("img//winddie.png", g_screen);
    if(mRet == false) return -1;
    exp_main.SetClips();
    //GAME TEXT
    TextObject time_game;
    time_game.SetColor(TextObject::WHITE_TEXT);
    TextObject mark_game;
    mark_game.SetColor(TextObject::WHITE_TEXT);
    int mark_value = 0;
    TextObject money_game;
    money_game.SetColor(TextObject::WHITE_TEXT);
    int money_count = 0;
    //FPS TIME
    Timer fps_timer;
    Timer Time;
    //MENU
    bool is_quit = false;
    int ret_menu = ShowMenu(g_screen,font_game);
    while(ret_menu != 0)
    {
        if(ret_menu == 1)
        {
            is_quit = true;
            break;
        }
        else if(ret_menu == 2)
        {
            //Intro
            int ret_intro = ShowIntro(g_screen,font_game);
            if(ret_intro == 0)
            {
                ret_menu = ShowMenu(g_screen,font_game);
            }
            else
            {
                is_quit = true;
                break;
            }
        }
    }
    Time.start();
    while(!is_quit)
    {
        fps_timer.start();
        int music_play = Mix_PlayingMusic();
        int music_pause = Mix_PausedMusic();
        while(SDL_PollEvent(&g_event) != 0)
        {
            if(g_event.type == SDL_QUIT)
            {
                is_quit = true;
            }
            if (g_event.type == SDL_KEYDOWN)
            {
                if (g_event.key.keysym.sym == SDLK_m)
				{
				    if(music_play == false)
                    {
                        if (Mix_PlayingMusic() == NULL)
                        {
                            Mix_PlayMusic(g_sound_music, 0);
                        }

                    }
                    else
                    {
                        Mix_HaltMusic();
                    }

				}
				else if (g_event.key.keysym.sym == SDLK_n)
				{
				    if(music_play == true && music_pause == false)
                    {
                        Mix_PauseMusic();
                    }
                    else if(music_play == true && music_pause == true)
                    {
                        Mix_ResumeMusic();

                    }
				}
				if(g_event.key.keysym.sym == SDLK_p)
                {
                    Time.paused();
                }
            }
            YasuoObject.HandleInputAction(g_event, g_screen);
        }
        //Pause
        if(Time.is_paused() == true)
        {
            //Text pause
            string strMark("Nghi ti met qua");
            mark_game.SetText(strMark);
            mark_game.LoadFromRenderText(font_game, g_screen);
            mark_game.RenderText(g_screen, SCREEN_WIDTH*0.5 - 150, 200);
            SDL_RenderPresent(g_screen);
            bool is_pause = true;
            {
                while(is_pause)
                {
                    music_play = Mix_PlayingMusic();
                    music_pause = Mix_PausedMusic();
                    while(SDL_PollEvent(&g_event) != 0)
                    {
                        if(g_event.type == SDL_QUIT)
                        {
                            is_pause = false;
                            is_quit = true;
                        }
                        if (g_event.type == SDL_KEYDOWN)
                        {
                            if (g_event.key.keysym.sym == SDLK_m)
                            {
                                if(music_play == false)
                                {
                                    if (Mix_PlayingMusic() == NULL)
                                    {
                                        Mix_PlayMusic(g_sound_music, 0);

                                    }
                                }
                                else
                                {
                                    Mix_HaltMusic();
                                }
                            }
                            else if (g_event.key.keysym.sym == SDLK_n)
                            {
                                if(music_play == true && music_pause == false)
                                {
                                    Mix_PauseMusic();
                                }
                                else if(music_play == true && music_pause == true)
                                {
                                    Mix_ResumeMusic();
                                }
                            }
                        }
                    }
                    continue;
                }
            }
        }
        //Music
        int playing = Mix_PlayingMusic();
        int pause_music = Mix_PausedMusic();
        if(playing == 1)
        {
            while(SDL_PollEvent(&g_event) != 0)
                    {
                        if(g_event.type == SDL_QUIT)
                        {

                            is_quit = true;
                        }
                        if (g_event.type == SDL_KEYDOWN)
                        {
                            if (g_event.key.keysym.sym == SDLK_m)
                            {
                                Mix_PauseMusic();
                            }

                        }
                    }
        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);
        //SHOW BACKGROUND
        g_background.Render(g_screen, NULL);
        Map map_data = game_map.GetMap();

        vector<BulletObject*> bullet_arr = YasuoObject.get_bullet_list();
        //SHOW MAIN
        YasuoObject.HandleBullet(g_screen);
        YasuoObject.SetMapXY(map_data.start_x_, map_data.start_y_);
        YasuoObject.GoPlayer(map_data);
        YasuoObject.Show(g_screen);
        SDL_Rect rect_yasuo = YasuoObject.GetRectFrame();
        //SHOW MAP
        game_map.SetMap(map_data);
        game_map.DrawMap(g_screen);
        //Draw Geometric
        GeometricFormat rectangle_size(0,0, SCREEN_WIDTH, 40);
        ColorData color_data_1(0, 80, 150);
        Geometric::RenderRetangle(rectangle_size, color_data_1, g_screen);
        GeometricFormat outLineSize(1, 1, SCREEN_WIDTH - 1, 38);
        ColorData color_data_2(0, 0, 0);
        Geometric::RenderOutline(outLineSize, color_data_2, g_screen);
        //SHOW ITEM MAIN
        if(YasuoObject.get_health_up() == true)
        {
            health_ys++;
            if(health_ys <= 5)
            {
                yasuo_health.Increase();
            }
            else
            {
                health_ys = 5;
            }
            YasuoObject.set_health_up(false);
        }

        yasuo_health.Show(g_screen);
        yasuo_money.Show(g_screen);

        //SHOW BOSS
        int val = MAX_MAP_X*TILE_SIZE -  (map_data.start_x_ + YasuoObject.GetRect().x);
        if(val <= SCREEN_WIDTH + 500 && boss_coming == true)
        {
            Boss_obj->SetMapXY(map_data.start_x_, map_data.start_y_);
            Boss_obj->GoBoss(map_data);
            Boss_obj->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
            Boss_obj->Show(g_screen);
            SDL_Rect rect_boss = Boss_obj->GetRectFrame();
            //check col boss and main(main die)
            if(YasuoObject.Immortal() == false)
            {
                bool boss_col = CheckCollision(rect_boss, rect_yasuo);
                bool bullet_col = false;
                vector<BulletObject*> bBullet_list = Boss_obj->get_bullet_list();
                for (int i=0; i < bBullet_list.size(); i++)
                {
                    BulletObject* pb_bullet = bBullet_list[i];
                    if(pb_bullet != NULL)
                    {
                        bullet_col = CheckCollision(pb_bullet->GetRect(), rect_yasuo);
                        if(bullet_col == true)
                        {
                            Boss_obj->RemoveBullet(i);
                            break;
                        }
                    }
                }
                if(boss_col == true || bullet_col == true)
                {
                    int frame_exp_width = exp_main.get_frame_width();
                    int frame_exp_height = exp_main.get_frame_height();
                    for(int ex = 0;ex < NUM_FRAME_EXP; ex++)
                    {
                        int x_pos = YasuoObject.GetRect().x - frame_exp_width*0.5;
                        int y_pos = YasuoObject.GetRect().y - frame_exp_height*0.5;
                        exp_main.set_frame(ex);
                        exp_main.SetRect(x_pos, y_pos);
                        exp_main.Show(g_screen);
                        SDL_RenderPresent(g_screen);
                    }

                    health_ys--;
                    if(health_ys > 0)
                    {
                        YasuoObject.SetRect(0, 0);
                        YasuoObject.set_come_back_time(50);
                        SDL_Delay(1000);
                        yasuo_health.Decrease();
                        yasuo_health.Render(g_screen);
                    }
                    else
                    {
                        SDL_Delay(1000);
                        int ret_death = ShowDeath(g_screen,font_game);
                        if(ret_death == 1)
                        {
                            is_quit = true;
                        }
                        else
                        {
                            is_quit = true;
                        }

                    }

                }

            }

            //check col bullet main and boss(kill boss)
            int exp_width = exp_boss.get_frame_width();
            int exp_height = exp_boss.get_frame_height();
            for (int bl = 0; bl < bullet_arr.size(); bl++)
            {
                BulletObject* p_bullet = bullet_arr[bl];
                if(p_bullet != NULL)
                {
                    SDL_Rect bRect = p_bullet->GetRect();
                    bool boss_col = CheckCollision(bRect, rect_boss);
                    if(boss_col == true)
                    {
                        health_boss --;
                        YasuoObject.RemoveBullet(bl);

                        if(health_boss <= 0)
                        {
                            mark_value += 100;
                            for(int ex = 0;ex < NUM_FRAME_EXP; ex++)
                            {
                                int x_pos = p_bullet->GetRect().x + exp_width*0.1;
                                int y_pos = p_bullet->GetRect().y - exp_height*0.4;
                                exp_boss.set_frame(ex);
                                exp_boss.SetRect(x_pos, y_pos);
                                exp_boss.Show(g_screen);
                                SDL_RenderPresent(g_screen);
                                SDL_Delay(200);

                            }
                            Boss_obj->Free();
                            boss_coming = false;
                            delete Boss_obj;
                            Boss_obj = NULL;
                        }

                    }
                }
            }

        }

        //check col main and threat(main die)
        for (int i=0; i < threats_list.size(); i++)
        {
            ThreatObject* p_threat = threats_list[i];
            if(p_threat != NULL)
            {
                p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
                p_threat->ImpMoveType(g_screen);
                p_threat->GoThreat(map_data);
                p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
                p_threat->Show(g_screen);


                if(YasuoObject.Immortal() == false)
                {
                    // check col bullet threat and main(main die)
                    bool col_1 = false;
                    vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();
                    for (int t=0; t < tBullet_list.size(); t++)
                    {
                        BulletObject* pt_bullet = tBullet_list[t];
                        if(pt_bullet != NULL)
                        {
                            col_1 = CheckCollision(pt_bullet->GetRect(), rect_yasuo);

                            if(col_1 == true)
                            {
                                p_threat->RemoveBullet(t);
                                break;
                            }
                        }
                    }

                    //check col threat and main(main die)
                    SDL_Rect rect_threat = p_threat->GetRectFrame();
                    bool col_2 = CheckCollision(rect_yasuo, rect_threat);
                    if(col_1 == true || col_2 == true )
                    {
                        int frame_exp_width = exp_main.get_frame_width();
                        int frame_exp_height = exp_main.get_frame_height();
                        for(int ex = 0;ex < NUM_FRAME_EXP; ex++)
                        {
                            int x_pos = YasuoObject.GetRect().x ;
                            int y_pos = YasuoObject.GetRect().y ;
                            exp_main.set_frame(ex);
                            exp_main.SetRect(x_pos, y_pos);
                            exp_main.Show(g_screen);
                            SDL_RenderPresent(g_screen);
                        }

                        health_ys --;
                        if(health_ys > 0)
                        {
                            YasuoObject.SetRect(0, 0);
                            YasuoObject.set_come_back_time(50);
                            SDL_Delay(1000);
                            yasuo_health.Decrease();
                            yasuo_health.Render(g_screen);
                        }
                        else
                        {
                            SDL_Delay(1000);
                            int ret_death = ShowDeath(g_screen,font_game);
                            if(ret_death == 1)
                            {
                                is_quit = true;
                            }
                            else
                            {
                                is_quit = true;

                            }

                        }
                    }
                }

            }
        }

        //Check col bullet and threat(kill threat)
        int frame_exp_width = exp_threat.get_frame_width();
        int frame_exp_height = exp_threat.get_frame_height();
        for (int bl = 0; bl < bullet_arr.size(); bl++)
        {
            BulletObject* p_bullet = bullet_arr[bl];
            if(p_bullet != NULL)
            {
                for (int t = 0; t < threats_list.size(); t++)
                {
                    ThreatObject* obj_threat = threats_list[t];
                    if(obj_threat != NULL)
                    {
                        SDL_Rect tRect = obj_threat->GetRectFrame();
                        SDL_Rect bRect = p_bullet->GetRect();
                        bool bCol = CheckCollision(tRect, bRect);
                        if(bCol == true)
                        {
                            mark_value++;
                            for(int ex = 0;ex < NUM_FRAME_EXP; ex++)
                            {
                                int x_pos = obj_threat->GetRect().x - frame_exp_width*0.5;
                                int y_pos = obj_threat->GetRect().y - frame_exp_height*0.5;
                                exp_threat.set_frame(ex);
                                exp_threat.SetRect(x_pos, y_pos);
                                exp_threat.Show(g_screen);
                                SDL_RenderPresent(g_screen);
                            }

                            YasuoObject.RemoveBullet(bl);
                            obj_threat->Free();
                            threats_list.erase(threats_list.begin() + t);
                        }
                    }
                }
            }
        }
        //FALL DOWN
        if(YasuoObject.get_health_down() == true)
        {
            health_ys--;
            if(health_ys > 0)
            {
                SDL_Delay(1000);
                yasuo_health.Decrease();
                yasuo_health.Render(g_screen);
            }
            else
            {
                SDL_Delay(1000);
                int ret_death = ShowDeath(g_screen,font_game);
                if(ret_death == 1)
                {
                    is_quit = true;
                }
                else
                {
                    is_quit = true;
                }

            }
            YasuoObject.set_health_down(false);
        }

        //EXIT PASS
        if(YasuoObject.get_exit() == true)
        {
            SDL_Delay(1000);
            int ret_end = ShowEnd(g_screen,font_game);
            if(ret_end == 1)
            {
                is_quit = true;
            }
        }
        //SHow game time
        string str_time = "Time: ";
        int val_time = 300 - Time.get_ticks()/1000;
        if(val_time <= 0)
        {
            Time.stop();
            if(MessageBox(NULL, "Het gio roi de y thoi gian di", "INFO", MB_OK | MB_ICONSTOP) == IDOK)
            {
                is_quit = true;
            }

        }
        else
        {
            string str_val = to_string(val_time);
            str_time += str_val;

            time_game.SetText(str_time);
            time_game.LoadFromRenderText(font_time, g_screen);
            time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15);
        }
        //TEXT
        //Text mark
        string val_str_mark = to_string(mark_value);
        string strMark("Mark: ");
        strMark += val_str_mark;
        mark_game.SetText(strMark);
        mark_game.LoadFromRenderText(font_time, g_screen);
        mark_game.RenderText(g_screen, SCREEN_WIDTH*0.5 - 50, 15);
        //Text money
        money_count = YasuoObject.get_money_loot();
        string money_str = to_string(money_count);
        string strMoney("Money: ");
        strMoney += money_str;
        money_game.SetText(strMoney);
        money_game.LoadFromRenderText(font_time, g_screen);
        money_game.RenderText(g_screen, SCREEN_WIDTH*0.5 - 250, 15);
        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderPresent(g_screen);
        //FPS game
        int real_time = fps_timer.get_ticks();
        int time_one_frame = 1000/FRAME_PER_SECOND;
        if(real_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_time;
            if(delay_time >= 0)
            SDL_Delay(delay_time);
        }


        //Update screen
        SDL_RenderPresent(g_screen);

    }

    //release memory
    for (int i=0; i < threats_list.size(); i++)
    {
        ThreatObject* p_threat = threats_list[i];
        if(p_threat != NULL)
        {
            p_threat->Free();
            delete p_threat;
            p_threat = NULL;
        }
    }
    if(Boss_obj != NULL)
    {
        delete Boss_obj;
        Boss_obj = NULL;
    }
    money_game.Free();
    mark_game.Free();
    threats_list.clear();
    close();
    return 0;

}
