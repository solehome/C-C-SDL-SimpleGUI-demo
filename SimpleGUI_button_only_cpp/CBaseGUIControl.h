#pragma once

#include "CSDL.h" 

class CBaseGUIControl
{
public:
	virtual void draw_myself(CSDL* sdl, int status=0);//0:normal, 1:press_down
	virtual void on_clicked(CSDL* sdl);
	virtual SDL_Rect get_rect();
};

