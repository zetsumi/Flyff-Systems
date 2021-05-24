#pragma once

#if defined(__TOWER_GUILD)

#define	TOWER_GUILD_CONFIG_FILE	"config_towerguild/tower_guild.json"

struct TWAward
{
	unsigned int	uItemId = 0;
	unsigned int	uCount = 0;

	TWAward(const TWAward&) = default;
	TWAward(TWAward&&) = default;
	TWAward& operator=(const TWAward&) = default;
};

struct TWMonster
{
	unsigned int	uMoverId = 0;
	D3DXVECTOR3		vPos{ 0,0,0 };
	unsigned int	uCount = 0;

	TWMonster(const TWMonster&) = default;
	TWMonster(TWMonster&&) = default;
	TWMonster& operator=(const TWMonster&) = default;
};

struct TWBoss
{
	unsigned int	uMoverId = 0;
	D3DXVECTOR3		vPos{ 0,0,0 };

	TWBoss(const TWBoss&) = default;
	TWBoss(TWBoss&&) = default;
	TWBoss& operator=(const TWBoss&) = default;
};

struct TWState
{
	std::vector<TWMonster>	vMonster{};
	std::vector<TWBoss>		vBoss{};
	unsigned int			uCountMonsterWave = 0;
	unsigned int			uCountBoss = 0;

	TWState() = default;
	TWState(const TWState&) = default;
	TWState(TWState&&) = default;
	TWState& operator=(const TWState&) = default;
};

struct TWStage
{
	unsigned int			uId = 0;
	unsigned int			uWorldId = 0;
	D3DXVECTOR3				vPosSpawn{ 0,0,0 };
	std::map<unsigned int, TWState>	mState{};
	std::vector<TWAward>	vAward{};

	TWStage() = default;
	TWStage(const TWStage&) = default;
	TWStage(TWStage&&) = default;
	TWStage& operator=(const TWStage&) = default;
};

struct TWGuild
{
	enum class TGSatte : unsigned short
	{
		WAVE,
		BOSS,
		FINISH
	};
	unsigned int	uStageId = 0;
	unsigned int	uCountPlayer = 0;
	std::vector<CUser*>	vUsers{};
	unsigned int	uWorldId = 0;
	unsigned int	uIndexState = 0;
	unsigned int	uWaveMonsterKilled = 0;
	unsigned int	uBossMonsterKilled = 0;
	TGSatte	eFlag = TGSatte::WAVE;

	TWGuild() = default;
	TWGuild(const TWGuild&) = default;
	TWGuild(TWGuild&&) = default;
	TWGuild& operator=(const TWGuild&) = default;
};

class CTowerGuild
{
	std::map<unsigned int, TWStage>	mStages{};
	std::map<unsigned int, TWGuild>	mGuilds{};

	CTowerGuild() = default;
	bool	Load(const std::string& filename);

#if defined(__WORLDSERVER)
	// CDP
	void	SpawnAwards(unsigned int uPlayerId, unsigned int uWorldId, unsigned int uLayer, unsigned int uStageId, const D3DXVECTOR3& vPos);

	void	SpawnBoss(unsigned int uWorldId, unsigned int uLayer, unsigned int uStageId, unsigned int uStateId);
	void	SpawnWave(unsigned int uWorldId, unsigned int uLayer, unsigned int uStageId, unsigned int uStateId);

	void    ProcessKillWave(CMover* pMover, CMover* pMonster);
	void    ProcessKillBoss(CMover* pMover, CMover* pMonster);
	void	ProcessFinish(CMover* pMover);

	void	SendStageState(TWGuild& twGuild, unsigned int uStageID, unsigned int uStateID);
	void	SendMoverKilled(TWGuild& twGuild, TWGuild::TGSatte state, unsigned int uCountKilled);
	void	SendFinishStage(TWGuild& twGuild, unsigned int uStageID);
#endif //__WORLDSERVER

public:
	unsigned int	uMaxIdStage = 0;
#if defined(__CLIENT)
	unsigned int		uCurrentStageId = 1;
	unsigned int		uCurrentIndexStateId = 0;
	unsigned int		uCurrentMonsterKilled = 0;
	std::string			szCurrentMaxMonster = "";
	std::string			szCurrentMaxState = "";
	std::string			szCurrentMaxStage = "";
	TWGuild::TGSatte	uCurrentStateFlag = TWGuild::TGSatte::WAVE;
#endif

	inline static CTowerGuild& Instance() { static CTowerGuild instance; return instance; }
	[[nodiscard]] bool	Load(void);
	[[nodiscard]] bool	IsInTowerGuild(CMover* pMover) const;
	[[noreturn]] void	ProcessKillMonster(CMover* pMover, CMover* pMonster);
#if defined(__CLIENT)
	[[nodiscard]] inline bool	IsInside(void) const
	{
		auto itStage = mStages.find(uCurrentStageId);
		if (itStage == mStages.end())
			return false;
		return itStage->second.uWorldId == g_pPlayer->GetWorld()->GetID();
	}
	[[nodiscard]] inline TWStage& GetStage(unsigned int uStageId) { return mStages[uStageId]; }
	[[nodiscard]] inline TWState& GetStateStage(unsigned int uStageId, unsigned int uStateId) { return mStages[uStageId].mState[uStateId]; }
	[[nodiscard]] inline size_t GetCountStage(void) const { return mStages.size(); }
#endif //__CLIENT

#if defined(__WORLDSERVER)
	void Enter(CUser* pUser);
	void Quit(CUser* pUser);

	void	InitializeStage(unsigned int GuildId, unsigned int uStageId);
#endif //__WORLDSERVER
};

#endif //#if defined(__TOWER_GUILD)
