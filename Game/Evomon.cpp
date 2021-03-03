Evomon::Evomon()
{
	this->SetActive(true);
}

Evomon::~Evomon()
{
	LIST_CLEAR(EvomonInfoList::iterator, this->evomon_list);
	LIST_CLEAR(EvomonRewardList::iterator, this->evomon_reward_list);
}

void Evomon::Load()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Evomon...");

	LIST_CLEAR(EvomonInfoList::iterator, this->evomon_list);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM evomon");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 i = 0;

			EvomonInfo * info = new EvomonInfo;

			info->SetID(fields[i++].GetUInt8());
			info->SetMonster(fields[i++].GetUInt16());
			info->SetLevel(fields[i++].GetUInt8());
			info->SetLife(fields[i++].GetInt32());
			info->SetAttackDamageMin(fields[i++].GetInt32());
			info->SetAttackDamageMax(fields[i++].GetInt32());
			info->SetAttackSuccessRate(fields[i++].GetInt32());
			info->SetDefense(fields[i++].GetInt32());
			info->SetDefenseSuccessRate(fields[i++].GetInt32());
			info->SetNextID(fields[i++].GetUInt8());
			info->SetNextIDRate(fields[i++].GetUInt16());
			info->SetSpecialRate(fields[i++].GetUInt16());
			info->SetSpecialMonster(fields[i++].GetUInt16());

			this->evomon_list.push_back(info);
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u evomon definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void Evomon::LoadReward()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Evomon Reward...");

	LIST_CLEAR(EvomonRewardList::iterator, this->evomon_reward_list);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM evomon_reward");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 i = 0;

			EvomonReward * info = new EvomonReward;

			info->SetScoreMin(fields[i++].GetInt32());
			info->SetScoreMax(fields[i++].GetInt32());
			info->SetRewardType(fields[i++].GetUInt8());
			info->SetRewardIndex(fields[i++].GetUInt16());
			info->SetRewardCount(fields[i++].GetInt32());

			this->evomon_reward_list.push_back(info);
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u evomon reward definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

EvomonInfo const* Evomon::GetEvomon(uint8 id, uint16 monster) const
{
	for ( EvomonInfoList::const_iterator it = this->evomon_list.begin(); it != this->evomon_list.end(); ++it )
	{
		if ( (*it)->GetID() == id && (*it)->GetMonster() == monster )
		{
			return (*it);
		}
	}

	return nullptr;
}

int32 Evomon::GetEvomonScore(uint8 id, uint16 monster) const
{
	EvomonInfo const* pEvomonInfo = this->GetEvomon(id, monster);

	if ( pEvomonInfo )
	{
		return pEvomonInfo->GetLevel();
	}
	else
	{
		return 0;
	}
}

uint8 Evomon::GetNextEvomon(uint8 id, uint16 monster) const
{
	EvomonInfo const* pEvomonInfo = this->GetEvomon(id, monster);

	if ( pEvomonInfo )
	{
		if (roll_chance_i(pEvomonInfo->GetNextIDRate(), 10000))
		{
			return pEvomonInfo->GetNextID();
		}
	}

	return -1;
}

uint8 Evomon::ScrollDrop(Player* pPlayer, Item const* pItem, uint8 x, uint8 y)
{
	if ( !pPlayer )
	{
		return 0;
	}

	if ( pItem->GetItem() != ITEMGET(14, 244) )
	{
		return 0;
	}

	if ( !sGameServer->IsEvomonEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Evomon is disabled.");
		return 1;
	}

	if ( !pPlayer->IsWorldFlag(WORLD_FLAG_ALLOW_SUMMON_SCROLL) )
	{
		this->ScrollDropResult(pPlayer, 4);
		return 1;
	}
	
	Monster* pPortal = pPlayer->GetEvomonPortal();
	TCType current_time = MyGetTickCount();

	if ( pPortal )
	{
		uint32 remain_time = pPlayer->GetEvomonTime() >= current_time ? pPlayer->GetEvomonTime() - current_time: 0;
		remain_time = (remain_time / IN_MILLISECONDS) / MINUTE;

		this->ScrollDropResult(pPlayer, 1, pPortal->GetWorldId(), pPortal->GetX(), pPortal->GetY(), remain_time);
		return 1;
	}
	/*if ( pPlayer->IsEvomon() )
	{
		this->ScrollDropResult(pPlayer, 1, pPlayer->GetEvomonWorld(), pPlayer->GetEvomonX(), pPlayer->GetEvomonY());
		return 1;
	}*/

	if ( pPlayer->IsInSafeZone() )
	{
		//pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't be used in the safe zone.");
		this->ScrollDropResult(pPlayer, 4);
		return 1;
	}

	Party* pParty = pPlayer->GetParty();

	if ( pParty )
	{
		if ( pParty->IsEvomonSummoned() )
		{
			this->ScrollDropResult(pPlayer, 3);
			//pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Party member already opened a portal.");
			return 1;
		}
	}
	
	Monster* pMonster = sObjectMgr->MonsterTryAdd(704, pPlayer->GetWorldId());

	if ( pMonster )
	{
		pMonster->SetWorldId(pPlayer->GetWorldId());
		pMonster->SetBasicLocation(pPlayer->GetX(), pPlayer->GetY(), pPlayer->GetX(), pPlayer->GetY());
		pMonster->SetInstance(pPlayer->GetInstance());
		pMonster->SetScriptName("evomon_portal_ai");
		pMonster->AddAdditionalDataInt(0, pPlayer->GetGUID());
		pMonster->AddAdditionalDataInt(1, 681);
		pMonster->AddAdditionalDataPtr(0, pPlayer);
		pMonster->AddToWorld();

		//pPlayer->SetEvomon(true);
		pPlayer->SetEvomonPortal(pMonster);
		pPlayer->SetEvomonTime(current_time + (30 * MINUTE * IN_MILLISECONDS));

		this->ScrollDropResult(pPlayer, 0);
		
		return 2;
	}
	
	return 1;
}

void Evomon::ScrollDropResult(Player* pPlayer, uint8 result, uint16 world, int16 x, int16 y, uint32 duration)
{
	if ( !pPlayer )
	{
		return;
	}

	EVOMON_RESULT pMsg;
	pMsg.result = result;
	pMsg.world = world;
	pMsg.x = x;
	pMsg.y = y;
	pMsg.remain_time = duration;
	
	pPlayer->SEND_PCT(pMsg);
}

void Evomon::SendScore(Player* pPlayer, uint8 result, int32 score)
{
	/*if ( !pPlayer )
	{
		return;
	}

	EVOMON_SCORE_DATA pMsg;
	pMsg.result = result;
	pMsg.score = score;

	pPlayer->SEND_PCT(pMsg);*/
}

void Evomon::GiveReward(Player* pPlayer, int32 score)
{
	if ( !pPlayer )
	{
		return;
	}

	EvomonReward const* pReward = nullptr;

	for ( EvomonRewardList::const_iterator it = this->evomon_reward_list.begin(); it != this->evomon_reward_list.end(); ++it )
	{
		if ( score >= (*it)->GetScoreMin() && score <= (*it)->GetScoreMax() )
		{
			pReward = *it;
			break;
		}
	}

	if ( !pReward )
	{
		return;
	}

	if ( sGameServer->IsEvomonPartyReward() )
	{
		Unit* pMember[MAX_PARTY_MEMBERS];
		uint8 count = 0;

		pPlayer->GetPartyMembers(pMember, count, 20);

		for ( int32 i = 0; i < count; ++i )
		{
			if ( !pMember[i] )
			{
				continue;
			}

			if ( Player* pMemberPlayer = pMember[i]->ToPlayer() )
			{
				pMemberPlayer->UpdateStatistic(STATISTIC_EVOMON_SCORE, score);

				sItemMgr->ItemSerialCreateGremoryCase(pMemberPlayer, Item(ITEMGET(14, 253)), GREMORY_CASE_TYPE_CHARACTER, GremoryCaseReward::EVOMON_REWARD);
			}
		}
	}
	else
	{
		pPlayer->UpdateStatistic(STATISTIC_EVOMON_SCORE, score);

		for ( int32 h = 0; h < pReward->GetRewardCount(); ++h )
		{
			sItemMgr->ItemSerialCreateGremoryCase(pPlayer, Item(ITEMGET(pReward->GetRewardType(), pReward->GetRewardIndex())), GREMORY_CASE_TYPE_CHARACTER, GremoryCaseReward::EVOMON_REWARD);
		}
	}
}

void Evomon::GiveSpecialReward(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	if ( sGameServer->IsEvomonPartySpecialReward() )
	{
		Unit* pMember[MAX_PARTY_MEMBERS];
		uint8 count = 0;

		pPlayer->GetPartyMembers(pMember, count, 20);

		for ( int32 i = 0; i < count; ++i )
		{
			if ( !pMember[i] )
			{
				continue;
			}

			if ( Player* pMemberPlayer = pMember[i]->ToPlayer() )
			{
				sItemMgr->ItemSerialCreateGremoryCase(pMemberPlayer, Item(ITEMGET(14, 253)), GREMORY_CASE_TYPE_CHARACTER, GremoryCaseReward::EVOMON_REWARD);
			}
		}
	}
	else
	{
		sItemMgr->ItemSerialCreateGremoryCase(pPlayer, Item(ITEMGET(14, 253)), GREMORY_CASE_TYPE_CHARACTER, GremoryCaseReward::EVOMON_REWARD);
	}
}