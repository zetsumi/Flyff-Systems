#pragma once

class CWndAwakeCustom : public CWndNeuz
{
	bool			m_bAuto = true;
	CWndStatic*		m_pRectTextureItem = nullptr;
	CTexture*		m_pItemTexture = nullptr;
	CTexture*		m_pGoldTexture = nullptr;
	CWndListBox*	m_plbItemAwake = nullptr;
	CWndListBox*	m_plbListingAwake = nullptr;
	CWndListBox*	m_plbListAwakeValided = nullptr;
	CItemElem*		m_pItem = nullptr;

	void	UpdateListAwake(void);

public:
	CWndAwakeCustom();
	~CWndAwakeCustom();

	virtual BOOL	Initialize(CWndBase* pWndParent = NULL, DWORD nType = MB_OK);
	virtual BOOL	OnChildNotify(UINT message, UINT nID, LRESULT* pLResult);
	virtual void	OnDraw(C2DRender* p2DRender);
	virtual	void	OnInitialUpdate();
	virtual BOOL	OnCommand(UINT nID, DWORD dwMessage, CWndBase* pWndBase);
	virtual	BOOL	OnDropIcon(LPSHORTCUT pShortcut, CPoint point) override;
	void			UpdateTooltip(void);
	bool			IsValidConditions(void);
	void			ReBuild(void);
};