#include "stdafx.h"
#include "TowerGuild.hpp"
#include "LexerParser.h"
#include "World.h"
#include "worldmng.h"

#if defined(__WORLDSERVER)
#include "User.h"
#include "DPSrvr.h"
#include "DPDatabaseClient.h"
extern CWorldMng g_WorldMng;
extern	CDPDatabaseClient	g_dpDBClient;
#endif //__WORLDSERVER

#if defined(__CLIENT)
#include "DPClient.h"
extern CDPClient	g_DPlay;
#endif //__CLIENT


bool CTowerGuild::Load(const std::string& filename)
{
	picojson::value v;
	CLexerParser::GetInstance()->LoadJson(std::ref(v), filename.c_str());
	picojson::object root = v.get<picojson::object>();



	for (const auto& it : root)
	{
		TWStage	stage{};
		unsigned long ulIdStage = std::strtoul(it.first.c_str(), NULL, 10);
		picojson::object params = it.second.get<picojson::object>();
		stage.uId = ulIdStage;
		
#pragma region Config spawn dans le monde
		picojson::object world = params["world"].get<picojson::object>();
		const char* szIDWorld = world["id"].get<std::string>().c_str();
		unsigned long ulWorldId = CScript::m_defines[szIDWorld];
		stage.uWorldId = ulWorldId;

		picojson::object posSpawnPlayer = world["position"].get<picojson::object>();
		stage.vPosSpawn.x = static_cast<float>(posSpawnPlayer["x"].get<double>());
		stage.vPosSpawn.y = static_cast<float>(posSpawnPlayer["y"].get<double>());
		stage.vPosSpawn.z = static_cast<float>(posSpawnPlayer["z"].get<double>());
#pragma endregion

#pragma region State des etage
		picojson::object state = params["state"].get<picojson::object>();
		for (const auto& itState : state)
		{
			TWState twState{};
			unsigned long ulIdWave = std::strtoul(itState.first.c_str(), NULL, 10);

			picojson::object paramsState = itState.second.get<picojson::object>();
#pragma region wave monstre
			picojson::array aWave = paramsState["wave"].get<picojson::array>();
			for (const auto& itWave : aWave)
			{
				TWMonster twMonster{};
				picojson::object monster = itWave.get<picojson::object>();

				auto strMover = monster["id"].get<std::string>();
				const char* szMoverId = strMover.c_str();
				unsigned long ulMoverId = CScript::m_defines[szMoverId];
				twMonster.uMoverId = ulMoverId;

				double count = monster["count"].get<double>();
				twMonster.uCount = count;

				picojson::object posMover = monster["position"].get<picojson::object>();
				twMonster.vPos.x = static_cast<float>(posMover["x"].get<double>());
				twMonster.vPos.y = static_cast<float>(posMover["y"].get<double>());
				twMonster.vPos.z = static_cast<float>(posMover["z"].get<double>());

				twState.vMonster.emplace_back(twMonster);
				twState.uCountMonsterWave += count;
			}
#pragma endregion wave monstre

#pragma region boss
			picojson::array aBoss = paramsState["boss"].get<picojson::array>();
			for (const auto& itBoss : aBoss)
			{
				TWBoss twBoss{};
				picojson::object boos = itBoss.get<picojson::object>();

				auto strMover = boos["id"].get<std::string>();
				const char* szMoverId = strMover.c_str();
				unsigned long ulMoverId = CScript::m_defines[szMoverId];
				twBoss.uMoverId = ulMoverId;

				picojson::object posMover = boos["position"].get<picojson::object>();
				twBoss.vPos.x = static_cast<float>(posMover["x"].get<double>());
				twBoss.vPos.y = static_cast<float>(posMover["y"].get<double>());
				twBoss.vPos.z = static_cast<float>(posMover["z"].get<double>());
				
				++twState.uCountBoss;
				twState.vBoss.emplace_back(twBoss);
			}
#pragma endregion boss
			stage.mState[ulIdWave] = twState;
		}
#pragma endregion State des etage

#pragma region Vien on set les recompenses
		picojson::array aAwards = params["awards"].get<picojson::array>();
		for (const auto& itAwards : aAwards)
		{
			TWAward twAward{};
			picojson::object award = itAwards.get<picojson::object>();

			auto strID = award["id"].get<std::string>();
			const char* szIDItem = strID.c_str();
			unsigned long IdItem = CScript::m_defines[szIDItem];
			twAward.uItemId = IdItem;
			twAward.uCount = static_cast<unsigned int>(award["count"].get<double>());
			stage.vAward.emplace_back(twAward);
		}
#pragma endregion
		mStages[stage.uId] = stage;
		if (uMaxIdStage < stage.uId)
			uMaxIdStage = stage.uId;
	}

	return true;
}


bool CTowerGuild::Load(void)
{
	picojson::value v;
	CLexerParser::GetInstance()->LoadJson(std::ref(v), TOWER_GUILD_CONFIG_FILE);
	picojson::object root = v.get<picojson::object>();

	picojson::array listConfig = root["list"].get<picojson::array>();
	for (const auto& it : listConfig)
	{
		const std::string filename = it.get<std::string>();
		OUTPUTDEBUGSTRING("%s filename{%s}", __FUNCTION__, filename.c_str());
		if (Load(filename) == false)
			return false;
	}

	return true;
}


#if defined(__WORLDSERVER)
void CTowerGuild::SpawnAwards(unsigned int uPlayerId, unsigned int uWorldId, unsigned int uLayer, unsigned int uStageId, const D3DXVECTOR3& vPos)
{
	const TWStage& stage = mStages[uStageId];

	for (const TWAward& award : stage.vAward)
	{
		CItemElem* pItemElem = new CItemElem;
		pItemElem->m_dwItemId = award.uItemId;
		pItemElem->m_nItemNum = 1;
		pItemElem->m_nHitPoint = -1;
		pItemElem->SetSerialNumber();
		CItem* pItem = new CItem;
		pItem->m_pItemBase = pItemElem;
		pItem->m_idOwn = NULL_ID;
		pItem->m_dwDropTime = timeGetTime();
		pItem->m_bDropMob = TRUE;
		pItem->SetIndex(D3DDEVICE, pItem->m_pItemBase->m_dwItemId);
		pItem->SetPos(vPos);
		CWorld* pWorld = g_WorldMng.GetWorld(uWorldId);
		pWorld->ADDOBJ(pItem, TRUE, uLayer);
	}
}

void CTowerGuild::SpawnBoss(unsigned int uWorldId, unsigned int uLayer, unsigned int uStageId, unsigned int uStateId)
{
	CWorld* pWorld = g_WorldMng.GetWorld(uWorldId);
	if (pWorld == nullptr)
		return;
	auto vBoss = mStages[uStageId].mState[uStateId].vBoss;
	for (const auto& it : vBoss)
	{
		CObj* pObj = CreateObj(D3DDEVICE, OT_MOVER, it.uMoverId);
		if (pObj == nullptr)
			break;
		pObj->SetPos(it.vPos);
		pObj->InitMotion(MTI_STAND);
		pObj->UpdateLocalMatrix();
		((CMover*)pObj)->m_bActiveAttack = TRUE;
		pWorld->ADDOBJ(pObj, TRUE, uLayer);
	}
}

void CTowerGuild::SpawnWave(unsigned int uWorldId, unsigned int uLayer, unsigned int uStageId, unsigned int uStateId)
{
	CWorld* pWorld = g_WorldMng.GetWorld(uWorldId);
	if (pWorld == nullptr)
		return;
	auto vMonster = mStages[uStageId].mState[uStateId].vMonster;
	for (const auto& it : vMonster)
	{
		for (unsigned int i = 0; i < it.uCount; ++i)
		{
			CObj* pObj = CreateObj(D3DDEVICE, OT_MOVER, it.uMoverId);
			if (pObj == nullptr)
				break;
			pObj->SetPos(it.vPos);
			pObj->InitMotion(MTI_STAND);
			pObj->UpdateLocalMatrix();
			((CMover*)pObj)->m_bActiveAttack = TRUE;
			pWorld->ADDOBJ(pObj, TRUE, uLayer);
		}
	}
}

void CTowerGuild::ProcessKillWave(CMover* pMover, CMover* pMonster)
{
	TWGuild& twGuild = mGuilds[pMover->m_idGuild];
	TWStage stage = mStages[twGuild.uStageId];
	TWState state = stage.mState[twGuild.uIndexState];

	bool bExist = false;
	for (const auto& it : state.vMonster)
	{
		if (it.uMoverId == pMonster->GetIndex())
		{
			bExist = true;
			break;
		}
	}
	if (bExist == false)
		return;

	++twGuild.uWaveMonsterKilled;
	if (twGuild.uWaveMonsterKilled >= state.uCountMonsterWave)
	{
		twGuild.uWaveMonsterKilled = 0;
		twGuild.uBossMonsterKilled = 0;
		twGuild.eFlag = TWGuild::TGSatte::BOSS;
		SpawnBoss(stage.uWorldId, pMover->m_idGuild, twGuild.uStageId, twGuild.uIndexState);
	}
	SendMoverKilled(twGuild, twGuild.eFlag, twGuild.uWaveMonsterKilled);
}

void CTowerGuild::ProcessKillBoss(CMover* pMover, CMover* pMonster)
{
	TWGuild& twGuild = mGuilds[pMover->m_idGuild];
	TWStage stage = mStages[twGuild.uStageId];
	TWState state = stage.mState[twGuild.uIndexState];

	bool bExist = false;
	for (const auto& it : state.vBoss)
	{
		if (it.uMoverId == pMonster->GetIndex())
		{
			bExist = true;
			break;
		}
	}
	if (bExist == false)
		return;

	++twGuild.uBossMonsterKilled;
	if (twGuild.uBossMonsterKilled >= state.vBoss.size())
	{
		twGuild.uBossMonsterKilled = 0;

		if (twGuild.uIndexState >= stage.mState.size() - 1)
		{
			twGuild.uIndexState = 0;
			twGuild.eFlag = TWGuild::TGSatte::FINISH;
			SpawnAwards(pMover->GetId(), stage.uWorldId, pMover->m_idGuild, twGuild.uStageId, pMonster->GetPos());
			SendFinishStage(twGuild, twGuild.uStageId);
			ProcessFinish(pMover);
			return;
		}
		twGuild.eFlag = TWGuild::TGSatte::WAVE;
		twGuild.uWaveMonsterKilled = 0;
		++twGuild.uIndexState;
		SpawnWave(stage.uWorldId, pMover->m_idGuild, twGuild.uStageId, twGuild.uIndexState);
	}
	SendMoverKilled(twGuild, twGuild.eFlag, twGuild.uBossMonsterKilled);
}

void CTowerGuild::ProcessFinish(CMover* pMover)
{
	TWGuild& twGuild = mGuilds[pMover->m_idGuild];

	twGuild.eFlag = TWGuild::TGSatte::WAVE;
	twGuild.uBossMonsterKilled = 0;
	twGuild.uWaveMonsterKilled = 0;
	twGuild.uIndexState = 0;
	twGuild.uWorldId = 0;
	++twGuild.uStageId;
	pMover->GetGuild()->m_uTowerGuildStageId = twGuild.uStageId;
	g_dpDBClient.SendGuildSaveTowerGuild(pMover->m_idGuild, twGuild.uStageId);
}

void CTowerGuild::ProcessKillMonster(CMover* pMover, CMover* pMonster)
{
	TWGuild& twGuild = mGuilds[pMover->m_idGuild];

	switch (twGuild.eFlag)
	{
	case TWGuild::TGSatte::WAVE: ProcessKillWave(pMover, pMonster); break;
	case TWGuild::TGSatte::BOSS: ProcessKillBoss(pMover, pMonster); break;
	default: break;
	}
	SendStageState(twGuild, twGuild.uStageId, twGuild.uIndexState);
}

bool CTowerGuild::IsInTowerGuild(CMover* pMover) const
{
	if (IsInvalidObj(pMover) == TRUE || pMover->m_idGuild == 0)
		return false;

	auto it = mGuilds.find(pMover->m_idGuild);
	if (it == mGuilds.end())
		return false;
	auto itU = std::find(it->second.vUsers.begin(), it->second.vUsers.end(), pMover);
	if (itU == it->second.vUsers.end())
		return false;
	return true;
}

void CTowerGuild::Enter(CUser* pUser)
{
	if (IsInvalidObj(pUser))
		return;
	CGuild* pGuild = pUser->GetGuild();
	if (pGuild == nullptr || pUser->m_idGuild == 0)
		return;

	auto itStage = mStages.find(pGuild->m_uTowerGuildStageId);
	if (itStage == mStages.end())
		return;

	unsigned int uWorldId = itStage->second.uWorldId;
	D3DXVECTOR3 vPos = itStage->second.vPosSpawn;
	CWorld* pWorld = g_WorldMng.GetWorld(uWorldId);
	if (pWorld == nullptr)
		return;

	unsigned int uGuildId = pUser->m_idGuild;
	if (pWorld->m_linkMap.GetLinkMap(uGuildId) == nullptr)
	{
		if (pWorld->CreateLayer(static_cast<int>(uGuildId)) == FALSE)
			return;
	}

	auto it = mGuilds.find(pUser->m_idGuild);
	if (it == mGuilds.end())
	{
		mGuilds[pUser->m_idGuild] = TWGuild();
		it = mGuilds.find(pUser->m_idGuild);
	}
	else
	{
		if (std::find_if(it->second.vUsers.begin(), it->second.vUsers.end(), [pUser](const CMover* it) {return pUser == it; })
			!= it->second.vUsers.end())
			return;
	}
	pUser->SetMarkingPos();
	if (pUser->Replace(g_uIdofMulti, uWorldId, vPos, REPLACE_NORMAL, pUser->m_idGuild) == TRUE)
	{
		it->second.uStageId = pGuild->m_uTowerGuildStageId;
		it->second.uWorldId = uWorldId;
		it->second.vUsers.emplace_back(pUser);
		if (it->second.vUsers.size() == 1)
			InitializeStage(pUser->m_idGuild, pGuild->m_uTowerGuildStageId);
		const TWGuild& twGuild = mGuilds[pUser->m_idGuild];
		pUser->SendStageStateTowerGuild(pGuild->m_uTowerGuildStageId, twGuild.uIndexState, static_cast<unsigned short>(twGuild.eFlag));
	}
}

void CTowerGuild::Quit(CUser* pUser)
{
	pUser->Replace(g_uIdofMulti, pUser->m_idMarkingWorld, pUser->m_vMarkingPos, REPLACE_FORCE, 0);
	auto it = mGuilds.find(pUser->m_idGuild);
	if (it == mGuilds.end())
		return;

	it->second.vUsers.erase(std::find(it->second.vUsers.begin(), it->second.vUsers.end(), pUser));

	if (it->second.vUsers.size() == 0)
	{
		CWorld* pWorld = g_WorldMng.GetWorld(it->second.uWorldId);
		if (pWorld == nullptr)
			return;
		pWorld->Invalidate(static_cast<int>(pUser->m_idGuild));
		mGuilds.erase(pUser->m_idGuild);
	}

}

void CTowerGuild::InitializeStage(unsigned int uGuildId, unsigned int uStageId)
{
	TWGuild twGuild = mGuilds[uGuildId];
	SpawnWave(twGuild.uWorldId, uGuildId, uStageId, 0);
}


// NETWORK
void CTowerGuild::SendStageState(TWGuild& twGuild, unsigned int uStageID, unsigned int uStateID)
{
	for (CUser* pUser : twGuild.vUsers)
		if (IsValidObj(pUser))
			pUser->SendStageStateTowerGuild(uStageID, uStateID, static_cast<unsigned short>(twGuild.eFlag));
}

void CTowerGuild::SendMoverKilled(TWGuild& twGuild, TWGuild::TGSatte state, unsigned int uCountKilled)
{
	for (CUser* pUser : twGuild.vUsers)
		if (IsValidObj(pUser))
			pUser->SendMonsterKilledTowerGuild(state, uCountKilled);
}

void CTowerGuild::SendFinishStage(TWGuild& twGuild, unsigned int uStageID)
{
	for (CUser* pUser : twGuild.vUsers)
		if (IsValidObj(pUser))
			pUser->SendFinishStageTowerGuild(uStageID);
}

void CDPSrvr::OnEnterTowerGuild(CAr& ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long)
{
	CUser* pUser = g_UserMng.GetUser(dpidCache, dpidUser);
	CTowerGuild::Instance().Enter(pUser);
}

void CDPSrvr::OnQuitTowerGuild(CAr& ar, DPID dpidCache, DPID dpidUser, LPBYTE, u_long)
{
	CUser* pUser = g_UserMng.GetUser(dpidCache, dpidUser);
	CTowerGuild::Instance().Quit(pUser);
}

void CUser::SendStageStateTowerGuild(unsigned int uStageID, unsigned int uStateID, unsigned short uStateFlag)
{
	if (IsDelete())	return;
	m_Snapshot.cb++;
	m_Snapshot.ar << GetId();
	m_Snapshot.ar << SNAPSHOTTYPE_TOWER_GUILD_STAGE_STATE;
	m_Snapshot.ar << uStageID << uStateID << uStateFlag;
}

void CUser::SendMonsterKilledTowerGuild(TWGuild::TGSatte state, unsigned int uCountKilled)
{
	if (IsDelete())	return;
	m_Snapshot.cb++;
	m_Snapshot.ar << GetId();
	m_Snapshot.ar << SNAPSHOTTYPE_TOWER_GUILD_MOMSTER_KILLED;
	m_Snapshot.ar << static_cast<unsigned short>(state) << uCountKilled;

}

void CUser::SendFinishStageTowerGuild(unsigned int uStageId)
{
	if (IsDelete())	return;
	m_Snapshot.cb++;
	m_Snapshot.ar << GetId();
	m_Snapshot.ar << SNAPSHOTTYPE_TOWER_GUILD_FINISH_STAGE;
	m_Snapshot.ar << uStageId;
}

void CDPDatabaseClient::SendGuildSaveTowerGuild(unsigned int uIdGuild, unsigned int uStageId)
{
	BEFORESENDDUAL(ar, PACKETTYPE_TG_SAVE, DPID_UNKNOWN, DPID_UNKNOWN);
	ar << uIdGuild;
	ar << uStageId;
	SEND(ar, this, DPID_SERVERPLAYER);
}
#endif //__WORLDSERVER


#if defined(__CLIENT)
void CDPClient::OnStageStateTowerGuild(OBJID objid, CAr& ar)
{
	unsigned int uStageID = 0;
	unsigned int uIndexStateID = 0;
	unsigned short tmp = 0;

	ar >> uStageID;
	ar >> uIndexStateID;
	ar >> tmp;

	auto pGuild = g_pPlayer->GetGuild();

	if (pGuild->m_uTowerGuildStageId < CTowerGuild::Instance().uMaxIdStage
		&& pGuild->m_uTowerGuildStageId != uStageID)
	{
		pGuild->m_uTowerGuildStageId = uStageID;
	}
	TWStage& stage = CTowerGuild::Instance().GetStage(uStageID);
	TWState& state = CTowerGuild::Instance().GetStateStage(uStageID, uIndexStateID);

	CTowerGuild::Instance().szCurrentMaxStage.assign(std::to_string(CTowerGuild::Instance().GetCountStage()));
	CTowerGuild::Instance().uCurrentStateFlag = static_cast<TWGuild::TGSatte>(tmp);
	CTowerGuild::Instance().uCurrentStageId = uStageID;
	CTowerGuild::Instance().uCurrentIndexStateId = uIndexStateID;
	CTowerGuild::Instance().szCurrentMaxState = std::to_string(stage.mState.size());
	if (CTowerGuild::Instance().uCurrentStateFlag == TWGuild::TGSatte::WAVE)
		CTowerGuild::Instance().szCurrentMaxMonster = std::to_string(state.uCountMonsterWave);
	else if (CTowerGuild::Instance().uCurrentStateFlag == TWGuild::TGSatte::BOSS)
		CTowerGuild::Instance().szCurrentMaxMonster = std::to_string(state.uCountBoss);
}

void CDPClient::OnKilledMonsterTowerGuild(OBJID objid, CAr& ar)
{
	unsigned short tmp = 0;
	unsigned int uCountKilled = 0;

	ar >> tmp;
	ar >> uCountKilled;
	CTowerGuild::Instance().uCurrentMonsterKilled = uCountKilled;
	CTowerGuild::Instance().uCurrentStateFlag = static_cast<TWGuild::TGSatte>(tmp);

	CString msg;
	msg.Format("state{%u} uCountKilled{%u}", tmp, uCountKilled);
	g_WndMng.PutString(msg);

}

void CDPClient::OnFinishStageTowerGuild(OBJID objid, CAr& ar)
{
	unsigned int uStageId = 0;
	ar >> uStageId;

	CString msg;
	msg.Format("Finish Stage{%u}", uStageId);
	g_WndMng.PutString(msg);
}
#endif //__CLIENT

