# Les donjons dans Flyff

Comment ca fonctionne ?

## Partie Ressource

Les donjons sont configuré via deux fichiers lua `InstanceDungeonBase.lua` et `PartyDungeon.lua`.<br>

### InstanceDungeonBase

Constantes :
```lua
nMaxInstanceDungeon = 40

CLASS_NORMAL = 1
CLASS_MASTER = 2
CLASS_HERO   = 4

ID_NORMAL  = 0
ID_MIDBOSS = 1
ID_BOSS    = 2
```

***AddDungeon( strWorldId )*** :
`strWorldId` : Une string, le nom du define de donjon.<br>
Permet d'ajouter un donjon à la liste.<br>
Initialise des valeurs par défaut au donjon.<br>

***SetClass( ... )*** :
`...` : Une liste d'arguments correspondant a un rang de classe.<br>
Tous les rang de classe listé seront admis dans l'instance.<br>

***SetLevel( nMinLevel, nMaxLevel )*** :
`nMinLevel` : Niveau minimun (inclus)<br>
`nMaxLevel` : Niveau maximun (inclus)<br>
Définie la range des niveaux accpté dans le donjon.<br>


***SetCoolTime( dwCoolTime )*** :
`dwColTime` : Temps d'attente<br>
Définie le temps d'attente entre deux entré.<br>

***SetMonster( nType, strMonsterId, bRed, x, y, z )*** :
`nType`        : Type du bosse<br>
`strMonsterId` : String correspondant au define du bosse.<br>
`bRed`         : Aggressivité, true ou false.<br>
`x`            : Location en X.<br>
`y`            : Location en Y.<br>
`z`            : Location en Z.<br>
Ajoute un bosse dans le donjon.<br>


***SetTeleportPos( nType, x, y, z )*** :
.... Il faut que je fasse des recherche.<br>

### PartyDungeon

Ce fichier définie le contenue du donjon ainsi que ces règles.<br>
L'ensemble des fonctions et constantes utilisé dans ce fichier proviennent du fichier `InstanceDungeonBase.lua`.<br>

Prenons pour exemple :
```lua
AddDungeon( "WI_INSTANCE_OMINOUS" )
--{
	SetClass( CLASS_NORMAL )
	SetLevel( 1, 130 )
	SetCoolTime( MIN(30) )
	SetMonster( ID_NORMAL,  "MI_PRICKANT04",        true, 1280, 101, 1640 )
	SetMonster( ID_NORMAL,  "MI_MAULMOUSE04",       true, 1234, 101, 1393 )
	SetMonster( ID_NORMAL,  "MI_CRIPESCENTIPEDE04", true, 1089, 101, 1590 )
	SetMonster( ID_MIDBOSS, "MI_LYCANOS01",         true, 1078, 101, 1359 )
	SetMonster( ID_BOSS,    "MI_VEMPAIN01",         true, 1079, 101, 1457 )
--}
```
Cette config définie le donjon ***WI_INSTANCE_OMINOUS*** :
* Il faut un rang minimun de `CLASS_NORMAL`.
* Les personnages de niveau `1` à `160` inclus peuvent rentrer.
* Le temps d'attente entre deux entré est de `360 minutes`.
* On ajout le bosse `MI_PRICKANT04`        de type `ID_NORMAL`  en position `1280, 101, 1640`, il est `aggressif`.
* On ajout le bosse `MI_MAULMOUSE04`       de type `ID_NORMAL`  en position `1234, 101, 1393`, il est `aggressif`.
* On ajout le bosse `MI_CRIPESCENTIPEDE04` de type `ID_NORMAL`  en position `1089, 101, 1590`, il est `aggressif`.
* On ajout le bosse `MI_LYCANOS01`         de type `ID_MIDBOSS` en position `1078, 101, 1359`, il est `aggressif`.
* On ajout le bosse `MI_VEMPAIN01`         de type `ID_BOSS`    en position `1079, 101, 1457`, il est `aggressif`.
<br>

Les type de bosses sont important pour connaitre l'ordre d'apparission.<br>
L'ordre est ainsi *ID_NORMAL* => *ID_MIDBOSS* => *ID_BOSS*.<br>
Il faut que tous les bosses de type `ID_NORMAL` soit mort pour faire apparaitre les bosses de type `ID_MIDBOSS`, de même pour `ID_MIDBOSS` vers `ID_BOSS`.<br>


## Partie Source

Il faut savoir que les parties joué en donjon sont traité dans le CoreServer et le WorldServer.

Lorsqu'un joueur passe dans un trigger permettant de le téléporter vers une autre map il y a une communiquation qui s'effectue entre le WorldServer et le CoreServer.

#### Objets C++

La manipulation des donjons se font via les objets :
* CInstanceDungeonBase
* CInstanceDungeonHelper
* CInstanceDungeonParty

`CInstanceDungeonHelper` permet l'abstraction des donjons.
`CInstanceDungeonBase` permet d'effectuer les règles décrite dans le fichier de configuration, stock l'ensemble des créature présente dans le donjon. Interface principal des donjons.
`CInstanceDungeonParty` ajout de fonctions spécifique pour les groupes.

#### Procédure

##### Entré

La détection qu'un joueur souhaite rentré dans un donjon s'effectue via un trigger `CMover::ProcessRegion`.
```cpp
    if (pPortkey)
    {
        if (IsFly())
        {
            return;
        }

        if (CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon(pPortkey->m_dwIdTeleWorld))
        {
            if (!CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon(static_cast<CUser*>(this)->GetWorld()->GetID()))
            {
                CInstanceDungeonHelper::GetInstance()->EnteranceDungeon(static_cast<CUser*>(this), pPortkey->m_dwIdTeleWorld);
            }
            else
            {
                REPLACE(g_uIdofMulti, pPortkey->m_dwIdTeleWorld, pPortkey->m_vTeleWorld, REPLACE_NORMAL, static_cast<CUser*>(this)->GetLayer());
            }
            return;
        }
        if (ReplaceInspection(pPortkey) == FALSE)
        {
            return;
        }
        REPLACE(g_uIdofMulti, pPortkey->m_dwIdTeleWorld, pPortkey->m_vTeleWorld, REPLACE_NORMAL, nTempLayer);
    }
```

Détaillons un peut tout ca :

On cherche à savoir si la destination est un donjon `pPortkey->m_dwIdTeleWorld`.
```cpp
if (CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon(pPortkey->m_dwIdTeleWorld))
```

Si le monde actuel n'est pas un donjon `static_cast<CUser*>(this)->GetWorld()->GetID())`.
Si c'est le cas on demande à faire rentrer le joueur dans le donjon `pPortkey->m_dwIdTeleWorld`.
```cpp
if (!CInstanceDungeonHelper::GetInstance()->IsInstanceDungeon(static_cast<CUser*>(this)->GetWorld()->GetID()))
{
    CInstanceDungeonHelper::GetInstance()->EnteranceDungeon(static_cast<CUser*>(this), pPortkey->m_dwIdTeleWorld);
}
````
Sinon on téléporte le joueur vers `pPortkey->m_dwIdTeleWorld`.
```cpp
else
{
    REPLACE(g_uIdofMulti, pPortkey->m_dwIdTeleWorld, pPortkey->m_vTeleWorld, REPLACE_NORMAL, static_cast<CUser*>(this)->GetLayer());
}
```

Prenons le cas ou le joueur n'est pas dans un donjon est souhaite se téléporté dans un donjon.
La fonction `CInstanceDungeonHelper::GetInstance()->EnteranceDungeon` fera appel à `CInstanceDungeonBase::TeleportToDungeon(CUser* pUser, DWORD dwWorldId, DWORD dwDungeonId)`, lors de son premier appel la variable `ID_INFO* pInfo = GetDungeonInfo(dwDungeonId, dwWorldId);` vaura nullptr ! Mais pourquoi ? Car l'information sur un donjon ne se fait qu'après réception d'une validation du CoreServer.
C'est seulement au prochaine appelle à `CMover::ProcessRegion` que le joueur sera téléporté.

Le appelle à `EnteranceDungeon` mène donc à la procédure [La Création](#La-Création).


##### La Création

Si aucune instance du donjon n'existe alors le WorldServer envoit un paquet au CoreServer.
Le CoreServer recoit un messsage `PACKETTYPE_INSTANCEDUNGEON_CREATE` provenant du WorldServer indiquant qu'un joueur souhaite rentrer dans un donjon.

Le CoreServer va donc enregistrer la demande et renvoyer un message `PACKETTYPE_INSTANCEDUNGEON_CREATE` indiquant que la création a été réussie.

Le CoreServer ne fait qu'enregistrer la création d'un donjon si il n'existe pas déjà.
Le WorldServer va aussi effectuer la même opération après la réception de `PACKETTYPE_INSTANCEDUNGEON_CREATE` à la différence prète que le WorldServer lui va créer une nouvelle `LinkMap` et effectuer d'autre vérification.
Le WorldServer va donc vérifier les conditions entré dans le ficher de configuration `PartyDungeon.lua`.

Donc le joueur sera téléporté qu'une fois que le CoreServer valide la création et que le WorldServer recoit la validation et valide lui aussi la création.

##### La Destruction

Un donjon n'existe que tant qu'il y a des joueurs dedans.
A chaque fois qu'un joueur quitte le donjon il y a check sur le nombre de joueur restant.
Si aucun joueur n'est présent le WorldServer indique au CoreServer qu'un donjon doit être détruit.


##### Etape Du Donjon

A chaque fois qu'un joueur tue une créature dans le donjon la procédure effectue une vérification sur l'ID du monstre, si le joueur tue un des boss listé alors on avance dans les étapes du donjon.


## Problème

##### Trigger
La fonction `CMover::ProcessRegion` doit être appellé deux fois !
....

