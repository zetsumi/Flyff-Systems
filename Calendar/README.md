# Introduction

Si vous lisez ce fichier au format `MD` utiliser le site web [dilliger](https://dillinger.io/) pour avoir la mise en page.

Systeme realiser dans le cadre d'une session de [live](https://www.twitch.tv/s4oul/).
L'idee du systeme revient a Zeepone !
Vous avez des idees et vous souhaitez la voir realiser venez la proposer sur le discord [AstralStudio](https://discord.gg/fZP7TWq
).

# Calendrier

`Calendar` recuperation de bonus quotidien.
![](calendar.PNG)

## Amelioration

Fermer la fenetre quand on quitte le jeu ou on se deconne. <br>
Utilisation de `emplace_back`. <br>
Deplace les `items bonus` du json dans un clef `bonus`.<br>
Historique des mois precedent. <br>


# Integration

Deplacer le fichier `calendar.json` dans votre repertoire contenant `WorldServer.exe`. <br>
Vous devez placer les fichier `WndCalendar.hpp` et `WndCalendar.cpp` dans `_Interface` puis ajouter les a votre solution `Neuz` via `VisualStudio`. <br>
Vous devez placer les fichier `picojson.hpp` dans `_Common` puis ajouter les a votre solutions `Neuz` via `VisualStudio`. <br>
Vous devez placer les fichier `Calendar.hpp` et `Calendar.cpp` dans `_Common` puis ajouter les a vos solutions `Neuz` et `WorldServer` via `VisualStudio`. <br>
Vous devez placer les fichier `LexerParser.hpp` et `LexerParser.cpp` dans `_Common` puis ajouter les a vos solutions `Neuz` et `WorldServer` via `VisualStudio`. <br>
Executer les .sql dans MSSQL creer la table `CALENDAR_TBL`. <br>

## VersionCommon.h
```cpp
#define		__CALENDAR							//	Recompense Calendrier. Idea by @zeepone
```

## Resdata.inc
```cpp
APP_CALENDAR "WndTile00.tga" 1 320 320 0x2410000 26
{
// Title String
"Calendar"
}
{
// ToolTip
""
}
{

}
```

## Resdata.h
```cpp
#if defined(__CALENDAR)
#define	APP_CALENDAR													2033
#endif //__CALENDAR
```
Attention la valeur `2033` peut etre deja prise ! Remplacer la par une autre valeur si necessaire.

## MsgHdr.h
```cpp
#if defined(__CALENDAR)
#define PACKETYPE_GET_CALENDAR              (DWORD)0x98100603
#define PACKETTYPE_DB_CALENDAR_SUCCESS_DAY  (DWORD)0x98100604
#endif //__CALENDAR
```
```cpp
#if defined(__CALENDAR)
#define SNAPSHOTTYPE_CALENDAR_INFORMATION  	 (WORD)0x8862
#define SNAPSHOTTYPE_CALENDAR_SUCCESS_DAY  	 (WORD)0x8863
#define SNAPSHOTTYPE_CALENDAR_SUCCESS_MONTH  	 (WORD)0x8864
#endif //__CALENDAR
```

## Project.cpp
```cpp
#if defined(__CALENDAR) && defined(__WORLDSERVER)
#include "Calendar.hpp"
#endif //__CALENDAR
```
A la fin de la fonction ___BOOL CProject::OpenProject()___
```cpp
#if defined(__CALENDAR) && defined(__WORLDSERVER)
    if (CCalendar::GetInstance().Load() == false)
        return FALSE;
#endif //__CALENDAR
```

## DBManagerJoin.cpp
```cpp
#if defined(__CALENDAR)
void CDbManager::SendCalendar(CQuery* pQuery, CMover* pMover, LPDB_OVERLAPPED_PLUS lpDBOP)
{
	std::map<unsigned int, std::vector<unsigned int>>	calendar;
	CString szRequeteCalendar;
	szRequeteCalendar.Format("SELECT day, month FROM CALENDAR_TBL WHERE idplayer='%07d' AND serverindex='%02d'",
		pMover->m_idPlayer, g_appInfo.dwSys);
	if (pQuery->Exec(szRequeteCalendar.GetBuffer()) != FALSE)
	{
		while (pQuery->Fetch())
		{
			int nMonth = pQuery->GetInt("month");
			int nDay = pQuery->GetInt("day");
			if (calendar.find(nMonth) == calendar.end())
				calendar[nMonth] = std::vector<unsigned int>();
			calendar[nMonth].push_back(nDay);
		}
		BEFORESENDDUAL(ar, PACKETYPE_GET_CALENDAR, lpDBOP->dpidCache, lpDBOP->dpidUser);
		ar << pMover->m_idPlayer;
		ar << calendar.size();
		for (auto it : calendar)
		{
			ar << it.first;
			ar << it.second.size();
			for (auto v : it.second)
			{
				ar << v;
			}
		}
		SEND(ar, CDPTrans::GetInstance(), lpDBOP->dpid);
	}
}

void CDbManager::SetCalendarSuccessDay(CQuery* pQuery, LPDB_OVERLAPPED_PLUS lpDBOP)
{
	CAr ar(lpDBOP->lpBuf, lpDBOP->uBufSize);

	DWORD dwIdPlayer, dwMonth, dwDay;
	ar >> dwIdPlayer;
	ar >> dwMonth;
	ar >> dwDay;

	CString req;
	req.Format("INSERT INTO CALENDAR_TBL (idplayer,serverindex,month,day) VALUES('%07d','%02d',%u,%u)",
		dwIdPlayer, g_appInfo.dwSys,
		dwMonth, dwDay);
	if (!pQuery->Exec(req.GetBuffer()))
	{
		AfxMessageBox("error");
	}

	FreeRequest(lpDBOP);

}
#endif //__CALENDAR
```

## DBManager.cpp
Dans la fonction ___void CDbManager::GuildThread( void )___ <br>
Ajout le `case`
```cpp
#if defined(__CALENDAR)
			case CALENDAR_SUCCESS_DAY:
			{
				SetCalendarSuccessDay(pQuery, lpDbOverlappedPlus);
			} break;
#endif //__CALENDAR
```

## DBManager.h
Dans `enum QUERYMODE` ajouter l'enum
```cpp
#if defined(__CALENDAR)
	CALENDAR_SUCCESS_DAY
#endif //__CALENDAR
};
```
Dans la class ___class CDbManager___ en `public`
```cpp
#if defined(__CALENDAR)
	void	SendCalendar(CQuery* pQuery, CMover* pMover, LPDB_OVERLAPPED_PLUS lpDBOP);
	void	SetCalendarSuccessDay(CQuery* pQuery, LPDB_OVERLAPPED_PLUS lpDbOverlappedPlus);
#endif //__CALENDAR
```

## dptrans.h
Dans la class ___class CDPTrans : public CDPMng___
```cpp
#if defined(__CALENDAR)
	void	OnCalendarSuccessDay(CAr& ar, DPID dpid, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize);
#endif 
```

## dptrans.cpp
Dans la fonction ___CDPTrans::CDPTrans()___
```cpp
#if defined(__CALENDAR)
	ON_MSG(PACKETTYPE_DB_CALENDAR_SUCCESS_DAY, &CDPTrans::OnCalendarSuccessDay);
#endif //__CALENDAR
```
Ajouter à la fin du fichier :
```cpp
#if defined(__CALENDAR)
void CDPTrans::OnCalendarSuccessDay(CAr& ar, DPID dpid, DPID dpidCache, DPID dpidUser, LPBYTE lpBuf, u_long uBufSize)
{
	LPDB_OVERLAPPED_PLUS lpDbOverlappedPlus = g_DbManager.AllocRequest();
	g_DbManager.MakeRequest(lpDbOverlappedPlus, lpBuf, uBufSize);
	lpDbOverlappedPlus->nQueryMode = CALENDAR_SUCCESS_DAY;
	PostQueuedCompletionStatus(g_DbManager.m_hIOCPGuild, 1, NULL, &lpDbOverlappedPlus->Overlapped);
}
#endif //
```

## FuncApplet.cpp
```cpp
#if defined(__CALENDAR)
#include "WndCalendar.hpp"
#endif //
```

```cpp
#if defined(__CALENDAR)
DECLAREAPPLET(AppMain_WndCalendar, new CWndCalendar);
#endif //__CALENDAR
```

```cpp
#if defined(__CALENDAR)
    AddAppletFunc(AppMain_WndCalendar, APP_CALENDAR, _T("WndCalendar"), _T("Icon_Applet.dds"), "Calendar", 0);
#endif //__AWAKE_CUSTOM
```

## DPClient.cpp
```cpp
#if defined(__CALENDAR)
#include "WndCalendar.hpp"
#include "Calendar.hpp"
#endif //__CALENDAR
```
```cpp
#if defined(__CALENDAR)
			case SNAPSHOTTYPE_CALENDAR_INFORMATION: OnCalendarInformations(objid, ar); break;
			case SNAPSHOTTYPE_CALENDAR_SUCCESS_DAY: OnCalendarDaySuccess(objid, ar); break;
			case SNAPSHOTTYPE_CALENDAR_SUCCESS_MONTH: OnCalendarMonthSucess(objid, ar); break;
#endif //__CALENDAR
```
```cpp
#if defined(__CALENDAR)
void CDPClient::OnCalendarInformations(OBJID objid, CAr& ar)
{
	char szMonth[10];
	DWORD dwSize;

	CCalendar::GetInstance().Clear();

	ar.ReadString(szMonth);
	ar >> dwSize;
	while (dwSize > 0)
	{
		DWORD dwID, dwCount;
		ar >> dwID;
		ar >> dwCount;
		CCalendar::GetInstance().Add(dwID, dwCount);
		--dwSize;
	}
	CCalendar::GetInstance().SetMonth(szMonth);
	DWORD dwIDBonus, dwCountBonus;

	ar >> dwIDBonus;
	ar >> dwCountBonus;

	CCalendar::GetInstance().SetBonus(dwIDBonus, dwCountBonus);

	// Open window Calendar
	CWndCalendar* pWnd = (CWndCalendar*)g_WndMng.GetWndBase(APP_CALENDAR);
	if (pWnd == nullptr)
	{
		pWnd = new CWndCalendar();
		pWnd->Initialize();
	}
}

void CDPClient::OnCalendarDaySuccess(OBJID objid, CAr& ar)
{
	DWORD dwMonth, dwDay;
	ar >> dwMonth;
	ar >> dwDay;
	CCalendar::GetInstance().SuccessDay(dwDay);
}

void CDPClient::OnCalendarMonthSucess(OBJID objid, CAr& ar)
{
	DWORD dwMonth;
	ar >> dwMonth;
}

#endif //__CALENDAR
```


## DPClient.h
Dans la class ___class CDPClient : public CDPMng___
```cpp
#if defined(__CALENDAR)
	void	OnCalendarInformations(OBJID objid, CAr& ar);
	void	OnCalendarDaySuccess(OBJID objid, CAr& ar);
	void	OnCalendarMonthSucess(OBJID objid, CAr& ar);
#endif //__CALENDAR
```

## DPDatabaseClient.cpp
```cpp
#if defined(__CALENDAR)
#include "Calendar.hpp"
#endif //__CALENDAR
```
```cpp
#if defined(__CALENDAR)
void CDPDatabaseClient::OnGetCalendar(CAr& ar, DPID, DPID)
{
	DWORD dwIdPlayer, dwMonthNumber, dwMonth, dwDay, dwDayNumber;

	ar >> dwIdPlayer;

	CUser* pUser = g_UserMng.GetUserByPlayerID(dwIdPlayer);
	if (IsInvalidObj(pUser))
		return;

	ar >> dwMonthNumber;
	for (unsigned int m = 0; m < dwMonthNumber; ++m)
	{
		ar >> dwMonth;
		if (pUser->m_mmAwardsCalendar.find(dwMonth) == pUser->m_mmAwardsCalendar.end())
			pUser->m_mmAwardsCalendar[dwMonth] = std::map<unsigned int, bool>();

		ar >> dwDayNumber;
		for (unsigned int d = 0; d < dwDayNumber; ++d)
		{
			ar >> dwDay;
			pUser->m_mmAwardsCalendar[dwMonth][dwDay] = true;
			pUser->SendCalendarDaySuccess(dwMonth, dwDay);
		}
	}
	CCalendar::GetInstance().ApplyAward(pUser);
}

void	CDPDatabaseClient::SendCalendarDaySuccess(unsigned int uIdPlayer, unsigned int uMonth, unsigned int uDay)
{
	BEFORESENDDUAL(ar, PACKETTYPE_DB_CALENDAR_SUCCESS_DAY, DPID_UNKNOWN, DPID_UNKNOWN);
	ar << uIdPlayer << uMonth << uDay;
	SEND(ar, this, DPID_SERVERPLAYER);
}
#endif //__CALENDAR
```

## DPDatabaseclient.h
sous
```cpp
private:
	USES_PFNENTRIES;
```
Ajouter :
```cpp
#if defined(__CALENDAR)
	void	OnGetCalendar(CAr& ar, DPID, DPID);
#endif //__CALENDAR
```
En `public` ajouter :
```cpp
#if defined(__CALENDAR)
	void	SendCalendarDaySuccess(unsigned int uIdPlayer, unsigned int uMonth, unsigned int uDay);
#endif //__CALENDAR
```

## User.cpp
```cpp
#if defined(__CALENDAR)
#include <ctime>
#include "Calendar.hpp"
#endif//__CALENDAR
```
A la fin de la fonction ___void CUser::Open(DWORD dwWorldId)___
```cpp
#if defined(__CALENDAR)
    SendCalendarInformations();
#endif //__CALENDAR
```
```cpp
#if defined(__CALENDAR)
void CUser::SendCalendarInformations()
{
    if (IsDelete())
        return;
    ++m_Snapshot.cb;
    m_Snapshot.ar << GetId();
    m_Snapshot.ar << SNAPSHOTTYPE_CALENDAR_INFORMATION;
    time_t now = time(0);
    tm* ltm = localtime(&now);
    unsigned int uMonth = static_cast<unsigned int>(ltm->tm_mon + 1);
    const std::vector<CCalendar::CalendarItem>& calendar = CCalendar::GetInstance().getCalenderFromMonth(uMonth);
    const char* szMonth = CCalendar::GetInstance().m_mMonthConvert.at(uMonth).c_str();
    m_Snapshot.ar.WriteString(szMonth);
    m_Snapshot.ar << (DWORD)calendar.size();
    for (auto it : calendar)
    {
        CCalendar::CalendarItem info = it;
        m_Snapshot.ar << it.dwID;
        m_Snapshot.ar << it.dwCount;
    }
    CCalendar::CalendarItem itemBonus = calendar.at(calendar.size() - 1);
    m_Snapshot.ar << itemBonus.dwIDBonus;
    m_Snapshot.ar << itemBonus.dwCountBonus;

}
void CUser::SendCalendarDaySuccess(unsigned int uMonth, unsigned int uDay)
{
    time_t now = time(0);
    tm* ltm = localtime(&now);
    unsigned int uCurrentMonth = static_cast<unsigned int>(ltm->tm_mon + 1);

    if (uCurrentMonth == uMonth)
    {
        if (IsDelete())	return;
        m_Snapshot.cb++;
        m_Snapshot.ar << GetId();
        m_Snapshot.ar << SNAPSHOTTYPE_CALENDAR_SUCCESS_DAY;
        m_Snapshot.ar << (DWORD)uMonth;
        m_Snapshot.ar << (DWORD)uDay;
    }

}

void CUser::SendCalendarMonthSuccess(unsigned int uMonth)
{
    time_t now = time(0);
    tm* ltm = localtime(&now);
    unsigned int uCurrentMonth = static_cast<unsigned int>(ltm->tm_mon + 1);

    if (uCurrentMonth == uMonth)
    {
        if (IsDelete())	return;
        m_Snapshot.cb++;
        m_Snapshot.ar << GetId();
        m_Snapshot.ar << SNAPSHOTTYPE_CALENDAR_SUCCESS_MONTH;
        m_Snapshot.ar << (DWORD)uMonth;
    }
}

#endif //__CALENDAR
```

## User.h
Dans la class ___class CUser : public CMover___
```cpp
#if defined(__CALENDAR)
    std::map<unsigned int, std::map<unsigned int, bool>>    m_mmAwardsCalendar;
#endif //__CALENDAR
```
```cpp
#if defined(__CALENDAR)
    void    SendCalendarInformations(void);
    void    SendCalendarDaySuccess(unsigned int uMonth, unsigned int uDay);
    void    SendCalendarMonthSuccess(unsigned int uMonth);
#endif //__CALENDAR
```
