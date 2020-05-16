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
	m_vShop.erase(std::remove(m_vShop.begin(), m_vShop.end(), id), m_vShop.end());
}

bool CShapFavo::Find(unsigned int id) const
{
	return std::find(m_vShop.begin(), m_vShop.end(), id) != m_vShop.end();
}
#endif //__SHOP_FAVORED