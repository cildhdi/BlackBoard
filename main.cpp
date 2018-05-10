#include "BbWnd.h"
KrUI::KrUIManager* pUm = KrUI::KrUIManager::GetpKrUIManager();

void InitBbWnd()
{
	unsigned int cx = GetSystemMetrics(SM_CXSCREEN), cy = GetSystemMetrics(SM_CYSCREEN);
	BbWnd* pBbWnd = dynamic_cast<BbWnd*>(pUm->AddWindow(new BbWnd, L"BlackBoard", cx / 6, cy / 6, cx / 1.5, cy / 1.5, WS_OVERLAPPEDWINDOW));
	pBbWnd->SetCaptionHeight(0);
	pBbWnd->RegMsg(WM_CLOSE, KrUI::MsgFuncAdapter(std::bind(PostQuitMessage, 0)));
	SetTimer(pBbWnd->GetHWND(), 1000, 100, nullptr);
	pBbWnd->SetBgColor(Gdiplus::Color::Black);
	pBbWnd->RegMsg(WM_LBUTTONDOWN, [pBbWnd](MSGFUNC_ARGS)->LRESULT
	{
		if (current_draw_operate == nullptr)
		{
			switch (nsGlobalData::g_draw_info.draw_type)
			{
			case nsGlobalData::Line:
				current_draw_operate = std::make_shared<nsDrawOperate::Line>(nsGlobalData::g_draw_info);
				break;
			case nsGlobalData::Rectangle:
				current_draw_operate = std::make_shared<nsDrawOperate::Rectangle>(nsGlobalData::g_draw_info);
				break;
			case nsGlobalData::Elliptic:
				current_draw_operate = std::make_shared<nsDrawOperate::Elliptic>(nsGlobalData::g_draw_info);
				break;
			case nsGlobalData::Circle:
				current_draw_operate = std::make_shared<nsDrawOperate::Circle>(nsGlobalData::g_draw_info);
				break;
			case nsGlobalData::Eraser:
			{
				auto c = nsGlobalData::g_draw_info.line_color;
				nsGlobalData::g_draw_info.line_color = pBbWnd->GetBgColor();
				current_draw_operate = std::make_shared<nsDrawOperate::Chalk>(nsGlobalData::g_draw_info);
				nsGlobalData::g_draw_info.line_color = c;
			}
			break;
			case nsGlobalData::Chalk:
				current_draw_operate = std::make_shared<nsDrawOperate::Chalk>(nsGlobalData::g_draw_info);
				break;
			}
			if (current_draw_operate != nullptr)
			{
				current_draw_operate->SetFirstPoint(GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
				current_draw_operate->SetSecondPoint(GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
				pBbWnd->GetOperators().push_back(current_draw_operate);
			}
		}
		return 0;
	});
	pBbWnd->RegMsg(WM_MOUSEMOVE, [pBbWnd](MSGFUNC_ARGS)->LRESULT
	{
		if (current_draw_operate != nullptr)
		{
			if (nsGlobalData::g_draw_info.draw_type == nsGlobalData::Chalk || nsGlobalData::g_draw_info.draw_type == nsGlobalData::Eraser)
			{
				current_draw_operate->AddPoint(GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
			}
			else
			{
				current_draw_operate->SetSecondPoint(GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
			}
			if (MOVE_TIME == 5)
			{
				pBbWnd->Update();
				MOVE_TIME = 0;
			}
			MOVE_TIME++;
		}
		return 0;
	});
	pBbWnd->RegMsg(WM_LBUTTONUP, KrUI::MsgFuncAdapter([pBbWnd]()
	{
		if (current_draw_operate != nullptr)
		{
			pBbWnd->Update();
			current_draw_operate = nullptr;
		}
	}));
	pBbWnd->RegMsg(WM_RBUTTONUP, [pBbWnd](MSGFUNC_ARGS)->LRESULT
	{
		if (dynamic_cast<BbWnd*>(pBbWnd)->GetOperators().size() > 0)
		{
			dynamic_cast<BbWnd*>(pBbWnd)->GetOperators()[dynamic_cast<BbWnd*>(pBbWnd)->GetOperators().size() - 1].~shared_ptr();
			dynamic_cast<BbWnd*>(pBbWnd)->GetOperators().pop_back();
			pBbWnd->Update();
		}
		return 0;
	});
	pBbWnd->Show();
}

void InitToolWnd()
{
	auto pWnd_Tools = pUm->AddWindow(L"BlackBoard-Tools", 100, 100, 100, 700, WS_BORDER);
	::SetWindowPos(pWnd_Tools->GetHWND(), HWND_TOPMOST, pWnd_Tools->GetX(), pWnd_Tools->GetY(), pWnd_Tools->GetWidth(), pWnd_Tools->GetHeight(), SWP_NOMOVE);
	pWnd_Tools->AddLabel(L"选择工具：", 5, 5, 100, 40);
	auto pToolList = pWnd_Tools->AddList(L"", 0, 50, 155, 400);
	pToolList->RegMsg(KM_LISTITEMCLICK, KrUI::MsgFuncAdapter([pToolList]()
	{
		auto i = pToolList->GetSelectedItems();
		if (i.empty())
		{
			nsGlobalData::g_draw_info.draw_type = nsGlobalData::None;
		}
		else
		{
			nsGlobalData::g_draw_info.draw_type = static_cast<nsGlobalData::DrawType>(i[0].m_Index);
		}
	}));
	pToolList->SetBgColor(Gdiplus::Color(245, 245, 245));
	pToolList->AddItem(L"无", nsGlobalData::None);
	pToolList->AddItem(L"直线", nsGlobalData::Line);
	pToolList->AddItem(L"矩形", nsGlobalData::Rectangle);
	pToolList->AddItem(L"圆", nsGlobalData::Circle);
	pToolList->AddItem(L"椭圆", nsGlobalData::Elliptic);
	pToolList->AddItem(L"笔", nsGlobalData::Chalk);
	pToolList->AddItem(L"橡皮擦", nsGlobalData::Eraser);
	pWnd_Tools->AddLabel(L"输入线宽：", 5, pToolList->GetY() + pToolList->GetHeight() + 5, 155, 40);
	auto pLen = pWnd_Tools->AddEdit(L"3", 10, pToolList->GetY() + pToolList->GetHeight() + 45, 100, 30);
	pLen->RegMsg(KM_TEXTCHANGE, KrUI::MsgFuncAdapter([pLen, pWnd_Tools]()
	{
		try
		{
			nsGlobalData::g_draw_info.line_width = std::stoul(pLen->GetText());
		}
		catch (const std::exception&)
		{
			MessageBoxW(pWnd_Tools->GetHWND(), L"请输入数值", L"警告", MB_OK);
			pLen->SetText(std::to_wstring(3));
		}
	}));
	pWnd_Tools->AddLabel(L"点击选择颜色：", 5, pLen->GetY() + pLen->GetHeight() + 5, 155, 40);
	auto pBtn = pWnd_Tools->AddButton(L"颜色", 30, pLen->GetY() + pLen->GetHeight() + 45);
	static COLORREF colors[16] = { 0 };
	pBtn->RegMsg(KM_CLICK, KrUI::MsgFuncAdapter([pWnd_Tools]()
	{
		static CHOOSECOLOR stChooseColor;
		static COLORREF rgbLineColor, rgb;
		stChooseColor.lStructSize = sizeof(CHOOSECOLOR);
		stChooseColor.hwndOwner = pWnd_Tools->GetHWND();
		stChooseColor.rgbResult = rgbLineColor;
		stChooseColor.lpCustColors = colors;
		stChooseColor.Flags = CC_RGBINIT;
		stChooseColor.lCustData = 0;
		stChooseColor.lpfnHook = NULL;
		stChooseColor.lpTemplateName = NULL;
		if (ChooseColor(&stChooseColor))
		{
			nsGlobalData::g_draw_info.line_color = Gdiplus::Color(GetRValue(stChooseColor.rgbResult), GetGValue(stChooseColor.rgbResult), GetBValue(stChooseColor.rgbResult));
		}
	}));
	pWnd_Tools->Show();
}
//主函数入口
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!pUm->Initialize(hInstance)) return 0;//
	InitBbWnd();
	InitToolWnd();
	return pUm->MessageLoop();
}
