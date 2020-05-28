#include "stdafx.h"

#if defined(__DIFFICULTY_INSTANCE)
#include "defineText.h"
#include "AppDefine.h"
#include "WndManager.h"
#include "DPClient.h"
#include "WndDifficultyDungeon.hpp"
#include "MsgHdr.h"
#include "dplay.h"

extern CDPClient g_DPlay;


BOOL CWndDifficultyDungeon::Initialize(CWndBase* pWndParent, DWORD nType)
{
	return CWndNeuz::InitDialog(g_Neuz.GetSafeHwnd(), APP_DIFFICULTY_INSTANCE, 0, CPoint(0, 0), pWndParent);
}

void CWndDifficultyDungeon::OnInitialUpdate()
{
	CWndNeuz::OnInitialUpdate();
	MoveParentCenter();
}

BOOL CWndDifficultyDungeon::OnChildNotify(UINT message, UINT nID, LRESULT* pLResult)
{
	switch (nID)
	{
	case WIDC_BUTTON1:
	{
		g_DPlay.SendDifficultyInstance(0);
		Destroy();
	}break;
	case WIDC_BUTTON2:
	{
		g_DPlay.SendDifficultyInstance(1);
		Destroy();
	}break;
	case WIDC_BUTTON3:
	{
		g_DPlay.SendDifficultyInstance(2);
		Destroy();
	}break;
	default:
		break;
	}
	return CWndNeuz::OnChildNotify(message, nID, pLResult);
}

void CDPClient::SendDifficultyInstance(unsigned short mode)
{
	BEFORESENDSOLE(ar, PACKETTYPE_SET_MODE_DIFFICULTY_DUNGEON, DPID_UNKNOWN);
	ar << mode;
	SEND(ar, this, DPID_SERVERPLAYER);
}

#endif //__DIFFICULTY_INSTANCE
