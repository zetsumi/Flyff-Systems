# Introduction

Si vous lisez ce fichier au format `MD` utiliser le site web [dilliger](https://dillinger.io/) pour avoir la mise en page.

Systeme realiser dans le cadre d'une session de [live](https://www.twitch.tv/s4oul/).
L'idee du systeme revient a Zeepone !
Vous avez des idees et vous souhaitez la voir realiser venez la proposer sur le discord [AstralStudio](https://discord.gg/fZP7TWq
).

# Awake Custom
`Awake Custom` permet d'eveille vos armes automatiquement.
![](UIAwakeCustom.PNG)

Legendes :
* [1] Affichage de larmes a eveille.
* [2] Le cout en gold.
* [3] Liste des eveilles possible.
* [4] Liste des eveilles rechercher.
* [5] Eveilles present sur l'arme

Utilisation : 
Placer votre armes dans l'interface.
Selectionner les eveilles desirer.
Appuyer sur `Accept`, les eveilles automatique se lancera.
Lorsqu'un eveille contient au moins une des conditions selectionner les eveilles se termine.

# A ameliorer
Le placement de l'arme s'effectue dans l'interface `global` et non dans le petit encadrer prevue a la base.
Supprimer un eveille present dans `[4]`.

# Integration

Vous devez placer les fichier `WndAwakeCustom.hpp` et `WndAwakeCustom.cpp` dans `_Interface` puis ajouter les a votre solution `Neuz` via `VisualStudio`.

## VersionCommon.h

```cpp
#define		__AWAKE_CUSTOM						//	Eveille Automatique. Idea by @zeepone
```

## ResData.h
```cpp
#if defined(__AWAKE_CUSTOM)
#define APP_AWAKE_CUSTOM												2032
#endif //__AWAKE_CUSTOM
```
Attention la valeur `2032` peut etre deja prise ! Remplacer la par une autre valeur si necessaire.

## ResData.txt.txt
```cpp
IDS_RESDATA_INC_EMPTY
```

## ResData.inc
```cpp
APP_AWAKE_CUSTOM "WndTile00.tga" 1 624 304 0x2410000 26
{
// Title String
"Awake Custom"
}
{
// ToolTip
IDS_RESDATA_INC_EMPTY
}
{
    WTYPE_STATIC WIDC_STATIC "WndChgElemItem.bmp" 0 42 24 74 56 0x220000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_EMPTY
    }
    {
    // ToolTip
    IDS_RESDATA_INC_EMPTY
    }
    WTYPE_LISTBOX WIDC_LISTBOX1 "WndEditTile00.tga" 1 18 146 297 245 0x20020000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_EMPTY
    }
    {
    // ToolTip
    IDS_RESDATA_INC_EMPTY
    }
    WTYPE_BUTTON WIDC_BUTTON1 "ButtAccept.tga" 0 36 112 111 134 0x220010 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_EMPTY
    }
    {
    // ToolTip
    IDS_RESDATA_INC_EMPTY
    }
    WTYPE_BUTTON WIDC_BUTTON_2 "ButtCancel.tga" 0 160 112 235 134 0x220010 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_EMPTY
    }
    {
    // ToolTip
    IDS_RESDATA_INC_EMPTY
    }
    WTYPE_LISTBOX WIDC_LISTBOX2 "WndEditTile00.tga" 1 308 146 587 245 0x20020000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_EMPTY
    }
    {
    // ToolTip
    IDS_RESDATA_INC_EMPTY
    }
    WTYPE_LISTBOX WIDC_LISTBOX3 "WndEditTile00.tga" 1 308 22 587 121 0x20020000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_EMPTY
    }
    {
    // ToolTip
    IDS_RESDATA_INC_EMPTY
    }
    WTYPE_TEXT WIDC_TEXT2 "WndEditTile00.tga" 1 160 26 230 54 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_EMPTY
    }
    {
    // ToolTip
    IDS_RESDATA_INC_EMPTY
    }
}
```
Interface en v15 !

## MoverParam.cpp
```cpp
#if defined(__CLIENT) && defined(__AWAKE_CUSTOM)
#include "WndAwakeCustom.hpp"
#endif //__AWAKE_CUSTOM
```

Dans la fonction ___void CMover::UpdateItemEx___ 
chercher 
```cpp
		CWndInventory* pWnd		= (CWndInventory*)g_WndMng.GetWndBase( APP_INVENTORY );
		if( pWnd )
			pWnd->UpdateTooltip();
```
ajouter en dessous `dans le scope __CLIENT`
```cpp
#if defined(__AWAKE_CUSTOM)
		CWndAwakeCustom* pWndAwakeCustom = (CWndAwakeCustom*)g_WndMng.GetWndBase(APP_AWAKE_CUSTOM);
		if (pWndAwakeCustom)
			pWndAwakeCustom->UpdateTooltip();
#endif //__AWAKE_CUSTOM
```

## FuncApplet.cpp
```cpp
#if defined(__AWAKE_CUSTOM)
#include "WndAwakeCustom.hpp"
#endif //__AWAKE_CUSTOM
```
```cpp
#if defined(__AWAKE_CUSTOM)
DECLAREAPPLET(AppMain_WndAwakeCustom, new CWndAwakeCustom);
#endif //__AWAKE_CUSTOM
```

Dans la fonction ___CWndMgr::AddAllApplet___
Apres
```cpp
    AddAppletFunc(AppMain_WndMessenger, APP_MESSENGER_, _T("WndMessenger"), _T("Icon_Messenger.dds"), GETTEXT(TID_TIP_MESSENGER), SHORTCUT_WND_MESSENGER);
```
Ajouter
```cpp
#if defined(__AWAKE_CUSTOM)
    AddAppletFunc(AppMain_WndAwakeCustom, APP_AWAKE_CUSTOM, _T("WndAwakeCustom"), _T("Icon_Applet.dds"), "Awake Custom", 0);
#endif //__AWAKE_CUSTOM
```
A la place de `0` vous pouvez choisir le raccourcie souhaitez `x` par exemple.


## WndTaskBar.cpp
Dans la fonction ___CWndTaskMenu::OnInitialUpdate___
```cpp
#if defined(__AWAKE_CUSTOM)
        AppendMenu(m_pMenuAdmin, 0, APP_AWAKE_CUSTOM, "Awake Custom");
#endif //__AWAKE_CUSTOM
```
Remplacer `m_pMenuAdmin` par le menu souhaiter. La variable `m_pMenuAdmin` est propre a ma source.
Exemple vous pouvez utiliser `m_pMenu1` ou `m_pMenu2`.

## DPClient.h
Remplacer :
```cpp
void SendAwakening(int nItem);
```
Par :
```cpp
#if defined(__AWAKE_CUSTOM)
	void SendAwakening(int nItem, BOOL bChecker = TRUE);
#else
	void SendAwakening(int nItem, BOOL bChecker = TRUE);
#endif //__AWAKE_CUSTOM
```

## DPClient.cpp
```cpp
#if defined(__AWAKE_CUSTOM)
#include "WndAwakeCustom.hpp"
#endif //
```

Dans la fonction ___CDPClient::OnUpdateItemEx___
```cpp
	pMover->UpdateItemEx(id, cParam, iValue);
#if defined(__AWAKE_CUSTOM)
	CWndAwakeCustom* pWndAwakeCustom = (CWndAwakeCustom*)g_WndMng.GetWndBase(APP_AWAKE_CUSTOM);
	if (pWndAwakeCustom)
	{
		if (pWndAwakeCustom->IsValidConditions() == false)
			pWndAwakeCustom->ReBuild();
	}
#endif //__AWAKE_CUSTOM
```

Remplacer le nom de la fonction
```cpp 
CDPClient::SendAwakening(int nItem)
```
Par
```cpp
CDPClient::SendAwakening(int nItem, BOOL bChecker)
```
Puis ajouter
```cpp
	BEFORESENDSOLE(ar, PACKETTYPE_AWAKENING, DPID_UNKNOWN);
#if defined(__AWAKE_CUSTOM)
	ar << bChecker;
#endif //__AWAKE_CUSTOM
```
Pour un resultat final :
```cpp
void CDPClient::SendAwakening(int nItem, BOOL bChecker)
{
	BEFORESENDSOLE(ar, PACKETTYPE_AWAKENING, DPID_UNKNOWN);
#if defined(__AWAKE_CUSTOM)
	ar << bChecker;
#endif //__AWAKE_CUSTOM
	ar << nItem;
	SEND(ar, this, DPID_SERVERPLAYER);
}
```

## DPSsrv.cpp

Dans la fonction ___CDPSrvr::OnAwakening___
```cpp
	CUser* pUser = g_UserMng.GetUser(dpidCache, dpidUser);
	if (IsValidObj(pUser))
	{
#if defined(__AWAKE_CUSTOM)
		BOOL bChecker = FALSE;
		ar >> bChecker;
#endif // __AWAKE_CUSTOM
#if defined(__AWAKE_CUSTOM)
		if (bChecker == TRUE)
		{
#endif //__AWAKE_CUSTOM
			if (!CNpcChecker::GetInstance()->IsCloseNpc(MMI_ITEM_AWAKENING, pUser->GetWorld(), pUser->GetPos()))
				return;
#if defined(__AWAKE_CUSTOM)
		}
#endif //__AWAKE_CUSTOM
```
Remplacer la condition :
```cpp
if (g_xRandomOptionProperty->GetRandomOptionSize(pItem->GetRandomOptItemId()) > 0)
```
Par :
```cpp
#if defined(__AWAKE_CUSTOM)
			if (bChecker == TRUE && g_xRandomOptionProperty->GetRandomOptionSize(pItem->GetRandomOptItemId()) > 0)
#else
			if (g_xRandomOptionProperty->GetRandomOptionSize(pItem->GetRandomOptItemId()) > 0)
#endif
```

## randomoption.h
Dans la class ___CRandomOptionProperty___ ajouter
```cpp
public:
	const auto& getRandomOptionProb(void) const { return m_aRandomOption; }
```

## WndManager.h
ajouter
```cpp
char* FindDstString(int nDstParam);
```