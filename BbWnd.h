#pragma once

#ifndef _BBWND_H_
#define _BBWND_H_

#include "KrUI/KrUI.h"
#include "GlobalData.h"
#include "DrawOperate.h"
#include <memory>
#include <vector>
#ifdef _DEBUG
#pragma comment(lib,"KrUI/Debug/KrUI.lib")
#else
#pragma comment(lib,"KrUI/Release/KrUI.lib")
#endif

static int MOVE_TIME = 0;
static std::shared_ptr<nsDrawOperate::DrawOperate> current_draw_operate = nullptr;

class BbWnd :public KrUI::KrWindow
{
private:
	std::vector<std::shared_ptr<nsDrawOperate::DrawOperate>> m_DrawOperators;
public:
	std::vector<std::shared_ptr<nsDrawOperate::DrawOperate>>& GetOperators();
	void Draw();
};

#endif