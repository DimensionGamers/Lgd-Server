WorldItem::WorldItem(uint16 entry): Object(entry)
{
	this->SetType(OBJECT_TYPE_WORLD_ITEM);
	this->Clear();
}

WorldItem::~WorldItem()
{

}

void WorldItem::Clear()
{
	this->item.clear();
	this->SetState(WORLD_ITEM_STATE_NONE);
	this->SetOwner(nullptr);
	this->SetOnlyOwner(false);
	this->SetVisible(false);
	this->GetLiveTime()->Reset();
	this->GetLootingTime()->Reset();
	this->Init();
}

bool WorldItem::EventFinished() const
{
	if ( this->GetEventId() == EVENT_NONE )
		return false;

	switch ( this->GetEventId() )
	{
	case EVENT_BLOOD_CASTLE:
		{
			if ( sBloodCastleMgr->GetGround(this->GetEventGround())->GetState() == BLOOD_CASTLE_STATE_NONE )
				return true;
		} break;

	case EVENT_DEVIL_SQUARE:
		{
			if ( sDevilSquareMgr->GetGround(this->GetEventGround())->GetState() == DEVIL_SQUARE_STATE_NONE )
				return true;
		} break;

	case EVENT_CHAOS_CASTLE:
		{
			if ( sChaosCastleMgr->GetGround(this->GetEventGround())->GetState() == CHAOS_CASTLE_STATE_NONE )
				return true;
		} break;

	case EVENT_ILLUSION_TEMPLE:
		{
			if ( sIllusionTempleMgr->GetGround(this->GetEventGround())->GetState() == ILLUSION_TEMPLE_STATE_NONE )
				return true;
		} break;

	case EVENT_CHAOS_CASTLE_SURVIVAL:
		{
			if ( sChaosCastleSurvivalMgr->GetGround()->GetState() == CHAOS_CASTLE_STATE_NONE )
				return true;
		} break;

	case EVENT_INSTANCED_DUNGEON:
	{
		if (!sDungeon->FindInstance(this->GetInstance()))
		{
			return true;
		}
	} break;
	}

	return false;
}

bool WorldItem::GiveCheck(Player * pPlayer)
{
	if ( !this->IsVisible() && this->GetOwner() != pPlayer )
	{
		return false;
	}

	if ( !this->item.IsItem() )
	{
		return false;
	}

	if ( !this->IsLive() )
	{
		return false;
	}

	if ( !pPlayer->GetHelper()->IsStarted() || !pPlayer->GetHelper()->IsOffline() )
	{
		if ( !IN_RANGE(this, pPlayer, 3) )
		{
			return false;
		}
	}

	bool success = true;

	if ( !pPlayer->IsAdministrator() )
	{
		if ( this->item.IsQuestItem() && this->GetOwner() != pPlayer )
		{
			success = false;
		}
		else if ( this->IsOnlyOwner() && this->GetOwner() != pPlayer )
		{
			success = false;
		}
		else if ( this->GetOwner() && this->GetOwner() != pPlayer && !this->GetLootingTime()->Finished() )
		{
			if ( !this->GetOwner()->IsPlayer() )
			{
				success = false;
			}
			else if ( this->GetOwner()->ToPlayer()->GetPartyID() == PARTY_NULL || this->GetOwner()->ToPlayer()->GetPartyID() != pPlayer->GetPartyID() )
			{
				success = false;
			}
		}
	}

	if ( !success )
	{
		pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "This item doesn't belong to you %s", pPlayer->GetName());
	}
	
	return success;
}

void WorldItem::Give()
{
	this->SetState(WORLD_ITEM_STATE_DELETE);
	this->SetLive(false);
}

bool WorldItem::Update(uint32 diff)
{
	this->UpdateTimer();

	if ( !this->IsSecondPassed() )
		return true;

	this->CreateFrustrum();

	switch ( this->GetState() )
	{
	case WORLD_ITEM_STATE_STANDBY:
		{
			this->SetState(WORLD_ITEM_STATE_READY);
		} break;

	case WORLD_ITEM_STATE_READY:
		{
			if ( this->IsVisible() )
			{
				if (!sDungeon->IsDungeon(this->GetWorldId()))
				{
					if (!this->item.IsEventArchangelWeapon())
					{
						if (this->GetLiveTime()->Elapsed())
						{
							this->SetState(WORLD_ITEM_STATE_DELETE);

							//sLog->outInfo(LOG_ITEMS, "[ Item Dissapeared ] %s", this->item.BuildLog(0).c_str());
						}
					}
					else if (this->GetEventId() == EVENT_NONE)
					{
						this->SetState(WORLD_ITEM_STATE_DELETE);

						//sLog->outInfo(LOG_ITEMS, "[ Item Dissapeared ] %s", this->item.BuildLog(0).c_str());
					}
				}
			}

			if ( !this->item.IsItem() )
			{
				this->SetState(WORLD_ITEM_STATE_DELETE);
			}

			if ( this->EventFinished() )
			{
				this->SetState(WORLD_ITEM_STATE_DELETE);
			}
		} break;

	case WORLD_ITEM_STATE_DELETE:
		{
			this->Clear();
		} break;
	}

	return true;
}

void WorldItem::MakeVisible()
{
	this->GetLiveTime()->Start();
	this->GetLootingTime()->Start();
	this->SetVisible(!this->IsVisible());
}