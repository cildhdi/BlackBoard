#pragma once
#ifndef _DRAW_OPERATE_H_
#define _DRAW_OPERATE_H_
#include "GlobalData.h"

#include <vector>

namespace nsDrawOperate
{
	class DrawOperate
	{
		using VP = std::vector<Gdiplus::Point>;
	protected:
		nsGlobalData::DrawInfo m_DrawInfo;
		VP m_Points;
	public:
		VP & GetPoints();
		DrawOperate(nsGlobalData::DrawInfo& draw_info);
		virtual void Draw(Gdiplus::Graphics* pGraphics);
		void SetFirstPoint(unsigned int x, unsigned int y);
		void SetSecondPoint(unsigned int x, unsigned int y);
		void AddPoint(unsigned int x, unsigned int y);
	};

	class Line :public DrawOperate
	{
	public:
		Line(nsGlobalData::DrawInfo& draw_info) :DrawOperate(draw_info) {}
		void Draw(Gdiplus::Graphics* pGraphics);
	};

	class Rectangle :public DrawOperate
	{
	public:
		Rectangle(nsGlobalData::DrawInfo& draw_info) :DrawOperate(draw_info) {}
		void Draw(Gdiplus::Graphics* pGraphics);
	};

	class Elliptic :public DrawOperate
	{
	public:
		Elliptic(nsGlobalData::DrawInfo& draw_info) :DrawOperate(draw_info) {}
		void Draw(Gdiplus::Graphics* pGraphics);
	};

	class Circle :public DrawOperate
	{
	public:
		Circle(nsGlobalData::DrawInfo& draw_info) :DrawOperate(draw_info) {}
		void Draw(Gdiplus::Graphics* pGraphics);
	};

	class Chalk :public DrawOperate
	{
	public:
		Chalk(nsGlobalData::DrawInfo& draw_info) :DrawOperate(draw_info) {}
		void Draw(Gdiplus::Graphics* pGraphics);
	};
}


#endif