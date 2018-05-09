#pragma once
#ifndef _GLOBAL_DATA_H_
#define _GLOBAL_DATA_H_
#include <windows.h>
#include <gdiplus.h>
namespace nsGlobalData
{
	enum DrawType
	{
		None, Line, Rectangle, Circle, Elliptic, Chalk, Eraser
	};

	typedef struct
	{
		DrawType draw_type;
		unsigned int line_width;
		Gdiplus::Color line_color;
	}DrawInfo;

	static DrawInfo g_draw_info = { DrawType::None,3,static_cast<Gdiplus::Color>(Gdiplus::Color::White) };

}//namespace nsGlobalData

#endif