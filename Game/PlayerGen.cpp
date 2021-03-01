/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerGen.cpp"
*
*/

void Player::GenJoinRequest(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		return;
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	if ( this->GetInterfaceState()->GetID() != InterfaceData::QuestSupportGiver )
	{
		sLog->outError(LOG_GENS, "GenJoinRequest() [%s][%s] - Wrong Interface: %s", 
			this->GetAccountData()->GetAccount(), this->GetName(), this->GetInterfaceState()->GetID().c_str());
		return;
	}

	POINTER_PCT_LOG(GEN_JOIN_REQUEST, lpMsg, Packet, 0);

	if ( lpMsg->family != GEN_DUPRIAN && lpMsg->family != GEN_VANERT )
	{
		sLog->outError(LOG_GENS, "GenJoinRequest() [%s][%s] - Wrong Gen Family: %d", 
			this->GetAccountData()->GetAccount(), this->GetName(), lpMsg->family);
		return;
	}

	if ( this->GetGen()->GetFamily() == lpMsg->family )
	{
		this->GenJoinResult(1);
		return;
	}

	if ( this->GetGen()->IsFamily() )
	{
		this->GenRewardResult(5);
		return;
	}

	if ( this->GetTotalLevel() < sGameServer->GetGenJoinMinLevel() )
	{
		this->GenJoinResult(3);
		return;
	}

	if ( (sGameServer->GetGenChangeTime() + this->GetGen()->GetLeftDate()) > time(nullptr) )
	{
		this->GenJoinResult(2);
		return;
	}

	if ( sGameServer->guild_join_gens.get() && this->GuildGet() )
	{
		this->GenJoinResult(5);
		return;
	}

	if ( !sGameServer->party_allow_dif_gens && this->GetPartyID() != PARTY_NULL )
	{
		this->GenJoinResult(6);
		return;
	}

	if ( sGameServer->guild_join_gens.get() && (this->IsUseGuildMatching() || this->IsUseGuildMatchingJoin()) )
	{
		this->GenJoinResult(8);
		return;
	}

	/*if ( !sGameServer->party_allow_dif_gens && (sPartyMatching->IsPartyMatching(this->GetGUID()) || sPartyMatching->IsPartyMatchingJoin(this->GetGUID())) )
	{
		this->GenJoinResult(8);
		return;
	}*/

	this->GetGen()->Reset();
	this->GetGen()->SetFamily(lpMsg->family);
	this->GetGen()->SetLevel(GEN_LEVEL_PRIVATE);
	this->GetGen()->SetJoinDate(time(nullptr));

	this->GenJoinResult(0, lpMsg->family);
	this->GenSendInfo(false, true);

	SQLTransaction trans = MuDatabase.BeginTransaction();
	this->SaveDBGen(trans);
	MuDatabase.CommitTransaction(trans);

	this->UpdateQuestMULevel();

	sLog->outInfo(LOG_GENS, "%s - Joined Family: %d", this->BuildLog().c_str(), lpMsg->family);
}

void Player::GenJoinResult(uint8 result, uint8 family)
{
	GEN_JOIN_RESULT pMsg(result, family);
	this->SEND_PCT(pMsg);
}

void Player::GenDeleteRequest()
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		return;
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	if ( this->GetInterfaceState()->GetID() != InterfaceData::QuestSupportGiver || 
		 !this->GetInterfaceState()->GetTarget() ||
		 !this->GetInterfaceState()->GetTarget()->IsCreature() ||
		 !this->GetInterfaceState()->GetTarget()->ToCreature()->IsNpc() )
	{
		sLog->outError(LOG_GENS, "GenDeleteRequest() [%s][%s] - Wrong Interface: %s", 
			this->GetAccountData()->GetAccount(), this->GetName(), this->GetInterfaceState()->GetID().c_str());
		return;
	}

	if ( !this->GetGen()->IsFamily() )
	{
		this->GenDeleteResult(1);
		return;
	}

	Monster* mMonster = this->GetInterfaceState()->GetTarget()->ToCreature();
	
	if ( mMonster->GetGen()->GetFamily() != this->GetGen()->GetFamily() )
	{
		this->GenDeleteResult(3);
		return;
	}

	if ( sGameServer->guild_join_gens.get() && this->GuildGet() )
	{
		this->GenDeleteResult(2);
		return;
	}

	if ( !sGameServer->party_allow_dif_gens && this->GetPartyID() != PARTY_NULL )
	{
		this->GenDeleteResult(2);
		return;
	}

	if ( sGameServer->guild_join_gens.get() && (this->IsUseGuildMatching() || this->IsUseGuildMatchingJoin()) )
	{
		this->GenDeleteResult(4);
		return;
	}

	/*if ( !sGameServer->party_allow_dif_gens && (sPartyMatching->IsPartyMatchingJoin(this->GetGUID())) )
	{
		this->GenDeleteResult(4);
		return;
	}*/

	this->QuestMURemoveGens();

	this->GetGen()->Reset();
	this->GetGen()->SetLevel(GEN_LEVEL_NONE);
	this->GetGen()->SetLeftDate(time(nullptr));
	this->m_gen_victim_map.clear();

	this->GenDeleteResult(0);
	this->GenSendInfo(false, true);

	SQLTransaction trans = MuDatabase.BeginTransaction();

	this->SaveDBGen(trans);
	this->SaveDBGenKill(trans);
	this->SaveDBQuestMU(trans);

	MuDatabase.CommitTransaction(trans);

	sLog->outInfo(LOG_GENS, "%s - Left Family", this->BuildLog().c_str());
}

void Player::GenDeleteResult(uint8 result)
{
	GEN_DELETE_RESULT pMsg(result);
	this->SEND_PCT(pMsg);
}
	
void Player::GenRewardRequest(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		return;
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	if ( this->GetInterfaceState()->GetID() != InterfaceData::QuestSupportGiver )
	{
		sLog->outError(LOG_GENS, "GenRewardRequest() [%s][%s] - Wrong Interface: %s", 
			this->GetAccountData()->GetAccount(), this->GetName(), this->GetInterfaceState()->GetID().c_str());
		return;
	}

	POINTER_PCT_LOG(GEN_REWARD_REQUEST, lpMsg, Packet, 0);

	if ( lpMsg->family != GEN_DUPRIAN && lpMsg->family != GEN_VANERT )
	{
		sLog->outError(LOG_GENS, "GenRewardRequest() [%s][%s] - Wrong Gen Family: %d", 
			this->GetAccountData()->GetAccount(), this->GetName(), lpMsg->family);
		return;
	}

	if ( !this->GetGen()->IsFamily() )
	{
		this->GenRewardResult(6);
		return;
	}

	if ( this->GetGen()->GetFamily() != lpMsg->family )
	{
		this->GenRewardResult(5);
		return;
	}

	if ( Custom::SystemTimer().GetDayOfWeek() < MONDAY || Custom::SystemTimer().GetDayOfWeek() > FRIDAY )
	{
		this->GenRewardResult(1);
		return;
	}

	if ( this->GetGen()->GetRewardDate() >= sGenMgr->GetRewardDate() )
	{
		this->GenRewardResult(4);
		return;
	}

	GenRewardData const* reward = sGenMgr->GetReward(this->GetGen()->GetFamily(), this->GetGen()->GetLevel());

	if ( !reward )
	{
		this->GenRewardResult(2);
		return;
	}

	if ( !this->GenSatisfyGetReward(reward) )
	{
		this->GenRewardResult(3);
		return;
	}

	this->GetGen()->SetRewardDate(time(nullptr));
	this->GenRewardResult(0);

	sLog->outInfo(LOG_GENS, "%s - Rewarded", this->BuildLog().c_str());
}

void Player::GenRewardResult(uint8 result)
{
	GEN_REWARD_RESULT pMsg(result);
	this->SEND_PCT(pMsg);
}
	
void Player::GenInfoRequest()
{
	if (!this->Object::IsPlaying())
	{
		return;
	}

	if (!this->GetGen()->IsFamily())
	{
		return;
	}

	this->GenSendInfo(true, false);
}

bool Player::IsGenPVP(Unit* pVictim) const
{
	if (!this->IsWorldFlag(WORLD_FLAG_BATTLE_ZONE))
	{
		return false;
	}

	if (!pVictim)
	{
		return false;
	}

	if (!pVictim->IsPlayer())
	{
		return false;
	}

	return (this->GetGen()->GetFamily() != pVictim->GetGen()->GetFamily() && this->GetGen()->IsFamily() && pVictim->GetGen()->IsFamily());
}

bool Player::GenSatisfyGetReward(GenRewardData const* reward)
{
	switch (reward->GetType())
	{
	case GEN_REWARD_ITEM:
		{
			this->GetInventory()->TemporaryStart();

			for (uint32 n = 0; n < reward->item.count; ++n)
			{
				if (!this->GetInventory()->TemporaryItemAdd(Item(reward->GetItem())))
				{
					this->GetInventory()->TemporaryEnd();
					return false;
				}
			}

			this->GetInventory()->TemporaryEnd();

			for (uint32 n = 0; n < reward->item.count; ++n)
			{
				sItemMgr->ItemSerialCreateItem(this, serial_create_inventory, Item(reward->GetItem(), cast(uint8, reward->item.level)));
			}

			return true;
		} break;

	case GEN_REWARD_ZEN:
		{
			if (this->MoneyReachMaximum(reward->zen.value))
			{
				this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You have reached maximum zen");
				return false;
			}

			this->MoneyAdd(reward->zen.value);
			return true;
		} break;

	case GEN_REWARD_EXP:
		{
			int64 exp = reward->experience.value;

			this->GiveExperience(exp, true, true);
			return true;
		} break;

	case GEN_REWARD_CONTRIBUTION:
		{
			this->GenAddContribution(reward->contribution.value);
			return true;
		} break;
	}

	return false;
}

void Player::GenAddContribution(int32 value)
{
	if (value <= 0)
	{
		return;
	}

	this->GetGen()->IncreaseContribution(value);

	this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Gained %d contribution points", value);

	this->GenUpdateLevel();
}

void Player::GenDecContribution(int32 value)
{
	if (value <= 0 || this->GetGen()->GetContribution() <= 0)
	{
		return;
	}

	if (this->GetGen()->GetContribution() < value)
	{
		value = this->GetGen()->GetContribution();
	}

	this->GetGen()->ReduceContribution(value);
	this->GetGen()->LimitMinContribution(0);

	this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Lost %d contribution points", value);

	this->GenUpdateLevel();
}

void Player::GenUpdateLevel()
{
	uint8 current_level = this->GetGen()->GetLevel();

	this->GetGen()->SetLevel(sGenMgr->GetRankingLevel(this));

	if (this->GetGen()->GetLevel() > current_level)
	{
		this->GenSendInfo(true, true);
		this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Your Gens level has been decreased.");
	}
	else if (this->GetGen()->GetLevel() < current_level)
	{
		this->GenSendInfo(true, true);
		this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Congratulations. Your Gens level has been increased.");
	}
}

void Player::GenKillReward(Player* pVictim, int32 level, uint8 count)
{
	if (!this->IsGenPVP(pVictim) || !count || !level)
	{
		return;
	}

	if (this->GenApplySanction(pVictim))
	{
		return;
	}

	if (pVictim->GetGen()->GetContribution() <= 0)
	{
		return;
	}

	GenKillPoints const* pGenKillPoints = sGenMgr->GetKillPoints(level, pVictim->GetTotalLevel());

	if (pGenKillPoints)
	{
		this->GenAddContribution(pGenKillPoints->GetPointsAdd(count));
	}
}

void Player::GenDeathPenalty(int32 level, uint8 count)
{
	if (!level || !count)
	{
		return;
	}

	GenKillPoints const* pGenKillPoints = sGenMgr->GetKillPoints(this->GetTotalLevel(), level);

	if (pGenKillPoints)
	{
		this->GenDecContribution(pGenKillPoints->GetPointsDec(count));
	}
}

bool Player::GenApplySanction(Player* pVictim)
{
	if (!sGameServer->IsGenPenalization() || !pVictim)
		return false;

	GenVictimData & gen_victim = this->m_gen_victim_map[pVictim->GetGUID()];

	if (time(nullptr) < (gen_victim.last_hit.get() + sGameServer->GetGenPenalizationTime()))
	{
		gen_victim.count.op_inc();
	}
	else
	{
		gen_victim.count.set(1);
	}

	gen_victim.last_hit.set(time(nullptr));

	return gen_victim.count > sGameServer->GetGenPenalizationCount();
}

void Player::GenUpdate()
{
	if (!this->GetGen()->IsFamily())
	{
		return;
	}

	if (!this->GetTimer(PLAYER_TIMER_GENS_UPDATE)->Elapsed(20 * MINUTE * IN_MILLISECONDS))
	{
		return;
	}

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_SELECT_GEN);
	stmt->setUInt32(0, this->GetGUID());

	this->_genRankingCallBack = MuDatabase.AsyncQuery(stmt);
}

void Player::GenUpdateVictim()
{
	for (GenVictimDataMap::const_iterator it = this->m_gen_victim_map.begin(); it != this->m_gen_victim_map.end();)
	{
		if (time(nullptr) > (it->second.last_hit.get() + sGameServer->GetGenPenalizationTime()))
		{
			this->m_gen_victim_map.erase(it++);
			continue;
		}

		++it;
	}
}

void Player::GenSendInfo(bool info, bool viewport)
{
	if (info)
	{
		GEN_STATUS pMsg(this->GetGen()->GetFamily(), this->GetGen()->GetRanking(), this->GetGen()->GetLevel(), this->GetGen()->GetContribution(), 0);

		GenRankingData const* pGenRankingData = sGenMgr->GetRanking(this->GetGen()->GetFamily(), this->GetGen()->GetLevel());

		if (pGenRankingData && pGenRankingData->GetType() == GEN_RANKING_CONTRIBUTION)
		{
			pMsg.next_contribution = pGenRankingData->GetValue();
		}

		this->SEND_PCT(pMsg);
	}

	if (viewport)
	{
		VIEWPORT_CREATE_GEN_COMPLETE pMsg(this->GetGen()->GetFamily(), this->GetEntry(), this->GetGen()->GetLevel(), this->GetGen()->GetContribution());

		this->SEND_PCT(pMsg);

		if (this->IsWorldFlag(WORLD_FLAG_BATTLE_ZONE))
		{
			pMsg.family = 0;
		}

		if (this->SendViewportData())
		{
			this->SEND_PCT_VP(pMsg);
		}
	}
}