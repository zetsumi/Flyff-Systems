#include "stdafx.h"

#if defined(__CALENDAR)
#include "Calendar.hpp"
#include "LexerParser.h"

#if defined(__WORLDSERVER)
#include "User.h"
#include "DPDatabaseClient.h"

extern CDPDatabaseClient g_dpDBClient;
#endif //__WORLDSERVER

CCalendar& CCalendar::GetInstance(void)
{
	static CCalendar instance;
	return std::ref(instance);
}

#if defined(__WORLDSERVER)
bool	CCalendar::Load()
{
	CLexerParser* l = CLexerParser::GetInstance();

	picojson::value v;
	l->LoadJson(v, "calendar.json");
	picojson::object& root = v.get<picojson::object>();

	for (auto month : root)
	{
		char* szMonth = (char*)calloc(10, sizeof(char));
		memcpy_s(szMonth, 10, month.first.c_str(), 10);
		szMonth[9] = '\0';
		m_mAwards[szMonth] = std::vector<CalendarItem>();

		picojson::object& oItems = month.second.get<picojson::object>();
		for (unsigned int uDay = 1; uDay < 31; ++uDay)
		{
			auto ito = oItems.find(std::to_string(uDay));
			if (ito == oItems.end())
				break;
			auto itItem = (*ito);
			CalendarItem ci;
			picojson::object& infos = itItem.second.get<picojson::object>();
			const char* name = infos["item"].get<std::string>().c_str();
			ci.dwID = CScript::m_defines[name];
			ci.dwCount = static_cast<DWORD>(infos["count"].get<double>());

			if (infos.find("item_bonus") != infos.end())
			{
				const char* szNameItemBonus = infos["item_bonus"].get<std::string>().c_str();
				ci.dwIDBonus = CScript::m_defines[szNameItemBonus];
				ci.dwCountBonus = static_cast<unsigned int>(infos["count_bonus"].get<double>());
			}
			m_mAwards[szMonth].push_back(ci);

		}

	}
	return true;
}

const std::vector<CCalendar::CalendarItem>& CCalendar::getCalenderFromMonth(unsigned int uMonth) const
{
	std::string szMonth = m_mMonthConvert.at(uMonth);
	for (auto it : m_mAwards)
	{
		if (strcmp(it.first, szMonth.c_str()) == 0)
		{
			auto itm = m_mAwards.find(it.first);
			return std::ref((*itm).second);
		}
	}
	return std::forward<std::vector<CCalendar::CalendarItem>>(std::vector<CCalendar::CalendarItem>());
}

void CCalendar::ApplyAward(CUser* pUser)
{
	if (IsInvalidObj(pUser))
		return;

	time_t now = time(0);
	tm* ltm = localtime(&now);
	unsigned int uMonth = static_cast<unsigned int>(ltm->tm_mon + 1);

	bool bNeedGift = false;

	if (pUser->m_mmAwardsCalendar.find(uMonth) == pUser->m_mmAwardsCalendar.end())
	{
		bNeedGift = true;
		pUser->m_mmAwardsCalendar[uMonth] = std::map<unsigned int, bool>();
	}

	if (pUser->m_mmAwardsCalendar[uMonth].find(ltm->tm_mday) == pUser->m_mmAwardsCalendar[uMonth].end())
	{
		pUser->m_mmAwardsCalendar[uMonth][ltm->tm_mday] = true;
		auto calendar = getCalenderFromMonth(uMonth);
		auto item = calendar.at(ltm->tm_mday);
		CItemElem itemElem;
		itemElem.m_dwItemId = item.dwID;
		itemElem.m_nItemNum = item.dwCount;
		itemElem.SetSerialNumber();
		g_dpDBClient.SendQueryPostMail(pUser->m_idPlayer, 0, std::ref(itemElem),
			0, "calendar gift", "nice !");
		g_dpDBClient.SendCalendarDaySuccess(pUser->m_idPlayer, uMonth, ltm->tm_mday);
		pUser->SendCalendarDaySuccess(uMonth, ltm->tm_mday);

		unsigned int uDaySuccess = pUser->m_mmAwardsCalendar[uMonth].size();
		unsigned int uDayMonthTotal = getCalenderFromMonth(uMonth).size();
		if (uDaySuccess == uDayMonthTotal)
		{
			CItemElem itemElemBonus;
			itemElemBonus.m_dwItemId = item.dwIDBonus;
			itemElemBonus.m_nItemNum = item.dwCountBonus;
			itemElemBonus.SetSerialNumber();
			g_dpDBClient.SendQueryPostMail(pUser->m_idPlayer, 0, std::ref(itemElemBonus),
				0, "calendar gift bonus", "nice !");
		}
	}
}

#endif //__WORLDSERVER

#if defined(__CLIENT)
void CCalendar::Add(DWORD dwID, DWORD dwCount)
{
	CalendarItem ci;
	ci.dwID = dwID;
	ci.dwCount = dwCount;
	ItemProp* pItemProp = prj.GetItemProp(dwID);
	if (pItemProp != nullptr)
	{
		ci.m_pTextureItem = CWndBase::m_textureMng.AddTexture(
			g_Neuz.m_pd3dDevice,
			MakePath(DIR_ITEM, pItemProp->szIcon),
			0xffff00ff);
	}
	m_vAwards.push_back(ci);
}

void	CCalendar::Clear(void)
{
	m_vAwards.clear();
}

const std::vector<CCalendar::CalendarItem>& CCalendar::GetAwards(void) const
{
	return std::ref(m_vAwards);
}

void CCalendar::SetMonth(unsigned int uMonth)
{
	std::string szMonth = m_mMonthConvert.at(uMonth);
	m_szMonth.clear();
	m_szMonth.assign(szMonth);
}

void CCalendar::SetMonth(const char* szMonth)
{
	m_szMonth.clear();
	m_szMonth.assign(szMonth);
}

void CCalendar::SetBonus(unsigned int uIDItem, unsigned int uCountItem)
{
	uIDBonus = uIDItem;
	uCountBonus = uCountItem;
}

unsigned int CCalendar::GetIDBonus(void) const
{
	return uIDBonus;
}

unsigned int CCalendar::GetCountBonus(void) const
{
	return uCountBonus;
}

const std::string& CCalendar::GetMonth(void) const
{
	return std::ref(m_szMonth);
}

void CCalendar::SuccessDay(unsigned int uDay)
{
	m_vAwards.at(uDay - 1).bSuccess = true;
}
#endif //__CLIENT

#endif//__CALENDAR