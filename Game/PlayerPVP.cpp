bool Player::AttackAllowedToPlayer(Player* pPlayer, bool targeted)
{
	if ( this->GetPartyID() != PARTY_NULL && (this->GetPartyID() == pPlayer->GetPartyID()) && !sGameServer->party_allow_pk )
		return false;

	if ( this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() < sGameServer->GetPlayerPVPMinLevel() || 
		 pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() < sGameServer->GetPlayerPVPMinLevel() )
		return false;

	if ( this->GetHelper()->IsStarted() )
	{
		if ( pPlayer->GetPKLevelGroup() <= PK_STATUS_MURDER_LVL_1 || !this->IsWorldFlag(WORLD_FLAG_ALLOW_PK_HELPER_KILL) )
		{
			return false;
		}
	}
	Guild* pGuild01 = this->GuildGet();
	Guild* pGuild02 = pPlayer->GuildGet();

	if ( this->IsWorldFlag(WORLD_FLAG_ALLOW_GUILD_WAR) )
	{
		if ( pGuild01 && 
			 pGuild01->GetWarData()->GetState() == GUILD_WAR_STATE_ACTIVE &&
			 pGuild01->GetWarData()->GetEnemy() != pGuild02 )
		{
			return false;
		}

		if ( !sBattleSoccerMgr->IsAttackAllowed(this, pPlayer) )
			return false;
	}

	if ( !sGameServer->IsPKTimeAllowed() && this->IsWorldFlag(WORLD_FLAG_ALLOW_PK_TIME) )
		return false;

	if ( DS_MAP_RANGE(this->GetWorldId()) || BC_MAP_RANGE(this->GetWorldId()) || TS_MAP_RANGE(this->GetWorldId()) )
	{
		return this->GuildRelationShip(pPlayer) == GUILD_RELATIONSHIP_RIVAL && sGameServer->guild_hostil_event_kill == 1;
	}
	else if ( IF_MAP_RANGE(this->GetWorldId()) )
	{
		return false;
	}
	else if ( CC_MAP_RANGE(this->GetWorldId()) )
	{
		if ( this->GetEventId() == EVENT_CHAOS_CASTLE_SURVIVAL )
		{
			return sChaosCastleSurvivalMgr->GetGround()->GetState() == CHAOS_CASTLE_STATE_PLAYING;
		}
		else if ( this->GetEventId() == EVENT_CHAOS_CASTLE && CHAOS_CASTLE_GROUND_RANGE(this->GetEventGround()) )
		{
			return sChaosCastleMgr->GetGround(this->GetEventGround())->GetState() == CHAOS_CASTLE_STATE_PLAYING;
		}
		else
		{
			return false;
		}
	}
	else if ( AW_MAP_RANGE(this->GetWorldId()) )
	{
		if ( sProtectorOfAcheron->GetState() != PROTECTOR_OF_ACHERON_STATE_PLAYING )
		{
			return sArkaWar->IsPkAllowed(this, pPlayer);
		}
	}
	else if ( this->GetWorldId() == WORLD_RAKLION_BOSS )
	{
		if ( !sRaklion->GetStateData(sRaklion->GetState())->IsPKAllowed() )
		{
			return false;
		}
	}
	else if ( this->GetWorldId() == WORLD_LAST_MAN_STANDING )
	{
		return sLastManStanding->GetState() == LAST_MAN_STANDING_STATE_PLAYING;
	}
	else if (this->GetWorldId() == WORLD_SPECIAL_EVENT_MAP)
	{
		return sGameServer->IsSpecialMapPVP();
	}
	else if (sDungeon->IsDungeon(this->GetWorldId()))
	{
		return false;
	}
	else if ( this->GetWorldId() == WORLD_CASTLE_SIEGE )
	{
		if ( sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START )
		{
			return true;
		}
	}
	else if ( DG_MAP_RANGE(this->GetWorldId()) )
	{
		if ( this->GetEventId() == EVENT_DOPPELGANGER && DOPPELGANGER_GROUND_RANGE(this->GetEventGround()) )
		{
			DoppelgangerGround const* pGround = sDoppelganger->GetGround(this->GetEventGround());

			if ( pGround && (pGround->GetState() == DOPPELGANGER_GROUND_STATE_PLAYING_PVP) )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	if ( this->IsOnDuel(pPlayer) )
		return true;

	if ( this->IsDuelClassic() && this->GetDuelTarget() != pPlayer )
	{
		return false;
	}

	if ( pPlayer->IsDuelClassic() && pPlayer->GetDuelTarget() != this )
	{
		return false;
	}

	if ( !this->IsActionAllowed(PlayerAction::PLAYER_ACTION_PVP) )
	{
		return false;
	}

	if (!sGameServer->IsPVPEnabled())
	{
		return false;
	}

	if (sWorldMgr->IsFreePK(this->GetWorldId(), this->GetX(), this->GetY()))
	{
		return true;
	}

	if ( this->IsGenPVP(pPlayer) )
	{
		return targeted;
	}

	if ( this->GuildRelationShip(pPlayer) != GUILD_RELATIONSHIP_RIVAL )
	{
		if ( this->GetPKLevel() > PK_STATUS_COMMONER && !sWorldMgr->IsPKBoss(this->GetWorldId()) )
		{
			if ( this->GetPKCount() >= sGameServer->GetPlayerMurderLimitCount() )
			{
				return false;
			}
		}
	}

	return targeted;
}

void Player::PKLevelSend()
{
	PK_LEVEL pMsg(this->GetEntry(), this->GetPKLevel());

	if ((this->GetWorldId() == WORLD_LAST_MAN_STANDING) || (this->GetWorldId() == WORLD_SPECIAL_EVENT_MAP && sGameServer->IsSpecialMapPVP()))
	{
		pMsg.pk_level = PK_STATUS_MURDER_LVL_3;
	}

	this->sendPacket(MAKE_PCT(pMsg));
	this->sendPacket_viewport(MAKE_PCT(pMsg));
}

void Player::PKUpdate(int32 level)
{
	if ( this->GetPKLevel() == PK_STATUS_COMMONER )
	{
		return;
	}

	if ( !this->IsLive() )
	{
		return;
	}

	this->ReducePKPoints(level);
	this->LimitMinPKPoints(0);

	if ( this->GetPKLevel() < PK_STATUS_COMMONER )
	{
		this->PKUpdateHero();
	}
	else
	{
		this->PKUpdateMurder();
	}
}

void Player::PKUpdateMurder()
{
	uint8 pk_level = this->PKMurderCalculate();

	if ( pk_level == this->GetPKLevel() )
	{
		return;
	}

	this->SetPKLevel(pk_level);
	this->PKLevelSend();
}
	
void Player::PKUpdateHero()
{
	uint8 pk_level = this->PKHeroCalculate();

	if ( pk_level == this->GetPKLevel() )
	{
		return;
	}

	this->SetPKLevel(pk_level);
	this->PKLevelSend();
}

void Player::PKPlayerMurder(Player* pPlayer, Unit* pKiller,  uint8 damage_flag)
{
	if ( !pPlayer || !pKiller )
	{
		return;
	}

	if ( pKiller == pPlayer )
	{
		return;
	}

	if ( Player* pPlayerKiller = pKiller->ToPlayer() )
	{
		pPlayerKiller->UpdateStatistic(STATISTIC_KILLED_PLAYERS, 1);

		if ( pPlayerKiller->IsInGuildWar(pPlayer) && pPlayerKiller->IsWorldFlag(WORLD_FLAG_ALLOW_GUILD_WAR) )
		{
			sGuildWarMgr->IncreaseScore(pPlayerKiller, pPlayer);
			return;
		}

		if ( pPlayerKiller->IsOnDuel(pPlayer) )
		{
			pPlayerKiller->UpdateDuelScore();
			return;
		}

		sWorldBoss->KillPlayer(pPlayerKiller, pPlayer);

		if ( pPlayerKiller->IsGenPVP(pPlayer) )
		{
			if ( Party* pParty = pPlayerKiller->GetParty() )
			{
				pParty->GenKillReward(pPlayer);
			}
			else
			{
				pPlayerKiller->GenKillReward(pPlayer, pPlayerKiller->GetTotalLevel());
				pPlayer->GenDeathPenalty(pPlayerKiller->GetTotalLevel());
			}
		
			return;
		}

		if ( (pPlayerKiller->GetWorldId() == WORLD_LAST_MAN_STANDING) && (sLastManStanding->GetState() == LAST_MAN_STANDING_STATE_PLAYING) )
		{
			sLastManStanding->PlayerKill(pPlayerKiller);
			return;
		}

		if ( pPlayerKiller->IsAdministrator() || pPlayer->IsAdministrator() )
		{
			return;
		}

		if ( pPlayerKiller->GetWorldId() == WORLD_VULCANUS_ROOM )
		{
			return;
		}

		if ( CC_MAP_RANGE(pPlayerKiller->GetWorldId()) || 
			 IT_MAP_RANGE(pPlayerKiller->GetWorldId()) || 
			 BC_MAP_RANGE(pPlayerKiller->GetWorldId()) || 
			 DS_MAP_RANGE(pPlayerKiller->GetWorldId()) ||
			 DG_MAP_RANGE(pPlayerKiller->GetWorldId()) )
		{
			return;
		}

		if ( AW_MAP_RANGE(pPlayerKiller->GetWorldId()) )
		{
			if ( sArkaWar->GetState() == ARKA_WAR_STATE_PLAYING )
			{
				sArkaWar->PlayerKill(pPlayerKiller, pPlayer);
				pPlayerKiller->UpdateStatistic(STATISTIC_ARKA_WAR_KILLS, 1);
				return;
			}
		}

		if ( pPlayerKiller->GetWorldId() == WORLD_CASTLE_SIEGE )
		{
			if ( sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START )
			{
				if ( pPlayerKiller->GetCastleSiegeJoinSide() != 0 )
				{
					pPlayerKiller->KillCountAdd(1);
					pPlayerKiller->UpdateStatistic(STATISTIC_CASTLE_SIEGE_KILLS, 1);
					return;
				}
			}
			else if ( pPlayerKiller->GetY() > 113 && pPlayer->GetY() > 113 )
			{
				if ( sCastleSiege->CastleOwnerMember(pPlayerKiller) || sCastleSiege->CastleOwnerUnionMember(pPlayerKiller) )
					return;
			}
		}

		if ( pPlayerKiller->GuildRelationShip(pPlayer) == GUILD_RELATIONSHIP_RIVAL )
		{
			return;
		}

		if ( pPlayer->GetWorldId() == WORLD_STADIUM )
		{
			return;
		}

		if (pPlayer->GetWorldId() == WORLD_SPECIAL_EVENT_MAP)
		{
			return;
		}

		if ( sWorldBoss->IsKillPlayer(pPlayerKiller, pPlayer) )
		{
			return;
		}

		if ( damage_flag == DAMAGE_TYPE_REFLECT )
		{
			return;
		}
	}

	if ( !sGameServer->IsPVPEnabled() )
	{
		return;
	}

	if ( sGameServer->IsPlayerPKItemDrop() )
	{
		int32 drop_rate = 0;

		switch ( pPlayer->GetPKLevel() )
		{
		case PK_STATUS_COMMONER:
			{
				drop_rate = pKiller->IsPlayer() ? 0: 6;
			} break;

		case PK_STATUS_MURDER_LVL_1:
			{
				drop_rate = pKiller->IsPlayer() ? 25: 25;
			} break;

		case PK_STATUS_MURDER_LVL_2:
			{
				drop_rate = pKiller->IsPlayer() ? 50: 50;
			} break;

		case PK_STATUS_MURDER_LVL_3:
			{
				drop_rate = pKiller->IsPlayer() ? 90: 90;
			} break;
		}

		if (roll_chance_i(drop_rate) && pPlayer->GetPKLevel() >= sGameServer->GetPlayerPKItemDropMinLevel())
		{
			int32 count = 100;

			while(count-- > 0 )
			{
				uint8 slot = Random<uint8>(PLAYER_MAX_EQUIPMENT);

				Item const* pItem = pPlayer->GetInventory()->GetItem(slot);

				if ( !pItem->IsItem() )
					continue;

				if ( !pItem->IsFlag(ITEM_FLAG_ALLOW_PK_DROP) )
					continue;

				if ( pItem->GetExpireDate() )
					continue;

				if (pItem->GetExe() && !sGameServer->IsPlayerPKItemDropExcellent())
					continue;

				if ( pItem->IsAncient() && !sGameServer->IsPlayerPKItemDropAncient() )
					continue;

				if ( pItem->IsHarmonyUpgrade() && !sGameServer->IsPlayerPKItemDropHarmony() )
					continue;

				if ( pItem->Is380Option() && !sGameServer->IsPlayerPKItemDrop380() )
					continue;

				if ( pItem->GetLevel() > sGameServer->GetPlayerPKItemDropItemMaxLevel() )
					continue;

				ITEM_DROP pMsg(pPlayer->GetX(), pPlayer->GetY(), slot);
				pPlayer->ItemDrop((uint8*)&pMsg, true);
				break;
			}
		}
	}

	if ( Player* pPlayerKiller = pKiller->ToPlayer() )
	{
		if (sWorldMgr->IsFreePK(pPlayerKiller->GetWorldId(), pPlayerKiller->GetX(), pPlayerKiller->GetY()))
		{
			return;
		}

		if ( pPlayer->GetPKLevelGroup() > PK_STATUS_MURDER_LVL_1 )
		{
			if ( pPlayerKiller->IsMurder() )
			{
				return;
			}

			if ( pPlayer->IsHighMurder() )
			{
				pPlayerKiller->UpdateStatistic(STATISTIC_PK_KILLED, 1);
				pPlayerKiller->PKModifyStatus(1, 1);
			}

			return;
		}

		bool self_defense = false;

		SelfDefenseDataMap const& sdefense = pPlayer->GetSelfDefenseData();
		SelfDefenseData * data = nullptr;

		// Chequeo si la victima me habia pegado a mi o a un miembro del party
		for ( SelfDefenseDataMap::const_iterator it = sdefense.begin(); it != sdefense.end(); ++it )
		{
			data = it->second;

			if ( !data )
				continue;

			if ( Player* player = data->GetPlayer() )
			{
				if ( player->GetGUID() == pPlayerKiller->GetGUID() )
				{
					self_defense = true;
					break;
				}

				if ( pPlayerKiller->GetPartyID() == player->GetPartyID() && pPlayerKiller->GetPartyID() != PARTY_NULL )
				{
					self_defense = true;
					break;
				}
			}
		}

		if ( self_defense )
		{
			return;
		}

		pPlayerKiller->PKModifyStatus(1, 0);
	}
}

void Player::PKModifyStatus(int32 count, uint8 type)
{
	if ( type == 0 ) // Killer
	{
		if ( this->IsHero() || this->GetPKLevel() == PK_STATUS_COMMONER )
		{
			this->SetPKPoints(0);
			this->SetPKCount(0);
			this->SetPKLevel(PK_STATUS_COMMONER);
		}

		this->IncreasePKCount(count);
		this->LimitMinPKCount(0);
		this->LimitMaxPKCount(sGameServer->GetPlayerMurderCount());

		switch ( this->GetPKLevel() )
		{
		case PK_STATUS_COMMONER:
			{
				this->SetPKPoints(21600);
			} break;

		case PK_STATUS_MURDER_LVL_1:
			{
				this->SetPKPoints(43200);
			} break;

		case PK_STATUS_MURDER_LVL_2:
			{
				this->SetPKPoints(86400);
				this->IncreasePKPoints(3600 * (this->GetPKCount() - 3));
				this->LimitMaxPKPoints(sGameServer->GetPlayerMurderCount() * 3600);
			} break;
		}

		this->SetPKLevel(this->PKMurderCalculate());
	}
	else if ( type == 1 ) // Hero
	{
		this->IncreasePKCount(count);
		this->LimitMinPKCount(0);
		this->LimitMaxPKCount(sGameServer->GetPlayerHeroCount());

		switch ( this->GetPKLevel() )
		{
		case PK_STATUS_COMMONER:
			{
				this->SetPKPoints(21600);
			} break;

		case PK_STATUS_HERO_LVL_1:
			{
				this->SetPKPoints(43200);
			} break;

		case PK_STATUS_HERO_LVL_2:
		case PK_STATUS_HERO:
			{
				this->IncreasePKPoints(3600);
				this->LimitMaxPKPoints(sGameServer->GetPlayerHeroCount() * 3600);
			} break;
		}

		this->SetPKLevel(this->PKHeroCalculate());
	}

	this->PKLevelSend();

	if ( this->GetPKLevel() == PK_STATUS_MURDER_LVL_1 )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "[Warning] If you commit a murder again, you're branded as Outlaw.");
	}
}

uint8 Player::PKMurderCalculate() const
{
	uint8 pk_level = PK_STATUS_COMMONER;

	if ( this->GetPKPoints() > 43200 )
	{
		pk_level = PK_STATUS_MURDER_LVL_3;
	}
	else if ( this->GetPKPoints() > 21600 )
	{
		pk_level = PK_STATUS_MURDER_LVL_2;
	}
	else if ( this->GetPKPoints() > 0 )
	{
		pk_level = PK_STATUS_MURDER_LVL_1;
	}

	return pk_level;
}

uint8 Player::PKHeroCalculate() const
{
	uint8 pk_level = PK_STATUS_COMMONER;

	if ( this->GetPKPoints() > 43200 )
	{
		pk_level = PK_STATUS_HERO;
	}
	else if ( this->GetPKPoints() > 21600 )
	{
		pk_level = PK_STATUS_HERO_LVL_2;
	}
	else if ( this->GetPKPoints() > 0 )
	{
		pk_level = PK_STATUS_HERO_LVL_1;
	}

	return pk_level;
}

bool Player::UseIndulgence()
{
	if ( !this->IsMurder() )
	{
		return false;
	}

	this->SetPKCount(0);
	this->SetPKPoints(0);
	this->SetPKLevel(PK_STATUS_COMMONER);
	this->PKLevelSend();

	return true;
}

uint8 Player::GetPKLevelGroup() const
{
	Party* pParty = this->GetParty();

	if ( !pParty )
		return this->GetPKLevel();

	return pParty->GetPKLevel();
}

bool Player::IsInSelfDefense()
{
	if ( !sGameServer->IsPVPEnabled() )
	{
		return false;
	}

	if ( !sGameServer->IsPlayerSelfDefenseEnabled() )
		return false;

	return !this->self_defense_map.empty();
}

bool Player::IsInSelfDefense(Player* pPlayer)
{
	if ( !sGameServer->IsPVPEnabled() )
	{
		return false;
	}

	if ( !pPlayer )
		return false;

	if ( pPlayer->IsAdministrator() && pPlayer->IsInChallenge() )
		return false;

	if ( CC_MAP_RANGE(this->GetWorldId()) ||
		 DG_MAP_RANGE(this->GetWorldId()) )
	{
		return false;
	}

	if ( AW_MAP_RANGE(this->GetWorldId()) && sArkaWar->GetState() == ARKA_WAR_STATE_PLAYING )
	{
		return false;
	}

	if ( this->GetWorldId() == WORLD_VULCANUS_ROOM )
		return false;

	if ( (this->IsDuelClassic() && this->GetDuelTarget() == pPlayer) )
	{
		return false;
	}

	if ( (pPlayer->IsDuelClassic() && pPlayer->GetDuelTarget() == this) )
	{
		return false;
	}

	if ( pPlayer->GetPKLevelGroup() > PK_STATUS_MURDER_LVL_1 )
		return false;

	if ( this->IsGenPVP(pPlayer) )
		return false;

	if ( this->GuildRelationShip(pPlayer) == GUILD_RELATIONSHIP_RIVAL )
		return false;

	if ( this->IsInGuildWar(pPlayer) )
		return false;

	if ( this->GetWorldId() == WORLD_STADIUM )
	{
		return false;
	}

	if ( this->GetWorldId() == WORLD_CASTLE_SIEGE && sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START )
	{
		return (this->GetCastleSiegeJoinSide() == 0);
	}
	
	return true;
}

void Player::SelfDefenseActive(Player* pPlayer)
{
	if ( !sGameServer->IsPVPEnabled() )
	{
		return;
	}

	if ( !pPlayer )
		return;

	if ( !this->IsInSelfDefense(pPlayer) )
		return;

	if ( !sGameServer->IsPlayerSelfDefenseEnabled() )
		return;

	SelfDefenseDataMap const& sdefense = pPlayer->GetSelfDefenseData();
	SelfDefenseData * data = nullptr;

	for ( SelfDefenseDataMap::const_iterator it = sdefense.begin(); it != sdefense.end(); ++it )
	{
		data = it->second;

		if ( !data )
			continue;

		if ( Player* player = data->GetPlayer() )
		{
			if ( player->GetGUID() == this->GetGUID() )
				return;

			if ( player->GetPartyID() == this->GetPartyID() && this->GetPartyID() != PARTY_NULL )
				return;
		}
	}

	///- Busco si ya lo tengo en la lista
	SelfDefenseDataMap::iterator it = this->self_defense_map.find(pPlayer->GetGUID());

	if ( it != this->self_defense_map.end() ) ///- Si ya lo tengo entonces actualizo el tiempo
	{
		if ( it->second )
			it->second->SetAttackTime(GetTickCount());

		return;
	}

	///- Si no lo tengo entonces lo agrego a la lista
	this->self_defense_map[pPlayer->GetGUID()] = new SelfDefenseData(pPlayer);

	if ( Party* pParty = pPlayer->GetParty() )
	{
		pParty->SendNotice(NOTICE_NORMAL_BLUE, "Self defense is initiated by %s's attack!", this->GetName());
	}
	else
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Self defense is initiated by %s's attack!", this->GetName());
	}

	this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Self defense is initiated by %s's attack!", this->GetName());
}

void Player::SelfDefenseUpdate()
{
	if ( !sGameServer->IsPVPEnabled() )
	{
		return;
	}

	if ( !this->IsLive() )
		return;

	uint32 cur_time = GetTickCount();
	SelfDefenseData * data = nullptr;
	Player* pPlayer = nullptr;

	for ( SelfDefenseDataMap::iterator it = this->self_defense_map.begin(); it != this->self_defense_map.end(); )
	{
		data = it->second;

		if ( !data )
		{
			this->self_defense_map.erase(it++);
			continue;
		}

		pPlayer = data->GetPlayer();

		if ( !pPlayer )
		{
			delete it->second;
			this->self_defense_map.erase(it++);
			continue;
		}

		if ( !pPlayer->IsPlaying() )
		{
			this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s's Self Defense finished.", pPlayer->GetName());
			delete it->second;
			this->self_defense_map.erase(it++);
			continue;
		}

		if ( (cur_time - data->GetAttackTime()) > sGameServer->GetPlayerSelfDefenseTime() )
		{
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Self defense against %s diminishes.", this->GetName());
			this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s's Self Defense finished.", pPlayer->GetName());
			delete it->second;
			this->self_defense_map.erase(it++);
			continue;
		}

		++it;
	}
}

void Player::SelfDefenseClear()
{
	SelfDefenseData * data = nullptr;

	for ( SelfDefenseDataMap::iterator it = this->self_defense_map.begin(); it != this->self_defense_map.end(); ++it )
	{
		data = it->second;

		if ( !data )
			continue;

		if ( Player* player = data->GetPlayer() )
		{
			player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Self defense against %s diminishes.", this->GetName());
			this->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s's Self Defense finished.", player->GetName());
		}
	}

	MAP_CLEAR(SelfDefenseDataMap::iterator, this->self_defense_map);
}