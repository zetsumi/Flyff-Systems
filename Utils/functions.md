# GetDlgItem
Crédit : [SquonK](https://gist.github.com/SPSquonK/59606a52ebec03204cf16e19665311c3)
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


# Fix syncrhonisation des DST
Les DST ne sont pas envoyer lors de la creation d'un mover.
Dans ___void	CMover::serializeStoringNPC(CAr& ar)___ :
Apres :
```cpp
	ar << (u_char)m_nMoveEvent;
	ar << m_nMoveEventCnt;
	ar << m_fSpeedFactor;
```
Ajouter :
```cpp
#if defined(__CLIENT) || defined(__WORLDSERVER)
	for (unsigned int i = 0; i < MAX_ADJPARAMARY; ++i)
		ar << m_adjParamAry[i];
	for (unsigned int i = 0; i < MAX_ADJPARAMARY; ++i)
		ar << m_chgParamAry[i];
#endif //defined(__CLIENT) || defined(__WORLDSERVER)
```

Dans la fonction ___void	CMover::serializeLoadingNPC(CAr& ar)___ :
Apres :
```cpp
#endif	// __CLIENT
	ar >> m_fSpeedFactor;
```
Ajouter :
```cpp
#if defined(__CLIENT) || defined(__WORLDSERVER)
	for (unsigned int i = 0; i < MAX_ADJPARAMARY; ++i)
		ar >> m_adjParamAry[i];
	for (unsigned int i = 0; i < MAX_ADJPARAMARY; ++i)
		ar >> m_chgParamAry[i];
#endif //defined(__CLIENT) || defined(__WORLDSERVER)
```

## CPP17
Assurez-vous de compiler en C++14 avant !<br>
La version de c++ 17 pose les problèmes suivant :<br>
* Suppression de`function objects` [std::unary_function](https://en.cppreference.com/w/cpp/utility/functional/unary_function) et [std::binary_function](https://en.cppreference.com/w/cpp/utility/functional/binary_function).<br>
* Intégration de [std::byte](https://en.cppreference.com/w/cpp/types/byte#:~:text=std%3A%3Abyte%20is%20a,is%20not%20an%20arithmetic%20type.).<br>
<br>

Le code de Flyff de base utilise `BYTE` soit `typedef unsigned char BYTE;` ainsi que `using namespace std;`.<br>
Afin de résoudre les déclarations ambigue entre `byte`et `BYTE` je recommande de supprimer toutes les références à `using namespace std;` vous allez devoir ajouter les `std::` devant tous les références aux objets/fonctions de la [stl](https://en.cppreference.com/w/).<br>
Exemple `vector<int>` devient `std::vector<int>` cette étape ne demande aucune connaissance particulière uniquement d'ajouter les préfixe `std::`.<br>
Pourquoi ne pas utilisé `using namespace` ? ==> [The Cherno : Why I don't "using namespace std"](https://www.youtube.com/watch?v=4NYC-VU-svE).<br>
<br>

Pour remplacer `std::unary_function` ou `std::binary_function` vous pouvez les remplacer par des [lambda](https://en.cppreference.com/w/cpp/language/lambda).<br>
Exemple :
```cpp
struct IsSameID : public std::binary_function< GH_Fntr_Info, OBJID, bool>
{
    bool operator()( const GH_Fntr_Info& kInfo, OBJID id_) const 
    {
        return (kInfo.objId == id_);
    }
};
```
```cpp
VECFurnitureIter finder = find_if( m_vecFntInfo.begin(), m_vecFntInfo.end(), std::bind2nd( IsSameID(), objID_ ) );
```
<br>

Supprimer la structure `struct IsSameID`.
Vous ne donc plus faire appelle à `IsSameID()` il faut donc ajouter le remplacer par une lambda :
```cpp
[objID_](const auto& it) { return it.objId == objID_; }
```
Ce qui devient donc :
```cpp
VECFurnitureIter finder = find_if(m_vecFntInfo.begin(), m_vecFntInfo.end(),
        [objID_](const auto& it) { return it.objId == objID_; }
    );
```
