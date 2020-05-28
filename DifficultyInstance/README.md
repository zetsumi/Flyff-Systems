# Introduction

Si vous lisez ce fichier au format `MD` utiliser le site web [dilliger](https://dillinger.io/) pour avoir la mise en page.<br>
Systeme réalisé dans le cadre d'une session de [live](https://www.twitch.tv/s4oul/).<br>
L'idee du systeme revient a Zeepone !<br>
Vous avez des idées et vous souhaitez la voir réaliser venez la proposer sur le discord [AstralStudio](https://discord.gg/fZP7TWq).<br>

# Difficulty Instance

![](https://img.icons8.com/color/24/000000/error.png) La premier version presente de nombreux bugs ! ![](https://img.icons8.com/color/24/000000/error.png)
<br>
---
Il est conseiller d'implementer ce system uniquement si vous avez de bonnes connaissances en `C++` afin de le modifier.<br>
Une version stable sera release dans un second temps.<br>
---
<br>
Ce systeme permet de choisir une difficulte une fois rentrer dans le donjon.<br>
Seul le lead du groupe peut changer la difficulte.<br>
La difficulte choisie modifie les stats et la taille des monstres.<br>
Les monstres de type `ID_MIDBOSS` et `ID_BOSS` seront impactes.
Les stats modifie sont `STA`, `STR`, `HP`.<br>

# Bugs
La fenetre ne se ferme pas automatique.<br>
La fenetre peut s'ouvrir 2 ou 3 fois.<br>

# Amelioration
Application les changements de stats sur les `ID_NORMAL`.<br>
Applications les changements sur les mobs des couloirs.<br>
Ajouter un bonus de drops celon la difficulte choisie.<br>

# Integration
Vous devez placer les fichier `WndDifficultyDungeon.hpp` et `WndDifficultyDungeon.cpp` dans `_Interface` puis ajouter les a votre solution `Neuz` via `VisualStudio`. <br>


### ResData.h
```cpp
#if defined(__DIFFICULTY_INSTANCE)
#define APP_DIFFICULTY_INSTANCE											2034
#endif //__DIFFICULTY_INSTANCE
```
Attention la valeur `2034` peut etre deja prise ! Remplacer la par une autre valeur si necessaire.<br>

### ResData.inc
```
APP_DIFFICULTY_INSTANCE "WndTile00.tga" 1 128 192 0x2410000 26
{
// Title String
"Dungeon Difficulty"
}
{
// ToolTip
""
}
{
    WTYPE_BUTTON WIDC_BUTTON1 "" 0 22 22 101 46 0x220010 0 0 0 0
    {
    // Title String
    "Easy"
    }
    {
    // ToolTip
    ""
    }
    WTYPE_BUTTON WIDC_BUTTON2 "" 0 24 64 103 88 0x220010 0 0 0 0
    {
    // Title String
    "Normal"
    }
    {
    // ToolTip
    ""
    }
    WTYPE_BUTTON WIDC_BUTTON3 "" 0 24 104 103 128 0x220010 0 0 0 0
    {
    // Title String
    "Hard"
    }
    {
    // ToolTip
    ""
    }
}
```

### MsgHdr.h
```cpp
#if defined(__DIFFICULTY_INSTANCE)
#define PACKETTYPE_SET_MODE_DIFFICULTY_DUNGEON  (DWORD)0x98100605
#endif //__DIFFICULTY_INSTANCE
```
```cpp
#if defined(__DIFFICULTY_INSTANCE)
#define SNAPSHOTTYPE_CHOOSE_DIFFICULTY_DUNGEON     (WORD)0x8903
#endif //__DIFFICULTY_INSTANCE
```

### InstanceDungeon.hpp
```cpp
#if defined(__DIFFICULTY_INSTANCE)
enum class MODE_DIFFICULTY : unsigned short
{
	EASY,
	NORMAL,
	HARD
};
#endif //__DIFFICULTY_INSTANCE
```
Dans la structure ___INSTACNEDUNGEON_INFO___ :
```cpp
#if defined(__DIFFICULTY_INSTANCE)
	MODE_DIFFICULTY	eModeDifficulty = MODE_DIFFICULTY::EASY;
	bool	bModifiable = true;
#endif //__DIFFICULTY_INSTANCE
```


### InstanceDungeonBase.h
Ajouter dans la class ___CInstanceDungeonBase___ :
```cpp
#if defined(__DIFFICULTY_INSTANCE)
	void	SetModeDifficulty(MODE_DIFFICULTY mode, DWORD  dwDungeonId, DWORD dwWorldID);
	void	SendChooseModeDifficulty(ID_INFO* pInfo, CUser* pUser, DWORD  dwDungeonId);
#endif //__DIFFICULTY_INSTANCE
```

### InstanceDungeonBase.cpp

Dans la fonction ___void CInstanceDungeonBase::CreateMonster(DWORD dwDungeonId, DWORD dwWorldId)___ :
Apres :
```cpp
				pObj->InitMotion(MTI_STAND);
				pObj->UpdateLocalMatrix();
```
Ajouter :
```cpp
#if defined(__DIFFICULTY_INSTANCE)
				D3DXVECTOR3 scale = pObj->GetScale();

				float factorScale = 1.f;
				float factorStatBonus = 1.f;
				float factorHpBonus = 0.f;
				if (pInfo->eModeDifficulty == MODE_DIFFICULTY::NORMAL)
				{
					factorStatBonus = 1.3f;
					factorScale = 1.3f;
					factorHpBonus = 0.3f;
				}
				else if (pInfo->eModeDifficulty == MODE_DIFFICULTY::HARD)
				{
					factorStatBonus = 2.f;
					factorScale = 2.f;
					factorHpBonus = 1.f;
				}
				scale.x *= factorScale;
				pObj->SetScale(scale);

				CMover* pMover = dynamic_cast<CMover*>(pObj);
				float hpBonus = pMover->GetHitPoint() * factorHpBonus;
				if (factorHpBonus > 0)
				{
					pMover->SetDestParam(DST_HP_MAX, static_cast<int>(pMover->GetHitPoint() * factorHpBonus), NULL_CHGPARAM, TRUE);
					pMover->SetDestParam(DST_HP, static_cast<int>(pMover->GetHitPoint() * factorHpBonus), NULL_CHGPARAM, TRUE);
				}
				pMover->SetDestParam(DST_STR, static_cast<int>(pMover->GetStr() * factorStatBonus), NULL_CHGPARAM, TRUE);
				pMover->SetDestParam(DST_STA, static_cast<int>(pMover->GetSta() * factorStatBonus), NULL_CHGPARAM, TRUE);
#endif //__DIFFICULTY_INSTANCE
```

Dans la fonction ___BOOL CInstanceDungeonBase::TeleportToDungeon(CUser* pUser, DWORD dwWorldId, DWORD dwDungeonId)___ :
Apres :
```cpp
			if (!pCT_Info || pCT_Info->dwDungeonId != dwDungeonId)
				CInstanceDungeonHelper::GetInstance()->SendInstanceDungeonSetCoolTimeInfo(g_uKey, GetType(), pUser->m_idPlayer, COOLTIME_INFO(dwWorldId, dwDungeonId, (GetCoolTime(dwWorldId) + GetTickCount())));
```
Ajouter :
```cpp
#if defined(__DIFFICULTY_INSTANCE)
			SendChooseModeDifficulty(pInfo, pUser, dwDungeonId);
#endif //__DIFFICULTY_INSTANCE
```
Avant :
```cpp
return TRUE;
```
Ajouter :
```cpp
#if defined(__DIFFICULTY_INSTANCE)
	SendChooseModeDifficulty(pInfo, pUser, dwDungeonId);
#endif //__DIFFICULTY_INSTANCE
```

Dans la fonction __void CInstanceDungeonBase::SetInstanceDungeonKill(DWORD dwWorldId, DWORD dwDungeonId, DWORD dwMonsterId)__ :
Supprimer les 2 !!:
```cpp
ID_INFO* pInfo = GetDungeonInfo(dwDungeonId, dwWorldId);
```
Puis ajouter :
```cpp
void CInstanceDungeonBase::SetInstanceDungeonKill(DWORD dwWorldId, DWORD dwDungeonId, DWORD dwMonsterId)
{
	ID_INFO* pInfo = GetDungeonInfo(dwDungeonId, dwWorldId);
	pInfo->bModifiable = false;
```

Ajouter les fonctions :
```cpp
#if defined(__DIFFICULTY_INSTANCE)
void CInstanceDungeonBase::SetModeDifficulty(MODE_DIFFICULTY mode, DWORD  dwDungeonId, DWORD dwWorldID)
{
	auto pInfo = GetDungeonInfo(dwDungeonId, dwWorldID);
	if (pInfo->bModifiable == true)
		pInfo->eModeDifficulty = mode;
}
void CInstanceDungeonBase::SendChooseModeDifficulty(ID_INFO* pInfo, CUser* pUser, DWORD  dwDungeonId)
{
	if (pInfo == nullptr || pInfo->bModifiable == true)
	{
		u_long idLeader = g_PartyMng.GetParty(dwDungeonId)->GetLeader()->m_idPlayer;
		if (idLeader == pUser->m_idPlayer)
		{
			pUser->SendChooseDifficultyDungeon();
		}
	}
}
#endif //__DIFFICULTY_INSTANCE
```

### DPClient.h
Ajouter dans la class ___lass CDPClient : public CDPMng___ :
```cpp
#if defined(__DIFFICULTY_INSTANCE)
	void	SendDifficultyInstance(unsigned short mode);
#endif //__DIFFICULTY_INSTANCE
```
```cpp
#if defined(__DIFFICULTY_INSTANCE)
	void	OnChooseDifficultyDungeon(OBJID objid, CAr& ar);
#endif //__DIFFICULTY_INSTANCE
```

### DPClient.cpp
```cpp
#if defined(__DIFFICULTY_INSTANCE)
			case SNAPSHOTTYPE_CHOOSE_DIFFICULTY_DUNGEON: OnChooseDifficultyDungeon(objid, ar); break;
#endif //__DIFFICULTY_INSTANCE
```
```cpp
#if defined(__DIFFICULTY_INSTANCE)
#include "WndDifficultyDungeon.hpp"
#include "ResData.h"
void CDPClient::OnChooseDifficultyDungeon(OBJID objid, CAr& ar)
{
	auto pWnd = g_WndMng.GetApplet(APP_DIFFICULTY_INSTANCE);
	if (pWnd == nullptr)
	{
		pWnd = new CWndDifficultyDungeon();
		pWnd->Initialize();
	}
	BYTE b;
	ar >> b;
}
#endif //__DIFFICULTY_INSTANCE
```


### DPSrvr.h
Dans la class ___class CDPSrvr : public CDPMng___ :
```cpp
#if defined(__DIFFICULTY_INSTANCE)
void    OnSetModeDifficultyInstance(CAr& ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long);
#endif //__DIFFICULTY_INSTANCE
```


### DPSrvr.cpp
```cpp
#if defined(__DIFFICULTY_INSTANCE)
	ON_MSG(PACKETTYPE_SET_MODE_DIFFICULTY_DUNGEON, &CDPSrvr::OnSetModeDifficultyInstance);
#endif //__DIFFICULTY_INSTANCE
```

```cpp
#if defined(__DIFFICULTY_INSTANCE)
void CDPSrvr::OnSetModeDifficultyInstance(CAr& ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long)
{
	CUser* pUser = g_UserMng.GetUser(dpidCache, dpidUser);
	if (IsInvalidObj(pUser))
		return;

	u_long idP = pUser->GetPartyId();
	if (idP == 0)
		return;

	unsigned short mode;
	ar >> mode;

	auto instance = CInstanceDungeonHelper::GetInstance()->GetDungeonPtr(IDTYPE_PARTY);
	instance->SetModeDifficulty(static_cast<MODE_DIFFICULTY>(mode), idP, pUser->GetWorld()->GetID());
}
#endif //__DIFFICULTY_INSTANCE
```

### User.h
Ajouter dans la class ___class CUser : public CMover___ :
```cpp
#if defined(__DIFFICULTY_INSTANCE)
    void    SendChooseDifficultyDungeon(void);
#endif //
```


### User.cpp
```cpp
#if defined(__DIFFICULTY_INSTANCE)
void CUser::SendChooseDifficultyDungeon(void)
{
    if (IsDelete())	return;

    m_Snapshot.cb++;
    m_Snapshot.ar << GetId();
    m_Snapshot.ar << SNAPSHOTTYPE_CHOOSE_DIFFICULTY_DUNGEON;
    m_Snapshot.ar << (BYTE)1;

}
#endif //__DIFFICULTY_INSTANCE
```