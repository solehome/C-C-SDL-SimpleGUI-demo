// SimpleGUI_button_only_refactor.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "gui_header.h"

int main()
{
	init_gui();

	create_window(100, 100, 640, 480);
	create_button(119, "change bg color", 100, 100, 300, 100);
	create_button(120, "change btn title", 100, 250, 300, 100);

	bool quit = false;
	while (!quit)
	{
		scan_event();

		if (if_event_quit())
			quit = true;

		if (if_event_btn_clicked(119))
		{
			set_background_color(0, 255, 0);
		}

		if (if_event_btn_clicked(120))
			change_button_title(120, "oh yeah!");

		draw_all();
	}
	
	release_gui();

	return 0;
}