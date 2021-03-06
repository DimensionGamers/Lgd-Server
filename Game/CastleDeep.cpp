CastleDeep::CastleDeep()
{

}

CastleDeep::~CastleDeep()
{
	MAP_CLEAR(CastleDeepStageMap::iterator, this->stage_map);
}

void CastleDeep::Load()
{
	MAP_CLEAR(CastleDeepStageMap::iterator, this->stage_map);

	sLog->outInfo(LOG_DEFAULT, "Loading Castle Deep Settings...");

	ReadScript file("EventData/CastleDeep.txt", "rb");

	if ( !file.open() )
	{
		file.LogOpenError();
		return;
	}

	while ( true )
	{
		if ( file.GetToken() == END )
			break;

		uint8 section = file.TokenNumber;

		while ( true )
		{
			file.GetToken();

			if ( file.IsEnd("end") )
				break;

			switch ( section )
			{
			case 0:
				{
					int32 stage = file.GetInt32();
					file.GetToken();	uint16 rate = file.GetUInt16();

					CastleDeepStageMap::const_iterator it = this->stage_map.find(stage);

					if ( it == this->stage_map.end() && rate > 0 )
					{
						CastleDeepStage* pData = new CastleDeepStage;
						pData->SetStage(stage);
						pData->SetRate(rate);

						this->stage_map[pData->GetStage()] = pData;
					}
				} break;
			}
		}
	}

	sLog->outInfo(LOG_DEFAULT, "Castle Deep Settings Loaded");
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CastleDeep::Start(int32 notify)
{
	if ( !sGameServer->IsCastleDeepEnabled() )
	{
		return;
	}

	if ( sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START )
	{
		return;
	}

	if ( this->GetState() != CASTLE_DEEP_STATE_NONE )
	{
		return;
	}

	if ( this->stage_map.empty() )
	{
		return;
	}

	this->GetTime()->Start(notify * IN_MILLISECONDS);

	if ( notify > 0 )
	{
		this->SetState_Notify();
	}
	else
	{
		this->SetState_Playing();
	}
}

void CastleDeep::Update()
{
	switch ( this->GetState() )
	{
	case CASTLE_DEEP_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case CASTLE_DEEP_STATE_NOTIFY:
		{
			this->ProcState_Notify();
		} break;

	case CASTLE_DEEP_STATE_PLAYING:
		{
			this->ProcState_Playing();
		} break;
	}
}

void CastleDeep::SetState_None()
{
	this->SetState(CASTLE_DEEP_STATE_NONE);
	this->GetTime()->Reset();
	this->SetStage(-1);
	this->SetCurrentMinute(-1);
}

void CastleDeep::SetState_Notify()
{
	this->SetState(CASTLE_DEEP_STATE_NOTIFY);
	this->SetStage(-1);
	this->SetCurrentMinute(-1);
}

void CastleDeep::SetState_Playing()
{
	this->SetState(CASTLE_DEEP_STATE_PLAYING);
	this->GetTime()->Reset();
	this->SetCurrentMinute(-1);
	this->GetRandomStage();
	this->AddMonster();

	sServerLink->NoticeSendNormal(NOTICE_GLOBAL, "Loren Deep Event has started.");
}

void CastleDeep::ProcState_None()
{

}

void CastleDeep::ProcState_Notify()
{
	this->SendRemainMinutes("Loren Deep Event will start after %d minute(s)", 5, 30);

	if ( this->GetTime()->Elapsed() )
	{
		this->SetState_Playing();
	}
}

void CastleDeep::ProcState_Playing()
{
	if ( this->GetTime()->Elapsed() )
	{
		sObjectMgr->SendNoticeToAllNormal(NOTICE_GLOBAL, "You are the hero that will help rid the MU continent of Kundun's evil forces.God Bless you!");

		this->SetState_None();
	}
}

void CastleDeep::GetRandomStage()
{
	RandomValue<int32> m_RandomValue(-1);

	for ( CastleDeepStageMap::const_iterator it = this->stage_map.begin(); it != this->stage_map.end(); ++it )
	{
		m_RandomValue.AddValue(it->second->GetStage(), it->second->GetRate());
	}

	this->SetStage(m_RandomValue.GetRandomValue(RANDOM_POOL_RATE));
}

void CastleDeep::AddMonster()
{
	uint32 delay = 0;

	auto event_monsters = sMonsterManager->GetEventMonsters(EVENT_CASTLE_DEEP);
	for (auto itr = event_monsters.first; itr != event_monsters.second; ++itr)
	{
		auto const& event_monster = itr->second;

		if (event_monster->castle_deep.stage != GetStage())
		{
			continue;
		}

		auto monster = sObjectMgr->MonsterTryAdd(event_monster->MonsterId, event_monster->MapId);
		if (monster)
		{
			monster->SetEventDBData(event_monster);
			monster->AddAdditionalDataInt(0, event_monster->castle_deep.type);
			monster->SetDespawnTime(event_monster->castle_deep.despawn_time * IN_MILLISECONDS);
			monster->SetDespawnType(MONSTER_DESPAWN_DIE);

			if (event_monster->castle_deep.type == 1 || event_monster->castle_deep.type == 2)
				monster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
			else
				monster->SetRespawnType(GAME_OBJECT_RESPAWN_NORMAL);

			monster->AddToWorld();

			if (event_monster->SpawnDelay > delay)
				delay = event_monster->SpawnDelay;
		}
	}

	this->GetTime()->Start((delay + (5 * MINUTE)) * IN_MILLISECONDS);
}

void CastleDeep::SendRemainMinutes(std::string const& message, int32 period, int32 max_minute)
{
	if ( !this->IsMinuteElapsed(period) )
	{
		return;
	}

	if ( max_minute != -1 && (this->GetCurrentMinute() + 1) > max_minute )
	{
		return;
	}

	sServerLink->NoticeSend(NOTICE_GLOBAL, message.c_str(), this->GetCurrentMinute() + 1);
}