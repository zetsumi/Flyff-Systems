#pragma once

#if defined(__SHOP_FAVORED)
#include <vector>

class CShapFavo
{
	std::vector<unsigned int>	m_vShop{};
	CShapFavo() = default;
	CShapFavo(const CShapFavo*) = delete;
	CShapFavo(const CShapFavo&) = delete;
	CShapFavo(CShapFavo&&) = delete;
	CShapFavo& operator=(const CShapFavo&) = delete;
	CShapFavo& operator=(CShapFavo&&) = delete;

public:
	static CShapFavo& GetInstance(void);
	void	Add(unsigned int id);
	void	Remove(unsigned int id);
	bool	Find(unsigned int id);
};
#endif //__SHOP_FAVORED
