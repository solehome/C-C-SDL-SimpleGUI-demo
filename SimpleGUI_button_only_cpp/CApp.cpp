#include "CApp.h"

CApp::CApp(int x, int y, int w, int h, std::string title)
{
	g_sdl = NULL;

	g_sdl = new CSDL(x, y, w, h, title);
}

CApp::~CApp()
{
	if (g_sdl)
		delete g_sdl;

	for (CBaseGUIControl* control : g_control_lst) {
		delete control;
	}
}

void CApp::run()
{
	bool quit = false;
	long cnt = 0;

	SDL_Event msg;

	while (!quit)
	{
		SDL_PollEvent(&msg);

		if (msg.type == SDL_QUIT)
			quit = true;

		if (msg.type == SDL_MOUSEBUTTONDOWN)
		{
			//check every control
			for (size_t i=0; i<g_control_lst.size(); i++)
			{
				SDL_Rect rect = g_control_lst[i]->get_rect();
				if (msg.button.x > rect.x&& msg.button.x < rect.x + rect.w)
				{
					if (msg.button.y > rect.y&& msg.button.y < rect.y + rect.h)
					{
						g_control_status_lst[i] = 1;
					}
				}
			}
		}

		if (msg.type == SDL_MOUSEBUTTONUP)
		{
			//check every control
			for (size_t i = 0; i < g_control_lst.size(); i++)
			{
				if (g_control_status_lst[i] == 1)
				{
					g_control_lst[i]->on_clicked(g_sdl);
					g_control_status_lst[i] = 0;
				}
			}
		}

		//clear the window
		SDL_Color bg_color = g_sdl->get_bg_color();
		g_sdl->set_draw_color(bg_color.r, bg_color.g, bg_color.b, 0);
		g_sdl->clear_all();
		
		
		//draw all the button
		for (size_t i = 0; i < g_control_lst.size(); i++) {
			g_control_lst[i]->draw_myself(g_sdl, g_control_status_lst[i]);
		}

		g_sdl->show_all();
	}
}

void CApp::add_control(CBaseGUIControl* control)
{
	g_control_lst.push_back(control);
	g_control_status_lst.push_back(0);
}
