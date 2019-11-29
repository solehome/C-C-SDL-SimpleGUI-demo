#pragma once
#include <string>
#include <vector>
#include "CBaseGUIControl.h"
#include "CSDL.h"

class CApp
{
public:
	CApp(int x, int y, int w, int h, std::string title);
	~CApp();

	void run();

	void add_control(CBaseGUIControl* control);

private:
	CSDL* g_sdl;

	std::vector<CBaseGUIControl*> g_control_lst;
	std::vector<int> g_control_status_lst; //0:normal, 1:press_down
};

