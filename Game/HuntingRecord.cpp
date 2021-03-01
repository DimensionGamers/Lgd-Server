HuntingRecord::HuntingRecord()
{

}

HuntingRecord::~HuntingRecord()
{

}

void HuntingRecord::SetVisible(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, false, false))
	{
		return;
	}

	POINTER_PCT_CHECK(HUNTING_RECORD_VISIBLE, lpMsg, Packet, 0);

	pPlayer->SetHuntingRecordVisible(lpMsg->visible == 1);
}

void HuntingRecord::RecordRequest(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer, false))
	{
		return;
	}

	POINTER_PCT_LOG(HUNTING_RECORD_REQUEST, lpMsg, Packet, 0);

	Player* pTarget = sObjectMgr->FindPlayer(lpMsg->index);

	if (!pTarget || !pTarget->IsPlaying())
	{
		return;
	}

	if (pTarget != pPlayer && !pTarget->IsHuntingRecordVisible())
	{
		return;
	}

	if (lpMsg->index != pPlayer->GetEntry() && lpMsg->index != pPlayer->GetCurrentTargetPlayer())
	{
		return;
	}

	pPlayer->SetHuntingRecordTarget(lpMsg->index);
	pPlayer->SetHuntingRecordWorld(lpMsg->world);

	/*if ( pPlayer->IsHuntingRecordActive() )
	{
	return;
	}*/

	pPlayer->SetHuntingRecordActive(true);
	HuntingRecordData const* pData = nullptr;

	uint16 world_id = pTarget->GetWorldId();

	if (KUBERA_MAP_RANGE(world_id))
	{
		world_id = WORLD_KUBERA_MINE_1;
	}

	if (world_id == lpMsg->world)
	{
		this->SendCurrentDay(pPlayer);
		pData = this->GetCurrentRecord(pTarget);
	}
	else
	{
		HUNTING_RECORD_CURRENT_DAY pMsg;
		pPlayer->SEND_PCT(pMsg);
	}

	uint8 buffer[8192];
	POINTER_PCT(HUNTING_RECORD_HEAD, head, buffer, 0);
	POINTER_PCT(HUNTING_RECORD_BODY, body, buffer, sizeof(HUNTING_RECORD_HEAD));
	uint16 count = 0;
	head->result = pTarget->IsHuntingRecordVisible();

	HuntingRecordDataMap::const_iterator itr = pTarget->hunting_record_data.find(lpMsg->world);

	if (itr != pTarget->hunting_record_data.end())
	{
		for (HuntingRecordDataList::const_iterator it = itr->second.begin(); it != itr->second.end(); ++it)
		{
			if (lpMsg->world == world_id && pData)
			{
				if (pData->GetYear() == (*it)->GetYear() &&
					pData->GetMonth() == (*it)->GetMonth() &&
					pData->GetDay() == (*it)->GetDay())
				{
					continue;
				}
			}

			body[count].unk1 = 0;
			body[count].year = (*it)->GetYear();
			body[count].month = (*it)->GetMonth();
			body[count].day = (*it)->GetDay();
			body[count].level = (*it)->GetLevel();
			body[count].duration = (*it)->GetDuration();
			body[count].damage = (*it)->GetDamage();
			body[count].elemental_damage = (*it)->GetElementalDamage();
			body[count].healing = (*it)->GetHealing();
			body[count].killed_count = (*it)->GetKilledCount();
			Util::ConvertToByte((*it)->GetEarnedExperience(), body[count].experience);

			++count;
		}
	}

	head->countH = HIBYTE(count);
	head->countL = LOBYTE(count);

	head->h.set(0xEC, 0x50, sizeof(HUNTING_RECORD_HEAD) + (count * sizeof(HUNTING_RECORD_BODY)));
	pPlayer->sendPacket(buffer, head->h.get_size());
}

void HuntingRecord::Close(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	pPlayer->SetHuntingRecordActive(false);
	pPlayer->SetHuntingRecordTarget(-1);
}

void HuntingRecord::Start(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	pPlayer->GetCurrentHuntingData()->Reset();
}

void HuntingRecord::Update(Player* pPlayer, uint64 damage, uint64 elemental_damage, uint32 count, uint32 healing, uint64 experience)
{
	if ( !pPlayer )
	{
		return;
	}

	if ( !pPlayer->GetHelper()->IsStarted() )
	{
		return;
	}

	HuntingRecordData * pHuntingRecordData = this->GetCurrentRecord(pPlayer, true);

	if ( !pHuntingRecordData )
	{
		return;
	}

	pHuntingRecordData->IncreaseDamage(damage);
	pHuntingRecordData->IncreaseElementalDamage(elemental_damage);
	pHuntingRecordData->IncreaseKilledCount(count);
	pHuntingRecordData->IncreaseHealing(healing);
	pHuntingRecordData->IncreaseEarnedExperience(experience);
	pHuntingRecordData->SetLevel(pPlayer->GetTotalLevel());

	pPlayer->GetCurrentHuntingData()->IncreaseDamage(damage);
	pPlayer->GetCurrentHuntingData()->IncreaseElementalDamage(elemental_damage);
	pPlayer->GetCurrentHuntingData()->IncreaseKilledCount(count);
	pPlayer->GetCurrentHuntingData()->IncreaseHealing(healing);
	pPlayer->GetCurrentHuntingData()->IncreaseEarnedExperience(experience);
}

void HuntingRecord::Update(Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}

	if (pPlayer->GetHelper()->IsStarted())
	{
		HuntingRecordData * pHuntingRecordData = this->GetCurrentRecord(pPlayer, true);

		if (pHuntingRecordData)
		{
			pHuntingRecordData->IncreaseDuration(1);
			pHuntingRecordData->SetLevel(pPlayer->GetTotalLevel());
		}

		pPlayer->GetCurrentHuntingData()->IncreaseDuration(1);
	}

	this->SendCurrentDay(pPlayer);
	
	if (pPlayer->GetHelper()->IsStarted())
	{
		HUNTING_RECORD_TIME pMsg;
		pMsg.time = pPlayer->GetCurrentHuntingData()->GetDuration();
		pMsg.damage = pPlayer->GetCurrentHuntingData()->GetDamage();
		pMsg.elemental_damage = pPlayer->GetCurrentHuntingData()->GetElementalDamage();
		pMsg.healing = pPlayer->GetCurrentHuntingData()->GetHealing();
		pMsg.killed_count = pPlayer->GetCurrentHuntingData()->GetKilledCount();
		Util::ConvertToByte(pPlayer->GetCurrentHuntingData()->GetEarnedExperience(), pMsg.experience);

		pPlayer->SEND_PCT(pMsg);
	}
}

void HuntingRecord::SendCurrentDay(Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}

	if (!pPlayer->IsHuntingRecordActive())
	{
		return;
	}

	Player* pTarget = sObjectMgr->FindPlayer(pPlayer->GetHuntingRecordTarget());

	if (!pTarget || !pTarget->IsPlaying())
	{
		this->Close(pPlayer);
		return;
	}

	uint16 world_id = pTarget->GetWorldId();

	if (KUBERA_MAP_RANGE(world_id))
	{
		world_id = WORLD_KUBERA_MINE_1;
	}

	if (world_id != pPlayer->GetHuntingRecordWorld())
	{
		return;
	}
	
	HUNTING_RECORD_CURRENT_DAY pMsg;

	HuntingRecordData const* pHuntingRecordData = this->GetCurrentRecord(pTarget);

	if (pHuntingRecordData)
	{
		pMsg.id = 1;
		pMsg.year = pHuntingRecordData->GetYear();
		pMsg.month = pHuntingRecordData->GetMonth();
		pMsg.day = pHuntingRecordData->GetDay();
		pMsg.level = pHuntingRecordData->GetLevel();
		pMsg.duration = pHuntingRecordData->GetDuration();
		pMsg.damage = pHuntingRecordData->GetDamage();
		pMsg.elemental_damage = pHuntingRecordData->GetElementalDamage();
		pMsg.healing = pHuntingRecordData->GetHealing();
		pMsg.killed_count = pHuntingRecordData->GetKilledCount();
		Util::ConvertToByte(pHuntingRecordData->GetEarnedExperience(), pMsg.experience);
	}

	pPlayer->SEND_PCT(pMsg);
}

HuntingRecordData * HuntingRecord::GetCurrentRecord(Player* pPlayer, bool create)
{
	if ( !pPlayer )
	{
		return nullptr;
	}

	uint16 world_id = pPlayer->GetWorldId();

	if (KUBERA_MAP_RANGE(world_id))
	{
		world_id = WORLD_KUBERA_MINE_1;
	}

	HuntingRecordDataMap::iterator itr = pPlayer->hunting_record_data.find(world_id);
	Custom::SystemTimer cur_time;

	if ( itr != pPlayer->hunting_record_data.end() )
	{
		for ( HuntingRecordDataList::iterator it = itr->second.begin(); it != itr->second.end(); ++it )
		{
			if ( (*it)->GetYear() == cur_time.GetYear() && 
				 (*it)->GetMonth() == cur_time.GetMonth() &&
				 (*it)->GetDay() == cur_time.GetDay() )
			{
				return (*it);
			}
		}
	}

	if ( create )
	{
		HuntingRecordData * pData = new HuntingRecordData;
		pData->SetYear(cur_time.GetYear());
		pData->SetMonth(cur_time.GetMonth());
		pData->SetDay(cur_time.GetDay());
		pData->SetLevel(pPlayer->GetTotalLevel());
		pData->SetDuration(0);
		pData->SetDamage(0);
		pData->SetElementalDamage(0);
		pData->SetHealing(0);
		pData->SetKilledCount(0);
		pData->SetEarnedExperience(0);

		pPlayer->hunting_record_data[world_id].push_back(pData);
		return pData;
	}

	return nullptr;
}