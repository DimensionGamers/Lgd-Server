/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerMix.cpp"
*
*/

void Player::ChaosMixButton(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this, false) )
	{
		return;
	}

	MixHandler(this, 0).ExecuteMix(Packet);
}

void Player::ChaosMixClose()
{
	if ( !this->IsPlaying() || !this->IsLive() )
		return;

	if ( this->GetInterfaceState()->GetID() == InterfaceData::None )
	{
		return;
	}

	sLog->outInfo(LOG_PLAYER, "Closing %s's Mix interface (%s - %d)",
		this->GetName(), this->GetInterfaceState()->GetID().c_str(), this->GetInterfaceState()->GetState());

	if ( this->GetInterfaceState()->GetID() == InterfaceData::AcheronEntrance )
	{
		this->GetInterfaceState()->Reset();
		return;
	}

	this->ChaosMixCloseResult();

	this->MixSafeItemRecoverAttempt();
	
	this->GetInterfaceState()->Reset();
}

void Player::ChaosMixCloseResult()
{
	CHAOS_MIX_CLOSE_RESULT pMsg;
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::ChaosMixSend(uint8 type, bool state)
{
	if ( type == 0 && !this->GetInterfaceState()->IsCommonMix() )
		return;

	if ( type == 1 && this->GetInterfaceState()->GetID() != InterfaceData::PetTrainer )
		return;
	
	if ( state && this->GetInterfaceState()->GetState() == 1 )
		return;

	uint8 buffer[2000];
	POINTER_PCT(CHAOS_MIX_ITEM, head, buffer, 0);
	POINTER_PCT(CHAOS_MIX_ITEM_BODY, body, buffer, sizeof(CHAOS_MIX_ITEM));
	head->count = 0;

	chaos_box_loop(i)
	{
		if ( !this->GetMixInventory()->GetItem(i)->IsItem() )
			continue;

		body[head->count].slot = i;
		this->GetMixInventory()->GetItem(i)->ConvertToBuffer(body[head->count].item_info);

		head->count++;
	}

	head->h.set(0x31, sizeof(CHAOS_MIX_ITEM) + (head->count * sizeof(CHAOS_MIX_ITEM_BODY)));
	head->type = type == 1 ? 5 : 3;

	this->sendPacket(buffer, head->h.get_size());
}

void Player::ChaosMixResult(uint8 result, uint8 * item_info, uint8 slot)
{
	CHAOS_MIX_RESULT pMsg(result, item_info, slot);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::ChaosMixPentagramResult(uint8 result, uint8 gremory)
{
	MIX_PENTAGRAM_RESULT pMsg(result, gremory);
	this->SEND_PCT(pMsg);
}

void Player::ChaosMixSocketResult(uint8 result, uint8 gremory, uint8 slot)
{
	SOCKET_MIX_RESULT pMsg(result, gremory, slot);
	this->SEND_PCT(pMsg);
}

void Player::ChaosMixMassCombination(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this, false) )
	{
		return;
	}

	MixHandler(this, 1).ExecuteMassCombination(Packet);
}

void Player::ChaosMixMassCombinationResult(uint8 result)
{
	CHAOS_MIX_MASS_COMBINATION_RESULT pMsg(result);
	this->SEND_PCT(pMsg);
}

void Player::MixSafeItemRecoverAttempt()
{
	if ( sGameServer->IsChaosMachineSafeItemMove() )
	{
		chaos_box_loop(i)
		{
			Item & item = this->GetMixInventory()->item[i];

			if ( !item.IsItem() )
			{
				continue;
			}

			item.SetExpireDate(time(nullptr) + (14 * DAY));
			item.SetPersonalStorePrice(GremoryCaseReward::EVENT);

			uint8 result = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->AddItem(item);

			if ( result == uint8(-1) )
			{
				sLog->outInfo(LOG_CHAOS_MIX,"%s -- %s ITEM -> %s",
					__FUNCTION__, this->BuildLog().c_str(), item.BuildLog(i).c_str());
			}
		}
	}
	
	this->GetMixInventory()->Clear();
	this->SetMixCompleted(false);
}