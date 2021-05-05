# Les donjons dans Flyff

Comment ca fonctionne ?

## Partie Ressource

Les donjons sont configuré via deux fichiers lua `InstanceDungeonBase.lua` et `PartyDungeon.lua`.

### InstanceDungeonBase

Constantes :
```lua
nMaxInstanceDungeon = 40

CLASS_NORMAL    = 1
CLASS_MASTER    = 2
CLASS_HERO      = 4

ID_NORMAL   = 0
ID_MIDBOSS  = 1
ID_BOSS     = 2
```

***AddDungeon( strWorldId )*** :
`strWorldId` : Une string, le nom du define de donjon.
Permet d'ajouter un donjon à la liste.
Initialise des valeurs par défaut au donjon.

***SetClass( ... )*** :
`...` : Une liste d'arguments correspondant a un rang de classe.
Tous les rang de classe listé seront admis dans l'instance.

***SetLevel( nMinLevel, nMaxLevel )*** :
`nMinLevel` : Niveau minimun (inclus)
`nMaxLevel` : Niveau maximun (inclus)
Définie la range des niveaux accpté dans le donjon.


***SetCoolTime( dwCoolTime )*** :
`dwColTime` : Temps d'attente
Définie le temps d'attente entre deux entré.

***SetMonster( nType, strMonsterId, bRed, x, y, z )*** :
`nType`        : Type du bosse
`strMonsterId` : String correspondant au define du bosse.
`bRed`         : Aggressivité, true ou false.
`x`            : Location en X.
`y`            : Location en Y.
`z`            : Location en Z.
Ajoute un bosse dans le donjon.


***SetTeleportPos( nType, x, y, z )*** :
.... Il faut que je fasse des recherche.

### PartyDungeon

Ce fichier définie le contenue du donjon ainsi que ces règles.
L'ensemble des fonctions et constantes utilisé dans ce fichier proviennent du fichier `InstanceDungeonBase.lua`.

Prenons pour exemple :
```lua
AddDungeon( "WI_INSTANCE_OMINOUS" )
--{
	SetClass( CLASS_NORMAL )
	SetLevel( 1, 130 )
	SetCoolTime( MIN(30) )
	SetMonster( ID_NORMAL, "MI_PRICKANT04", true, 1280, 101, 1640 )
	SetMonster( ID_NORMAL, "MI_MAULMOUSE04", true, 1234, 101, 1393 )
	SetMonster( ID_NORMAL, "MI_CRIPESCENTIPEDE04", true, 1089, 101, 1590 )
	SetMonster( ID_MIDBOSS, "MI_LYCANOS01", true, 1078, 101, 1359 )
	SetMonster( ID_BOSS, "MI_VEMPAIN01", true, 1079, 101, 1457 )
--}
```
Cette config définie le donjon `WI_INSTANCE_OMINOUS`.
Il faut un rang minimun de `CLASS_NORMAL`.
Les personnages de niveau `1` à `160` inclus peuvent rentrer.
Le temps d'attente entre deux entré est de `360 minutes`.
On ajout le bosse `MI_PRICKANT04` de type `ID_NORMAL` en position `1280, 101, 1640`, il est `aggressif`.
On ajout le bosse `MI_MAULMOUSE04` de type `ID_NORMAL` en position `1234, 101, 1393`, il est `aggressif`.
On ajout le bosse `MI_CRIPESCENTIPEDE04` de type `ID_NORMAL` en position `1089, 101, 1590`, il est `aggressif`.
On ajout le bosse `MI_LYCANOS01` de type `ID_MIDBOSS` en position `1078, 101, 1359`, il est `aggressif`.
On ajout le bosse `MI_VEMPAIN01` de type `ID_BOSS` en position `1079, 101, 1457`, il est `aggressif`.

Les type de bosses sont important pour connaitre l'ordre d'apparission.
L'ordre est ainsi ID_NORMAL => ID_MIDBOSS => ID_BOSS.
Il faut que tous les bosses de type ID_NORMAL soit mort pour faire apparaitre les bosses de type ID_MIDBOSS, de même pour ID_MIDBOSS vers ID_BOSS.


## Partie Source

Il faut savoir que les parties joué en donjon sont traité dans le CoreServer et le WorldServer.
Schema communiquation basique.


### Trigger
Référence sur l'article trigger.

### CoreServer

### WorldServer

