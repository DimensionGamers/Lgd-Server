/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerStatFruit.cpp"
*
*/

uint8 ConvertFruitToStat(uint8 level)
{
	return (level == STAT_FRUIT_ENERGY) ? ENERGY: 
		   (level == STAT_FRUIT_VITALITY) ? VITALITY: 
		   (level == STAT_FRUIT_AGILITY) ? AGILITY: 
		   (level == STAT_FRUIT_STRENGTH) ? STRENGTH:
		   LEADERSHIP;
}

uint8 ConvertFruitToStatItem(uint16 item)
{
	return (item == ITEMGET(13, 57)) ? STAT_FRUIT_ENERGY: 
		   (item == ITEMGET(13, 56)) ? STAT_FRUIT_VITALITY: 
		   (item == ITEMGET(13, 55)) ? STAT_FRUIT_AGILITY: 
		   (item == ITEMGET(13, 54)) ? STAT_FRUIT_STRENGTH:
		   STAT_FRUIT_LEADERSHIP;
}

void Player::UsePlusStatFruit(uint8 slot)
{
	uint8 level = this->GetInventory()->item[slot].GetLevel();

	if ( !this->UseStatFruitAllowed(level) )
	{
		this->UseStatFruitResult(STAT_FRUIT_RESULT_ADD_FAILED,level);
		return;
	}

	if ( !this->CanUsePlusStatFruit() )
	{
		this->UseStatFruitResult(STAT_FRUIT_RESULT_POINT_AT_MAXIMUM,level);
		return;
	}

	uint8 real_stat = ConvertFruitToStat(level);

	if ( this->GetStat(real_stat) >= sGameServer->GetMaxStat(real_stat) )
	{
		this->UseStatFruitResult(STAT_FRUIT_RESULT_POINT_AT_MAXIMUM,level);
		return;
	}

	this->ClearItem(slot);

	int32 add_point = this->GetFruitPointsAdd();
	uint32 max_add_point = sCharacterBase->GetStatFruitMaxAdd(this->GetClass(),this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel());

	int32 rate = 100 * add_point / max_add_point;
	int32 success_rate = 0;

	if ( add_point <= 10 )
		success_rate = 100;
	else if ( rate <= 10 )
		success_rate = 70;
	else if ( rate <= 30 )
		success_rate = 60;
	else if ( rate <= 50 )
		success_rate = 50;
	else
		success_rate = 40;

	int32 add_stat = 0;

	if ( RAND%100 < success_rate )
	{
		int32 add_stat_rate = RAND%100;

		if ( add_stat_rate < 70 )
			add_stat = 1;
		else if ( add_stat_rate < 95 )
			add_stat = 2;
		else
			add_stat = 3;

		if ( (add_stat + add_point) > max_add_point )
			add_stat = max_add_point - add_point;

		if ( (this->GetStat(real_stat) + add_stat) >= sGameServer->GetMaxStat(real_stat) )
			add_stat = sGameServer->GetMaxStat(real_stat) - this->GetStat(real_stat);

		if ( add_stat < 0 )
			add_stat = 0;

		this->IncreaseFruitPointsAdd(add_stat);
		this->UseStatFruitResult(STAT_FRUIT_RESULT_ADDED, level, add_stat);
	}
	else
	{
		this->UseStatFruitResult(STAT_FRUIT_RESULT_ADD_FAILED, level);
		return;
	}

	if ( add_stat <= 0 )
		return;

	this->IncreaseStat(real_stat, add_stat);
	this->CalculateCharacter();
}
	
void Player::UseMinusStatFruit(uint8 slot)
{
	uint8 level = this->GetInventory()->item[slot].GetLevel();

	if ( !this->UseStatFruitAllowed(level) )
	{
		this->UseStatFruitResult(STAT_FRUIT_RESULT_DECREASE_FAILED,level);
		return;
	}

	if ( !this->CanUseMinusStatFruit() )
	{
		this->UseStatFruitResult(STAT_FRUIT_RESULT_POINT_AT_MAXIMUM_2,level);
		return;
	}

	uint8 real_stat = ConvertFruitToStat(level);

	CharacterBaseData const* char_info = sCharacterBase->GetCharacterBase(this->GetClass());

	if ( !char_info )
	{
		this->UseStatFruitResult(STAT_FRUIT_RESULT_DECREASE_FAILED,level);
		return;
	}

	if ( this->GetStat(real_stat) <= char_info->GetStat(real_stat) )
	{
		this->UseStatFruitResult(STAT_FRUIT_RESULT_DECREASE_DEFAULT_STAT,level);
		return;
	}

	this->ClearItem(slot);

	int32 dec_point = this->GetFruitPointsDec();
	int32 max_dec_point = sCharacterBase->GetStatFruitMaxDec(this->GetClass(),this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel());

	int32 rate = 100 * dec_point / max_dec_point;
	int32 success_rate = 0;

	if( dec_point <= 10 )
		success_rate = 100;
	else if ( rate <= 10 )
		success_rate = 70;
	else if ( rate <= 30 )
		success_rate = 60;
	else if ( rate <= 50 )
		success_rate = 50;
	else
		success_rate = 40;

	int32 dec_stat = 0;

	if ( RAND%100 < success_rate )
	{
		int32 dec_stat_rate = RAND%100;

		if(dec_stat_rate < 50)
			dec_stat = 1;
		else if(dec_stat_rate < 75)
			dec_stat = 3;
		else if(dec_stat_rate < 91)
			dec_stat = 5;
		else if(dec_stat_rate < 98)
			dec_stat = 7;
		else
			dec_stat = 9;

		if ( (dec_point + dec_stat) >= max_dec_point )
			dec_stat = max_dec_point - dec_point;

		if ( (this->GetStat(real_stat) - dec_stat) < char_info->GetStat(real_stat) )
			dec_stat = this->GetStat(real_stat) - char_info->GetStat(real_stat);

		if ( dec_stat < 0 )
			dec_stat = 0;

		this->IncreaseFruitPointsDec(dec_stat);
		this->UseStatFruitResult(STAT_FRUIT_RESULT_DECREASED, level, dec_stat);
	}
	else
	{
		this->UseStatFruitResult(STAT_FRUIT_RESULT_DECREASE_FAILED,level);
		return;
	}

	if ( dec_stat <= 0 )
		return;

	this->ReduceStat(real_stat, dec_stat);
	this->GetLevelData(LEVEL_DATA_NORMAL)->IncreasePoints(dec_stat);
	this->CalculateCharacter();
}

bool Player::UseStatFruitAllowed(uint8 level)
{
	if ( this->GetClass() >= Character::MAX_CLASS )
		return false;

	if ( level >= STAT_FRUIT_MAX )
		return false;

	if ( level == STAT_FRUIT_LEADERSHIP && this->GetClass() != Character::DARK_LORD )
		return false;

	if (this->IsItemEquipped())
	{
		this->UseStatFruitResult(STAT_FRUIT_RESULT_DECREASE_EQUIPMENT, level);
		return false;
	}

	return true;
}

void Player::UseStatFruitResult(uint8 result, uint8 fruit, uint32 stat)
{
	USE_STAT_FRUIT pMsg(result, fruit, stat);
	this->sendPacket(MAKE_PCT(pMsg));
}

bool Player::CanUsePlusStatFruit()
{
	uint32 max_stat = sCharacterBase->GetStatFruitMaxAdd(this->GetClass(),this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel());

	return (this->GetFruitPointsAdd() < max_stat);
}
	
bool Player::CanUseMinusStatFruit()
{
	uint32 max_stat = sCharacterBase->GetStatFruitMaxDec(this->GetClass(),this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel());

	return (this->GetFruitPointsDec() < max_stat);
}

void Player::UseCashShopFruit(uint8 slot)
{
	Item const* pItem = this->GetInventory()->GetItem(slot);

	uint8 stat = ConvertFruitToStatItem(pItem->GetItem());
	uint8 real_stat = ConvertFruitToStat(stat);

	if (this->IsItemEquipped())
	{
		this->UseStatFruitResult(STAT_FRUIT_RESULT_DECREASE_EQUIPMENT, stat, 0);
		return;
	}

	if (real_stat == LEADERSHIP && this->GetClass() != Character::DARK_LORD)
	{
		this->UseStatFruitResult(7, stat);
		return;
	}

	CharacterBaseData const* char_info = sCharacterBase->GetCharacterBase(this->GetClass());

	if ( !char_info )
	{
		this->UseStatFruitResult(7, stat);
		return;
	}

	int32 original_stat = char_info->GetStat(real_stat);
	int32 dec_stat = static_cast<int32>(pItem->GetDurability() * 10.0f);
	int32 stat_difference = this->GetStat(real_stat) - dec_stat;
	
	if ( stat_difference < original_stat )
	{
		dec_stat = this->GetStat(real_stat) - original_stat;
	}

	if ( dec_stat <= 0 )
	{
		this->UseStatFruitResult(STAT_FRUIT_RESULT_DECREASE_DEFAULT_STAT, stat);
		return;
	}

	this->ClearItem(slot);

	this->UseStatFruitResult(STAT_FRUIT_RESULT_EX_DECREASE_SUCCESS, stat, dec_stat);

	this->ReduceStat(real_stat, dec_stat);
	this->GetLevelData(LEVEL_DATA_NORMAL)->IncreasePoints(dec_stat);
	this->CalculateCharacter();
}