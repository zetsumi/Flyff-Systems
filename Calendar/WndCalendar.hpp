#pragma once

#if defined(__CALENDAR)

class CWndCalendar : public CWndNeuz
{
	CTexture* m_pTextureFrame = nullptr;
	CTexture* m_pTextureItemBonus = nullptr;

public:
	CWndCalendar() = default;
	~CWndCalendar() = default;

	BOOL	Initialize(CWndBase* pWndParent = NULL, DWORD nType = MB_OK) override;
	void	OnDraw(C2DRender* p2DRender) override;
	void	OnInitialUpdate() override;
	void	SetTitle(const std::string& szMonth);
};

#endif //__CALENDAR