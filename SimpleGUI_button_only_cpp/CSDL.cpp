#include "CSDL.h"

CSDL::CSDL(int x, int y, int w, int h, std::string title)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logErr("SDL_Init");
	}
	if (TTF_Init() != 0) {
		logErr("TTF_Init");
	}

	g_font = TTF_OpenFont("simhei.ttf", 64);
	if (g_font == nullptr) {
		logErr("TTF_OpenFont");		
	}

	g_win = SDL_CreateWindow(title.c_str(), x, y, w, h, SDL_WINDOW_SHOWN);
	if (g_win == NULL) {
		logErr("SDL_CreateWindow");
	}

	g_ren = SDL_CreateRenderer(g_win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (g_ren == NULL) {
		SDL_DestroyWindow(g_win);
		logErr("SDL_CreateRenderer");
	}

	g_scr_width = w;
	g_scr_height = h;
}

CSDL::~CSDL()
{
	for(tTexture tex : g_texture_lst)
	{
		SDL_DestroyTexture(tex.ptr);
	}
	TTF_CloseFont(g_font);

	SDL_DestroyRenderer(g_ren);
	SDL_DestroyWindow(g_win);
	SDL_Quit();
}

void CSDL::draw_filled_rect(int x, int y, int w, int h, int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(g_ren, r, g, b, a);
	SDL_Rect btn_rect;
	btn_rect.w = w;
	btn_rect.h = h;
	btn_rect.x = x;
	btn_rect.y = y;
	SDL_RenderFillRect(g_ren, &btn_rect);
}

void CSDL::draw_text(std::string txt, int x, int y, int w, int h, int r, int g, int b, int a)
{
	SDL_Texture* texture = NULL;
	for (tTexture tex : g_texture_lst)
	{
		if (tex.text == txt && tex.color.r == r && tex.color.g == g && tex.color.b == b)
		{
			texture = tex.ptr;
			break;
		}
	}
	if (texture == NULL)
	{
		//We need to first render to a surface as that's what TTF_RenderText
		//returns, then load that surface into a texture
		SDL_Color color = { r, g, b, a };
		SDL_Surface* surf = TTF_RenderText_Blended(g_font, txt.c_str(), color);
		if (surf == nullptr) {
			TTF_CloseFont(g_font);
			logErr("TTF_RenderText");
		}
		texture = SDL_CreateTextureFromSurface(g_ren, surf);
		if (texture == nullptr) {
			logErr("CreateTexture");
		}
		//SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
		//Clean up the surface and font
		SDL_FreeSurface(surf);

		tTexture new_tex;
		new_tex.ptr = texture;
		new_tex.text = txt;		
		new_tex.color = color;
		g_texture_lst.push_back(new_tex);
	}

	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(g_ren, texture, NULL, &dst);
	
	//SDL_DestroyTexture(texture);	//加上这一行出现奇怪的行为：只显示字符；不加就存在内存泄漏。没办法只好用g_texture_lst把文字图像都保存起来，最后集中释放。
}

void CSDL::set_draw_color(int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(g_ren, r, g, b, a);
}

void CSDL::clear_all()
{
	SDL_RenderClear(g_ren);
}

void CSDL::show_all()
{
	SDL_RenderPresent(g_ren);
}

void CSDL::set_background_color(int r, int g, int b)
{
	bg_color.r = r;
	bg_color.g = g;
	bg_color.b = b;
}

SDL_Rect CSDL::get_window_size()
{
	SDL_Rect rect = { 0, 0, g_scr_width, g_scr_height };
	return rect;
}

void CSDL::logErr(const char* errStr)
{
	printf("Error %s: %s \n", errStr, SDL_GetError());
	SDL_Quit();
	exit(1);
}
