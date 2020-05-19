#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <windows.h>
#include <iostream>
#include <string>
#include <conio.h>
#include <stdio.h>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

using namespace std;

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

const int FRAME_PER_SECOND = 50;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const float GRAVITY_SPEED = 0.8;
const int MAX_FALL_SPEED = 8;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0xFF;

bool CheckCollision(const SDL_Rect object_1, const SDL_Rect object_2);
bool LoadMenu(SDL_Renderer* screen);
bool LoadDeath(SDL_Renderer* screen);
bool LoadEnd(SDL_Renderer* screen);
bool LoadIntro(SDL_Renderer* screen);
bool CheckFocusWithRect(const int& x, const int& y, const SDL_Rect& rect);
int ShowDeath(SDL_Renderer* des, TTF_Font* font);
int ShowMenu(SDL_Renderer* des, TTF_Font* font);
int ShowEnd(SDL_Renderer* des, TTF_Font* font);
int ShowIntro(SDL_Renderer* des, TTF_Font* font);

typedef struct Input
{
  int left_;
  int right_;
  int up_;
  int down_;
  int jump_;
};
#endif // COMMON_H_INCLUDED
