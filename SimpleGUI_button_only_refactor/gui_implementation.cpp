#include "gui_header.h"

int g_scr_width = 0;
int g_scr_height = 0;

SDL_Window* g_win = NULL;
SDL_Renderer* g_ren = NULL;
SDL_Event g_msg;

tButton g_button_list[MAX_BUTTON_NUM];
int g_current_button_num = 0;

SDL_Color g_bg_color;

void logErr(const char* errStr)
{
	printf("Error %s: %s \n", errStr, SDL_GetError());
	SDL_Quit();
	exit(1);
}

void init_gui() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logErr("SDL_Init");
	}
	if (TTF_Init() != 0) {
		logErr("TTF_Init");
	}

	g_bg_color.r = g_bg_color.g = g_bg_color.b = 0;
}

void release_gui()
{
	for (int i = 0; i < g_current_button_num; i++)
	{
		SDL_DestroyTexture(g_button_list[i].title_texture);
	}

	SDL_DestroyRenderer(g_ren);
	SDL_DestroyWindow(g_win);
	SDL_Quit();
}

void create_window(int x, int y, int w, int h)
{
	g_win = SDL_CreateWindow("Hello World!", x, y, w, h, SDL_WINDOW_SHOWN);
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

SDL_Texture* _renderText(const char* message, const char* fontFile,
	SDL_Color color, int fontSize, SDL_Renderer* renderer);

void create_button(const int ID, const char* title, int x, int y, int w, int h, int r_bg,
	int g_bg, int b_bg, int r_title, int g_title, int b_title)
{
	if(g_current_button_num == MAX_BUTTON_NUM)
		logErr("CAN not add more button.");

	g_button_list[g_current_button_num].ID = ID;
	strcpy_s(g_button_list[g_current_button_num].title, title);
	g_button_list[g_current_button_num].x = x;
	g_button_list[g_current_button_num].y = y;
	g_button_list[g_current_button_num].w = w;
	g_button_list[g_current_button_num].h = h;

	g_button_list[g_current_button_num].color_r_bg = r_bg;
	g_button_list[g_current_button_num].color_g_bg = g_bg;
	g_button_list[g_current_button_num].color_b_bg = b_bg;
	g_button_list[g_current_button_num].color_r_title = r_title;
	g_button_list[g_current_button_num].color_g_title = g_title;
	g_button_list[g_current_button_num].color_b_title = b_title;

	g_button_list[g_current_button_num].bPressed = false;
	g_button_list[g_current_button_num].pushing = false;

	SDL_Color color;
	color.a = 0;
	color.r = g_button_list[g_current_button_num].color_r_title;
	color.g = g_button_list[g_current_button_num].color_g_title;
	color.b = g_button_list[g_current_button_num].color_b_title;
	SDL_Texture* image = _renderText(title, FONT_PATH, color, 64, g_ren);
	if (image == nullptr) {
		SDL_DestroyRenderer(g_ren);
		SDL_DestroyWindow(g_win);

		TTF_Quit();
		SDL_Quit();
		return;
	}
	g_button_list[g_current_button_num].title_texture = image;

	g_current_button_num++;
}

void change_button_title(const int ID, const char* new_title)
{
	for (int i = 0; i < g_current_button_num; i++)
	{
		if (g_button_list[i].ID == ID)
		{
			SDL_DestroyTexture(g_button_list[i].title_texture);

			SDL_Color color;
			color.a = 0;
			color.r = g_button_list[i].color_r_title;
			color.g = g_button_list[i].color_g_title;
			color.b = g_button_list[i].color_b_title;
			SDL_Texture* image = _renderText(new_title, FONT_PATH, color, 64, g_ren);
			if (image == nullptr) {
				SDL_DestroyRenderer(g_ren);
				SDL_DestroyWindow(g_win);

				TTF_Quit();
				SDL_Quit();
				return;
			}
			g_button_list[i].title_texture = image;
			break;
		}
	}
}

void set_background_color(int r, int g, int b)
{
	g_bg_color.r = r;
	g_bg_color.g = g;
	g_bg_color.b = b;
}

void draw_all()
{
	//Update the screen
	SDL_RenderPresent(g_ren);
}

void _button_event_handler()
{
	for (int i = 0; i < g_current_button_num; i++)
	{
		if (g_msg.type == SDL_MOUSEBUTTONDOWN)
		{
			if (g_msg.button.x > g_button_list[i].x&& g_msg.button.x < g_button_list[i].x + g_button_list[i].w)
			{
				if (g_msg.button.y > g_button_list[i].y&& g_msg.button.y < g_button_list[i].y + g_button_list[i].h)
				{
					g_button_list[i].pushing = true;
				}
			}
		}
		if (g_msg.type == SDL_MOUSEBUTTONUP)
		{
			if (g_button_list[i].pushing)
			{
				g_button_list[i].pushing = false;
				g_button_list[i].bPressed = true;
			}
		}
	}
}

SDL_Texture* _renderText(const char* message, const char* fontFile,
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

void _renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, int w, int h) {
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

void _draw_buttons()
{
	for (int i = 0; i < g_current_button_num; i++)
	{
		// draw background color
		if (!g_button_list[i].pushing)
		{
			SDL_SetRenderDrawColor(g_ren, g_button_list[i].color_r_bg, g_button_list[i].color_g_bg,
				g_button_list[i].color_b_bg, 0);
		}
		else { // when the button is pushed, draw with the inverse color of its bg color
			SDL_SetRenderDrawColor(g_ren, 255-g_button_list[i].color_r_bg, 255-g_button_list[i].color_g_bg,
				255-g_button_list[i].color_b_bg, 0);
		}
		SDL_Rect btn_rect;
		btn_rect.w = g_button_list[i].w;
		btn_rect.h = g_button_list[i].h;
		btn_rect.x = g_button_list[i].x;
		btn_rect.y = g_button_list[i].y;
		SDL_RenderFillRect(g_ren, &btn_rect);

		// draw the title
		SDL_SetRenderDrawColor(g_ren, g_button_list[i].color_r_title, g_button_list[i].color_g_title,
			g_button_list[i].color_b_title, 0);
		
		//Get the texture w/h so we can center it in the screen
		int iW, iH;
		SDL_QueryTexture(g_button_list[i].title_texture, NULL, NULL, &iW, &iH);
		float textScale = float(iW) / g_button_list[i].w;
		if (float(iH) / g_button_list[i].h > textScale)
			textScale = float(iH) / g_button_list[i].h;

		iW = int(iW / textScale);
		iH = int(iH / textScale);
		int text_x = g_button_list[i].x + (g_button_list[i].w - iW)/2;
		int text_y = g_button_list[i].y + (g_button_list[i].h - iH)/2;
		int text_w = iW;
		int text_h = iH;

		_renderTexture(g_button_list[i].title_texture, g_ren, text_x, text_y, text_w, text_h);

	}
}
void scan_event()
{
	SDL_PollEvent(&g_msg);

	//process some events which are hidden to users
	if (g_msg.type == SDL_MOUSEBUTTONDOWN || g_msg.type == SDL_MOUSEBUTTONUP)
		_button_event_handler();

	//draw GUI controls
	SDL_SetRenderDrawColor(g_ren, g_bg_color.r, g_bg_color.g, g_bg_color.b, 0);
	SDL_RenderClear(g_ren);

	_draw_buttons();
}

bool if_event_quit()
{
	if (g_msg.type == SDL_QUIT) return true;	
	else return false;
}

bool if_event_btn_clicked(int ID)
{
	for (int i = 0; i < g_current_button_num; i++)
	{
		if (ID == g_button_list[i].ID) {
			if (g_button_list[i].bPressed)
			{
				g_button_list[i].bPressed = false;
				return true;
			}
		}
	}
	return false;
}

