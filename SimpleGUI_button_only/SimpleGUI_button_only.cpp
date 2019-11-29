// SimpleGUI_button_only.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "sdl.h"
#include "sdl_ttf.h"
#include <stdio.h>

#include <stdlib.h>
#include <locale.h>

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2_ttf.lib")

#undef main

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int bgColor_r = 255, bgColor_g = 0, bgColor_b = 0;

struct tButton {
	int x, y, w, h;
	int color_r_normal, color_g_normal, color_b_normal;
	int color_r_press, color_g_press, color_b_press;
	bool bPressed;
};

void logErr(const char* errStr);
SDL_Texture* renderText(const char* message, const char* fontFile,
	SDL_Color color, int fontSize, SDL_Renderer* renderer);
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, int w, int h);

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logErr("SDL_Init");		
	}
	if (TTF_Init() != 0) {
		logErr("TTF_Init");		
	}

	SDL_Window* win = SDL_CreateWindow("Hello World!", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		logErr("SDL_CreateWindow");		
	}

	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr) {
		SDL_DestroyWindow(win);
		logErr("SDL_CreateRenderer");
	}

	//create a button
	tButton btn;
	btn.w = 200;
	btn.h = 100;
	btn.x = (SCREEN_WIDTH - btn.w) / 2;
	btn.y = (SCREEN_HEIGHT - btn.h) / 2;
	btn.color_r_normal = 255;
	btn.color_g_normal = 0;
	btn.color_b_normal = 0;
	btn.color_r_press = 255;
	btn.color_g_press = 255;
	btn.color_b_press = 0;
	btn.bPressed = false;
	
	//create a texture to hold the text
	SDL_Color color = { 255, 255, 255, 255 };	

	SDL_Texture* image = renderText("Button", "simhei.ttf",
		color, 64, ren);
	if (image == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		
		TTF_Quit();
		SDL_Quit();
		return 1;
	}
	//Get the texture w/h so we can center it in the screen
	int iW, iH;
	SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
	float textScale = float(iW) / btn.w;
	iW = int(iW / textScale);
	iH = int(iH / textScale);
	int text_x = SCREEN_WIDTH / 2 - iW / 2;
	int text_y = SCREEN_HEIGHT / 2 - iH / 2;
	int text_w = iW;
	int text_h = iH;

	SDL_Event e;
	bool quit = false;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			if (e.type == SDL_KEYDOWN) {
				quit = true;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (e.button.x > btn.x&& e.button.x < btn.x + btn.w)
				{
					if (e.button.y > btn.y&& e.button.y < btn.y + btn.h)
					{
						btn.bPressed = true;
					}
				}				
			}
			if (e.type == SDL_MOUSEBUTTONUP) {
				if (btn.bPressed)
				{
					btn.bPressed = false;

					//call the button click handler
					bgColor_r = 100;
					bgColor_g = 100;
					bgColor_b = 100;
				}
				
			}
		}	
	    //A sleepy rendering loop, wait for 3 seconds and render and present the screen each time
		//First clear the renderer
		SDL_SetRenderDrawColor(ren, bgColor_r, bgColor_g, bgColor_b, 0);
		SDL_RenderClear(ren);
		//Draw the texture
		/*SDL_RenderCopy(ren, tex, NULL, NULL);*/

		//draw a button
		if(!btn.bPressed)
			SDL_SetRenderDrawColor(ren, btn.color_r_normal, btn.color_g_normal, btn.color_b_normal, 0);
		else
			SDL_SetRenderDrawColor(ren, btn.color_r_press, btn.color_g_press, btn.color_b_press, 0);
		SDL_Rect btn_rect;
		btn_rect.w = btn.w;
		btn_rect.h = btn.h;
		btn_rect.x = btn.x;
		btn_rect.y = btn.y;
		SDL_RenderFillRect(ren, &btn_rect);

		//draw the text
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);		
		renderTexture(image, ren, text_x, text_y, text_w, text_h);		
				
		//Update the screen
		SDL_RenderPresent(ren);		
	}

	//SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}

void logErr(const char* errStr)
{
	printf("Error %s: %s \n", errStr, SDL_GetError());
	SDL_Quit();
	exit(1);
}

/**
* Render the message we want to display to a texture for drawing
* @param message The message we want to display
* @param fontFile The font we want to use to render the text
* @param color The color we want the text to be
* @param fontSize The size we want the font to be
* @param renderer The renderer to load the texture in
* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
*/
SDL_Texture* renderText(const char* message, const char* fontFile,
	SDL_Color color, int fontSize, SDL_Renderer* renderer)
{
	//Open the font
	TTF_Font* font = TTF_OpenFont(fontFile, fontSize);
	if (font == nullptr) {
		logErr("TTF_OpenFont");		
		return nullptr;
	}
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface* surf = TTF_RenderText_Blended(font, message, color);
	if (surf == nullptr) {
		TTF_CloseFont(font);
		logErr("TTF_RenderText");		
		return nullptr;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr) {
		logErr("CreateTexture");		
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}

void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, int w, int h) {
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	//Query the texture to get its width and height to use
	//SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

