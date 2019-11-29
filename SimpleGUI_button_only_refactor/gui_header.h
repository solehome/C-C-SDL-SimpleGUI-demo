#pragma once

#include "sdl.h"
#include "sdl_ttf.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2_ttf.lib")
//#pragma comment(lib, "SDL2main.lib")
#undef main

#define MAX_BUTTON_NUM 10
#define MAX_TITLE_LEN 30
#define FONT_PATH "simhei.ttf"

struct tButton {
	int ID;
	char title[MAX_TITLE_LEN];
	int x, y, w, h;
	//int color_r_normal, color_g_normal, color_b_normal; // will use the inverse color of bg as the pressed color
	int color_r_bg, color_g_bg, color_b_bg;
	int color_r_title, color_g_title, color_b_title;

	bool pushing;
	bool bPressed;

	SDL_Texture* title_texture;
};

void init_gui();

void release_gui();

void create_window(int x, int y, int w, int h);

void create_button(const int ID, const char* title, int x, int y, int w, int h, int r_bg=125,
	int g_bg=125, int b_bg=0, int r_title=255, int g_title=0, int b_title=0);

void change_button_title(const int ID, const char* new_title);

void set_background_color(int r, int g, int b);

void draw_all();

void scan_event();

bool if_event_quit();

bool if_event_btn_clicked(int ID);