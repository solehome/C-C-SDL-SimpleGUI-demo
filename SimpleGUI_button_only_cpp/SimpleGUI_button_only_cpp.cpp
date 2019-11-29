// SimpleGUI_button_only_cpp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//// function example:  http://www.cplusplus.com/forum/general/120728/
//#include <iostream>     // std::cout
//#include <functional>   // std::function, std::negate
//
//// a function:
//int half(int x) { return x / 2; }
//
//// a function object class:
//struct third_t {
//	int operator()(int x) { return x / 3; }
//};
//
//// a class with data members:
//struct MyValue {
//	int value;
//	int fifth() { return value / 5; }
//};
//
//int main() {
//	std::function<int(int)> fn1 = half;                    // function
//	std::function<int(int)> fn2 = &half;                   // function pointer
//	std::function<int(int)> fn3 = third_t();               // function object
//	std::function<int(int)> fn4 = [](int x) {return x / 4; };  // lambda expression
//	std::function<int(int)> fn5 = std::negate<int>();      // standard function object
//
//	std::cout << "fn1(60): " << fn1(60) << '\n';
//	std::cout << "fn2(60): " << fn2(60) << '\n';
//	std::cout << "fn3(60): " << fn3(60) << '\n';
//	std::cout << "fn4(60): " << fn4(60) << '\n';
//	std::cout << "fn5(60): " << fn5(60) << '\n';
//
//	// stuff with members:
//	std::function<int(MyValue&)> value = &MyValue::value;  // pointer to data member
//	std::function<int(MyValue&)> fifth = &MyValue::fifth;  // pointer to member function
//
//	MyValue sixty{ 60 };
//
//	std::cout << "value(sixty): " << value(sixty) << '\n';
//	std::cout << "fifth(sixty): " << fifth(sixty) << '\n';
//
//	return 0;
//}

#include "CApp.h"
#include "CButton.h"
#include <iostream>
#include <random>

class CButton_change_bg_color : public CButton {
public:
	CButton_change_bg_color(int x, int y, int w, int h, std::string _title):
		CButton(x, y, w, h, _title)	{};

	virtual void on_clicked(CSDL* sdl) {
		sdl->set_background_color(0, 0, 255);
	};
};

class CShakeButton :public CButton
{
public:
	CShakeButton(int x, int y, int w, int h, std::string title, int _shake_strength):
		CButton(x, y, w, h, title) {
		shake_strength = _shake_strength;
		u = new std::uniform_int_distribution<int>(-shake_strength, shake_strength);
	};
	~CShakeButton() { delete u; };

	virtual void draw_myself(CSDL* sdl, int status = 0)
	{		
		int offset = (*u)(e);

		if (status == 1)
		{
			sdl->draw_filled_rect(rect.x + offset, rect.y + offset, rect.w, rect.h, 125, 125, 125, 0);
			sdl->draw_text(title, rect.x + 10+offset, rect.y + 10+ offset, rect.w - 20, rect.h - 20, 255, 0, 0, 0);
		}
		else
		{
			sdl->draw_filled_rect(rect.x, rect.y, rect.w, rect.h, 255, 255, 255, 0);
			sdl->draw_text(title, rect.x + 10, rect.y + 10, rect.w - 20, rect.h - 20, 255, 0, 0, 0);
		}		
	}

protected:
	int shake_strength;
	std::default_random_engine e;
	std::uniform_int_distribution<int>* u;
};

int main()
{
	CApp app(100, 100, 640, 480, "OK");

	CButton *btn1 = new CButton(100, 100, 300, 100, "push me");
	app.add_control(btn1);
	CButton_change_bg_color* btn2 = new CButton_change_bg_color(100, 220, 300, 100, "do something");
	app.add_control(btn2);
	CShakeButton* btn3 = new CShakeButton(100, 340, 300, 100, "shake me", 10);
	app.add_control(btn3);

	app.run();

	return 0;
}