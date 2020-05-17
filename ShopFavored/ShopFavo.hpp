#pragma once

#if defined(__SHOP_FAVORED)
#include <vector>

// Cette classe est un singleton permettant de mémoriser les joueurs ayant les
// boutiques marquées comme préférées par le joueur.
class CShapFavo
{
	std::vector<unsigned int>	m_vShop{};
	CShapFavo() = default;
	CShapFavo(const CShapFavo&) = delete;
	CShapFavo(CShapFavo&&) = delete;
	CShapFavo& operator=(const CShapFavo&) = delete;
	CShapFavo& operator=(CShapFavo&&) = delete;

public:
	static CShapFavo& GetInstance(void);
	void	Add(unsigned int id);
	void	Remove(unsigned int id);
	[[nodiscard]] bool	Find(unsigned int id) const;
};
#endif //__SHOP_FAVORED
