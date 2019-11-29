#include "CButton.h"

CButton::CButton(int x, int y, int w, int h, std::string _title)
{
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	title = _title;
}

void CButton::draw_myself(CSDL* sdl, int status)
{
	if(status==1)
		sdl->draw_filled_rect(rect.x, rect.y, rect.w, rect.h, 125, 125, 125, 0);
	else 
		sdl->draw_filled_rect(rect.x, rect.y, rect.w, rect.h, 255, 255, 255, 0);

	sdl->draw_text(title, rect.x + 10, rect.y + 10, rect.w - 20, rect.h - 20, 255, 0, 0, 0);
}

SDL_Rect CButton::get_rect()
{
	return rect;
}
