#include "DrawOperate.h"

namespace nsDrawOperate
{

	DrawOperate::DrawOperate(nsGlobalData::DrawInfo& draw_info) :m_DrawInfo(draw_info) {}
	void DrawOperate::Draw(Gdiplus::Graphics* pGraphics) {}

	std::vector<Gdiplus::Point>& DrawOperate::GetPoints()
	{
		return m_Points;
	}

	void DrawOperate::SetFirstPoint(unsigned int x, unsigned int y)
	{
		while (m_Points.size() < 1)
		{
			m_Points.push_back(Gdiplus::Point(0, 0));
		}
		m_Points[0].X = x;
		m_Points[0].Y = y;
	}
	void DrawOperate::SetSecondPoint(unsigned int x, unsigned int y)
	{
		while (m_Points.size() < 2)
		{
			m_Points.push_back(Gdiplus::Point(0, 0));
		}
		m_Points[1].X = x;
		m_Points[1].Y = y;
	}
	void DrawOperate::AddPoint(unsigned int x, unsigned int y)
	{
		m_Points.push_back(Gdiplus::Point(x, y));
	}


	void Line::Draw(Gdiplus::Graphics* pGraphics)
	{
		if (m_Points.size() > 1)
		{
			pGraphics->DrawLine(&Gdiplus::Pen(&Gdiplus::SolidBrush(m_DrawInfo.line_color), static_cast<Gdiplus::REAL>(m_DrawInfo.line_width)), m_Points.at(0), m_Points.at(1));
		}
	}

	void Rectangle::Draw(Gdiplus::Graphics* pGraphics)
	{
		if (m_Points.size() > 1)
		{
			Gdiplus::Rect rect;
			rect.X = m_Points.at(1).X < m_Points.at(0).X ? m_Points.at(1).X : m_Points.at(0).X;
			rect.Y = m_Points.at(1).Y < m_Points.at(0).Y ? m_Points.at(1).Y : m_Points.at(0).Y;
			rect.Width = abs(m_Points.at(1).X - m_Points.at(0).X);
			rect.Height = abs(m_Points.at(1).Y - m_Points.at(0).Y);
			pGraphics->DrawRectangle(&Gdiplus::Pen(&Gdiplus::SolidBrush(m_DrawInfo.line_color), static_cast<Gdiplus::REAL>(m_DrawInfo.line_width)), rect);
		}
	}

	void Circle::Draw(Gdiplus::Graphics* pGraphics)
	{
		if (m_Points.size() > 1)
		{
			Gdiplus::Rect rect;
			double r = sqrt(pow(abs(m_Points.at(1).X - m_Points.at(0).X), 2) + pow(abs(m_Points.at(1).Y - m_Points.at(0).Y), 2));
			rect.X = m_Points[0].X - static_cast<int>(r);
			rect.Y = m_Points[0].Y - static_cast<int>(r);
			rect.Width = static_cast<int>(2 * r);
			rect.Height = static_cast<int>(2 * r);
			pGraphics->DrawEllipse(&Gdiplus::Pen(&Gdiplus::SolidBrush(m_DrawInfo.line_color), static_cast<Gdiplus::REAL>(m_DrawInfo.line_width)), rect);
		}
	}

	void Elliptic::Draw(Gdiplus::Graphics* pGraphics)
	{
		if (m_Points.size() > 1)
		{
			Gdiplus::Rect rect;
			rect.X = m_Points[0].X - abs(m_Points.at(1).X - m_Points.at(0).X);
			rect.Y = m_Points[0].Y - abs(m_Points.at(1).Y - m_Points.at(0).Y);
			rect.Width = 2 * abs(m_Points.at(1).X - m_Points.at(0).X);
			rect.Height = 2 * abs(m_Points.at(1).Y - m_Points.at(0).Y);
			pGraphics->DrawEllipse(&Gdiplus::Pen(&Gdiplus::SolidBrush(m_DrawInfo.line_color), static_cast<Gdiplus::REAL>(m_DrawInfo.line_width)), rect);
		}
	}


	void Chalk::Draw(Gdiplus::Graphics* pGraphics)
	{
		if (m_Points.size() > 1)
		{
			for (size_t t = 0; t < m_Points.size() - 1; t++)
			{
				pGraphics->DrawLine(&Gdiplus::Pen(&Gdiplus::SolidBrush(m_DrawInfo.line_color), static_cast<Gdiplus::REAL>(m_DrawInfo.line_width)), m_Points.at(t), m_Points.at(t + 1));
			}
		}

	}


}