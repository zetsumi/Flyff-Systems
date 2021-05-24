#include "stdafx.h"

#if defined(__APP_CHANGE_JOB_ADMIN)
#include "defineText.h"
#include "AppDefine.h"
#include "WndManager.h"
#include "DPClient.h"
#include "WndChangeJobAdmin.hpp"


BOOL WndChangeJobAdmin::Initialize(CWndBase* pWndParent, DWORD nType)
{
	return CWndNeuz::InitDialog(g_Neuz.GetSafeHwnd(), nType, 0, CPoint(0, 0), pWndParent);
}

void WndChangeJobAdmin::OnInitialUpdate()
{
	CWndNeuz::OnInitialUpdate();

	btnOk = dynamic_cast<CWndButton*>(GetDlgItem(WIDC_ACCEPT));
	btnCancel = dynamic_cast<CWndButton*>(GetDlgItem(WIDC_CANCEL));
	editLevel = dynamic_cast<CWndEdit*>(GetDlgItem(WIDC_EDIT));
	listJob = dynamic_cast<CWndComboBox*>(GetDlgItem(WIDC_COMBOBOX1));
	checkHero = dynamic_cast<CWndButton*>(GetDlgItem(WIDC_CHECK1));
	checkMaster = dynamic_cast<CWndButton*>(GetDlgItem(WIDC_CHECK2));

	if (checkHero)
		checkHero->SetCheck(false);
	if (checkMaster)
		checkHero->SetCheck(false);

	if (editLevel)
		editLevel->ResetString();

	if (listJob)
	{
		for (unsigned int i = 0; i < MAX_JOB; ++i)
		{
			const char* jobName = prj.m_aJob[i].szName;
			unsigned int index = listJob->AddString(jobName);
			listJob->SetItemData(index, index);
		}
		listJob->SetCurSel(0);
	}

	MoveParentCenter();
}

BOOL WndChangeJobAdmin::OnChildNotify(UINT message, UINT nID, LRESULT* pLResult)
{
	switch (nID)
	{
	case WIDC_ACCEPT:
	{
		int cursorJob = listJob->GetCurSel();
		const char* jobName = prj.m_aJob[cursorJob].szName;
		const char* level = editLevel->GetString();
		CString cmd;

		if (checkHero->GetCheck() == true)
			cmd.Format("/level \"%s\" %s 2", jobName, level);
		else if (checkMaster->GetCheck() == true)
			cmd.Format("/level \"%s\" %s 1", jobName, level);
		else
			cmd.Format("/level \"%s\" %s", jobName, level);
		ParsingCommand(cmd.LockBuffer(), g_pPlayer);
	}break;
	case WIDC_CANCEL:
	{
		Destroy();
	}break;
	case WIDC_CHECK1: // hero
	{
		checkMaster->SetCheck(false);
	}break;
	case WIDC_CHECK2: // master
	{
		checkHero->SetCheck(false);
	}break;
	default:
		break;
	}
	return CWndNeuz::OnChildNotify(message, nID, pLResult);
}

#endif // __APP_CHANGE_JOB_ADMIN
