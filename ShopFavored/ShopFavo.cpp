#include "stdafx.h"

#if defined(__SHOP_FAVORED)
#include "ShopFavo.hpp"

CShapFavo& CShapFavo::GetInstance(void)
{
	static CShapFavo instance;
	return std::ref(instance);
}

void CShapFavo::Add(unsigned int id)
{
	if (Find(id) == false)
		m_vShop.emplace_back(id);
}

void CShapFavo::Remove(unsigned int id)
{
	auto itID = std::find(m_vShop.begin(), m_vShop.end(), id);
	if (itID == m_vShop.end())
		return;
	m_vShop.erase(itID);
}

bool CShapFavo::Find(unsigned int id)
{
	return std::find(m_vShop.begin(), m_vShop.end(), id) != m_vShop.end();
}
#endif //__SHOP_FAVORED