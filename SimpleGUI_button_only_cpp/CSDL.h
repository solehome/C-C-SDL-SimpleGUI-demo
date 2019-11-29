#pragma once

#include "sdl.h"
#include "sdl_ttf.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <string>
#include <sstream>
#include <vector>

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2_ttf.lib")
//#pragma comment(lib, "SDL2main.lib")
#undef main

struct tTexture {
	std::string text;
	SDL_Color color;

	SDL_Texture* ptr;
};

class CSDL
{
public:
	CSDL(int x, int y, int w, int h, std::string title);
	~CSDL();
	
	void draw_filled_rect(int x, int y, int w, int h, int r, int g, int b, int a);
	void draw_text(std::string txt, int x, int y, int w, int h, int r, int g, int b, int a);

	void set_draw_color(int r, int g, int b, int a);
	void clear_all();
	void show_all();
	void set_background_color(int r, int g, int b);
	SDL_Color get_bg_color() { return bg_color; };
	SDL_Rect get_window_size();

private:
	SDL_Window* g_win = NULL;
	SDL_Renderer* g_ren = NULL;

	TTF_Font* g_font = NULL;

	int g_scr_width = 0;
	int g_scr_height = 0;

	SDL_Color bg_color;
	
	std::vector<tTexture> g_texture_lst;

	void logErr(const char* errStr);
};

