# Introduction
Si vous lisez ce fichier au format `MD` utiliser le site web [dilliger](https://dillinger.io/) pour avoir la mise en page.<br>
Systeme réalisé dans le cadre d'une session de [live](https://www.twitch.tv/s4oul/).<br>
Vous avez des idées et vous souhaitez la voir réaliser venez la proposer sur le discord [AstralStudio](https://discord.gg/fZP7TWq).<br>

# Admin Change Level
`Admin Change Level` permet de changer de niveau et de job via une interface.<br>
Le system s'appuie sur la commande `/level`.<br>
![](AdminChangeLevel.png)

# Amelioration
Ajouter les 3ieme classe.<br>
Ajouter des verification sur les `level` et `expertise`.<br>
Remplacer des `checkboxes` par des `radio box`.<br>
Auto-Completion.<br>

# Integration

Ajouter les fichiers ***WndChangeJobAdmin.hpp*** et ***WndChangeJobAdmin.cpp*** dans le répertoire `_Interface_`, puis ajouter les à votre solution `Neuz`.

## VersionCommon.h
```cpp
#define		__APP_CHANGE_JOB_ADMIN				//	Fenetre permettant de changer de job rapidement en ADMIN
```

## ResData.h
Verifiez que ***2027*** n'est pas déjà pris !
```cpp
#if defined(__APP_CHANGE_JOB_ADMIN)
#define APP_CHANGE_JOB_ADMIN											2027
#endif //__APP_CHANGE_JOB_ADMIN
```

## resData.txt.txt
Si vous n'avez pas `IDS_RESDATA_INC_EMPTY` ajouter le.<br>

## ResData.inc
```cpp
APP_CHANGE_JOB_ADMIN "WndTile00.tga" 1 304 224 0x2410000 26
{
// Title String
"Change Job Administrator"
}
{
// ToolTip
IDS_RESDATA_INC_007051
}
{
    WTYPE_COMBOBOX WIDC_COMBOBOX1 "WndEditTile00.tga" 1 66 24 279 43 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_EMPTY
    }
    {
    // ToolTip
    IDS_RESDATA_INC_EMPTY
    }
    WTYPE_STATIC WIDC_STATIC "" 0 24 24 53 43 0x220000 0 0 0 0
    {
    // Title String
    "Job:"
    }
    {
    // ToolTip
    IDS_RESDATA_INC_EMPTY
    }
    WTYPE_STATIC WIDC_STATIC1 "" 0 22 62 61 81 0x220000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_007056
    }
    {
    // ToolTip
    IDS_RESDATA_INC_EMPTY
    }
    WTYPE_EDITCTRL WIDC_EDIT "" 1 70 64 279 84 0x20000 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_007058
    }
    {
    // ToolTip
    IDS_RESDATA_INC_EMPTY
    }
    WTYPE_BUTTON WIDC_ACCEPT "ButtOk.tga" 1 44 144 116 165 0x220010 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_EMPTY
    }
    {
    // ToolTip
    IDS_RESDATA_INC_EMPTY
    }
    WTYPE_BUTTON WIDC_CANCEL "ButtCancel.tga" 1 164 144 237 166 0x220010 0 0 0 0
    {
    // Title String
    IDS_RESDATA_INC_EMPTY
    }
    {
    // ToolTip
    IDS_RESDATA_INC_EMPTY
    }
    WTYPE_STATIC WIDC_STATIC2 "" 0 18 104 65 123 0x220000 0 0 0 0
    {
    // Title String
    "Expert:"
    }
    {
    // ToolTip
    IDS_RESDATA_INC_EMPTY
    }
    WTYPE_BUTTON WIDC_CHECK1 "" 0 80 108 159 122 0x220008 0 0 0 0
    {
    // Title String
    "hero"
    }
    {
    // ToolTip
    IDS_RESDATA_INC_EMPTY
    }
    WTYPE_BUTTON WIDC_CHECK2 "" 0 140 108 219 122 0x220008 0 0 0 0
    {
    // Title String
    "master"
    }
    {
    // ToolTip
    IDS_RESDATA_INC_EMPTY
    }
}
```

## FuncApplet.cpp
```cpp
#if defined(__APP_CHANGE_JOB_ADMIN)
#include "WndChangeJobAdmin.hpp"
#endif
```
```cpp
#if defined(__APP_CHANGE_JOB_ADMIN)
DECLAREAPPLET(AppMain_WndChangeJobAdmin, new WndChangeJobAdmin);
#endif
```
```cpp
#if defined(__APP_CHANGE_JOB_ADMIN)
    AddAppletFunc(AppMain_WndChangeJobAdmin, APP_CHANGE_JOB_ADMIN, _T("WndChangeJobAdmin"), _T("Icon_Applet.dds"), "Change Job Administrator", 0);
#endif //__APP_CHANGE_JOB_ADMIN
```

## WndTaskBar.cpp
Ajouter dans votre menu admin, pour moi le pointeur est ```m_pMenuAdmin```. A vous d'utiliser votre pointeur.<br>
```cpp
#if defined(__APP_CHANGE_JOB_ADMIN)
        AppendMenu(m_pMenuAdmin, 0, APP_CHANGE_JOB_ADMIN, "Change Job");
#endif // __APP_CHANGE_JOB_ADMIN
```