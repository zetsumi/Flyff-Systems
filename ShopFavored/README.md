# Introduction

Si vous lisez ce fichier au format `MD` utiliser le site web [dilliger](https://dillinger.io/) pour avoir la mise en page.

Systeme réalisé dans le cadre d'une session de [live](https://www.twitch.tv/s4oul/).
L'idee du systeme revient a ForEach !
Vous avez des idées et vous souhaitez la voir réaliser venez la proposer sur le discord [AstralStudio](https://discord.gg/fZP7TWq).

# ShopFavored

`ShopFavored` permet d'ajouter un indiquateur sur les boutiques que l'on suit.
![](VendorShopFavored.PNG)

Par défaut les indiquateurs d'une boutique sont :
![](texDialogBox_left.PNG)
![](texDialogBox_right.PNG)

Lors que vous suivez une boutique les indiquateurs seront :
![](texDialogBox_left_favored.PNG)
![](texDialogBox_right_favored.PNG)


## Amelioration
Changer les icons donnée, malgré que je sois un grand picaso !<br>


# Integration

Ajouter les fichiers ***texDialogBox_left_favored.tga*** et ***texDialogBox_right_favored.tga*** dans le répertoire `Theme\Default` de vos ressources.<br>
Ajouter les fichiers ***ShopFavo.hpp*** et ***ShopFavo.cpp*** dans le répertoire `_Common`, puis ajouter les à votre solution `Neuz`.

*** /!\ IMPORTANT /!\ ***
Vous allez avoir besoin de la fonction de la fonction ***GetDlgItem<T>(UINT id)***.
Crédit : [SquonK](https://github.com/SPSquonK)
Dans le fichier ***WndBase.h*** chercher :
```cpp
CWndBase* GetDlgItem(UINT nID);
```
Ajouter :
```cpp
	template<typename T>
	inline T* GetDlgItem(UINT nID)
	{
		CWndBase* pWnd = GetDlgItem(nID);
		return pWnd != nullptr ? dynamic_cast<T*>(pWnd) : nullptr;
	}
```


# Fichiers

## VersionCommon.h
```cpp
#define		__SHOP_FAVORED						//	Sauvergarder shopping prefere. Idea by @ForEach
```

## ResData.inc
Vous devez agrandir l'interface `APP_VENDOR_REVISION`.<br>
```cpp
APP_VENDOR_REVISION "WndTile00.tga" 1 460 464 0x2410000 26
```
Ajouter les boutons :
```cpp
    WTYPE_BUTTON WIDC_FAVORED "ButtFollow.tga" 0 288 8 440 28 0x220010 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003832
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003833
    }
    WTYPE_BUTTON WIDC_UNFAVORED "ButtUnFollow.tga" 0 288 8 440 28 0x220010 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003832
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003833
    }
```
Au final votre interface devrait etre :
```cpp
APP_VENDOR_REVISION "WndTile00.tga" 1 460 464 0x2410000 26
{
// Title String
IDS_RESDATA_INC_003820
}
{
// ToolTip
IDS_RESDATA_INC_003821
}
{
    WTYPE_EDITCTRL WIDC_EDIT1 "WndEditTile00.tga" 1 86 8 286 28 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003824
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003825
    }
    WTYPE_BUTTON WIDC_CHAT "ButtChat.BMP" 0 360 36 432 56 0x220010 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003830
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003831
    }
    WTYPE_BUTTON WIDC_OK "ButtOk.tga" 0 288 8 360 28 0x220010 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003832
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003833
    }
    WTYPE_BUTTON WIDC_FAVORED "ButtFollow.tga" 0 288 8 440 28 0x220010 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003832
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003833
    }
    WTYPE_BUTTON WIDC_UNFAVORED "ButtUnFollow.tga" 0 288 8 440 28 0x220010 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003832
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003833
    }
    WTYPE_BUTTON WIDC_CANCEL "ButtCancel.tga" 0 360 8 432 28 0x220010 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003834
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003835
    }
    WTYPE_STATIC WIDC_STATIC3 "WndEditTile200.tga" 1 6 8 86 24 0x2220001 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003882
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003883
    }
    WTYPE_STATIC WIDC_STATIC1 "WndEditTile200.tga" 1 6 38 288 54 0x2220001 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003884
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003885
    }
    WTYPE_EDITCTRL WIDC_EDIT2 "WndEditTile00.tga" 1 8 66 208 98 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003886
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003887
    }
    WTYPE_EDITCTRL WIDC_EDIT3 "WndEditTile00.tga" 1 228 66 428 100 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003888
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003889
    }
    WTYPE_EDITCTRL WIDC_EDIT4 "WndEditTile00.tga" 1 8 102 208 136 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003890
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003891
    }
    WTYPE_EDITCTRL WIDC_EDIT5 "WndEditTile00.tga" 1 228 102 428 136 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003892
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003893
    }
    WTYPE_EDITCTRL WIDC_EDIT6 "WndEditTile00.tga" 1 8 138 208 170 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003894
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003895
    }
    WTYPE_EDITCTRL WIDC_EDIT7 "WndEditTile00.tga" 1 228 138 428 170 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003896
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003897
    }
    WTYPE_EDITCTRL WIDC_EDIT8 "WndEditTile00.tga" 1 8 174 208 206 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003898
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003899
    }
    WTYPE_EDITCTRL WIDC_EDIT9 "WndEditTile00.tga" 1 228 174 428 206 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003900
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003901
    }
    WTYPE_EDITCTRL WIDC_EDIT10 "WndEditTile00.tga" 1 8 210 208 242 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003902
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003903
    }
    WTYPE_EDITCTRL WIDC_EDIT11 "WndEditTile00.tga" 1 228 210 428 242 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003904
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003905
    }
    WTYPE_EDITCTRL WIDC_EDIT12 "WndEditTile00.tga" 1 8 246 208 278 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003906
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003907
    }
    WTYPE_EDITCTRL WIDC_EDIT13 "WndEditTile00.tga" 1 228 246 428 278 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003908
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003909
    }
    WTYPE_EDITCTRL WIDC_EDIT14 "WndEditTile00.tga" 1 8 282 208 314 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003910
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003911
    }
    WTYPE_EDITCTRL WIDC_EDIT15 "WndEditTile00.tga" 1 228 282 428 314 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003912
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003913
    }
    WTYPE_EDITCTRL WIDC_EDIT16 "WndEditTile00.tga" 1 8 318 208 350 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003914
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003915
    }
    WTYPE_EDITCTRL WIDC_EDIT17 "WndEditTile00.tga" 1 228 318 428 350 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003916
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003917
    }
    WTYPE_EDITCTRL WIDC_EDIT18 "WndEditTile00.tga" 1 8 354 208 386 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003918
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003919
    }
    WTYPE_EDITCTRL WIDC_EDIT19 "WndEditTile00.tga" 1 228 354 428 386 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003920
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003921
    }
    WTYPE_EDITCTRL WIDC_EDIT20 "WndEditTile00.tga" 1 8 392 208 424 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003922
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003923
    }
    WTYPE_EDITCTRL WIDC_EDIT21 "WndEditTile00.tga" 1 228 392 428 424 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_003924
    }
    {
    // ToolTip
    IDS_RESDATA_INC_003925
    }
    WTYPE_BUTTON WIDC_RESET "ButtReset.BMP" 0 288 36 360 56 0x220010 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_005096
    }
    {
    // ToolTip
    IDS_RESDATA_INC_005097
    }
}
```

## ResData.h
```cpp
#if defined(__SHOP_FAVORED)
#define	WIDC_FAVORED					1347
#define	WIDC_UNFAVORED					1345
#endif// __SHOP_FAVORED
```

## DialogMsg.h
Dans la class ***CDialogMsg*** ajouter :
```cpp
#if defined(__SHOP_FAVORED)
	CTexture* m_pTexFavored[3];
#endif //__SHOP_FAVORED
```

## DialogMsg.cpp
```cpp
#if defined(__SHOP_FAVORED)
#include "ShopFavo.hpp"
#endif //__SHOP_FAVORED
```

Dans la fonction ___HRESULT CDialogMsg::InitDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice)___
Chercher :
```cpp
m_pTex[2] = CWndBase::m_textureMng.AddTexture(g_Neuz.m_pd3dDevice, MakePath(DIR_THEME, "texDialogBox_right.tga"), 0xffff00ff);
```
Ajouter :
```cpp
	m_pTexFavored[0] = CWndBase::m_textureMng.AddTexture(g_Neuz.m_pd3dDevice, MakePath(DIR_THEME, "texDialogBox_left_favored.tga"), 0xffff00ff);
	m_pTexFavored[1] = CWndBase::m_textureMng.AddTexture(g_Neuz.m_pd3dDevice, MakePath(DIR_THEME, "texDialogBox_center.TGA"), 0xffff00ff);
	m_pTexFavored[2] = CWndBase::m_textureMng.AddTexture(g_Neuz.m_pd3dDevice, MakePath(DIR_THEME, "texDialogBox_right_favored.tga"), 0xffff00ff);
```

Dans la fonction ___void CDialogMsg::Render(C2DRender * p2DRender)___
Chercher :
```cpp
            ptTex1.x = (LONG)(vOut.x - 24);
			ptTex1.y = (LONG)(vOut.y - fGap);
```
Ajouter :
```cpp
#if defined(__SHOP_FAVORED)
			CMover* pMover = dynamic_cast<CMover*>(pObj);
			bool bIsFavo = CShapFavo::GetInstance().Find(pMover->m_idPlayer);
			CTexture* pTexShop[3] = {
				bIsFavo == true ? m_pTexFavored[0] : m_pTex[0],
				bIsFavo == true ? m_pTexFavored[1] : m_pTex[1],
				bIsFavo == true ? m_pTexFavored[2] : m_pTex[2],
			};
#endif //__SHOP_FAVORED

```
Remplacer :
```cpp
p2DRender->RenderTexture(ptTex1, m_pTex[0], nAlpha, 1.0f, 1.0f);
```
Par :
```cpp
if defined(__SHOP_FAVORED)
			p2DRender->RenderTexture(ptTex1, pTexShop[0], nAlpha, 1.0f, 1.0f);
#else
            p2DRender->RenderTexture(ptTex1, m_pTex[0], nAlpha, 1.0f, 1.0f);
#endif //__SHOP_FAVORED
```

Remplacer :
```cpp
			p2DRender->RenderTextureEx(ptTex1, ptTex2, m_pTex[1], nAlpha, 1.0f, 1.0f);
```
Par :
```cpp
#if defined(__SHOP_FAVORED)
			p2DRender->RenderTextureEx(ptTex1, ptTex2, pTexShop[1], nAlpha, 1.0f, 1.0f);
#else
			p2DRender->RenderTextureEx(ptTex1, ptTex2, m_pTex[1], nAlpha, 1.0f, 1.0f);
#endif //__SHOP_FAVORED
```

Remplacer :
```cpp
p2DRender->RenderTexture(ptTex1, m_pTex[2], nAlpha, 1.0f, 1.0f);
```
Par :
```cpp
#if defined(__SHOP_FAVORED)
			p2DRender->RenderTexture(ptTex1, pTexShop[2], nAlpha, 1.0f, 1.0f);
#else
            p2DRender->RenderTexture(ptTex1, m_pTex[2], nAlpha, 1.0f, 1.0f);
#endif //__SHOP_FAVORED
```

## WndVendor.h
Ajouter les variables dans le scope `private`.
```cpp
	CWndButton* m_pBtnFollow = nullptr;
	CWndButton* m_pBtnUnFollow = nullptr;
	CWndButton* m_pBtnOk = nullptr;
	CWndButton* m_pBtnCancel = nullptr;
```

## WndVendor.cpp
```cpp
#if defined(__SHOP_FAVORED)
#include "ShopFavo.hpp"
#endif //__SHOP_FAVORED
```

Dans la fonction ___void CWndVendor::SetVendor( CMover* pVendor )___.
Chercher :
```cpp
		pButton->SetVisible( FALSE );
		pButton	= (CWndButton*)GetDlgItem( WIDC_CANCEL );
		pButton->SetVisible( FALSE );
```
Ajouter :
```cpp
#if defined(__SHOP_FAVORED)
		if (CShapFavo::GetInstance().Find(m_pVendor->m_idPlayer) == true)
			m_pBtnUnFollow->SetVisible(TRUE);
		else
			m_pBtnFollow->SetVisible(TRUE);
#endif //__SHOP_FAVORED
```

Dans la fonction ___void CWndVendor::OnInitialUpdate()___
Chercher :
```cpp
m_wndctrlVendor.Create( WLVS_ICON | WBS_NODRAWFRAME, CRect( 5, 60, 440, 460 ), this, 100 );
```
Ajouter :
```cpp
	m_pBtnCancel = GetDlgItem<CWndButton>(WIDC_RESET);
	m_pBtnOk = GetDlgItem<CWndButton>(WIDC_OK);
	m_pBtnFollow = GetDlgItem<CWndButton>(WIDC_FAVORED);
	m_pBtnUnFollow = GetDlgItem<CWndButton>(WIDC_UNFAVORED);
	m_pBtnFollow->SetVisible(FALSE);
	m_pBtnUnFollow->SetVisible(FALSE);
```

Dans la fonction ___CWndVendor::OnInitialUpdate()___
Remplacer :
```cpp
	if( !m_pVendor->IsActiveMover() )
	{
		CWndButton* pButton	= (CWndButton*)GetDlgItem( WIDC_OK );
		pButton->SetVisible( FALSE );
		pButton	= (CWndButton*)GetDlgItem( WIDC_CANCEL );
        pButton->SetVisible( FALSE );
    }
```
Par :
```cpp
	if( !m_pVendor->IsActiveMover() )
	{
        if (m_pBtnOk != nullptr)
		    m_pBtnOk->SetVisible( FALSE );
		auto pButton = GetDlgItem<CWndButton>(WIDC_CANCEL);
        if (pButton != nullptr)
		    pButton->SetVisible(FALSE);
	}
```

Dans la fonction ___BOOL CWndVendor::Process()___.
Remplacer la fonction par :
```cpp
BOOL CWndVendor::Process()
{
	if (g_pPlayer->m_vtInfo.IsVendorOpen())
	{
        if (m_pBtnCancel != nullptr)
		    m_pBtnCancel->SetVisible(FALSE);
	}
	else
	{
        if (m_pBtnCancel != nullptr)
		    m_pBtnCancel->SetVisible(TRUE);
	}
	return TRUE;
}
```

Dans la fonction ___BOOL CWndVendor::OnChildNotify( UINT message, UINT nID, LRESULT* pLResult )___
Apres :
```cpp
		else if(nID == WIDC_RESET)
		{
			
			CItemBase	apItem_VT[MAX_VENDITEM];		/// vendor and trader share pointer array
			ZeroMemory( apItem_VT, sizeof( apItem_VT ) );

			//if( g_pPlayer->m_vtInfo.IsVendorOpen() )
			//{
				for( int i = 0; i < MAX_VENDITEM; i++ )
				{
					// 리스트를 클리어 한다. 
					CItemBase* pItemBase = g_pPlayer->m_vtInfo.GetItem(i);
					if( pItemBase != NULL )
					{
						if( pItemBase->m_dwObjId != 0 )
							g_DPlay.SendUnregisterPVendorItem( i );
					}
				}

				// 저장버퍼도 클리어
				memset(g_Neuz.m_aSavedInven, 0, sizeof(g_Neuz.m_aSavedInven));
			//}
		}
```
Ajouter :
```cpp
#if defined(__SHOP_FAVORED)
		else if (nID == WIDC_FAVORED)
		{
			if (m_pVendor != nullptr)
			{
				CShapFavo::GetInstance().Add(m_pVendor->m_idPlayer);
				m_pBtnUnFollow->SetVisible(TRUE);
				m_pBtnFollow->SetVisible(FALSE);
			}

		}
		else if (nID == WIDC_UNFAVORED)
		{
			if (m_pVendor != nullptr)
			{
				CShapFavo::GetInstance().Remove(m_pVendor->m_idPlayer);
				m_pBtnUnFollow->SetVisible(FALSE);
				m_pBtnFollow->SetVisible(TRUE);
			}
		}
#endif //__SHOP_FAVORED
```
