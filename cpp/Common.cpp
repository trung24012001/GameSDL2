#include "Common.h"
#include "BaseObject.h"
#include "TextObject.h"

BaseObject g_menu;
BaseObject g_death;
BaseObject g_end;
BaseObject g_intro;
bool CheckCollision(const SDL_Rect object_1, const SDL_Rect object_2)
{
      int left_a = object_1.x;
      int right_a = object_1.x + object_1.w;
      int top_a = object_1.y;
      int bottom_a = object_1.y + object_1.h;

      int left_b = object_2.x;
      int right_b = object_2.x + object_2.w;
      int top_b = object_2.y;
      int bottom_b = object_2.y + object_2.h;

      // Case 1: size object 1 < size object 2
      if (left_a > left_b && left_a < right_b)
      {
        if (top_a > top_b && top_a < bottom_b)
        {
          return true;
        }
      }

      if (left_a > left_b && left_a < right_b)
      {
        if (bottom_a > top_b && bottom_a < bottom_b)
        {
          return true;
        }
      }

      if (right_a > left_b && right_a < right_b)
      {
        if (top_a > top_b && top_a < bottom_b)
        {
          return true;
        }
      }

      if (right_a > left_b && right_a < right_b)
      {
        if (bottom_a > top_b && bottom_a < bottom_b)
        {
          return true;
        }
      }

      // Case 2: size object 1 > size object 2
      if (left_b > left_a && left_b < right_a)
      {
        if (top_b > top_a && top_b < bottom_a)
        {
          return true;
        }
      }

      if (left_b > left_a && left_b < right_a)
      {
        if (bottom_b > top_a && bottom_b < bottom_a)
        {
          return true;
        }
      }

      if (right_b > left_a && right_b < right_a)
      {
        if (top_b > top_a && top_b < bottom_a)
        {
          return true;
        }
      }

      if (right_b > left_a && right_b < right_a)
      {
        if (bottom_b > top_a && bottom_b < bottom_a)
        {
          return true;
        }
      }

       // Case 3: size object 1 = size object 2
      if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)
      {
        return true;
      }

      return false;
}

bool LoadMenu(SDL_Renderer* screen)
{
    bool ret = g_menu.LoadImg("img//menu.png", screen);
    if(ret == false)
    {
        cout << "menu fail";
        return false;
    }

    return true;
}
bool LoadDeath(SDL_Renderer* screen)
{
    bool ret = g_death.LoadImg("img//dead.png", screen);
    if(ret == false)
    {
        return false;
    }

    return true;
}
bool LoadEnd(SDL_Renderer* screen)
{
    bool ret = g_end.LoadImg("img//End.png", screen);
    if(ret == false)
    {
        return false;
    }

    return true;
}
bool LoadIntro(SDL_Renderer* screen)
{
    bool ret = g_intro.LoadImg("img//Introduction.png", screen);
    if(ret == false)
    {
        return false;
    }

    return true;
}
bool CheckFocusWithRect(const int& x, const int& y, const SDL_Rect& rect)
{
    if(x >= rect.x && x <= rect.x + rect.w &&
       y >= rect.y && y <= rect.y + rect.h )
    {
        return true;
    }
    return false;
}
int ShowMenu(SDL_Renderer* des, TTF_Font* font)
{
    if(LoadMenu(des) == false)
        return 1;

    const int kMenuItemNum = 3;
    SDL_Rect pos_item[kMenuItemNum];
    pos_item[0].x = 100;
    pos_item[0].y = 400;

    pos_item[1].x = 100;
    pos_item[1].y = 540;

    pos_item[2].x = 100;
    pos_item[2].y = 470;

    TextObject text_menu[kMenuItemNum];

    text_menu[0].SetText("Choi thoi");
    text_menu[0].SetColor(TextObject::BLACK_TEXT);
    text_menu[0].LoadFromRenderText(font, des);

    text_menu[2].SetText("Choi the nao");
    text_menu[2].SetColor(TextObject::BLACK_TEXT);
    text_menu[2].LoadFromRenderText(font, des);

    text_menu[1].SetText("Thoi khong choi nua");
    text_menu[1].SetColor(TextObject::BLACK_TEXT);
    text_menu[1].LoadFromRenderText(font, des);


    bool selected[kMenuItemNum] = {0,0,0};
    int xm = 0;
    int ym = 0;
    SDL_Event m_event;
    while(true)
    {
        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);
        g_menu.Render(des, NULL);
        for (int i = 0;i < kMenuItemNum;i++)
        {
            text_menu[i].RenderText(des, pos_item[i].x, pos_item[i].y);
        }
        while(SDL_PollEvent(&m_event))
        {
            switch (m_event.type)
            {
            case SDL_QUIT:
                return 1;
            case SDL_MOUSEMOTION:
                {
                    xm = m_event.motion.x;
                    ym = m_event.motion.y;

                    for (int i = 0;i < kMenuItemNum;i++)
                    {
                        if(CheckFocusWithRect(xm,ym,text_menu[i].GetRect()))
                        {
                            if(selected[i] == false)
                            {
                                selected[i] = true;
                                text_menu[i].SetColor(TextObject::RED_TEXT);
                                text_menu[i].LoadFromRenderText(font, des);
                                text_menu[i].RenderText(des, pos_item[i].x, pos_item[i].y);


                            }
                        }
                        else
                        {
                            if(selected[i] == true)
                            {
                                selected[i] = false;
                                text_menu[i].SetColor(TextObject::BLACK_TEXT);
                                text_menu[i].LoadFromRenderText(font, des);
                                text_menu[i].RenderText(des, pos_item[i].x, pos_item[i].y);

                            }
                        }
                    }
                    break;
                }
            case SDL_MOUSEBUTTONDOWN:
                {
                    xm = m_event.button.x;
                    ym = m_event.button.y;
                    for (int i = 0;i < kMenuItemNum;i++)
                    {
                        if(CheckFocusWithRect(xm,ym,text_menu[i].GetRect()))
                        {
                            g_menu.Free();
                            return i;
                        }
                    }
                    break;
                }
            case SDL_KEYDOWN:
                {
                    if(m_event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        g_menu.Free();
                        return 1;
                    }
                    break;
                }
            default:
                break;
            }
        }
        SDL_RenderPresent(des);
    }
    g_menu.Free();
    return 1;
}
int ShowDeath(SDL_Renderer* des, TTF_Font* font)
{
    if(LoadDeath(des) == false)
        return 1;

    const int kDeathItemNum = 2;
    SDL_Rect pos_item[kDeathItemNum];
    pos_item[0].x = 50;
    pos_item[0].y = 300;

    pos_item[1].x = 50;
    pos_item[1].y = 400;

    TextObject text_death[kDeathItemNum];

    text_death[0].SetText("Chet roi a");
    text_death[0].SetColor(TextObject::BLACK_TEXT);
    text_death[0].LoadFromRenderText(font, des);

    text_death[1].SetText("Thoi Quit nhe");
    text_death[1].SetColor(TextObject::BLACK_TEXT);
    text_death[1].LoadFromRenderText(font, des);


    bool selected[kDeathItemNum] = {0,0};
    int xm = 0;
    int ym = 0;
    SDL_Event m_event;
    while(true)
    {
        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);
        g_death.Render(des, NULL);
        for (int i = 0;i < kDeathItemNum;i++)
        {
            text_death[i].RenderText(des, pos_item[i].x, pos_item[i].y);
        }
        while(SDL_PollEvent(&m_event))
        {
            switch (m_event.type)
            {
            case SDL_QUIT:
                return 1;
            case SDL_MOUSEMOTION:
                {
                    xm = m_event.motion.x;
                    ym = m_event.motion.y;

                    for (int i = 0;i < kDeathItemNum;i++)
                    {
                        if(CheckFocusWithRect(xm,ym,text_death[i].GetRect()))
                        {
                            if(selected[i] == false)
                            {
                                selected[i] = true;
                                text_death[i].SetColor(TextObject::RED_TEXT);
                                text_death[i].LoadFromRenderText(font, des);
                                text_death[i].RenderText(des, pos_item[i].x, pos_item[i].y);


                            }
                        }
                        else
                        {
                            if(selected[i] == true)
                            {
                                selected[i] = false;
                                text_death[i].SetColor(TextObject::BLACK_TEXT);
                                text_death[i].LoadFromRenderText(font, des);
                                text_death[i].RenderText(des, pos_item[i].x, pos_item[i].y);

                            }
                        }
                    }
                    break;
                }
            case SDL_MOUSEBUTTONDOWN:
                {
                    xm = m_event.button.x;
                    ym = m_event.button.y;
                    for (int i = 0;i < kDeathItemNum;i++)
                    {
                        if(CheckFocusWithRect(xm,ym,text_death[i].GetRect()))
                        {
                            g_death.Free();
                            return i;
                        }
                    }
                    break;
                }
            case SDL_KEYDOWN:
                {
                    if(m_event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        g_death.Free();
                        return 1;
                    }
                    break;
                }
            default:
                break;
            }
        }
        SDL_RenderPresent(des);
    }
    g_death.Free();
    return 1;
}
int ShowEnd(SDL_Renderer* des, TTF_Font* font)
{
    if(LoadEnd(des) == false)
        return 1;

    const int kEndItemNum = 2;
    SDL_Rect pos_item[kEndItemNum];
    pos_item[0].x = 500;
    pos_item[0].y = 450;

    pos_item[1].x = 630;
    pos_item[1].y = 530;

    TextObject text_end[kEndItemNum];

    text_end[0].SetText("Tao la Yasuo");
    text_end[0].SetColor(TextObject::WHITE_TEXT);
    text_end[0].LoadFromRenderText(font, des);

    text_end[1].SetText("-Yasuo");
    text_end[1].SetColor(TextObject::WHITE_TEXT);
    text_end[1].LoadFromRenderText(font, des);

    bool selected[kEndItemNum] = {0, 0};
    int xm = 0;
    int ym = 0;
    SDL_Event m_event;
    while(true)
    {
        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);
        g_end.Render(des, NULL);
        for (int i = 0;i < kEndItemNum;i++)
        {
            text_end[i].RenderText(des, pos_item[i].x, pos_item[i].y);
        }
        while(SDL_PollEvent(&m_event))
        {
            switch (m_event.type)
            {
            case SDL_QUIT:
                return 1;
            case SDL_MOUSEMOTION:
                {
                    xm = m_event.motion.x;
                    ym = m_event.motion.y;

                    for (int i = 0;i < kEndItemNum;i++)
                    {
                        if(CheckFocusWithRect(xm,ym,text_end[i].GetRect()))
                        {
                            if(selected[i] == false)
                            {
                                selected[i] = true;
                                text_end[i].SetColor(TextObject::RED_TEXT);
                                text_end[i].LoadFromRenderText(font, des);
                                text_end[i].RenderText(des, pos_item[i].x, pos_item[i].y);


                            }
                        }
                        else
                        {
                            if(selected[i] == true)
                            {
                                selected[i] = false;
                                text_end[i].SetColor(TextObject::WHITE_TEXT);
                                text_end[i].LoadFromRenderText(font, des);
                                text_end[i].RenderText(des, pos_item[i].x, pos_item[i].y);

                            }
                        }
                    }
                    break;
                }
            case SDL_MOUSEBUTTONDOWN:
                {
                    xm = m_event.button.x;
                    ym = m_event.button.y;
                    for (int i = 0;i < kEndItemNum;i++)
                    {
                        if(CheckFocusWithRect(xm,ym,text_end[i].GetRect()))
                        {
                            g_end.Free();
                            return 1;
                        }
                    }
                    break;
                }
            case SDL_KEYDOWN:
                {
                    if(m_event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        g_end.Free();
                        return 1;
                    }
                    break;
                }
            default:
                break;
            }
        }
        SDL_RenderPresent(des);
    }
    g_end.Free();
    return 1;
}
int ShowIntro(SDL_Renderer* des, TTF_Font* font)
{
    if(LoadIntro(des) == false)
        return 1;

    const int kIntroItemNum = 1;
    SDL_Rect pos_item[kIntroItemNum];
    pos_item[0].x = 80;
    pos_item[0].y = 550;

    TextObject text_intro[kIntroItemNum];

    text_intro[0].SetText("Ve thoi");
    text_intro[0].SetColor(TextObject::BLACK_TEXT);
    text_intro[0].LoadFromRenderText(font, des);

    bool selected[kIntroItemNum] = {0};
    int xm = 0;
    int ym = 0;
    SDL_Event m_event;
    while(true)
    {
        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);
        g_intro.Render(des, NULL);
        for (int i = 0;i < kIntroItemNum;i++)
        {
            text_intro[i].RenderText(des, pos_item[i].x, pos_item[i].y);
        }
        while(SDL_PollEvent(&m_event))
        {
            switch (m_event.type)
            {
            case SDL_QUIT:
                return 1;
            case SDL_MOUSEMOTION:
                {
                    xm = m_event.motion.x;
                    ym = m_event.motion.y;

                    for (int i = 0;i < kIntroItemNum;i++)
                    {
                        if(CheckFocusWithRect(xm,ym,text_intro[i].GetRect()))
                        {
                            if(selected[i] == false)
                            {
                                selected[i] = true;
                                text_intro[i].SetColor(TextObject::RED_TEXT);
                                text_intro[i].LoadFromRenderText(font, des);
                                text_intro[i].RenderText(des, pos_item[i].x, pos_item[i].y);


                            }
                        }
                        else
                        {
                            if(selected[i] == true)
                            {
                                selected[i] = false;
                                text_intro[i].SetColor(TextObject::BLACK_TEXT);
                                text_intro[i].LoadFromRenderText(font, des);
                                text_intro[i].RenderText(des, pos_item[i].x, pos_item[i].y);

                            }
                        }
                    }
                    break;
                }
            case SDL_MOUSEBUTTONDOWN:
                {
                    xm = m_event.button.x;
                    ym = m_event.button.y;
                    for (int i = 0;i < kIntroItemNum;i++)
                    {
                        if(CheckFocusWithRect(xm,ym,text_intro[i].GetRect()))
                        {
                            g_intro.Free();
                            return i;
                        }
                    }
                    break;
                }
            case SDL_KEYDOWN:
                {
                    if(m_event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        g_intro.Free();
                        return 1;
                    }
                    break;
                }
            default:
                break;
            }
        }
        SDL_RenderPresent(des);
    }
    g_intro.Free();
    return 1;
}
