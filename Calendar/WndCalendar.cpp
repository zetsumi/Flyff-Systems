#include "stdafx.h"

#include "AppDefine.h"
#include "WndCalendar.hpp"
#include "DPClient.h"
#if defined(__CALENDAR)
#include "Calendar.hpp"
#endif //

void CWndCalendar::OnInitialUpdate()
{
	CWndNeuz::OnInitialUpdate();

	m_pTextureFrame = CWndBase::m_textureMng.AddTexture(
		g_Neuz.m_pd3dDevice,
		MakePath(DIR_THEME, "WndChgElemItem.bmp"),
		0xffff00ff);
	std::string szMonth = CCalendar::GetInstance().GetMonth();
	SetTitle(szMonth);
	MoveParentCenter();
}

void CWndCalendar::SetTitle(const std::string& szMonth)
{
	CString	szTitle;
	szTitle.Format("Calendar: %s", szMonth.c_str());
	CWndNeuz::SetTitle(szTitle);
}


BOOL CWndCalendar::Initialize(CWndBase* pWndParent, DWORD nType)
{
	return CWndNeuz::InitDialog(g_Neuz.GetSafeHwnd(), APP_CALENDAR, 0, CPoint(0, 0), pWndParent);
}

void CWndCalendar::OnDraw(C2DRender* p2DRender)
{
	unsigned int uDays = 1;
	CPoint pt;
	pt.y = 10;
	pt.x = 10;
	const std::vector<CCalendar::CalendarItem>& vAwards = CCalendar::GetInstance().GetAwards();
	CTexture* pTextureItem = nullptr;
	while (uDays <= 31)
	{
		if (vAwards.size() > 0 && vAwards.size() >= uDays)
		{
			m_pTextureFrame->Render(p2DRender, pt);
			CCalendar::CalendarItem it = vAwards.at(uDays - 1);
			pTextureItem = it.m_pTextureItem;
			CString szCount;
			szCount.Format("%u", it.dwCount);
			pTextureItem->Render(p2DRender, pt);
			p2DRender->TextOut(pt.x + 29, pt.y + 20, szCount, 0xFFFF00FF);

			CString szDays;
			szDays.Format("%u", uDays);
			if (it.bSuccess == false)
				p2DRender->TextOut(pt.x, pt.y - 10, szDays, 0xFF0000FF);
			else
				p2DRender->TextOut(pt.x, pt.y - 10, szDays, 0xFFFF00FF);

			if (uDays % 7 == 0)
			{
				pt.y += 33;
				pt.y += 10;
				pt.x = 10;
			}
			else
			{
				pt.x += (33 + 10);
			}
		}
		else
			break;
		++uDays;
	}

	// Case bonus
	pt.y += (33 + 10);
	pt.x = 10;
	m_pTextureFrame->Render(p2DRender, pt);

	unsigned int nIDItemBonus = CCalendar::GetInstance().GetIDBonus();
	unsigned int nCountBonus = CCalendar::GetInstance().GetCountBonus();
	ItemProp* pProp = prj.GetItemProp(nIDItemBonus);
	if (pProp != nullptr)
	{
		pTextureItem = CWndBase::m_textureMng.AddTexture(
			g_Neuz.m_pd3dDevice,
			MakePath(DIR_ITEM, pProp->szIcon),
			0xffff00ff);
		pTextureItem->Render(p2DRender, pt);
		CString szCount;
		szCount.Format("%u", nCountBonus);
		p2DRender->TextOut(pt.x + 29, pt.y + 20, szCount, 0xFFFF00FF);
	}
}