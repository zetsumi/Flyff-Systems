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