#include "stdafx.h"
#include "AppDefine.h"
#include "WndAwakeCustom.hpp"
#include "randomoption.h"
#include "DPClient.h"

extern CDPClient	g_DPlay;

void CWndAwakeCustom::UpdateListAwake(void)
{
	int nKind = g_xRandomOptionProperty->GetRandomOptionKind(m_pItem);
	int nSize = g_xRandomOptionProperty->GetRandomOptionSize(m_pItem->GetRandomOptItemId());

	m_plbItemAwake->ResetContent();

	CString	str;
	for (int i = 0; i < nSize; ++i)
	{
		int nDst, nAdj;
		if (!g_xRandomOptionProperty->GetParam(m_pItem->GetRandomOptItemId(), i, &nDst, &nAdj))
			continue;
		if (IsDst_Rate(nDst))
		{
			if (nDst == DST_ATTACKSPEED)
				str.Format("\n%s %c%d%% ", FindDstString(nDst), (nAdj > 0 ? '+' : '-'), ::abs(nAdj / 2 / 10));
			else
				str.Format("\n%s %c%d%%", FindDstString(nDst), (nAdj > 0 ? '+' : '-'), ::abs(nAdj));
		}
		else
			str.Format("\n%s %c%d", FindDstString(nDst), (nAdj > 0 ? '+' : '-'), ::abs(nAdj));

		m_plbItemAwake->AddString(str.GetBuffer());
	}
}

CWndAwakeCustom::CWndAwakeCustom()
{
}

CWndAwakeCustom::~CWndAwakeCustom()
{
}

BOOL CWndAwakeCustom::Initialize(CWndBase* pWndParent, DWORD nType)
{
	return CWndNeuz::InitDialog(g_Neuz.GetSafeHwnd(), APP_AWAKE_CUSTOM, 0, CPoint(0, 0), pWndParent);
}

BOOL CWndAwakeCustom::OnChildNotify(UINT message, UINT nID, LRESULT* pLResult)
{
	switch (nID)
	{
	case WIDC_BUTTON1: {
		if (m_pItem != nullptr)
		{
			if (m_plbListAwakeValided->GetCount() > 0)
				g_DPlay.SendAwakening(m_pItem->m_dwObjId, FALSE);
		}
	}break;
	case WIDC_BUTTON_2: {
		Destroy();
	}break;
	case WIDC_LISTBOX1: {

		int nCur = m_plbListingAwake->GetCurSel();
		CString szDst = m_plbListingAwake->GetString(nCur);

		bool bExist = false;
		int length = m_plbListAwakeValided->GetCount();
		for (int i = 0; i < length; ++i)
		{
			CString it = m_plbListAwakeValided->GetString(i);
			if (strcmp(it.GetBuffer(), szDst.GetBuffer()) == 0)
			{
				bExist = true;
				break;
			}
		}		
		if (bExist == false)
		{
			int nDst = m_plbListingAwake->GetItemData(nCur);
			int nLength = m_plbListAwakeValided->AddString(szDst.GetBuffer());
			m_plbListAwakeValided->SetItemData(nLength, nDst);
		}
	}break;
	default:
		break;
	}
	return CWndNeuz::OnChildNotify(message, nID, pLResult);
}

void CWndAwakeCustom::OnDraw(C2DRender* p2DRender)
{
	if (m_pItemTexture != nullptr)
	{
		CRect rect = m_pRectTextureItem->GetWndRect();
		m_pItemTexture->Render(p2DRender, CPoint(rect.left, rect.top));
	}
	if (m_pGoldTexture != nullptr)
	{
		CRect rect;
		rect.left = 130;
		rect.top = 22;
		m_pGoldTexture->Render(p2DRender, CPoint(rect.left, rect.top));
	}
}

void CWndAwakeCustom::OnInitialUpdate()
{
	CWndNeuz::OnInitialUpdate();

	m_pRectTextureItem = (CWndStatic*)GetDlgItem(WIDC_STATIC);
	m_plbItemAwake = (CWndListBox*)GetDlgItem(WIDC_LISTBOX3);
	m_plbListingAwake = (CWndListBox*)GetDlgItem(WIDC_LISTBOX1);
	m_plbListAwakeValided = (CWndListBox*)GetDlgItem(WIDC_LISTBOX2);
	m_pGoldTexture = CWndBase::m_textureMng.AddTexture(g_Neuz.m_pd3dDevice, MakePath(DIR_ITEM, "itm_GolGolSeed.dds"), 0xffff00ff);

	CWndText* pTextGold = (CWndText*)GetDlgItem(WIDC_TEXT2);
	if (pTextGold != nullptr)
	{
		pTextGold->AddString("200.000");
	}

	const auto& randomOp = g_xRandomOptionProperty->getRandomOptionProb();
	auto vRandomOp = randomOp[CRandomOptionProperty::eAwakening];

	for (const RANDOM_OPTION& it : vRandomOp)
	{
		char* dst = FindDstString(it.nDst);
		int nIndex = m_plbListingAwake->AddString(dst);
		m_plbListingAwake->SetItemData(nIndex, it.nDst);
	}

	MoveParentCenter();
}

BOOL CWndAwakeCustom::OnCommand(UINT nID, DWORD dwMessage, CWndBase* pWndBase)
{
	return 0;
}

BOOL CWndAwakeCustom::OnDropIcon(LPSHORTCUT pShortcut, CPoint point)
{
	m_pItem = (CItemElem*)g_pPlayer->GetItemId(pShortcut->m_dwId);
	if (m_pItem == nullptr)
		return TRUE;

	ItemProp* pItemProp = m_pItem->GetProp();
	if (pItemProp == nullptr)
		return TRUE;

	m_pItemTexture = CWndBase::m_textureMng.AddTexture(g_Neuz.m_pd3dDevice, MakePath(DIR_ITEM, pItemProp->szIcon), 0xffff00ff);

	UpdateListAwake();
	return TRUE;
}

void CWndAwakeCustom::UpdateTooltip(void)
{
	UpdateListAwake();
}

bool CWndAwakeCustom::IsValidConditions(void)
{
	if (m_bAuto == false)
		return true;

	int nKind = g_xRandomOptionProperty->GetRandomOptionKind(m_pItem);
	int nSize = g_xRandomOptionProperty->GetRandomOptionSize(m_pItem->GetRandomOptItemId());
	for (int i = 0; i < nSize; ++i)
	{
		int nDst, nAdj;
		if (!g_xRandomOptionProperty->GetParam(m_pItem->GetRandomOptItemId(), i, &nDst, &nAdj))
			continue;
		int count = m_plbListAwakeValided->GetCount();
		if (count == 0)
			return true;
		for (int indexListAwakeValided = 0; indexListAwakeValided < count; ++indexListAwakeValided)
		{
			int nItemDst = m_plbListAwakeValided->GetItemData(indexListAwakeValided);
			if (nItemDst == nDst)
			{
				if (nAdj > 0)
					return true;
			}
		}
	}
	return false;
}

void CWndAwakeCustom::ReBuild(void)
{
	if (m_pItem != nullptr)
		g_DPlay.SendAwakening(m_pItem->m_dwObjId, FALSE);
}
