int32 ArkaWarStateTime[ARKA_WAR_STATE_MAX] = { 0, 0, 0, 600, 600, 180, 300, 1800, 600, 30, 30 };

ArkaWar::ArkaWar()
{
	this->ChangeState(ARKA_WAR_STATE_NONE, true);
}

ArkaWar::~ArkaWar()
{
	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		LIST_CLEAR(ArkaWarMonsterRespawnAreaList::iterator, this->monster_respawn_area[i]);
		LIST_CLEAR(ArkaWarPlayerPositionList::iterator, this->player_position_list[i]);
		this->monster_list[i].clear();
	}

	MAP_CLEAR(ArkaWarPlayerMap::iterator, this->m_ArkaWarPlayer);
	LIST_CLEAR(ArkaWarRewardExperienceList::iterator, this->reward_experience);
	LIST_CLEAR(ArkaWarExchangeDataList::iterator, this->exchange_list);
	MAP_CLEAR(ArkaWarObeliskDataMap::iterator, this->m_ArkaWarObeliskDataMap);
	MAP_CLEAR(ArkaWarBuffDataMap::iterator, this->m_ArkaWarBuffDataMap);
}

void ArkaWar::Load()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Arka War Setting...");

	ReadScript file("EventData/ArkaWar.txt", "rb");

	if ( !file.open() )
	{
		file.LogOpenError();
		return;
	}

	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		LIST_CLEAR(ArkaWarMonsterRespawnAreaList::iterator, this->monster_respawn_area[i]);
		LIST_CLEAR(ArkaWarPlayerPositionList::iterator, this->player_position_list[i]);
		this->monster_list[i].clear();
	}

	LIST_CLEAR(ArkaWarRewardExperienceList::iterator, this->reward_experience);
	LIST_CLEAR(ArkaWarExchangeDataList::iterator, this->exchange_list);
	MAP_CLEAR(ArkaWarObeliskDataMap::iterator, this->m_ArkaWarObeliskDataMap);
	MAP_CLEAR(ArkaWarBuffDataMap::iterator, this->m_ArkaWarBuffDataMap);

	for ( int32 i = 0; i < ARKA_WAR_STATE_MAX; ++i )
	{
		ArkaWarStateTime[i] = 0;
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
					uint8 state = file.GetUInt8();

					file.GetToken();	int32 duartion = file.GetInt32();

					if ( state < ARKA_WAR_STATE_MAX )
					{
						ArkaWarStateTime[state] = duartion;
					}
				} break;

			case 1:
				{
					uint8 id = file.GetUInt8();

					file.GetToken();	uint16 world = file.GetUInt16();
					file.GetToken();	int16 x = file.GetInt16();
					file.GetToken();	int16 y = file.GetInt16();
					file.GetToken();	int32 life = file.GetInt32();
					file.GetToken();	int32 occupy_life = file.GetInt32();

					if ( id < MAX_ARKA_WAR_ZONES )
					{
						this->GetObeliskBasicData(id)->SetWorld(world);
						this->GetObeliskBasicData(id)->SetX(x);
						this->GetObeliskBasicData(id)->SetY(y);
						this->GetObeliskBasicData(id)->SetLife(life);
						this->GetObeliskBasicData(id)->SetOccupyLife(occupy_life);
					}
				} break;

			case 2:
				{
					uint8 id = file.GetUInt8();
					file.GetToken();	uint8 aura_id = file.GetUInt8();

					file.GetToken();	uint16 world = file.GetUInt16();
					file.GetToken();	int16 x = file.GetInt16();
					file.GetToken();	int16 y = file.GetInt16();

					if ( id < MAX_ARKA_WAR_ZONES && aura_id < MAX_ARKA_WAR_AURA )
					{
						this->GetObeliskBasicData(id)->GetAuraBasicData(aura_id)->SetWorld(world);
						this->GetObeliskBasicData(id)->GetAuraBasicData(aura_id)->SetX(x);
						this->GetObeliskBasicData(id)->GetAuraBasicData(aura_id)->SetY(y);
					}
				} break;

			case 3:
				{
					uint8 id = file.GetUInt8();

					file.GetToken();	uint16 world = file.GetUInt16();
					file.GetToken();	int32 count = file.GetInt32();
					file.GetToken();	int32 respawn_time = file.GetInt32();

					if ( id < MAX_ARKA_WAR_ZONES )
					{
						this->GetMonsterInfo(id)->SetWorld(world);
						this->GetMonsterInfo(id)->SetCount(count);
						this->GetMonsterInfo(id)->SetRespawnTime(respawn_time);
					}
				} break;

			case 4:
				{
					uint8 id = file.GetUInt8();

					file.GetToken();	int16 x1 = file.GetInt16();
					file.GetToken();	int16 y1 = file.GetInt16();
					file.GetToken();	int16 x2 = file.GetInt16();
					file.GetToken();	int16 y2 = file.GetInt16();

					if ( id < MAX_ARKA_WAR_ZONES )
					{
						ArkaWarMonsterRespawnArea * add_area = new ArkaWarMonsterRespawnArea;
						add_area->SetX1(x1);
						add_area->SetY1(y1);
						add_area->SetX2(x2);
						add_area->SetY2(y2);

						this->monster_respawn_area[id].push_back(add_area);
					}
				} break;

			case 5:
				{
					uint8 id = file.GetUInt8();

					file.GetToken();	uint16 monster = file.GetUInt16();

					if ( id < MAX_ARKA_WAR_ZONES )
					{
						this->monster_list[id].push_back(monster);
					}
				} break;

			case 6:
				{
					uint8 id = file.GetUInt8();

					file.GetToken();	uint16 world = file.GetUInt16();
					file.GetToken();	int16 x1 = file.GetInt16();
					file.GetToken();	int16 y1 = file.GetInt16();
					file.GetToken();	int16 x2 = file.GetInt16();
					file.GetToken();	int16 y2 = file.GetInt16();

					if ( id < MAX_ARKA_WAR_ZONES )
					{
						ArkaWarPlayerPosition * add_position = new ArkaWarPlayerPosition;
						add_position->SetWorld(world);
						add_position->SetX1(x1);
						add_position->SetY1(y1);
						add_position->SetX2(x2);
						add_position->SetY2(y2);

						this->player_position_list[id].push_back(add_position);
					}
				} break;

			case 7:
				{
					uint32 kill_points_min = file.GetUInt32();

					file.GetToken();	uint32 kill_points_max = file.GetUInt32();
					file.GetToken();	int32 experience = file.GetInt32();

					ArkaWarRewardExperience * add_reward = new ArkaWarRewardExperience;
					add_reward->SetKillPointsMin(kill_points_min);
					add_reward->SetKillPointsMax(kill_points_max);
					add_reward->SetExperience(experience);

					this->reward_experience.push_back(add_reward);
				} break;

			case 8:
				{
					int32 trophy_min = file.GetInt32();

					file.GetToken();	int32 trophy_max = file.GetInt32();
					file.GetToken();	int32 success_rate = file.GetInt32();
					file.GetToken();	int32 add_success_rate = file.GetInt32();
					file.GetToken();	std::string itembag = file.TokenString;

					ArkaWarExchangeData * add_data = new ArkaWarExchangeData;
					add_data->SetTrophyMin(trophy_min);
					add_data->SetTrophyMax(trophy_max);
					add_data->SetSuccessRate(success_rate);
					add_data->SetAddSuccessRate(add_success_rate);
					add_data->SetItemBag(itembag);
					
					this->exchange_list.push_back(add_data);
				} break;

			case 9:
				{
					ArkaWarObeliskData* pData = new ArkaWarObeliskData();

					pData->SetID(file.GetUInt8());

					file.GetToken();	pData->SetMonster(file.GetUInt16());
					file.GetToken();	pData->SetElementalAttribute(file.GetUInt8());

					this->m_ArkaWarObeliskDataMap[pData->GetID()] = pData;
				} break;

			case 10:
				{
					ArkaWarBuffData * pData = new ArkaWarBuffData();

					pData->SetID(file.GetUInt8());

					file.GetToken();	pData->SetBuff(file.GetUInt16());
					file.GetToken();	pData->SetElementalAttribute(file.GetUInt8());

					this->m_ArkaWarBuffDataMap[pData->GetID()] = pData;
				} break;
			}
		}
	}

	sLog->outInfo(LOG_DEFAULT, "Arka War Settings Loaded");
	sLog->outInfo(LOG_DEFAULT, " ");
}

void ArkaWar::Start()
{
	if ( !sGameServer->IsArkaWarEnabled() )
	{
		return;
	}

	if ( this->GetState() != ARKA_WAR_STATE_NONE )
	{
		return;
	}

	this->ChangeState(ARKA_WAR_STATE_CLOSED);
}

void ArkaWar::Update()
{
	if ( !sGameServer->IsArkaWarEnabled() )
	{
		return;
	}

	switch ( this->GetState() )
	{
	case ARKA_WAR_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case ARKA_WAR_STATE_CLOSED:
		{
			this->ProcState_Closed();
		} break;

	case ARKA_WAR_STATE_MASTER_REGISTER:
		{
			this->ProcState_MasterRegister();
		} break;

	case ARKA_WAR_STATE_MEMBER_REGISTER:
		{
			this->ProcState_MemberRegister();
		} break;

	case ARKA_WAR_STATE_READY:
		{
			this->ProcState_Ready();
		} break;

	case ARKA_WAR_STATE_STANDBY:
		{
			this->ProcState_Standby();
		} break;

	case ARKA_WAR_STATE_PLAYING:
		{
			this->ProcState_Playing();
		} break;

	case ARKA_WAR_STATE_RESULT:
		{
			this->ProcState_Result();
		} break;

	case ARKA_WAR_STATE_CLOSE:
		{
			this->ProcState_Close();
		} break;

	case ARKA_WAR_STATE_END:
		{
			this->ProcState_End();
		} break;
	}
}

void ArkaWar::ChangeState(uint8 state, bool from_server_link)
{
	if ( state < ARKA_WAR_STATE_NONE || state > ARKA_WAR_STATE_END )
	{
		state = ARKA_WAR_STATE_NONE;
	}

	this->SetState(state);

	if ( sGameServer->IsArkaWarEnabled() )
	{
		this->GetTime()->Start(ArkaWarStateTime[this->GetState()] * IN_MILLISECONDS);

		this->SendStateAll();

		if ( state == ARKA_WAR_STATE_STANDBY || state == ARKA_WAR_STATE_PLAYING )
		{
			this->InitCharacterCount();
		}
	}

	this->SetCurrentNotifyTime(0);
	this->GetNotifyTime()->Reset();
	this->GetPlayNotifyTime()->Reset();
	
	switch ( this->GetState() )
	{
	case ARKA_WAR_STATE_NONE:
		{
			this->SetState_None();
		} break;

	case ARKA_WAR_STATE_CLOSED:
		{
			this->SetState_Closed();
		} break;

	case ARKA_WAR_STATE_MASTER_REGISTER:
		{
			this->SetState_MasterRegister();
		} break;

	case ARKA_WAR_STATE_MEMBER_REGISTER:
		{
			this->SetState_MemberRegister();
		} break;

	case ARKA_WAR_STATE_READY:
		{
			this->SetState_Ready();
		} break;

	case ARKA_WAR_STATE_STANDBY:
		{
			this->SetState_Standby();
		} break;

	case ARKA_WAR_STATE_PLAYING:
		{
			this->SetState_Playing();
		} break;

	case ARKA_WAR_STATE_RESULT:
		{
			this->SetState_Result();
		} break;

	case ARKA_WAR_STATE_CLOSE:
		{
			this->SetState_Close();
		} break;

	case ARKA_WAR_STATE_END:
		{
			this->SetState_End();
		} break;
	}

	if ( !from_server_link )
	{
		sServerLink->EventStateUpdate(EVENT_ARKA_WAR, this->GetState(), 0);
	}
}

void ArkaWar::SetState_None()
{
	MAP_CLEAR(ArkaWarPlayerMap::iterator, this->m_ArkaWarPlayer);

	sLog->outInfo("arkawar", "%s", __FUNCTION__);
}

void ArkaWar::SetState_Closed()
{
	sLog->outInfo("arkawar", "%s", __FUNCTION__);
}

void ArkaWar::SetState_MasterRegister()
{
	sLog->outInfo("arkawar", "%s", __FUNCTION__);
}

void ArkaWar::SetState_MemberRegister()
{
	sLog->outInfo("arkawar", "%s", __FUNCTION__);
}

void ArkaWar::SetState_Ready()
{
	sLog->outInfo("arkawar", "%s", __FUNCTION__);
}

void ArkaWar::SetState_Standby()
{
	sLog->outInfo("arkawar", "%s", __FUNCTION__);
}

void ArkaWar::SetState_Playing()
{
	sLog->outInfo("arkawar", "%s", __FUNCTION__);
}

void ArkaWar::SetState_End()
{
	sLog->outInfo("arkawar", "%s", __FUNCTION__);
}

void ArkaWar::SetState_Result()
{
	sLog->outInfo("arkawar", "%s", __FUNCTION__);
}

void ArkaWar::SetState_Close()
{
	sLog->outInfo("arkawar", "%s", __FUNCTION__);
}

void ArkaWar::ProcState_None()
{
	
}

void ArkaWar::ProcState_Closed()
{
	if ( this->GetTime()->Finished() )
	{
		sServerLink->ArkaWarClear();
		
		this->ChangeState(ARKA_WAR_STATE_MASTER_REGISTER);
	}
}

void ArkaWar::ProcState_MasterRegister()
{
	int32 remain_minutes = this->GetTime()->GetRemain() / (MINUTE * IN_MILLISECONDS);

	if ( this->GetCurrentNotifyTime() != remain_minutes )
	{
		this->SetCurrentNotifyTime(remain_minutes);

		sServerLink->NoticeSend(NOTICE_GLOBAL, "%d minutes left to register as Guild Master for the Arca War", remain_minutes + 1);
	}

	if ( this->GetTime()->Finished() )
	{
		this->ChangeState(ARKA_WAR_STATE_MEMBER_REGISTER);
	}
}

void ArkaWar::ProcState_MemberRegister()
{
	int32 remain_minutes = this->GetTime()->GetRemain() / (MINUTE * IN_MILLISECONDS);

	if ( this->GetCurrentNotifyTime() != remain_minutes )
	{
		this->SetCurrentNotifyTime(remain_minutes);

		sServerLink->NoticeSend(NOTICE_GLOBAL, "%d minutes left to register as Guild member for the Arca War", remain_minutes + 1);
	}

	if ( this->GetTime()->Finished() )
	{
		this->ChangeState(ARKA_WAR_STATE_READY);
		sServerLink->ArkaWarList();
	}
}

void ArkaWar::ProcState_Ready()
{
	int32 remain_seconds = this->GetTime()->GetRemain() / IN_MILLISECONDS;

	if ( this->GetCurrentNotifyTime() != remain_seconds )
	{
		this->SetCurrentNotifyTime(remain_seconds);

		sServerLink->NoticeSend(NOTICE_GLOBAL, "%d seconds left until the Arca War starts. Please wait in a safe area.", remain_seconds + 1);
	}

	if ( this->GetTime()->Finished() )
	{
		sServerLink->NoticeSend(NOTICE_GLOBAL, "The Arca War has started. You can enter by talking to Sir Lesnar.");

		this->ChangeState(ARKA_WAR_STATE_STANDBY);
		this->SetRandomObeliskAttribute();
	}
}

void ArkaWar::ProcState_Standby()
{
	if ( this->GetNotifyTime()->Elapsed(MINUTE * IN_MILLISECONDS) )
	{
		sServerLink->NoticeSend(NOTICE_GLOBAL, "The Arca War has started. You can enter by talking to Sir Lesnar.");
	}

	if ( (this->GetTime()->GetRemain() <= 30000) && (this->GetTime()->GetRemain() >= 0) )
	{
		int32 remain_seconds = this->GetTime()->GetRemain() / IN_MILLISECONDS;

		if ( this->GetCurrentNotifyTime() != remain_seconds )
		{
			this->SetCurrentNotifyTime(remain_seconds);

			this->SendNotifyText(12, remain_seconds + 1);
		}
	}
	else
	{
		int32 remain_minutes = this->GetTime()->GetRemain() / (MINUTE * IN_MILLISECONDS);

		if ( this->GetCurrentNotifyTime() != remain_minutes )
		{
			this->SetCurrentNotifyTime(remain_minutes);

			this->SendNotifyText(11, remain_minutes + 1);
		}
	}

	if ( this->GetPlayNotifyTime()->Elapsed(IN_MILLISECONDS) )
	{
	}

	if ( this->GetPlayerPositionTime()->Elapsed(10 * IN_MILLISECONDS) )
	{
		this->SendPlayerPosition();
	}

	if ( this->GetTime()->Finished() )
	{
		this->ChangeState(ARKA_WAR_STATE_PLAYING);
		this->SetObelisk();
		this->AddMonster();
	}
}

void ArkaWar::ProcState_Playing()
{
	if ( this->GetNotifyTime()->Elapsed(3 * MINUTE * IN_MILLISECONDS) )
	{
		sServerLink->NoticeSend(NOTICE_GLOBAL, "The Arca War has started. You can enter by talking to Sir Lesnar.");
	}

	if ( this->GetTime()->GetRemain() <= 60000 && this->GetTime()->GetRemain() >= 0 )
	{
		int32 remian_seconds = this->GetTime()->GetRemain() / IN_MILLISECONDS;

		if ( this->GetCurrentNotifyTime() != remian_seconds )
		{
			this->SetCurrentNotifyTime(remian_seconds);

			this->SendNotifyText(7, remian_seconds + 1);
		}
	}
	else
	{
		int32 remain_minutes = this->GetTime()->GetRemain() / (MINUTE * IN_MILLISECONDS);

		if ( this->GetCurrentNotifyTime() != remain_minutes )
		{
			this->SetCurrentNotifyTime(remain_minutes);

			this->SendNotifyText(6, remain_minutes + 1);
		}
	}

	if ( this->GetPlayNotifyTime()->Elapsed(IN_MILLISECONDS) )
	{
		this->CheckObeliskAuraPlayer();
		//this->SendObeliskLife();
		this->SendObeliskState();
	}

	if ( this->GetPlayerPositionTime()->Elapsed(10 * IN_MILLISECONDS) )
	{
		this->SendPlayerPosition();

		this->UpdateCharacterCount();
	}

	if ( this->GetTime()->Finished() )
	{
		sServerLink->NoticeSend(NOTICE_GLOBAL, "Exit the Arca War.");

		this->ChangeState(ARKA_WAR_STATE_RESULT);
	}
}

void ArkaWar::ProcState_End()
{
	
}

void ArkaWar::ProcState_Result()
{
	if ( this->GetTime()->GetRemain() > 0 )
	{
		int32 remian_seconds = this->GetTime()->GetRemain() / IN_MILLISECONDS;

		if ( this->GetCurrentNotifyTime() != remian_seconds )
		{
			this->SetCurrentNotifyTime(remian_seconds);

			this->SendNotifyText(8, remian_seconds + 1);
		}
	}

	if ( this->GetTime()->Finished() )
	{
		PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
		for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
		{
			Player* pPlayer = it->second;

			if ( pPlayer && pPlayer->IsPlaying() && AW_MAP_RANGE(pPlayer->GetWorldId()) )
			{
				pPlayer->TeleportToGate(426);
			}
		}

		this->SendResult();

		sServerLink->ArkaWarGuildSave();

		this->InsertRanking();

		this->ChangeState(ARKA_WAR_STATE_CLOSE);
	}
}

void ArkaWar::ProcState_Close()
{
	if ( (this->GetTime()->GetRemain() <= 30000) && (this->GetTime()->GetRemain() >= 0) )
	{
		int32 remian_seconds = this->GetTime()->GetRemain() / IN_MILLISECONDS;

		if ( this->GetCurrentNotifyTime() != remian_seconds )
		{
			this->SetCurrentNotifyTime(remian_seconds);

			this->SendNotifyText(10, remian_seconds + 1);
		}
	}
	else
	{
		int32 remain_minutes = this->GetTime()->GetRemain() / (MINUTE * IN_MILLISECONDS);

		if ( this->GetCurrentNotifyTime() != remain_minutes )
		{
			this->SetCurrentNotifyTime(remain_minutes);

			this->SendNotifyText(9, remain_minutes + 1);
		}
	}

	if ( this->GetTime()->Finished() )
	{
		PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
		for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
		{
			Player* pPlayer = it->second;

			if ( pPlayer && pPlayer->IsPlaying() && AW_MAP_RANGE(pPlayer->GetWorldId()) )
			{
				pPlayer->TeleportToGate(17);
			}
		}

		this->ChangeState(ARKA_WAR_STATE_NONE);
	}
}

void ArkaWar::MasterRegisterRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( !Player::InterfaceCheck(pPlayer, InterfaceData::ArcaBattleLesnar, 0) )
	{
		return;
	}

	if ( this->GetState() != ARKA_WAR_STATE_MASTER_REGISTER )
	{
		this->MasterRegisterResult(pPlayer, ARKA_WAR_RESULT_MASTER_REGISTER_PERIOD_ENDED);
		return;
	}

	Guild* pGuild = pPlayer->GuildGet();

	if ( !pGuild )
	{
		this->MasterRegisterResult(pPlayer, ARKA_WAR_RESULT_MASTER_REGISTER_ONLY_GUILD_MASTER);
		return;
	}

	if ( pGuild->GetID() != pPlayer->GetGUID() )
	{
		this->MasterRegisterResult(pPlayer, ARKA_WAR_RESULT_MASTER_REGISTER_ONLY_GUILD_MASTER);
		return;
	}

	if ( pGuild->GetTotalMembers() < sGameServer->GetArkaWarRegisterMinMembers() )
	{
		this->MasterRegisterResult(pPlayer, ARKA_WAR_RESULT_MASTER_REGISTER_LOW_GUILD_MEMBERS);
		return;
	}
	
	sServerLink->ArkaWarMasterRegister(pPlayer, pPlayer->GetGuildID());
}

void ArkaWar::MasterRegisterResult(Player* pPlayer, uint8 result)
{
	ARKA_WAR_MASTER_REGISTER_RESULT pMsg(result);
	pPlayer->SEND_PCT(pMsg);
}
	
void ArkaWar::MemberRegisterRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( !Player::InterfaceCheck(pPlayer, InterfaceData::ArcaBattleLesnar, 0) )
	{
		return;
	}

	if ( this->GetState() != ARKA_WAR_STATE_MEMBER_REGISTER )
	{
		this->MemberRegisterResult(pPlayer, ARKA_WAR_RESULT_MEMBER_REGISTER_PERIOD_ENDED);
		return;
	}

	Guild* pGuild = pPlayer->GuildGet();

	if ( !pGuild )
	{
		this->MemberRegisterResult(pPlayer, ARKA_WAR_RESULT_MEMBER_REGISTER_NOT_IN_GUILD);
		return;
	}

	if ( pGuild->GetID() == pPlayer->GetGUID() )
	{
		this->MemberRegisterResult(pPlayer, ARKA_WAR_RESULT_MEMBER_REGISTER_ONLY_MEMBERS);
		return;
	}
	
	sServerLink->ArkaWarMemberRegister(pPlayer);
}

void ArkaWar::MemberRegisterResult(Player* pPlayer, uint8 result)
{
	ARKA_WAR_MEMBER_REGISTER_RESULT pMsg(result);
	pPlayer->SEND_PCT(pMsg);
}

void ArkaWar::EnterRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( this->GetState() != ARKA_WAR_STATE_STANDBY &&
		 this->GetState() != ARKA_WAR_STATE_PLAYING )
	{
		this->EnterResult(pPlayer, ARKA_WAR_RESULT_ENTER_PERIOD_ENDED);
		return;
	}

	POINTER_PCT(ARKA_WAR_ENTER_REQUEST, lpMsg, Packet, 0);

	if ( lpMsg->result == 0 )
	{
		if ( !Player::InterfaceCheck(pPlayer, InterfaceData::ArcaBattleLesnar, 0) )
		{
			return;
		}

		sArkaWar->EnterResult(pPlayer, 0, 53);
	}
	else if ( lpMsg->result == 1 )
	{
		sServerLink->ArkaWarEnter(pPlayer);
	}
}

void ArkaWar::EnterResult(Player* pPlayer, uint8 result, uint8 data)
{
	ARKA_WAR_ENTER_RESULT pMsg(result, data);
	pPlayer->SEND_PCT(pMsg);
}

void ArkaWar::ViewGuildMemberRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( !Player::InterfaceCheck(pPlayer, InterfaceData::ArcaBattleLesnar, 0) )
	{
		return;
	}

	Guild* pGuild = pPlayer->GuildGet();

	if ( !pGuild )
	{
		this->ViewGuildMemberResult(pPlayer, ARKA_WAR_RESULT_MEMBER_REGISTER_NOT_IN_GUILD, 0);
		return;
	}

	sServerLink->ArkaWarMemberCount(pPlayer);
}

void ArkaWar::ViewGuildMemberResult(Player* pPlayer, uint8 result, uint8 count)
{
	ARKA_WAR_GUILD_MEMBER_RESULT pMsg(result);
	pMsg.count = count;
	pPlayer->SEND_PCT(pMsg);
}

void ArkaWar::SignOfLordRegisterRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !Player::InterfaceCheck(pPlayer, InterfaceData::ArcaBattleLesnar, 0) )
	{
		return;
	}

	Guild* pGuild = pPlayer->GuildGet();

	if ( !pGuild )
	{
		this->SignOfLordRegisterResult(pPlayer, ARKA_WAR_RESULT_MEMBER_REGISTER_NOT_IN_GUILD);
		return;
	}

	if ( pGuild->GetMemberRanking(pPlayer) != GUILD_RANK_MASTER &&
		 pGuild->GetMemberRanking(pPlayer) != GUILD_RANK_BATTLE_MASTER &&
		 pGuild->GetMemberRanking(pPlayer) != GUILD_RANK_ASISTANT )
	{
		this->SignOfLordRegisterResult(pPlayer, ARKA_WAR_RESULT_SIGN_REGISTER_NOT_ALLOWED);
		return;
	}

	if ( this->GetState() != ARKA_WAR_STATE_MASTER_REGISTER &&
		 this->GetState() != ARKA_WAR_STATE_MEMBER_REGISTER )
	{
		this->SignOfLordRegisterResult(pPlayer, ARKA_WAR_RESULT_MASTER_REGISTER_PERIOD_ENDED);
		return;
	}

	if ( pGuild->GetTotalMembers() < sGameServer->GetArkaWarRegisterMinMembers() )
	{
		this->SignOfLordRegisterResult(pPlayer, ARKA_WAR_RESULT_SIGN_REGISTER_LOW_MEMBERS);
		return;
	}

	sServerLink->ArkaWarSignRegister(pPlayer, 0);
}

void ArkaWar::SignOfLordRegisterResult(Player* pPlayer, uint8 result, uint32 signs)
{
	if ( result == 0 )
	{
		ARKA_WAR_SIGN_OF_LORD_REGISTER_RESULT_OK pMsg(signs);
		pPlayer->SEND_PCT(pMsg);
	}
	else
	{
		ARKA_WAR_SIGN_OF_LORD_REGISTER_RESULT pMsg(result);
		pPlayer->SEND_PCT(pMsg);
	}
}
	
void ArkaWar::SignOfLordCheckRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( !Player::InterfaceCheck(pPlayer, InterfaceData::ArcaBattleLesnar, 0) )
	{
		return;
	}

	if ( this->GetState() != ARKA_WAR_STATE_MASTER_REGISTER &&
		 this->GetState() != ARKA_WAR_STATE_MEMBER_REGISTER &&
		 this->GetState() != ARKA_WAR_STATE_READY &&
		 this->GetState() != ARKA_WAR_STATE_STANDBY &&
		 this->GetState() != ARKA_WAR_STATE_PLAYING )
	{
		ARKA_WAR_SIGN_OF_LORD_CHECK_HEAD pMsg;
		pPlayer->SEND_PCT(pMsg);
		return;
	}

	sServerLink->ArkaWarSignOfLordCheckRequest(pPlayer);
}

void ArkaWar::ExchangeOpen(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( !Player::InterfaceCheck(pPlayer, InterfaceData::ArcaBattleLesnar, 0) )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	pPlayer->GetInterfaceState()->Set(InterfaceData::ArkaWarExchange, nullptr);
	pPlayer->SetMixCompleted(false);
	pPlayer->GetMixInventory()->Clear();
}

void ArkaWar::GuildList(uint8 * Packet)
{
	POINTER_PCT(SL_ARKA_WAR_LIST_HEAD, head, Packet, 0);
	POINTER_PCT(SL_ARKA_WAR_LIST_GUILD_BODY, body, Packet, sizeof(SL_ARKA_WAR_LIST_HEAD));

	for ( int32 i = 0; i < MAX_ARKA_WAR_GUILD; ++i )
	{
		this->GetArkaWarGuild(i)->Clear();
	}

	if ( head->count > MAX_ARKA_WAR_GUILD )
	{
		head->count = MAX_ARKA_WAR_GUILD;
	}

	for ( int32 i = 0; i < head->count; ++i )
	{
		this->GetArkaWarGuild(i)->SetID(body[i].guild);
	}
}
	
void ArkaWar::MemberList(uint8 * Packet)
{
	POINTER_PCT(SL_ARKA_WAR_LIST_HEAD, head, Packet, 0);
	POINTER_PCT(SL_ARKA_WAR_LIST_MEMBER_BODY, body, Packet, sizeof(SL_ARKA_WAR_LIST_HEAD));

	MAP_CLEAR(ArkaWarPlayerMap::iterator, this->m_ArkaWarPlayer);

	for ( int32 i = 0; i < head->count; ++i )
	{
		ArkaWarPlayer * add_player = new ArkaWarPlayer;
		add_player->SetGUID(body[i].id);
		add_player->SetName(body[i].name);

		this->m_ArkaWarPlayer[add_player->GetGUID()] = add_player;
	}
}

void ArkaWar::SendNoticeToAll(const char * msg, ...)
{
	ARG(buffer, msg);

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( pPlayer && pPlayer->IsPlaying() && AW_MAP_RANGE(pPlayer->GetWorldId()) )
		{
			pPlayer->SendNoticeNormal(NOTICE_GLOBAL, buffer);
		}
	}
}

void ArkaWar::SendNoticeExToAll(uint8 type, uint32 color, uint8 count, uint16 delay, uint8 speed, const char * msg, ...)
{
	ARG(buffer, msg);

	PMSG_NOTICE pMsg;
	pMsg.type = type;
	pMsg.btCount = count;
	pMsg.btSpeed = speed;
	pMsg.dwColor = color;
	pMsg.wDelay = delay;
	memcpy(pMsg.Notice, buffer, strlen(buffer));
	pMsg.h.size += strlen(buffer);

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( pPlayer && pPlayer->IsPlaying() && AW_MAP_RANGE(pPlayer->GetWorldId()) )
		{
			pPlayer->SEND_PCT(pMsg);
		}
	}
}

void ArkaWar::SendOccupyZone(Player* pPlayer, uint16 gate)
{
	if ( !pPlayer )
	{
		return;
	}

	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		if ( g_ArkaWarZoneBasicData[i].gate == gate )
		{
			Guild* pGuild = sGuildMgr->GetGuild(this->GetOccupyGuild(i)->GetGuild());

			if ( pGuild )
			{
				ARKA_WAR_OCCUPY_ZONE pMsg(pGuild->GetName());
				pPlayer->SEND_PCT(pMsg);
			}
		}
	}
}

void ArkaWar::SendPlayInfo(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	ARKA_WAR_PLAYING_INFO pMsg;
	pMsg.guild_group = 1;

	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		pMsg.obelisk[i].group = i + 1;

		if ( this->GetObeliskState(i)->IsActive() )
		{
			pMsg.obelisk[i].attribute = this->GetObeliskState(i)->GetAttribute();
		}
		else
		{
			pMsg.obelisk[i].attribute = ELEMENTAL_ATTRIBUTE_NONE;
		}

		pMsg.obelisk[i].world = this->GetObeliskBasicData(i)->GetWorld();
		pMsg.obelisk[i].x = this->GetObeliskBasicData(i)->GetX();
		pMsg.obelisk[i].y = this->GetObeliskBasicData(i)->GetY();

		for ( int32 n = 0; n < MAX_ARKA_WAR_AURA; ++n )
		{
			pMsg.obelisk[i].aura[n].x = this->GetObeliskBasicData(i)->GetAuraBasicData(n)->GetX();
			pMsg.obelisk[i].aura[n].y = this->GetObeliskBasicData(i)->GetAuraBasicData(n)->GetY();
		}
	}

	pPlayer->SEND_PCT(pMsg);
}

bool ArkaWar::IsWinGuild(Player* pPlayer)
{
	if ( !pPlayer->GetGuildID() )
	{
		return false;
	}

	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		if ( this->GetOccupyGuild(i)->GetGuild() == pPlayer->GetGuildID() )
		{
			return true;
		}
	}

	return false;
}

ArkaWarPlayer * ArkaWar::GetPlayerInfo(uint32 id) const
{
	ArkaWarPlayerMap::const_iterator it = this->m_ArkaWarPlayer.find(id);

	if ( it != this->m_ArkaWarPlayer.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}
	
ArkaWarPlayer * ArkaWar::GetPlayerInfo(const char * name) const
{
	for ( ArkaWarPlayerMap::const_iterator it = this->m_ArkaWarPlayer.begin(); it != this->m_ArkaWarPlayer.end(); ++it )
	{
		if ( !memcmp(it->second->GetName(), name, MAX_CHARACTER_LENGTH) )
		{
			return it->second;
		}
	}

	return nullptr;
}

ArkaWarGuild const* ArkaWar::GetGuildInfo(uint32 id) const
{
	for ( int32 i = 0; i < MAX_ARKA_WAR_GUILD; ++i )
	{
		if ( this->GetArkaWarGuild(i)->GetID() == id )
		{
			return this->GetArkaWarGuild(i);
		}
	}

	return nullptr;
}

bool ArkaWar::ItemDrop(Player* pPlayer, Monster* pMonster)
{
	if ( !pPlayer || !pMonster )
	{
		return false;
	}

	if ( !pPlayer->GetGuildID() )
	{
		return false;
	}

	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		if ( !this->GetOccupyGuild(i)->GetGuild() )
		{
			continue;
		}

		if ( g_ArkaWarZoneBasicData[i].gate == pPlayer->GetCurrentGate() && this->GetOccupyGuild(i)->GetGuild() == pPlayer->GetGuildID() )
		{
			if ( sItemBagMgr->ExecuteItemBag(ITEM_BAG_ZONE, pMonster, "", pPlayer->GetCurrentGate()) )
			{
				return true;
			}
		}
	}

	return false;
}

void ArkaWar::SendObeliskLife()
{
	ARKA_WAR_OBELISK_LIFE pMsg;

	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		Monster* pMonster = this->GetObeliskState(i)->GetMonster();

		if ( !pMonster )
		{
			continue;
		}

		if ( !pMonster->IsLive() )
		{
			pMsg.data[i].life = 0;
		}
		else
		{
			pMsg.data[i].life = (int32)ceil(pMonster->PowerGet(POWER_LIFE) / (pMonster->PowerGetTotal(POWER_LIFE) / 10.0f)) & 0xF;
		}

		pMsg.data[i].attribute = this->GetObeliskState(i)->GetAttribute();
	}

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
		{
			continue;
		}

		if ( !pPlayer->IsPlaying() )
		{
			continue;
		}

		if ( !AW_MAP_RANGE(pPlayer->GetWorldId()) )
		{
			continue;
		}

		pPlayer->SEND_PCT(pMsg);
	}
}

void ArkaWar::SendObeliskState()
{
	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		Monster* pMonster = this->GetObeliskState(i)->GetMonster();

		if ( !pMonster )
		{
			continue;
		}

		ARKA_WAR_OBELISK_STATE pMsg;
		pMsg.state = pMonster->IsLive();
		pMsg.attribute = this->GetObeliskState(i)->GetAttribute();
		pMsg.aura_state = MAX_ARKA_WAR_AURA_STATE;

		for ( int32 n = 0; n < MAX_ARKA_WAR_AURA; ++n )
		{
			if ( this->GetObeliskState(i)->GetAuraStateData(n)->GetState() != MAX_ARKA_WAR_AURA_STATE )
			{
				pMsg.aura_state = 0;
				break;
			}
		}

		Object* pObject = nullptr;

		VIEWPORT_LOOP_OBJECT(pMonster, pObject)
			pObject->SEND_PCT(pMsg);
		VIEWPORT_CLOSE
	}
}

ArkaWarMonsterRespawnArea const* ArkaWar::GetRespawnArea(uint8 id) const
{
	if ( id < 0 || id >= MAX_ARKA_WAR_ZONES )
	{
		return nullptr;
	}

	RandomValue<ArkaWarMonsterRespawnArea const*> m_RandomValue(nullptr);

	for ( ArkaWarMonsterRespawnAreaList::const_iterator it = this->monster_respawn_area[id].begin(); it != this->monster_respawn_area[id].end(); ++it )
	{
		m_RandomValue.AddValue(*it, 0);
	}

	return m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM);
}

ArkaWarPlayerPosition const* ArkaWar::GetRandomPosition(uint8 id) const
{
	if ( id < 0 || id >= MAX_ARKA_WAR_ZONES )
	{
		return nullptr;
	}

	RandomValue<ArkaWarPlayerPosition const*> m_RandomValue(nullptr);

	for ( ArkaWarPlayerPositionList::const_iterator it = this->player_position_list[id].begin(); it != this->player_position_list[id].end(); ++it )
	{
		m_RandomValue.AddValue(*it, 0);
	}

	return m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM);
}

ArkaWarExchangeData const* ArkaWar::GetExchangeData(int32 trophy_count) const
{
	for ( ArkaWarExchangeDataList::const_iterator it = this->exchange_list.begin(); it != this->exchange_list.end(); ++it )
	{
		if ( (trophy_count >= (*it)->GetTrophyMin()) && (trophy_count <= (*it)->GetTrophyMax()) )
		{
			return (*it);
		}
	}

	return nullptr;
}

void ArkaWar::SetRandomObeliskAttribute()
{
	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		this->GetObeliskState(i)->Clear();
	}

	int32 count = 0;
	int32 fail = 0;

	do
	{
		if (fail >= 50)
		{
			break;
		}

		RandomValue<ArkaWarObeliskData const*> m_RandomValue(nullptr);

		for (ArkaWarObeliskDataMap::const_iterator itr = this->m_ArkaWarObeliskDataMap.begin(); itr != this->m_ArkaWarObeliskDataMap.end(); ++itr)
		{
			m_RandomValue.AddValue(itr->second, 0);
		}

		ArkaWarObeliskData const* pData = m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM);

		if (!pData)
		{
			fail++;
			continue;
		}

		bool exist = false;

		for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
		{
			if (this->GetObeliskState(i)->GetAttribute() == pData->GetElementalAttribute())
			{
				exist = true;
			}
		}

		if (!exist)
		{
			this->GetObeliskState(count)->SetAttribute(pData->GetElementalAttribute());
			this->GetObeliskState(count)->SetMonsterID(pData->GetMonster());
			count++;
		}
	}
	while( count < MAX_ARKA_WAR_ZONES );

	

	count = 0;

	while ( true )
	{
		int32 random_zone = Random(MAX_ARKA_WAR_ZONES);

		if ( this->GetObeliskState(random_zone)->IsActive() )
		{
			continue;
		}

		if ( g_ArkaWarZoneBattleData[random_zone].world == uint16(-1) )
		{
			continue;
		}
		
		this->GetObeliskState(random_zone)->SetActive(true);
		++count;

		if ( count >= 2 )
		{
			break;
		}
	}
}

void ArkaWar::SetObelisk()
{
	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		if ( this->GetObeliskBasicData(i)->GetWorld() == uint16(-1) )
		{
			continue;
		}

		if ( !this->GetObeliskState(i)->IsActive() )
		{
			continue;
		}

		Monster* pObelisk = sObjectMgr->MonsterTryAdd(this->GetObeliskState(i)->GetMonsterID(), this->GetObeliskBasicData(i)->GetWorld());

		if ( pObelisk )
		{
			pObelisk->SetWorldId(this->GetObeliskBasicData(i)->GetWorld());
			pObelisk->SetBasicLocation(this->GetObeliskBasicData(i)->GetX(), this->GetObeliskBasicData(i)->GetY(), this->GetObeliskBasicData(i)->GetX(), this->GetObeliskBasicData(i)->GetY());
			pObelisk->SetDirection(1);
			pObelisk->SetRespawn(IN_MILLISECONDS * 20);
			pObelisk->PowerSet(POWER_LIFE, this->GetObeliskBasicData(i)->GetLife());
			pObelisk->PowerSetMax(POWER_LIFE, this->GetObeliskBasicData(i)->GetLife());
			pObelisk->SetScriptMaxPower(POWER_LIFE, this->GetObeliskBasicData(i)->GetLife());
			pObelisk->SetScriptName("arka_war_obelisk_ai");
			pObelisk->SetEventId(EVENT_ARKA_WAR);
			pObelisk->AddAdditionalDataInt(0, i);
			pObelisk->AddToWorld();
			this->GetObeliskState(i)->SetMonster(pObelisk);
			
			sLog->outInfo("arkawar", "Add Obelisk [%u][%u]", pObelisk->GetEntry(), pObelisk->GetClass());
		}
	}

	this->SendNotifyText(0);
}

void ArkaWar::AddMonster()
{
	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		if ( this->monster_respawn_area[i].empty() || 
			 this->GetMonsterInfo(i)->GetWorld() == uint16(-1) || 
			 this->GetMonsterInfo(i)->GetCount() <= 0 ||
			 this->monster_list[i].empty() )
		{
			continue;
		}

		ArkaWarMonsterRespawnArea const* pRespawnArea = this->GetRespawnArea(i);

		if ( !pRespawnArea )
		{
			continue;
		}

		for ( int32 n = 0; n < this->GetMonsterInfo(i)->GetCount(); ++n )
		{
			RandomValue<uint16> m_RandomMonster(0);

			for ( ArkaWarMonsterList::const_iterator it = this->monster_list[i].begin(); it != this->monster_list[i].end(); ++it )
			{
				m_RandomMonster.AddValue(*it, 0);
			}

			uint16 monster = m_RandomMonster.GetRandomValue(RANDOM_POOL_RANDOM);

			if ( !monster )
			{
				continue;
			}

			Monster* pMonster = sObjectMgr->MonsterTryAdd(monster, this->GetMonsterInfo(i)->GetWorld());

			if ( !pMonster )
			{
				continue;
			}

			pMonster->SetWorldId(this->GetMonsterInfo(i)->GetWorld());
			pMonster->SetBasicLocation(pRespawnArea->GetX1(), pRespawnArea->GetY1(), pRespawnArea->GetX2(), pRespawnArea->GetY2());
			pMonster->SetMoveDistance(20);
			pMonster->SetRespawn(this->GetMonsterInfo(i)->GetRespawnTime() * MINUTE * IN_MILLISECONDS);
			pMonster->SetScriptName("arka_war_monster_ai");
			pMonster->SetEventId(EVENT_ARKA_WAR);
			pMonster->AddAdditionalDataInt(0, i);
			pMonster->AddToWorld();

			sLog->outInfo("arkawar", "Add Monster [%u][%u]", pMonster->GetEntry(), pMonster->GetClass());
		}
	}
}

void ArkaWar::SendPlayerPosition()
{
	for ( int32 i = 0; i < MAX_ARKA_WAR_GUILD; ++i )
	{
		Guild* pGuild = sGuildMgr->GetGuild(this->GetArkaWarGuild(i)->GetID());

		if ( !pGuild )
		{
			continue;
		}

		uint8 buffer[8192];
		POINTER_PCT(ARKA_WAR_PLAYER_POSITION_HEAD, head, buffer, 0);
		POINTER_PCT(ARKA_WAR_PLAYER_POSITION_BODY, body, buffer, sizeof(ARKA_WAR_PLAYER_POSITION_HEAD));
		head->result = 0;
		head->count = 0;
		head->guild = pGuild->GetID();
		head->guild_master_gate = 0;
		memcpy(head->master_name, pGuild->GetMember(0)->GetName(), MAX_CHARACTER_LENGTH + 1);
		head->master_x = 0;
		head->master_y = 0;

		if ( Player* pPlayer = sObjectMgr->FindPlayerByGUID(pGuild->GetID()) )
		{
			if ( AW_MAP_RANGE(pPlayer->GetWorldId()) )
			{
				head->guild_master_gate = pPlayer->GetCurrentGate();
				memcpy(head->master_name, pPlayer->GetName(), MAX_CHARACTER_LENGTH);
				head->master_x = pPlayer->GetX();
				head->master_y = pPlayer->GetY();
			}
		}

		for ( int32 n = 0; n < MAX_GUILD_MEMBER; ++n )
		{
			if ( !pGuild->GetMember(n)->GetID() )
			{
				continue;
			}

			ArkaWarPlayer* pArkaWarPlayer = this->GetPlayerInfo(pGuild->GetMember(n)->GetID());

			if ( !pArkaWarPlayer )
			{
				continue;
			}

			Player* pPlayer = sObjectMgr->FindPlayerByGUID(pArkaWarPlayer->GetGUID());

			if ( pPlayer )
			{
				body[head->count].guild_master_gate = pPlayer->GetCurrentGate();
				memcpy(body[head->count].name, pPlayer->GetName(), MAX_CHARACTER_LENGTH + 1);
				body[head->count].status = pGuild->GetMember(n)->GetRanking();
				body[head->count].x = pPlayer->GetX();
				body[head->count].y = pPlayer->GetY();
				++head->count;
			}
			else
			{
				body[head->count].guild_master_gate = 0;
				memcpy(body[head->count].name, pGuild->GetMember(n)->GetName(), MAX_CHARACTER_LENGTH + 1);
				body[head->count].status = pGuild->GetMember(n)->GetRanking();
				body[head->count].x = 0;
				body[head->count].y = 0;
				++head->count;
			}
		}

		head->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ARKA_WAR_PLAYER_POSITION, sizeof(ARKA_WAR_PLAYER_POSITION_HEAD) + (head->count * sizeof(ARKA_WAR_PLAYER_POSITION_BODY)));

		PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
		for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
		{
			Player* pPlayer = it->second;

			if ( !pPlayer )
			{
				continue;
			}

			if ( pPlayer->GetGuildID() != pGuild->GetID() )
			{
				continue;
			}

			pPlayer->sendPacket(buffer, head->h.get_size());
		}
	}
}

void ArkaWar::CheckObeliskAuraPlayer()
{
	///- Restauro los datos
	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		this->GetObeliskState(i)->SetAuraState(0);

		for ( int32 n = 0; n < MAX_ARKA_WAR_AURA; ++n )
		{
			this->GetObeliskState(i)->GetAuraStateData(n)->SetState(0);
			this->GetObeliskState(i)->GetAuraStateData(n)->SetPlayer(nullptr);
			this->GetObeliskState(i)->GetAuraStateData(n)->SetGuild(0);
		}
	}

	///- Reviso si algún jugador está sobre un aura del obelisk
	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
		{
			continue;
		}

		if ( !pPlayer->GetGuildID() )
		{
			continue;
		}

		for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
		{
			if ( this->GetObeliskBasicData(i)->GetWorld() == uint16(-1) || !this->GetObeliskState(i)->GetMonster() )
			{
				continue;
			}

			if ( this->GetObeliskBasicData(i)->GetWorld() != pPlayer->GetWorldId() )
			{
				continue;
			}

			for ( int32 n = 0; n < MAX_ARKA_WAR_AURA; ++n )
			{
				if ( this->GetObeliskState(i)->GetAuraStateData(n)->GetState() != 0 )
				{
					continue;
				}

				if ( this->GetObeliskBasicData(i)->GetAuraBasicData(n)->GetX() == pPlayer->GetX() &&
					 this->GetObeliskBasicData(i)->GetAuraBasicData(n)->GetY() == pPlayer->GetY() )
				{
					this->GetObeliskState(i)->GetAuraStateData(n)->SetState(1);
					this->GetObeliskState(i)->GetAuraStateData(n)->SetPlayer(pPlayer);
					this->GetObeliskState(i)->GetAuraStateData(n)->SetGuild(pPlayer->GetGuildID());

					sLog->outInfo("arkawar", "%s Obelisk foot hold: Pos X[%d] Y[%d] : [%s][%s][%u]",
						__FUNCTION__, pPlayer->GetX(), pPlayer->GetY(), pPlayer->GetAccountData()->GetAccount(), pPlayer->GetName(), pPlayer->GetGuildID());
				}
			}
		}
	}

	///- Reviso si se desbloquea o no el obelisk en base a los jugadores sobre las auras
	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		if ( this->GetObeliskBasicData(i)->GetWorld() == uint16(-1) || !this->GetObeliskState(i)->GetMonster() )
		{
			continue;
		}

		int32 aura_count = 0;
		
		for ( int32 n = 0; n < MAX_ARKA_WAR_AURA; ++n )
		{
			if ( this->GetObeliskState(i)->GetAuraStateData(n)->GetState() == 1 && this->GetObeliskState(i)->GetAuraStateData(n)->GetPlayer() )
			{
				aura_count++;
			}
		}

		if ( aura_count == MAX_ARKA_WAR_AURA )
		{
			this->GetObeliskState(i)->SetAuraState(2);

			for ( int32 n = 0; n < MAX_ARKA_WAR_AURA; ++n )
			{
				for ( int32 m = 0; m < MAX_ARKA_WAR_AURA; ++m )
				{
					if ( n == m )
					{
						continue;
					}

					if ( this->GetObeliskState(i)->GetAuraStateData(n)->GetGuild() != this->GetObeliskState(i)->GetAuraStateData(m)->GetGuild() )
					{
						this->GetObeliskState(i)->SetAuraState(1);
					}
				}
			}

			if ( this->GetObeliskState(i)->GetAuraState() == 2 )
			{
				if ( this->GetObeliskState(i)->GetAuraReleaseGuild() != this->GetObeliskState(i)->GetAuraStateData(0)->GetGuild() )
				{
					if ( this->GetObeliskState(i)->GetMonster()->IsLive() )
					{
						if ( this->GetObeliskState(i)->GetMonster()->GetState() == 1 )
						{
							this->GetObeliskState(i)->GetMonster()->PowerSet(POWER_LIFE, this->GetObeliskBasicData(i)->GetOccupyLife());
							this->GetObeliskState(i)->GetMonster()->PowerSetMax(POWER_LIFE, this->GetObeliskBasicData(i)->GetOccupyLife());
							this->GetObeliskState(i)->GetMonster()->SetScriptMaxPower(POWER_LIFE, this->GetObeliskBasicData(i)->GetOccupyLife());
						}
						else
						{
							this->GetObeliskState(i)->GetMonster()->PowerSet(POWER_LIFE, this->GetObeliskBasicData(i)->GetLife());
							this->GetObeliskState(i)->GetMonster()->PowerSetMax(POWER_LIFE, this->GetObeliskBasicData(i)->GetLife());
							this->GetObeliskState(i)->GetMonster()->SetScriptMaxPower(POWER_LIFE, this->GetObeliskBasicData(i)->GetLife());
						}
					}

					this->SendObeliskReleased(i, this->GetObeliskState(i)->GetAuraStateData(0)->GetGuild());
				}

				this->GetObeliskState(i)->SetAuraReleaseGuild(this->GetObeliskState(i)->GetAuraStateData(0)->GetGuild());

				for ( int32 n = 0; n < MAX_ARKA_WAR_AURA; ++n )
				{
					this->GetObeliskState(i)->GetAuraStateData(n)->SetState(2);
				}
			}
		}
		else if ( aura_count > 0 )
		{
			this->GetObeliskState(i)->SetAuraState(1);
		}
	}

	ARKA_WAR_OBELISK_INFO pMsg;

	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		if ( this->GetObeliskBasicData(i)->GetWorld() == uint16(-1) || !this->GetObeliskState(i)->GetMonster() )
		{
			continue;
		}

		pMsg.attribute = this->GetObeliskState(i)->GetAttribute();

		for ( int32 n = 0; n < MAX_ARKA_WAR_AURA; ++n )
		{
			pMsg.aura_state[n] = this->GetObeliskState(i)->GetAuraStateData(n)->GetState();
		}

		/*for ( int32 n = 0; n < MAX_ARKA_WAR_AURA; ++n )
		{
			Monster* pMonster = this->GetObeliskState(i)->GetAuraStateData(n)->GetMonster();

			if ( pMonster )
			{
				Object* pObject = nullptr;

				VIEWPORT_LOOP_OBJECT(pMonster, pObject)
					pObject->SEND_PCT(pMsg);
				VIEWPORT_CLOSE
			}
		}*/

		for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
		{
			Player* pPlayer = it->second;

			if ( !pPlayer )
			{
				continue;
			}

			if ( pPlayer->GetWorldId() != this->GetObeliskBasicData(i)->GetWorld() )
			{
				continue;
			}

			if ( Util::Distance(pPlayer->GetX(), pPlayer->GetY(), this->GetObeliskBasicData(i)->GetX(), this->GetObeliskBasicData(i)->GetY()) > 50 )
			{
				continue;
			}

			pPlayer->SEND_PCT(pMsg);
		}
	}

	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		if ( this->GetObeliskBasicData(i)->GetWorld() == uint16(-1) || !this->GetObeliskState(i)->GetMonster() || !this->GetObeliskState(i)->IsActive() )
		{
			continue;
		}

		this->SendObeliskGuildData(i);
	}
}

bool ArkaWar::IsPkAllowed(Player* pPlayer01, Player* pPlayer02)
{
	if ( !pPlayer01 || !pPlayer02 )
	{
		return false;
	}

	if ( this->GetState() != ARKA_WAR_STATE_PLAYING )
	{
		return false;
	}

	if ( !pPlayer01->GetGuildID() || !pPlayer02->GetGuildID() )
	{
		return false;
	}

	if ( pPlayer01->GetGuildID() == pPlayer02->GetGuildID() )
	{
		return false;
	}

	return true;
}

void ArkaWar::SendStateAll()
{
	ARKA_WAR_STATE pMsg(this->GetState());

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( pPlayer && pPlayer->IsPlaying() && AW_MAP_RANGE(pPlayer->GetWorldId()) )
		{
			pPlayer->SEND_PCT(pMsg);
		}
	}
}

void ArkaWar::SendState(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	ARKA_WAR_STATE pMsg(this->GetState());
	pPlayer->SEND_PCT(pMsg);
}

void ArkaWar::SendResult()
{
	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		this->GetOccupyGuild(i)->SetAttribute(this->GetObeliskState(i)->GetAttribute());
		this->GetOccupyGuild(i)->SetGuild(this->GetObeliskState(i)->GetOccupyGuild());
	}

	ARKA_WAR_RESULT pMsg;

	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		pMsg.obelisk[i].obelisk = this->GetOccupyGuild(i)->GetAttribute();
		memset(pMsg.obelisk[i].guild, 0, MAX_GUILD_NAME_LENGTH + 1);

		Guild* pGuild = sGuildMgr->GetGuild(this->GetOccupyGuild(i)->GetGuild());

		if ( pGuild )
		{
			memcpy(pMsg.obelisk[i].guild, pGuild->GetName(), MAX_GUILD_NAME_LENGTH);
		}
	}

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
		{
			continue;
		}

		if ( !pPlayer->GetGuildID() )
		{
			continue;
		}

		ArkaWarPlayer * pArkaWarPlayer = this->GetPlayerInfo(pPlayer->GetGUID());

		if ( !pArkaWarPlayer )
		{
			continue;
		}

		pMsg.result = 0;

		for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
		{
			if ( this->GetOccupyGuild(i)->GetGuild() == pPlayer->GetGuildID() )
			{
				pMsg.result = 1;
			}
		}
		
		int64 experience = this->CalculateRewardExperience(pPlayer, pArkaWarPlayer);
		int64 total_experience = experience;

		pPlayer->GiveExperience(experience, true, true);

		//gObjLevelUp(&gObj[n], &iRewardExp, 0, 7); //7 = ArcaBattle event type?

		pPlayer->GenAddContribution(pArkaWarPlayer->GetContributePoints());

		pMsg.booty_count = pArkaWarPlayer->GetBootyCount();
		pMsg.contribute_point = pArkaWarPlayer->GetContributePoints();
		pMsg.kill_point = pArkaWarPlayer->GetKillPoints();
		pMsg.reward_exp = total_experience;

		pPlayer->SEND_PCT(pMsg);

		sLog->outInfo("arkawar", "SendResult [%s] BootyCount[%u] ContributePoint[%u] KillPoint[%u] Experience[%I64d]",
			pPlayer->BuildLog().c_str(), pArkaWarPlayer->GetBootyCount(), pArkaWarPlayer->GetContributePoints(), pArkaWarPlayer->GetKillPoints(), total_experience);
	}
}

bool ArkaWar::GetBoxPosition(uint16 world, int16 ax, int16 ay, int16 aw, int16 ah, int16 & mx, int16 & my)
{
	World* pWorld = sWorldMgr->GetWorld(world);

	if ( !pWorld )
	{
		return false;
	}

	int32 count = 99; 
	int16 w;
	int16 h;
	int16 tx; 
	int16 ty;

	while( count-- > 0 )
	{
		w = aw - ax;
		h = ah - ay;

		tx = ax + Random(w);
		ty = ay + Random(h);

		WorldGrid const& attr = pWorld->GetGrid(tx, ty);

		if ( !attr.IsSafe() && !attr.IsLocked_1() && !attr.IsLocked_2() )
		{
			mx = tx;
			my = ty;
			return true;
		}
	}

	return false;
}

void ArkaWar::PlayerKill(Player* pPlayer, Player* pVictim)
{
	if ( this->GetState() != ARKA_WAR_STATE_PLAYING )
	{
		return;
	}

	if ( !pPlayer || !pVictim )
	{
		return;
	}

	ArkaWarPlayer* pArkaWarPlayer = this->GetPlayerInfo(pPlayer->GetGUID());
	ArkaWarPlayer* pArkaWarVictim = this->GetPlayerInfo(pVictim->GetGUID());

	if ( !pArkaWarPlayer || !pArkaWarVictim )
	{
		return;
	}

	int32 value = 0;
	int32 mul_value = 0;

	if ( pPlayer->GetTotalLevel() == pVictim->GetTotalLevel() )
	{
		value = ARKA_WAR_REWARD_GENS_USER1;
		mul_value = 3;
	}
	else if ( pPlayer->GetTotalLevel() < pVictim->GetTotalLevel() )
	{
		int32 diff = pVictim->GetTotalLevel() - pPlayer->GetTotalLevel();

		if( diff >= 51 )
        {
			value = ARKA_WAR_REWARD_GENS_USER2;
			mul_value = 3;
		}
		else if( diff >= 31 && diff <= 50 )
		{
			value = ARKA_WAR_REWARD_GENS_USER3;
			mul_value = 3;
		}
		else if( diff >= 11 && diff <= 30 )
		{
			value = ARKA_WAR_REWARD_GENS_USER4;
			mul_value = 3;
		}
		else if( diff > 0 && diff <= 10 )
		{
			value = ARKA_WAR_REWARD_GENS_USER5;
			mul_value = 3;
		}
	}
	else if ( pPlayer->GetTotalLevel() > pVictim->GetTotalLevel() )
	{
		int32 diff = pPlayer->GetTotalLevel() - pVictim->GetTotalLevel();

		if( diff >= 51 )
        {
			value = ARKA_WAR_REWARD_GENS_USER6;
			mul_value = 1;
		}
		else if( diff >= 31 && diff <= 50 )
		{
			value = ARKA_WAR_REWARD_GENS_USER7;
			mul_value = 2;
		}
		else if( diff >= 11 && diff <= 30 )
		{
			value = ARKA_WAR_REWARD_GENS_USER8;
			mul_value = 3;
		}
		else if( diff > 0 && diff <= 10 )
		{
			value = ARKA_WAR_REWARD_GENS_USER9;
			mul_value = 3;
		}
	}

	pArkaWarPlayer->IncreaseContributePoints(value * mul_value);

	sLog->outInfo("arkawar", "Player Kill [%s] Add ContributePoints[%u] to [%s]",
		pVictim->BuildLog().c_str(), pArkaWarPlayer->GetContributePoints(), pPlayer->BuildLog().c_str());

	pArkaWarPlayer->IncreaseKillPoints(ARKA_WAR_KILL_PLAYER_SCORE);

	sLog->outInfo("arkawar", "Player Kill [%s] Add KillPoints [%u] to [%s]",
		pVictim->BuildLog().c_str(), pArkaWarPlayer->GetKillPoints(), pPlayer->BuildLog().c_str());

	if ( !pArkaWarVictim->IsBootyDrop() )
	{
		pArkaWarVictim->SetBootyDrop(true);

		sItemMgr->ItemSerialCreate(pPlayer, pPlayer->GetWorldId(), pPlayer->GetX(), pPlayer->GetY(), Item(ARKA_WAR_KILL_TROPHY, 0, 1.0f), true, true, true);

		sLog->outInfo("arkawar", "Player [%s] Dropped ArcaWar Item Booty To [%s]",
			pVictim->BuildLog().c_str(), pPlayer->BuildLog().c_str());
	}
}

void ArkaWar::BootyItemGet(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	ArkaWarPlayer* pArkaWarPlayer = this->GetPlayerInfo(pPlayer->GetGUID());

	if ( !pArkaWarPlayer )
	{
		return;
	}

	pArkaWarPlayer->IncreaseBootyCount(1);

	sLog->outInfo("arkawar", "Booty Item Get [%s] Total Booty Count [%u]", 
		pPlayer->BuildLog().c_str(), pArkaWarPlayer->GetBootyCount());
}

int64 ArkaWar::CalculateRewardExperience(Player* pPlayer, ArkaWarPlayer * pArkaWarPlayer)
{
	uint32 kill_points = pArkaWarPlayer->GetKillPoints();

	for ( ArkaWarRewardExperienceList::const_iterator it = this->reward_experience.begin(); it != this->reward_experience.end(); ++it )
	{
		if ( (kill_points >= (*it)->GetKillPointsMin()) && (kill_points <= (*it)->GetKillPointsMax()) )
		{
			return (*it)->GetExperience();
		}
	}

	return 0;
}

bool ArkaWar::GetGatePosition(Player* pPlayer, uint16 gate, uint16 & world, int16 & x, int16 & y)
{
	if ( !sGameServer->IsArkaWarEnabled() )
	{
		return true;
	}

	if ( !pPlayer )
	{
		return false;
	}

	bool founded = false;

	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		if ( (g_ArkaWarZoneBattleData[i].gate == gate) && this->GetObeliskState(i)->IsActive() )
		{
			founded = true;
			break;
		}
	}

	if ( !founded )
	{
		return false;
	}

	///- Esto es para moverse donde esta el Guild Master pero solamente desde safe
	if ( this->GetGuildMasterPosition(pPlayer, gate, world, x, y) )
	{
		return true;
	}
	
	///- Esto es para moverse a alguna de las areas de batalla
	if ( this->GetPosition(gate, world, x, y) )
	{
		return true;
	}

	return true;
}

bool ArkaWar::GetGuildMasterPosition(Player* pPlayer, uint16 gate, uint16 & world, int16 & x, int16 & y)
{
	Guild* pGuild = pPlayer->GuildGet();

	if ( !pGuild )
	{
		sLog->outDebug("arkawar", "%s (1)", __FUNCTION__);
		return true;
	}

	ArkaWarGuild const* pArkaWarGuild = this->GetGuildInfo(pGuild->GetID());

	if ( !pArkaWarGuild )
	{
		sLog->outDebug("arkawar", "%s (2)", __FUNCTION__);
		return true;
	}

	if ( pGuild->GetID() == pPlayer->GetGUID() )
	{
		sLog->outDebug("arkawar", "%s (3)", __FUNCTION__);
		return false;
	}

	if ( pPlayer->HasBuff(BUFF_RESURRECTION_PENALTY) )
	{
		sLog->outDebug("arkawar", "%s (4)", __FUNCTION__);
		return false;
	}

	Player* pMaster = sObjectMgr->FindPlayerByGUID(pGuild->GetID());

	if ( !pMaster )
	{
		sLog->outDebug("arkawar", "%s (5)", __FUNCTION__);
		return false;
	}

	if ( pMaster->GetCurrentGate() != gate )
	{
		sLog->outDebug("arkawar", "%s (6)", __FUNCTION__);
		return false;
	}

	if ( pMaster->GetWorldId() != world )
	{
		sLog->outDebug("arkawar", "%s (7)", __FUNCTION__);
		return false;
	}

	return this->GetBoxPosition(pMaster->GetWorldId(), pMaster->GetX(), pMaster->GetY(), pMaster->GetX() + 3, pMaster->GetY() + 3, x, y);
}

bool ArkaWar::GetPosition(uint16 gate, uint16 & world, int16 & x, int16 & y)
{
	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		if ( g_ArkaWarZoneBattleData[i].gate == gate )
		{
			ArkaWarPlayerPosition const* pArkaWarPlayerPosition = this->GetRandomPosition(i);

			if ( !pArkaWarPlayerPosition )
			{
				sLog->outDebug("arkawar", "%s (1)", __FUNCTION__);
				return false;
			}

			if ( pArkaWarPlayerPosition->GetWorld() == uint16(-1) )
			{
				sLog->outDebug("arkawar", "%s (2)", __FUNCTION__);
				return false;
			}

			if ( this->GetBoxPosition(pArkaWarPlayerPosition->GetWorld(), pArkaWarPlayerPosition->GetX1(), pArkaWarPlayerPosition->GetY1(), pArkaWarPlayerPosition->GetX2(), pArkaWarPlayerPosition->GetY2(), x, y) )
			{
				sLog->outDebug("arkawar", "%s (3)", __FUNCTION__);
				world = pArkaWarPlayerPosition->GetWorld();
				return true;
			}

			sLog->outDebug("arkawar", "%s (4)", __FUNCTION__);
			return false;
		}
	}

	sLog->outDebug("arkawar", "%s (5)", __FUNCTION__);
	return false;
}

void ArkaWar::WinnerBuffVerify(Player* pPlayer)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !pPlayer->GetGuildID() )
	{
		for (ArkaWarBuffDataMap::const_iterator itr = this->m_ArkaWarBuffDataMap.begin(); itr != this->m_ArkaWarBuffDataMap.end(); ++itr)
		{
			ArkaWarBuffData const* pData = itr->second;

			if (pData)
			{
				if (pPlayer->HasBuff(pData->GetBuff()))
				{
					pPlayer->RemoveBuff(pData->GetBuff());
				}
			}
		}

		return;
	}

	if ( this->GetState() != ARKA_WAR_STATE_CLOSED &&
		 this->GetState() != ARKA_WAR_STATE_CLOSE &&
		 this->GetState() != ARKA_WAR_STATE_END &&
		 this->GetState() != ARKA_WAR_STATE_NONE )
	{
		for (ArkaWarBuffDataMap::const_iterator itr = this->m_ArkaWarBuffDataMap.begin(); itr != this->m_ArkaWarBuffDataMap.end(); ++itr)
		{
			ArkaWarBuffData const* pData = itr->second;

			if (pData)
			{
				if (pPlayer->HasBuff(pData->GetBuff()))
				{
					pPlayer->RemoveBuff(pData->GetBuff());
				}
			}
		}

		return;
	}

	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		if ( this->GetOccupyGuild(i)->GetGuild() == pPlayer->GetGuildID() )
		{
			for (ArkaWarBuffDataMap::const_iterator itr = this->m_ArkaWarBuffDataMap.begin(); itr != this->m_ArkaWarBuffDataMap.end(); ++itr)
			{
				ArkaWarBuffData const* pData = itr->second;

				if (!pData)
				{
					continue;
				}

				if (pData->GetElementalAttribute() == this->GetOccupyGuild(i)->GetAttribute())
				{
					if (!pPlayer->HasBuff(pData->GetBuff()))
					{
						buff_template const* buff_info = sSkillMgr->GetBuff(pData->GetBuff());

						if ( buff_info )
						{
							pPlayer->AddBuff(pData->GetBuff(),
								BuffEffect(buff_info->GetEffect(0), buff_info->GetValue(0)), 
								BuffEffect(buff_info->GetEffect(1), buff_info->GetValue(1)), 
								0, BUFF_FLAG_CONSTANT, pPlayer);
						}
					}
				}
			}
		}
	}
}

void ArkaWar::SendNotifyText(uint8 id, int32 value)
{
	ARKA_WAR_NOTIFY_TEXT pMsg(id, value);

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( pPlayer && pPlayer->IsPlaying() && AW_MAP_RANGE(pPlayer->GetWorldId()) )
		{
			pPlayer->SEND_PCT(pMsg);
		}
	}
}

void ArkaWar::SendNotifyTextObelisk(uint8 obelisk, uint8 id, uint32 guild)
{
	if ( obelisk >= MAX_ARKA_WAR_ZONES )
	{
		return;
	}

	ARKA_WAR_NOTIFY_TEXT pMsg(id, 0);
	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( pPlayer && pPlayer->IsPlaying() && AW_MAP_RANGE(pPlayer->GetWorldId()) && pPlayer->GetCurrentGate() == g_ArkaWarZoneBattleData[obelisk].gate && pPlayer->GetGuildID() == guild )
		{
			pPlayer->SEND_PCT(pMsg);
		}
	}
}

void ArkaWar::SendObeliskReleased(uint8 obelisk, uint32 guild)
{
	if ( obelisk >= MAX_ARKA_WAR_ZONES )
	{
		return;
	}

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( pPlayer && pPlayer->IsPlaying() && AW_MAP_RANGE(pPlayer->GetWorldId()) && pPlayer->GetCurrentGate() == g_ArkaWarZoneBattleData[obelisk].gate )
		{
			if ( pPlayer->GetGuildID() == guild )
			{
				ARKA_WAR_NOTIFY_TEXT pMsg(2, 0);
				pPlayer->SEND_PCT(pMsg);
			}
			else
			{
				ARKA_WAR_NOTIFY_TEXT pMsg(1, 0);
				pPlayer->SEND_PCT(pMsg);
			}
		}
	}
}

void ArkaWar::SendObeliskConquered(uint8 obelisk, uint32 guild)
{
	if ( obelisk >= MAX_ARKA_WAR_ZONES )
	{
		return;
	}

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( pPlayer && pPlayer->IsPlaying() && AW_MAP_RANGE(pPlayer->GetWorldId()) && pPlayer->GetCurrentGate() == g_ArkaWarZoneBattleData[obelisk].gate )
		{
			if ( pPlayer->GetGuildID() == guild )
			{
				ARKA_WAR_NOTIFY_TEXT pMsg(4, 0);
				pPlayer->SEND_PCT(pMsg);
			}
			else
			{
				ARKA_WAR_NOTIFY_TEXT pMsg(3, 0);
				pPlayer->SEND_PCT(pMsg);
			}
		}
	}
}

bool ArkaWar::IsObeliskOccupied(uint32 guild) const
{
	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		if ( this->GetObeliskState(i)->GetOccupyGuild() == guild )
		{
			return true;
		}
	}

	return false;
}

void ArkaWar::SendObeliskGuildData(uint8 id)
{
	if ( id >= MAX_ARKA_WAR_ZONES )
	{
		return;
	}

	ARKA_WAR_OBELISK_GUILD_STATE pMsg;
	pMsg.zone = id + 1;
	pMsg.attribute = this->GetObeliskState(id)->GetAttribute();

	Guild* pAuraRelease = sGuildMgr->GetGuild(this->GetObeliskState(id)->GetAuraReleaseGuild());
	Guild* pOccupy = sGuildMgr->GetGuild(this->GetObeliskState(id)->GetOccupyGuild());

	if ( pAuraRelease && (pAuraRelease != pOccupy) )
	{
		pMsg.state = 1;
		memcpy(pMsg.guild, pAuraRelease->GetName(), MAX_GUILD_NAME_LENGTH);
		pMsg.guild_id = pAuraRelease->GetID();
	}
	else if ( pOccupy )
	{
		pMsg.state = 2;
		memcpy(pMsg.guild, pOccupy->GetName(), MAX_GUILD_NAME_LENGTH);
		pMsg.guild_id = pOccupy->GetID();
	}
	
	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( pPlayer && pPlayer->IsPlaying() && AW_MAP_RANGE(pPlayer->GetWorldId()) )
		{
			pPlayer->SEND_PCT(pMsg);
		}
	}
}

void ArkaWar::InsertRanking()
{
	uint32 guild_id[2] = { 0, 0 };

	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		if ( !sArkaWar->GetOccupyGuild(i)->GetGuild() )
		{
			continue;
		}

		if ( guild_id[0] == 0 )
		{
			guild_id[0] = sArkaWar->GetOccupyGuild(i)->GetGuild();
		}
		else if ( guild_id[1] == 0 )
		{
			guild_id[1] = sArkaWar->GetOccupyGuild(i)->GetGuild();
		}
	}

	Guild* pGuild01 = sGuildMgr->GetGuild(guild_id[0]);
	Guild* pGuild02 = sGuildMgr->GetGuild(guild_id[1]);

	sGuildMgr->AddScore(pGuild01, sGameServer->GetGuildScoreArkaWar());
	sGuildMgr->AddScore(pGuild02, sGameServer->GetGuildScoreArkaWar());
	
	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_EVENT_RANKING_ARKA_WAR);
	stmt->setUInt32(0, guild_id[0]);
	stmt->setString(1, pGuild01 ? pGuild01->GetName() : "");
	stmt->setUInt32(2, guild_id[1]);
	stmt->setString(3, pGuild02 ? pGuild02->GetName() : "");
	stmt->setInt32(4, this->GetCharacterCount());
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);
}

void ArkaWar::UpdateCharacterCount()
{
	int32 count = 0;

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator itr = characters.begin(); itr != characters.end(); ++itr )
	{
		Player* pPlayer = itr->second;

		if ( !pPlayer )
		{
			continue;
		}

		if ( !AW_MAP_RANGE(pPlayer->GetWorldId()) )
		{
			continue;
		}

		if ( !pPlayer->GetGuildID() )
		{
			continue;
		}

		for ( int32 i = 0; i < MAX_ARKA_WAR_GUILD; ++i )
		{
			ArkaWarGuild const* pGuild = this->GetArkaWarGuild(i);

			if ( !pGuild )
			{
				continue;
			}

			if ( pGuild->GetID() == pPlayer->GetGuildID() )
			{
				++count;
			}
		}
	}

	if ( this->GetCharacterCount() < count )
	{
		this->SetCharacterCount(count);
	}
}