void Object::ViewportUpdate(uint32 diff)
{
	if ( Unit* pUnit = this->ToUnit() )
	{
		if ( pUnit->GetRegenStatus() == REGEN_STANDBY ||
			 pUnit->GetRegenStatus() == REGEN_READY )
		{
			return;
		}
	}

	if ( this->IsViewportControl() ) // Viewport Control
	{
		this->viewport_removed_map.clear();

		this->ViewportListRemove();
		this->ViewportListAdd();
	}

	this->SetViewportEmpty(this->viewport_data.empty());
}

void Object::ViewportListRemove()
{
	this->SetViewportEmpty(true);

	World * pWorld = this->GetWorld();

	if ( !pWorld )
		return;

	if ( this->viewport_data.empty() )
		return;

	ViewportData* data = nullptr;
	Object* pObject = nullptr;
	WorldItem* pWorldItem = nullptr;

	for ( ViewportDataList::iterator it = this->viewport_data.begin(); it != this->viewport_data.end(); )
	{
		data = it->second;

		if ( !data )
		{
			sLog->outError("root", "[%u][%s] Removing wrong ptr object", this->GetEntry(), this->GetName());
			this->viewport_data.erase(it++);
			continue;
		}

		pObject = data->GetObjectData();

		if ( !pObject )
		{
			sLog->outError("root", "[%u][%s] Removing wrong object", this->GetEntry(), this->GetName());
			delete it->second;
			this->viewport_data.erase(it++);
			continue;
		}

		if ( data->GetStatus() == Viewport::STATUS_NONE )
		{
			++it;
			continue;
		}

		switch ( pObject->GetType() )
		{
		case OBJECT_TYPE_WORLD_ITEM:
			{
				pWorldItem = pWorld->GetItem(pObject->GetEntry());

				if ( !pWorldItem )
				{
					data->SetStatus(Viewport::STATUS_DELETE);
				}
				else if ( !pWorldItem->item.IsItem() )
				{
					data->SetStatus(Viewport::STATUS_DELETE);
				}
				else if ( pWorldItem->GetState() == WORLD_ITEM_STATE_DELETE )
				{
					data->SetStatus(Viewport::STATUS_DELETE);
				}
				else if ( !this->CheckFrustrumViewport(pWorldItem) )
				{
					data->SetStatus(Viewport::STATUS_DELETE);
				}
			} break;

		default:
			{
				if ( this->ViewportListRemoveConditions(pObject) )
				{
					//sLog->outError("root", "[%u][%s] Removing object [%u]", this->GetEntry(), this->GetName(), pObject->GetHighGUID());
					data->SetStatus(Viewport::STATUS_DELETE);
				}
			} break;
		}

		if ( data->GetStatus() == Viewport::STATUS_DELETE )
		{
			this->ViewportSendObject(pObject, true);
			this->OnObjectRemove(pObject);
			delete it->second;
			this->viewport_data.erase(it++);

			this->viewport_removed_map[pObject->GetHighGUID()] = 1;
		}
		else
		{
			++it;
		}
	}
}
	
bool Object::ViewportListRemoveConditions(Object* pRemove)
{
	if ( !pRemove )
		return true;

	if ( pRemove->GetConnectStatus() != CONNECT_STATUS_PLAYING )
		return true;

	if ( pRemove->GetState() == OBJECT_STATE_STAND_BY && !pRemove->IsLive() )
		return true;

	if ( pRemove->GetState() == OBJECT_STATE_RESPAWN )
		return true;

	if ( pRemove->GetState() == OBJECT_STATE_TELEPORTING )
		return true;

	/*if (pRemove->IsUnit() && pRemove->ToUnit()->IsTeleporting())
	{
		return true;
	}*/

	//if ( pRemove->IsUnit() && pRemove->ToUnit()->IsInTeleportRemoveViewportState() )
	//	return true;
	
	if ( !this->CheckFrustrumViewport(pRemove) )
		return true;

	if ( !this->SameDimension(pRemove) )
		return true;

	if ( pRemove->IsUnit() && this->IsCreature() && this->ToCreature()->GetAI() && this->ToCreature()->GetAI()->HasFlag(MONSTER_AI_FLAG_VIEWPORT) )
	{
		return this->ToCreature()->GetAI()->ViewportListRemoveConditions(pRemove->ToUnit());
	}

	if ( Monster* pMonster = pRemove->ToCreature() )
	{
		if ( pMonster->HasBuff(BUFF_NARS_CLONE) )
		{
			return true;
		}
	}

	if ( Monster* pMonster = this->ToCreature() )
	{
		if ( pMonster->HasBuff(BUFF_NARS_CLONE) )
		{
			return true;
		}
	}

	return false;
}
	
void Object::ViewportListAdd()
{
	if ( this->IsUnit() && this->ToUnit()->GetRegenStatus() != REGEN_NONE )
		return;

	World * pWorld = this->GetWorld();

	if ( !pWorld )
		return;

	Object* pObject = nullptr;
	Player* pPlayer = this->ToPlayer();

	if ( pPlayer )
	{
		if ( pPlayer->IsAdministrator() && !pPlayer->IsAuthorizationEnabled() )
		{
			return;
		}

		WorldItem * item = nullptr;

		for ( uint16 i = 0; i < MAX_WORLD_ITEM; ++i )
		{
			item = pWorld->GetItem(i);

			if ( item )
			{
				if ( item->GetState() == WORLD_ITEM_STATE_STANDBY || item->GetState() == WORLD_ITEM_STATE_READY )
				{
					if ( item->IsVisible() || item->GetOwner() == this )
					{
						if ( this->CheckFrustrumViewport(item) )
						{
							if ( this->SameDimension(item) )
							{
								this->ViewportAdd(item);
							}
						}
					}
				}
			}
		}
	}

	WorldObjectMap const& wobjects = pWorld->GetAllObjects();
	for ( WorldObjectMap::const_iterator it = wobjects.begin(); it != wobjects.end(); ++it )
	{
		pObject = it->second;

		if ( !pObject )
			continue;

		if ( pObject == this )
			continue;

		if ( this->ViewportListAddConditions(pObject) )
		{
			if ( this->ViewportAdd(pObject) )
				this->OnObjectAdd(pObject);
		}
	}
}
		
bool Object::ViewportListAddConditions(Object* pAdd)
{
	if ( !pAdd )
		return false;

	if ( pAdd == this )
		return false;

	if ( pAdd->GetWorldId() != this->GetWorldId() )
		return false;

	if ( !pAdd->Object::IsPlaying() )
		return false;

	if ( pAdd->GetState() != OBJECT_STATE_STAND_BY && pAdd->GetState() != OBJECT_STATE_READY )
		return false;

	if ( pAdd->GetState() == OBJECT_STATE_RESPAWN )
		return false;

	if ( pAdd->GetState() == OBJECT_STATE_TELEPORTING )
		return false;

	if ( this->IsViewportRemoved(pAdd->GetHighGUID()) )
	{
		return false;
	}

	if (!sGameServer->IsTeleportFix())
	{
		if (pAdd->IsUnit() && pAdd->ToUnit()->IsInTeleportRemoveViewportState())
		{
			return false;
		}
	}

	/*if ( pAdd->IsUnit() && pAdd->ToUnit()->GetTeleportStatus() == TELEPORT_STANDBY )
	{
		return false;
	}*/
	
	//if ( pAdd->IsUnit() && pAdd->ToUnit()->GetTeleportStatus() == TELEPORT_READY )
	//	return false;

	//if ( pAdd->IsUnit() && pAdd->ToUnit()->IsTeleporting() )
	//	return false;

	if ( !this->CheckFrustrumViewport(pAdd) )
		return false;

	if ( !this->SameDimension(pAdd) )
		return false;

	if ( Player* pPlayer = pAdd->ToPlayer() )
	{
		if ( pPlayer->IsAdministrator() && !pPlayer->IsAuthorizationEnabled() )
		{
			return false;
		}
	}

	if ( pAdd->IsUnit() && this->IsCreature() && this->ToCreature()->GetAI() && this->ToCreature()->GetAI()->HasFlag(MONSTER_AI_FLAG_VIEWPORT) )
	{
		return this->ToCreature()->GetAI()->ViewportListAddConditions(pAdd->ToUnit());
	}
	else if ( !this->IsPlayer() )
	{
		if ( !pAdd->IsPlayer() )
		{
			if ( Monster* pMonster = pAdd->ToCreature() )
			{
				if ( !pMonster->GetSummoner()->IsPlayer() )
					return false;
			}
			else
				return false;
		}
	}

	if ( Monster* pMonster = pAdd->ToCreature() )
	{
		if ( pMonster->HasBuff(BUFF_NARS_CLONE) )
		{
			return false;
		}
	}

	if ( Monster* pMonster = this->ToCreature() )
	{
		if ( pMonster->HasBuff(BUFF_NARS_CLONE) )
		{
			return false;
		}
	}

	return true;
}

bool Object::ViewportExist(Object* pObject)
{
	if ( !pObject )
	{
		return false;
	}

	ViewportDataList::const_iterator it = this->viewport_data.find(pObject->GetHighGUID());

	/// - Reviso si ya lo tengo en VP
	if ( it != this->viewport_data.end() )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Object::ViewportAdd(Object* pObject)
{
	/// - No debería pasar, pero ante la duda revisamos que el objeto no sea nulo
	if ( !pObject )
	{
		return false;
	}

	ViewportDataList::const_iterator it = this->viewport_data.find(pObject->GetHighGUID());

	/// - Reviso si ya lo tengo en VP
	if ( it != this->viewport_data.end() )
		return true;

	/// - Reviso si tengo lugar en VP
	if ( this->viewport_data.size() >= MAX_VIEWPORT )
		return false;

	/// - Lo agrego a VP
	/// - Si no tiene control de viewport, entonces lo dejo ya listo
	ViewportData* pVPAdd = new ViewportData;
	pVPAdd->Set(pObject, Viewport::STATUS_READY);
	this->viewport_data[pObject->GetHighGUID()] = pVPAdd;

	if ( this->IsViewportControl() )
		this->ViewportSendObject(pObject, false);

	///- Fix para las puertas del CastleSiege
	if (Monster* pMonster = this->ToCreature())
	{
		if (MonsterAI *pAI = pMonster->GetAI())
		{
			pAI->EnteringVP(pObject->ToUnit());
		}
	}

	///- Fix para las puertas del CastleSiege
	if (Monster* pMonster = pObject->ToCreature())
	{
		if (MonsterAI *pAI = pMonster->GetAI())
		{
			pAI->EnteringVP(this->ToUnit());
		}
	}
	
	this->SetViewportEmpty(false);

	return true;
}

void Object::ViewportRemove(Object* pObject)
{
	/// - No debería pasar, pero ante la duda revisamos que el objeto no sea nulo
	if ( !pObject )
		return;

	ViewportDataList::const_iterator it = this->viewport_data.find(pObject->GetHighGUID());

	/// - Solamente borrarlo cuando existe
	if ( it != this->viewport_data.end() )
	{
		if ( this->IsViewportControl() ) /// - Si tiene control de viewport debo eliminar la visual
		{
			this->ViewportSendObject(pObject, true);
		}

		if ( it->second )
			delete it->second;
		this->viewport_data.erase(it);
	}
}

void Object::ViewportCreate(uint32 flags)
{
	if ( flags & VIEWPORT_CREATE_FLAG_ME )
	{
		uint8 buffer[2048];
		POINTER_PCT(VIEWPORT_CREATE_HEAD_C1, head, buffer, 0);
		uint16 size = sizeof(VIEWPORT_CREATE_HEAD_C1);
		head->h.c = 0xC2;

		ViewportConstruct(this, this, buffer, size);
		head->h.set_size(size);
		head->count = 1;
		this->sendPacket(buffer, size);

		if ( !(flags & VIEWPORT_CREATE_FLAG_ONLY_ME) )
		{
			if ( this->SendViewportData() )
			{
				this->sendPacket_viewport(buffer, size);
			}
		}
	}

	if ( Player* pPlayer = ToPlayer() )
	{
		if ( flags & VIEWPORT_CREATE_FLAG_GEN )
		{
			pPlayer->GenSendInfo(false, true);
		}

		if ( flags & VIEWPORT_CREATE_FLAG_GUILD )
		{
			pPlayer->GuildUpdateViewport();
		}

		if ( flags & VIEWPORT_CREATE_FLAG_SIEGE )
		{
			sCastleSiege->UpdateStatus(pPlayer);
		}

		pPlayer->KillCountSend();
	}
}

void Object::ViewportSendObject(Object* pObject, bool clear)
{
	if ( !pObject )
		return;

	if ( !this->IsPlayer() )
		return;

	Player* pMe = this->ToPlayer();

	if (!pMe)
	{
		return;
	}

	if ( clear )
	{
		if ( pObject->GetType() == OBJECT_TYPE_WORLD_ITEM )
		{
			VIEWPORT_DESTROY_ITEM_COMPLETE pMsg(pObject->GetEntry());
			this->SEND_PCT(pMsg);
		}
		else
		{
			VIEWPORT_DESTROY_COMPLETE pMsg(pObject->GetEntry());
			this->SEND_PCT(pMsg);
		}
	}
	else
	{
		if ( !pObject->SendViewportData() )
			return;

		if (sGameServer->IsTeleportFix())
		{
			if (pObject->IsUnit() && pObject->ToUnit()->IsTeleporting())
			{
				return;
			}
		}

		uint8 buffer[1000];
		POINTER_PCT(VIEWPORT_CREATE_HEAD_C1, head, buffer, 0);
		uint16 size = sizeof(VIEWPORT_CREATE_HEAD_C1);
		head->h.c = 0xC2;
		head->count = 1;

		ViewportConstruct(this, pObject, buffer, size);

		head->h.set_size(size);
		this->sendPacket(buffer, size);

		switch ( pObject->GetType() )
		{
		case OBJECT_TYPE_PLAYER:
			{
				Player * pPlayer = pObject->ToPlayer();

				VIEWPORT_CREATE_GEN_COMPLETE pGenData(pPlayer->GetGen()->GetFamily(), pPlayer->GetEntry(), pPlayer->GetGen()->GetLevel(), pPlayer->GetGen()->GetContribution());

				if ( this->ToPlayer()->IsGenPVP(pPlayer) )
				{
					pGenData.family = 0;
				}

				this->sendPacket((uint8*)&pGenData, pGenData.h.get_size());

				if ( !this->ToPlayer()->IsGenPVP(pPlayer) && (this->GetWorldId() != WORLD_LAST_MAN_STANDING || !sGameServer->IsLastManStandingHideCharacter()) )
				{
					Guild* pGuild = pPlayer->GuildGet();

					if ( pGuild )
					{
						GUILD_UPDATE_VIEWPORT_COMPLETE pGuildData;
						pGuildData.numberH = SET_NUMBERH(pPlayer->GetEntry()) & 0x7F;
						pGuildData.numberL = SET_NUMBERL(pPlayer->GetEntry());

						if ( pGuild->GetMemberRanking(pPlayer) == GUILD_RANK_MASTER )
							pGuildData.numberH |= 0x80;

						pGuildData.guild_id = pGuild->GetID();
						pGuildData.ranking = pGuild->GetMemberRanking(pPlayer);
						pGuildData.type = GUILD_TYPE_TEST;
						pGuildData.relationship = pPlayer->GuildRelationShip(this->ToPlayer());
						pGuildData.owner_status = sCastleSiege->CastleOwnerMember(pPlayer) || sCastleSiege->CastleOwnerUnionMember(pPlayer);

						this->SEND_PCT(pGuildData);
					}
				}

				if (pPlayer->GetPersonalStore()->IsOpen())
				{
					PERSONAL_STORE_VIEWPORT pMsg;
					pMsg.SetIndex(pPlayer->GetEntry());
					memcpy(pMsg.description, pPlayer->GetPersonalStore()->GetName(), PERSONAL_STORE_NAME_LENGTH);

					pMe->SendPacket(&pMsg);
				}
			} break;

		case OBJECT_TYPE_MONSTER:
			{
				if ( Monster * pMonster = pObject->ToCreature() )
				{
					sMonsterMgr->BuildMonsterCustomData(pMonster, this->ToPlayer());
				}
			} break;

		case OBJECT_TYPE_WORLD_ITEM:
			{

			} break;
		}
	}
}
 
void Object::ViewportDeleteSend()
{
	Object* pObject;
	VIEWPORT_LOOP(pObject)
		pObject->ViewportSendObject(this, true);
	VIEWPORT_CLOSE
}

int32 Object::GetViewportCount()
{
	int32 count = 0;
	Object* pObject;
	VIEWPORT_LOOP(pObject)
		++count;	
	VIEWPORT_CLOSE

	return count;
}

void Object::ViewportConstruct(Object* pUnit, Object* pConstruct, uint8 * buffer, uint16 & size)
{
	SafeAssert(pConstruct, "pConstruct == nullptr");
	SafeAssert(buffer, "buffer == nullptr");
	SafeAssert(size, "size == 0");

	POINTER_PCT(VIEWPORT_CREATE_HEAD_C1, head, buffer, 0);

	if ( Player* pPlayer = pConstruct->ToPlayer() )
	{
		if ( pPlayer->GetSkin() == uint16(-1) || pPlayer->GetGameOption()->GetChangeSkin() )
		{
			ViewportConstructPlayer(pUnit, pPlayer, buffer, size);
			head->h.headcode = HEADCODE_VIEWPORT_PLAYER;
		}
		else
		{
			ViewportConstructPlayerChange(pUnit, pPlayer, buffer, size);
			head->h.headcode = HEADCODE_VIEWPORT_PLAYER_CHANGE;
		}
	}
	else if ( Monster* pMonster = pConstruct->ToCreature() )
	{
		if ( !pMonster->IsSummoned() && !pMonster->GetSummonPlayer() )
		{
			ViewportConstructMonster(pUnit, pMonster, buffer, size);
			head->h.headcode = HEADCODE_VIEWPORT_MONSTER;
		}
		else
		{
			ViewportConstructMonsterSummon(pUnit, pMonster, buffer, size);
			head->h.headcode = HEADCODE_VIEWPORT_MONSTER_SUMMON;
		}
	}
	else if ( WorldItem* pWorldItem = pConstruct->ToWorldItem() )
	{
		ViewportConstructWorldItem(pUnit, pWorldItem, buffer, size);
		head->h.headcode = HEADCODE_VIEWPORT_ITEM;
	}
}

void Object::ViewportConstructMonster(Object* pUnit, Monster* pMonster, uint8 * buffer, uint16 & size)
{
	SafeAssert(pUnit, "pUnit == nullptr");
	SafeAssert(pMonster, "pMonster == nullptr");

	VIEWPORT_CREATE_MONSTER body;

	body.SetIndex(pMonster->GetEntry());
	body.SetClass(pMonster->GetModel());
	body.x = pMonster->GetX();
	body.y = pMonster->GetY();

	if ( sGameServer->IsNormalMonsterMovement() )
	{
		body.tx = pMonster->GetTX();
		body.ty = pMonster->GetTY();
	}
	else
	{
		body.tx = pMonster->GetX();
		body.ty = pMonster->GetY();
	}
	
	body.path = pMonster->GetDirection() << 4;

	if ( pMonster->GetState() == OBJECT_STATE_STAND_BY )
	{
		body.m_propIndex[0] |= 0x80;
	}

	/*if( pMonster->IsTeleporting() )
	{
		body.m_propIndex[0] |= 0x40;
	}*/

	if ( pUnit->IsUnit() )
	{
		if ( MonsterAI* pAI = pMonster->AIGroup(CS_LIFE_STONE_AI) )
		{
			if ( sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START )
			{
				if ( pMonster->GetCastleSiegeJoinSide() == pUnit->ToUnit()->GetCastleSiegeJoinSide() )
				{
					body.m_propClass[0] |= 0x80;
				}
			}

			body.m_propClass[0] |= (((LifeStoneBasicAI*)pAI)->GetCreationState() << 4 & 0x70);
		}
	}

	body.elemental_attribute = pMonster->GetElementalAttribute();

	body.SetLevel(pMonster->GetLevel());

	body.hp_max[0] = SET_NUMBERH(SET_NUMBERHW((pMonster->PowerGetTotal(POWER_LIFE))));
	body.hp_max[1] = SET_NUMBERH(SET_NUMBERLW((pMonster->PowerGetTotal(POWER_LIFE))));
	body.hp_max[2] = SET_NUMBERL(SET_NUMBERHW((pMonster->PowerGetTotal(POWER_LIFE))));
	body.hp_max[3] = SET_NUMBERL(SET_NUMBERLW((pMonster->PowerGetTotal(POWER_LIFE))));

	body.hp[0] = SET_NUMBERH(SET_NUMBERHW((pMonster->PowerGet(POWER_LIFE))));
	body.hp[1] = SET_NUMBERH(SET_NUMBERLW((pMonster->PowerGet(POWER_LIFE))));
	body.hp[2] = SET_NUMBERL(SET_NUMBERHW((pMonster->PowerGet(POWER_LIFE))));
	body.hp[3] = SET_NUMBERL(SET_NUMBERLW((pMonster->PowerGet(POWER_LIFE))));

	body.critical_damage_resistance = pMonster->GetFloatData(UNIT_FLOAT_RESIST_CRITICAL_DAMAGE_RATE); ///- Critical
	body.excellent_damage_resistance = pMonster->GetFloatData(UNIT_FLOAT_RESIST_EXCELLENT_DAMAGE_RATE); ///- Excellent
	body.SetDebuffResistance(pMonster->GetIntData(UNIT_INT_DEBUFF_RESISTANCE)); ///- Debuff
	body.damage_absorb = pMonster->GetDamageAbsorb();
	body.elite = pMonster->IsElite() ? 1 : 0;
	body.unk27 = 0;

	body.effect_count = pMonster->GetActiveBuffCount();

	memcpy(&buffer[size], &body, sizeof(body));

	size += sizeof(VIEWPORT_CREATE_MONSTER);

	ViewportConstructBuff(pMonster, buffer, size);
}

void Object::ViewportConstructMonsterSummon(Object* pUnit, Monster* pMonster, uint8 * buffer, uint16 & size)
{
	SafeAssert(pUnit, "pUnit == nullptr");
	SafeAssert(pMonster, "pMonster == nullptr");
	SafeAssert(pMonster->GetSummoner(), "pMonster->GetSummoner() == nullptr");
	SafeAssert(pMonster->IsCreature(), "pMonster->IsCreature() == false");
	SafeAssert(pMonster->ToCreature(), "pMonster->ToCreature() == nullptr");

	VIEWPORT_CREATE_MONSTER_SUMMONED body;

	body.SetIndex(pMonster->GetEntry());
	body.SetClass(pMonster->GetModel());
	body.x = pMonster->GetX();
	body.y = pMonster->GetY();

	if ( sGameServer->IsNormalMonsterMovement() )
	{
		body.tx = pMonster->GetTX();
		body.ty = pMonster->GetTY();
	}
	else
	{
		body.tx = pMonster->GetX();
		body.ty = pMonster->GetY();
	}

	body.path = pMonster->GetDirection() << 4;

	if ( pMonster->GetState() == OBJECT_STATE_STAND_BY )
	{
		body.m_propIndex[0] |= 0x80;
	}

	/*if( pMonster->IsTeleporting() )
	{
		body.m_propIndex[0] |= 0x40;
	}*/

	body.elemental_attribute = pMonster->GetElementalAttribute();

	body.SetLevel(pMonster->GetLevel());

	body.hp_max[0] = SET_NUMBERH(SET_NUMBERHW((pMonster->PowerGetTotal(POWER_LIFE))));
	body.hp_max[1] = SET_NUMBERH(SET_NUMBERLW((pMonster->PowerGetTotal(POWER_LIFE))));
	body.hp_max[2] = SET_NUMBERL(SET_NUMBERHW((pMonster->PowerGetTotal(POWER_LIFE))));
	body.hp_max[3] = SET_NUMBERL(SET_NUMBERLW((pMonster->PowerGetTotal(POWER_LIFE))));

	body.hp[0] = SET_NUMBERH(SET_NUMBERHW((pMonster->PowerGet(POWER_LIFE))));
	body.hp[1] = SET_NUMBERH(SET_NUMBERLW((pMonster->PowerGet(POWER_LIFE))));
	body.hp[2] = SET_NUMBERL(SET_NUMBERHW((pMonster->PowerGet(POWER_LIFE))));
	body.hp[3] = SET_NUMBERL(SET_NUMBERLW((pMonster->PowerGet(POWER_LIFE))));

	memset(body.summoner_name, 0, MAX_CHARACTER_LENGTH);
	memset(body.preview, 0, MAX_PREVIEW_DATA);
	body.type = pMonster->GetSummonType();
	Player* pPlayer = nullptr;
	
	if ( pPlayer = pMonster->GetSummonPlayer() )
	{
		memcpy(body.summoner_name, pPlayer->GetName(), MAX_CHARACTER_LENGTH);

		for ( int32 i = 0; i < MAX_PREVIEW_DATA; ++i )
		{
			body.preview[i] = pPlayer->preview[i].get();
		}
	}
	else if ( pPlayer = pMonster->GetSummoner()->ToPlayer() )
	{
		memcpy(body.summoner_name, pPlayer->GetName(), MAX_CHARACTER_LENGTH);
		for ( int32 i = 0; i < MAX_PREVIEW_DATA; ++i )
		{
			body.preview[i] = pPlayer->preview[i].get();
		}
	}

	body.effect_count = pMonster->GetActiveBuffCount();

	memcpy(&buffer[size], &body, sizeof(body));

	size += sizeof(VIEWPORT_CREATE_MONSTER_SUMMONED);

	ViewportConstructBuff(pMonster, buffer, size);
}

void Object::ViewportConstructPlayer(Object* pUnit, Player* pPlayer, uint8 * buffer, uint16 & size)
{
	SafeAssert(pUnit, "pUnit == nullptr");
	SafeAssert(pPlayer, "pPlayer == nullptr");
	SafeAssert(pPlayer->IsPlayer(), "pPlayer->IsPlayer() == false");
	SafeAssert(pPlayer->ToPlayer(), "pPlayer->ToPlayer() == nullptr");

	VIEWPORT_CREATE player_body;

	player_body.SetIndex(pPlayer->GetEntry());
	player_body.x = pPlayer->GetX();
	player_body.y = pPlayer->GetY();
	player_body.tx = pPlayer->GetTX();
	player_body.ty = pPlayer->GetTY();
	player_body.dir_and_pk_level = pPlayer->GetDirection() << 4;

	if ((pPlayer->GetWorldId() == WORLD_LAST_MAN_STANDING) || (pPlayer->GetWorldId() == WORLD_SPECIAL_EVENT_MAP && sGameServer->IsSpecialMapPVP()))
	{
		player_body.dir_and_pk_level |= PK_STATUS_MURDER_LVL_3;
	}
	else
	{
		player_body.dir_and_pk_level |= pPlayer->GetPKLevel() & 0x0F;
	}

	if ( pPlayer->GetState() == OBJECT_STATE_STAND_BY && !pPlayer->IsTeleporting() )
	{
		player_body.m_propIndex[0] |= 0x80;
	}

	if ( pUnit->IsPlayer() && pUnit->ToPlayer()->IsGenPVP(pPlayer) )
		memset(player_body.name, 0, MAX_CHARACTER_LENGTH);
	else
		memcpy(player_body.name, pPlayer->GetName(), MAX_CHARACTER_LENGTH);

	Util::memcpy(player_body.preview, pPlayer->preview, MAX_PREVIEW_DATA);

	if (pPlayer->GetViewState() == VIEW_STATE_TELEPORT)
	{
		player_body.preview[17] |= 0x20;
	}
	else if (pPlayer->IsRest())
	{
		player_body.preview[17] |= pPlayer->GetViewState() << 5;
	}

	player_body.Muun01.SetID(pPlayer->GetMuunInventory()->GetItem(0)->GetItem());
	player_body.Muun02.SetID(pPlayer->GetMuunInventory()->GetItem(1)->GetItem());
	player_body.Muun03.SetID(pPlayer->GetMuunInventory()->GetItem(2)->GetItem());
	player_body.SetMount(pPlayer->GetVisibleMount());
	player_body.DisplayMuun = pPlayer->GetDisplayMuunSlot();
	player_body.server = 0;
	player_body.Muun01.data = pPlayer->IsMuunItemStatus(0);
	player_body.Muun02.data = pPlayer->IsMuunItemStatus(1);
	player_body.Muun03.data = pPlayer->IsMuunItemStatus(2);
	player_body.dark_spirit = pPlayer->IsActiveInventoryItem(PET_DARK_RAVEN) ? 1 : 0;

	player_body.elemental_attribute = pPlayer->GetElementalAttribute();

	player_body.SetLevel(0);

	player_body.hp_max[0] = 0;
	player_body.hp_max[1] = 0;
	player_body.hp_max[2] = 0;
	player_body.hp_max[3] = 0;

	player_body.hp[0] = 0;
	player_body.hp[1] = 0;
	player_body.hp[2] = 0;
	player_body.hp[3] = 0;

	player_body.unk61 = 0;
	player_body.unk64 = 0; ///- TODO: Test Value
	player_body.unk66 = 0;
	player_body.unk67 = 0;

	if ( pPlayer->GetWorldId() == WORLD_LAST_MAN_STANDING && sGameServer->IsLastManStandingHideCharacter() )
	{
		memset(player_body.name, 0, MAX_CHARACTER_LENGTH);
		player_body.elemental_attribute = ELEMENTAL_ATTRIBUTE_NONE;
	}
	
	player_body.effect_count = pPlayer->GetActiveBuffCount();

	memcpy(&buffer[size], &player_body, sizeof(player_body));

	size += sizeof(VIEWPORT_CREATE);
		
	ViewportConstructBuff(pPlayer, buffer, size);

	buffer[size] = 0xC4;
	++size;
}

void Object::ViewportConstructPlayerChange(Object* pUnit, Player* pPlayer, uint8 * buffer, uint16 & size)
{
	SafeAssert(pUnit, "pUnit == nullptr");
	SafeAssert(pPlayer, "pPlayer == nullptr");
	SafeAssert(pPlayer->IsPlayer(), "pPlayer->IsPlayer() == false");
	SafeAssert(pPlayer->ToPlayer(), "pPlayer->ToPlayer() == nullptr");

	VIEWPORT_CREATE_CHANGE body;

	body.SetIndex(pPlayer->GetEntry());
	body.x = pPlayer->GetX();
	body.y = pPlayer->GetY();
	body.tx = pPlayer->GetTX();
	body.ty = pPlayer->GetTY();
	body.dir_and_pk_level = pPlayer->GetDirection() << 4;

	if ((pPlayer->GetWorldId() == WORLD_LAST_MAN_STANDING) || (pPlayer->GetWorldId() == WORLD_SPECIAL_EVENT_MAP && sGameServer->IsSpecialMapPVP()))
	{
		body.dir_and_pk_level |= PK_STATUS_MURDER_LVL_3;
	}
	else
	{
		body.dir_and_pk_level |= pPlayer->GetPKLevel() & 0x0F;
	}

	if ( pPlayer->GetState() == OBJECT_STATE_STAND_BY && !pPlayer->IsTeleporting() )
	{
		body.m_propIndex[0] |= 0x80;
	}

	if ( (pUnit->IsPlayer() && pUnit->ToPlayer()->IsGenPVP(pPlayer)) )
		memset(body.name, 0, MAX_CHARACTER_LENGTH);
	else
		memcpy(body.name, pPlayer->GetName(), MAX_CHARACTER_LENGTH);

	Util::memcpy(body.preview, pPlayer->preview, MAX_PREVIEW_DATA);

	if (pPlayer->GetViewState() == VIEW_STATE_TELEPORT)
	{
		body.preview[17] |= 0x20;
	}
	else if (pPlayer->IsRest())
	{
		body.preview[17] |= pPlayer->GetViewState() << 5;
	}

	body.SetSkin(pPlayer->GetSkin());

	body.Muun01.SetID(pPlayer->GetMuunInventory()->GetItem(0)->GetItem());
	body.Muun02.SetID(pPlayer->GetMuunInventory()->GetItem(1)->GetItem());
	body.Muun03.SetID(pPlayer->GetMuunInventory()->GetItem(2)->GetItem());
	body.SetMount(pPlayer->GetVisibleMount());
	body.DisplayMuun = pPlayer->GetDisplayMuunSlot();
	body.server = 0;
	body.Muun01.data = pPlayer->IsMuunItemStatus(0);
	body.Muun02.data = pPlayer->IsMuunItemStatus(1);
	body.Muun03.data = pPlayer->IsMuunItemStatus(2);
	body.dark_spirit = pPlayer->IsActiveInventoryItem(PET_DARK_RAVEN) ? 1 : 0;

	body.elemental_attribute = pPlayer->GetElementalAttribute();

	body.SetLevel(0);

	body.hp_max[0] = 0;
	body.hp_max[1] = 0;
	body.hp_max[2] = 0;
	body.hp_max[3] = 0;

	body.hp[0] = 0;
	body.hp[1] = 0;
	body.hp[2] = 0;
	body.hp[3] = 0;

	body.unk63 = 0;
	body.unk66 = 0; ///- TODO: Test Value

	if ( pPlayer->GetWorldId() == WORLD_LAST_MAN_STANDING && sGameServer->IsLastManStandingHideCharacter() )
	{
		memset(body.name, 0, MAX_CHARACTER_LENGTH);
		body.elemental_attribute = ELEMENTAL_ATTRIBUTE_NONE;
	}

	body.effect_count = pPlayer->GetActiveBuffCount();

	memcpy(&buffer[size], &body, sizeof(body));

	size += sizeof(VIEWPORT_CREATE_CHANGE);

	ViewportConstructBuff(pPlayer, buffer, size);

	buffer[size] = 0xC4;
	++size;
}

void Object::ViewportConstructCastleMachine(Object* pUnit, Unit* pConstruct, uint8 * buffer, uint16 & size)
{
	SafeAssert(pUnit, "pUnit == nullptr");
	SafeAssert(pConstruct, "pConstruct == nullptr");

	CASTLE_SIEGE_MACHINE_NOTIFY_BODY body;

	body.SetIndex(pConstruct->GetEntry());
	body.type = pConstruct->GetSendType();
	body.x = pConstruct->GetX();
	body.y = pConstruct->GetY();

	if ( pConstruct->IsPlayer() )
	{
		body.m_propClass[0] = CS_SET_CLASS(pConstruct->ToPlayer()->GetClass());
		body.m_propClass[1] = 0;
	}
	else if ( pConstruct->IsCreature() )
	{
		body.SetClass(pConstruct->ToCreature()->GetClass());
	}

	if ( pConstruct->IsPlayer() )
	{
		Util::memcpy(body.preview, &pConstruct->ToPlayer()->preview[1], MAX_PREVIEW_DATA - 1);
	}
	else
	{
		memset(body.preview, 0x00, MAX_PREVIEW_DATA - 1);
	}

	body.effect_count = 0;

	memcpy(&buffer[size], &body, sizeof(body));

	size += sizeof(CASTLE_SIEGE_MACHINE_NOTIFY_BODY);
}

void Object::ViewportConstructWorldItem(Object* pUnit, WorldItem* pWorldItem, uint8 * buffer, uint16 & size)
{
	VIEWPORT_CREATE_ITEM body;
	
	body.SetIndex(pWorldItem->GetEntry());

	if ( pWorldItem->GetState() == WORLD_ITEM_STATE_STANDBY )
	{
		body.m_propIndex[0] |= 0x80;
	}

	body.x = cast(uint8, pWorldItem->GetX());
	body.y = cast(uint8, pWorldItem->GetY());
	
	pWorldItem->item.ConvertToBuffer(body.item_info);

	memcpy(&buffer[size], &body, sizeof(body));

	size += sizeof(VIEWPORT_CREATE_ITEM);
}

void Object::ViewportConstructBuff(Object* pUnit, uint8 * buffer, uint16 & size)
{
	if ( !pUnit || !pUnit->IsUnit() )
		return;

	BUFF_LOOP(i)
	{
		BuffData * data = pUnit->ToUnit()->GetBuffData(i);

		if ( data->GetID() == 0 )
			continue;

		VIEWPORT_BUFF_DATA * buff_data = (VIEWPORT_BUFF_DATA*)&buffer[size];

		buff_data->buff = data->GetID();
		buff_data->unk1 = 0;
		buff_data->time = 0;
		size += sizeof(VIEWPORT_BUFF_DATA);
	}
}

void Object::ViewportClear()
{
	if ( !this->viewport_data.empty() )
	{
		ViewportData* pViewportData = nullptr;

		for ( ViewportDataList::iterator it = this->viewport_data.begin(); it != this->viewport_data.end(); ++it )
		{
			pViewportData = it->second;

			if ( !pViewportData )
				continue;

			if ( pViewportData->GetStatus() != Viewport::STATUS_NONE )
			{
				if ( this->IsViewportControl() ) /// - Si tiene control de viewport debo eliminar la visual
				{
					this->ViewportSendObject(pViewportData->GetObjectData(), true);
					this->OnObjectRemove(pViewportData->GetObjectData());
				}
			}
		}
	}

	MAP_CLEAR(ViewportDataList::iterator, this->viewport_data);
	this->viewport_removed_map.clear();
	this->SetViewportEmpty(true);
}

void Object::ViewportClose()
{
	if ( this->GetConnectStatus() == CONNECT_STATUS_PLAYING && !this->viewport_data.empty() )
	{
		Object* pObject = nullptr;
		ViewportData* data = nullptr;

		for ( ViewportDataList::const_iterator it = this->viewport_data.begin(); it != this->viewport_data.end(); ++it )
		{
			data = it->second;

			if ( data )
			{
				pObject = data->GetObjectData();

				if ( data->GetStatus() != Viewport::STATUS_NONE && pObject && pObject->GetType() != OBJECT_TYPE_WORLD_ITEM )
				{
					pObject->ViewportRemove(this);
				}
			}
		}
	}

	this->ViewportClear();
}

bool Object::ViewportAttack(Object* pTarget)
{
	if ( !pTarget )
		return false;

	if ( this == pTarget )
		return true;

	ViewportDataList::const_iterator it = this->viewport_data.find(pTarget->GetHighGUID());

	/// - Reviso si lo tengo en VP y está activo
	if ( it != this->viewport_data.end() )
	{
		if ( it->second->GetStatus() != Viewport::STATUS_READY )
			return false;
	}
	else
	{
		return false;
	}

	if ( Unit* pUnit = this->ToUnit() )
	{
		if ( pUnit->IsTeleporting() )
			return true;
	}

	it = pTarget->viewport_data.find(this->GetHighGUID());

	/// - Reviso si me tiene en VP y estoy activo
	if ( it != pTarget->viewport_data.end() )
	{
		if ( it->second->GetStatus() != Viewport::STATUS_READY )
			return false;
	}
	else
	{
		return false;
	}

	return true;
}

void Object::OnObjectAdd(Object* pObject)
{
	if ( !pObject )
		return;

	if ( pObject->IsViewportControl() )
		return;

	pObject->ViewportAdd(this);
}
	
void Object::OnObjectRemove(Object* pObject)
{
	if ( !pObject )
		return;

	if ( pObject->IsViewportControl() )
		return;

	ViewportDataList::const_iterator it = pObject->viewport_data.find(this->GetHighGUID());

	/// - Solamente borrarlo cuando existe
	if ( it != pObject->viewport_data.end() )
	{
		if ( it->second )
			delete it->second;
		pObject->viewport_data.erase(it);
	}
}