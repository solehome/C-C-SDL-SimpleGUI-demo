#pragma once
#include "CBaseGUIControl.h"
#include <string>

class CButton:public CBaseGUIControl
{
public:
	CButton() {};
	CButton(int x, int y, int w, int h, std::string title);

	virtual void draw_myself(CSDL* sdl, int status=0);
	virtual SDL_Rect get_rect();

protected:
	SDL_Rect rect;
	std::string title;
};

