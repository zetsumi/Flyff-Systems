# FlyFF et les drops !

Beaucoup de personne se plaignent des drops sur FlyFF !<br>
Mais qu'elle en est la cause ?<br>

Il y existe deux points importants pouvant créer des problème d'équilibrage sur le drop.<br>
Nous allons détailler ces points et proposer une solutions.

## Comment fonctionne le système

Tout d'abord un petit point sur le fonctionnement du système `DropItem`.<br>
Les propriété de DropItem() sont stockées dans `m_dropItems` de l'objet `CDropItemGenerator`.<br>
La variable `m_dropItems` est un `std::vector<DROPITEM>`.<br>

Maintenant que nous savons où les informations sont enregistrées regardons comme le drop est sélectionné.<br>
Dans le fichier `Mover.cpp` il y a la fonction `BOOL CMover::DropItem(CMover* pAttacker)`.<br>
C'est donc cette derniere qui effectue les différents types de drop, dans notre cas nous nous interessons uniquement au `DropItem` donc
à la variable `m_DropItemGenerator`.<br>
Plus loin dans la fonction nous pouvons voir :
```cpp
FLOAT fItemDropRate = nProbability * GetItemDropRateFactor(pAttacker);
if (xRandom(100) < fItemDropRate)
{
    int nSize = lpMoverProp->m_DropItemGenerator.GetSize();
    int nNumber = 0;
    DROPITEM* lpDropItem;
    for (int i = 0; i < nSize; i++)
    {
        lpDropItem = lpMoverProp->m_DropItemGenerator.GetAt(i, bUnique, GetPieceItemDropRateFactor(pAttacker));
        if (lpDropItem != nullptr)
        {
            // .....
        }
    }
}
```

C'est donc ici que l'on va sélectionner l'item à faire drop.<br>
Dans mon cas la fonction `DROPITEM* CDropItemGenerator::GetAt`
 indique qu'elle a besoin d'un variable `bUniqueMode` mais ne l'utilise pas !<br>
Si on regarde en détail la fonction voici comme sélectionne un item.<br>
```cpp
DROPITEM* CDropItemGenerator::GetAt(int nIndex, BOOL, float fProbability)
{
    if (fProbability <= 0.0f)
        return &m_dropItems[nIndex];

    // On récupère un item.
    DROPITEM* lpDropItem = &m_dropItems[nIndex];
    // On récupère la probabilité de l'item.
    DWORD dwProbability = lpDropItem->dwProbability;
    // On tire un nombre aléatoire.
    // Le nombre présente notre chance.
    DWORD dwRand = xRandom(3000000000);
#if defined(__WORLDSERVER)
    // On applique le chance de drops.
    dwRand = static_cast<DWORD>(dwRand / fProbability);
#endif
    // Si notre chance est plus petit que la proba alors on retourne l'objet.
    return(dwRand < dwProbability ? lpDropItem : nullptr);
}
```

Pour rappel :
```
Information sur les drops
9375           0.0003125%    320000
18750          0.000625%     160000
37500          0.00125%      80000
75000          0.0025%       40000
150000         0.005%        20000
300000         0.01%         10000
1000000        0.0333%       3000
1875000        0.0625%       1600
3000000        0.1%          1000
3750000        0.125%        800
5000000        0.166%        600
7500000        0.25%         400
15000000       0.5%          200
30000000       1%            100
300000000      10%           10
3000000000     100%          1
```

Imaginons qu'un item à une proba de `3000000` et notre chance est de `1000` alors on récupère l'objet.<br>
Imaginons qu'un item à une proba de `3000000` et notre chance est de `4000000` alors on ne récupère pas l'objet.<br>


## Un pourcentage pas répartie sur 100 %

Il y existe des personnes qui ne font pas attention à ce qu'elles configurent et le code source ne vérifie pas pour vous ! (pas de chance).<br>
Il faut faire attention que la somme de `DropItem` ne dépasse pas `3000000000`.<br>
Si on dépasse `3000000000` on a plus 100 part à partagé !<br>
Il est aussi recommandé de mettre la plus grande probabilité à la fin de la liste plutôt qu'au début.<br>
<br>
Dans le cas ou vous ne voulez pas relire tout votre `PropMoverEx.inc` il faut d'appliquer cette operation sur votre `m_dropItems`.<br>

```cpp
float total_percent = m_DropItemGenerator.getTotalPercent(); // somme de tout les chance de drop un item contenue dans m_dropItems.
float too = (total * 100) / 3000000;

LOOP pour chaque item dans m_dropItems.
    m_dropItems.dwProbability *= too;
```
Et vous voila repassé sur 100 parts !

## Les chance de drops perturbent !

### Limite d'objets

Dans `propMoverEtc.inc`, pour chaque monstre il faut donner le nombre d'objets maximums que l'on peut dropper sur le monstre.

```
MI_AIBATT1
{
	Maxitem = 2;
	DropGold(6, 9);
    DropItem(II_GEN_GEM_GEM_TWINKLESTONE, 300000000, 0, 1);

}
```

Par exemple, on peut dropper au plus deux objets sur un Jeune Aibatt, il donne entre 6 et 9 gold et il a 100% de chances de dropper une Pierre Brillante. Ici comme il n'y a qu'un objet droppable, la limite ne peut pas être atteinte.

Le paramètre Maxitem est utilisé ici :
```cpp
BOOL CMover::DropItem( CMover* pAttacker )
{
    // ...
        if(  (DWORD)( nNumber ) >= lpMoverProp->m_DropItemGenerator.m_dwMax )
            break;
    // ...
}
```

Supposons que l'on rajoute ces deux objets.

```
	DropItem(II_GEN_MAT_ORICHALCUM01, 3006250, 0, 1);
	DropItem(II_GEN_MAT_MOONSTONE, 1560000, 0, 1);
```
La valeur `Maxitem = 2` signifie que si on a déjà droppé une Pierre Brillante et un Orichalque, le jeu s'arrête là et ne prend pas en compte le dernier objet (la Pierre Lunaire).


### Impact des chances de drop


Prenons par exemple les chance de drops suivant : <br>
MaxItem = 1<br>
BATON : 1500000000 => 50% <br>
ARC   : 1500000000 => 50% <br>

L'ARC a t'il vraiment 50% de chance d'être drop ?<br>
Non parce que pour obtenir l'arc, il faut ne pas obtenir le bâton (50% de chances) puis obtenir l'arc (50% de chances) ce qui ne fait en réalité que 25% de chances d'obtenir l'arc.


Maintenant ajoutons 20% de chance de drop !<br>
```cpp
DWORD dwRand = xRandom(3000000000);
dwRand = static_cast<DWORD>(dwRand / fProbability);
```
Imaginons quand dans le premier cas nous obtenons `1500000000`, on ajoute les 20% de drop => `1200000000`.<br>
On peut considérer soit que notre score chance diminue (donc augment dans les fait) soit que notre item passe de 50% à 70%.<br>

On pourrait se dire que la liste des items passe donc en : <br>
```
BATON : 1500000000 => 70%
ARC   : 1500000000 => 70%
```

Mais en réalité, les chances d'obtenir l'arc ont diminué : de 25% elles sont passées à 30% x 70% = 21% !<br>


Pour résoudre cela il faut donc prendre en compte la liste complète des items et la limite d'objets que l'on peut obtenir par monstre !<br>
[Une solution courante est de mélangée la liste des drops](https://gist.github.com/SPSquonK/1bcfbd6cd861541a3e31a5cd09d1ae93). Néanmoins cette solution est peu satisfaisante car si elle rend équiprobable chaque objet ayant les mêmes chances, elle rend fausse les probabilités écrites dans le fichier `propMoverEtc.inc`.<br>
A vos claviers ;)