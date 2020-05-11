#pragma once

#if defined(__CALENDAR)

#include <unordered_map>
#include <vector>

class CCalendar
{
public:
	struct CalendarItem
	{
		DWORD	dwID = 0;
		DWORD	dwCount = 0;
#if defined(__WORLDSERVER)
		DWORD	dwIDBonus = 0;
		DWORD	dwCountBonus = 0;
#endif //__WORLDSERVER
#if defined(__CLIENT)
		bool	bSuccess = false;
		CTexture* m_pTextureItem = nullptr;
#endif //
		CalendarItem() = default;
		~CalendarItem() = default;
		CalendarItem(const CalendarItem& c) = default;
		CalendarItem(CalendarItem* c) : CalendarItem(*c) {};
	};
	std::map<unsigned int, std::string> m_mMonthConvert = {
		{1, "janvier"},
		{2, "fevrier"},
		{3, "mars"},
		{4, "avril"},
		{5, "mai"},
		{6, "juin"},
		{7, "juillet"},
		{8, "aout"},
		{9, "septembre"},
		{10, "octobre"},
		{11, "novembre"},
		{12, "decembre"}
	};

	static CCalendar&	GetInstance(void);
	~CCalendar() = default;

#if defined(__WORLDSERVER)
	bool	Load(void);
	const std::vector<CalendarItem>& getCalenderFromMonth(unsigned int) const;
	void	ApplyAward(CUser* pUser);
#endif //__WORLDSERVER

#if defined(__CLIENT)
	void	Add(DWORD dwID, DWORD dwCount);
	void	Clear(void);
	const	std::vector<CalendarItem>& GetAwards(void) const;
	void	SetMonth(unsigned int uDay);
	void	SetMonth(const char* szMonth);
	void	SetBonus(unsigned int uIDItem, unsigned int uCountItem);
	unsigned int	GetIDBonus(void) const;
	unsigned int	GetCountBonus(void) const;

	const std::string&	GetMonth(void) const;
	void	SuccessDay(unsigned int uDay);
#endif //__CLIENT

private:
	CCalendar() = default;
#if defined(__WORLDSERVER)
	std::unordered_map<char*, std::vector<CalendarItem>>	m_mAwards;
#endif //__WORLDSERVER

#if defined(__CLIENT)
	std::string m_szMonth = "unknow"; // represente les mois de l'annee. 1-12.
	std::vector<CalendarItem>	m_vAwards{};
	unsigned int	uIDBonus = 0;
	unsigned int	uCountBonus = 0;
#endif //__CLIENT
};

#endif //__CALENDAR