#include "BbWnd.h"

void BbWnd::Draw()
{
	for (auto& p : m_DrawOperators)
	{
		p->Draw(m_pGraphics);
	}
}

std::vector<std::shared_ptr<nsDrawOperate::DrawOperate>>& BbWnd::GetOperators()
{
	return m_DrawOperators;
}
