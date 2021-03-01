/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerSkillUse.cpp"
*
*/
void Player::NormalAttack(uint8 * Packet)
{
	SafeAssert(Packet, "Packet == nullptr");

	if ( !this->IsActionAllowed(PlayerAction::PLAYER_ACTION_ATTACK_REQUEST) )
		return;

	if ( this->IsInSafeZone() )
	{
		this->ProcessAttackSafe();
		this->PositionSend(this->GetX(),this->GetY());
		return;
	}

	POINTER_PCT_LOG(ATTACK_REQUEST, lpMsg, Packet, 0);

	Object * pVictim = sObjectMgr->FindObject(lpMsg->GetTarget());

	if ( !pVictim || !pVictim->IsPlaying() || !pVictim->IsLive() )
		return;

	if ( pVictim == this )
		return;

	if ( this->IsAdministrator() && pVictim->IsCreature() )
	{
		this->SendNotice(NOTICE_NORMAL_BLUE, "Target Index: %d", pVictim->GetEntry());
		this->SetCurrentTarget(pVictim->ToCreature());
	}

	if (!this->IsSkillCPS(0, lpMsg->attack_time))
	{
		return;
	}

	this->SetDirection(lpMsg->dir_dis);

	if ( Unit* pUnit = pVictim->ToUnit() )
	{
		this->Ping();

		this->AttackProc(pUnit, nullptr, false, 0, 0, 0);
	}

	this->ActionSend(lpMsg->GetTarget(), lpMsg->action, lpMsg->dir_dis);
}

void Player::NormalMagicAttack(uint8 * Packet)
{
	SafeAssert(Packet, "Packet == nullptr");

	if (!this->IsActionAllowed(PlayerAction::PLAYER_ACTION_MAGIC_REQUEST))
	{
		return;
	}

	POINTER_PCT_LOG(NORMAL_MAGIC_ATTACK, lpMsg, Packet, 0);

	if (this->IsInSafeZone() && (lpMsg->GetMagic() == SKILL_NOVA_CHARGE || lpMsg->GetMagic() == SKILL_TREE_ADD_NOVA_START_IMPROVED))
	{
		this->MagicAttackSend(ENTRY(this), SKILL_NOVA);
		this->PositionSend(this->GetX(), this->GetY());
		return;
	}

	if (this->IsInSafeZone() &&
		!BC_MAP_RANGE(this->GetWorldId()) &&
		!CC_MAP_RANGE(this->GetWorldId()) &&
		this->GetWorldId() != WORLD_CASTLE_SIEGE &&
		this->GetWorldId() != WORLD_CRYWOLF_FIRST_ZONE &&
		this->GetWorldId() != WORLD_KANTURU_BOSS &&
		!AW_MAP_RANGE(this->GetWorldId()))
	{
		this->ProcessAttackSafe();
		this->PositionSend(this->GetX(), this->GetY());
		return;
	}

	uint16 magic_tmp = lpMsg->GetMagic();

	if (magic_tmp == SKILL_NOVA_CHARGE)
	{
		magic_tmp = SKILL_NOVA;
	}
	else if (magic_tmp == SKILL_TREE_ADD_NOVA_START_IMPROVED)
	{
		magic_tmp = SKILL_TREE_ADD_NOVA_IMPROVED;
	}

	Skill * mSkill = this->MagicGet(magic_tmp);

	if (!mSkill)
	{
		return;
	}

	skill_template const* skill_info = sSkillMgr->GetSkill(mSkill->GetSkill());

	if (!skill_info)
	{
		return;
	}

	if (!this->IsSkillCPS(skill_info->GetBaseSkill(), lpMsg->attack_time))
	{
		sLog->outError("antihack", "%s -- %s Wrong skill time [Skill: %u][Time: %I64d][Count: %d]", __FUNCTION__, this->BuildLog().c_str(), magic_tmp, lpMsg->attack_time, this->GetSkillCPSCount(skill_info->GetBaseSkill()));
		return;
	}

	if (sGameServer->IsAntiHackSkillHeadcodeCheck())
	{
		if (skill_info->animation == 1)
		{
			sLog->outError("antihack", "%s -- %s Wrong Skill Headcode (%u)", __FUNCTION__, this->BuildLog().c_str(), magic_tmp);
			return;
		}
	}

	if (!this->IsSkillUseAllowed(skill_info))
	{
		return;
	}

	if (skill_info->GetMountCheck() == 2 && this->HasMount())
	{
		return;
	}

	Unit* pUnit = sObjectMgr->GetUnit(lpMsg->GetTarget());

	if (skill_info->GetBaseSkill() != SKILL_NOVA && skill_info->GetBaseSkill() != SKILL_ARCHANGELS_WILL && skill_info->GetBaseSkill() != SKILL_DEFENSE)
	{
		if (!pUnit || !pUnit->IsPlaying() || !pUnit->IsLive())
		{
			return;
		}

		if (!this->GetHelper()->IsStarted() || !this->GetHelper()->IsOffline())
		{
			int32 distance = Util::Distance(this->GetX(), this->GetY(), pUnit->GetX(), pUnit->GetY());
			int32 max_distance = SkillHandler::GetSkillRange(this, skill_info->GetID());

			if (distance > (max_distance + sGameServer->GetAttackRangeTolerance()))
			{
				sGameServer->LogAntiHack(this, ANTIHACK_ID_ATTACK_RANGE, "Wrong Attack Range Detected - Skill: %u - [%d / %d]", skill_info->GetID(), distance, max_distance);
				return;
			}
		}

		if (!this->ViewportAttack(pUnit))
		{
			return;
		}
	}

	this->Ping();

	SkillHandler(this, pUnit, mSkill, skill_info).SkillUseProc((lpMsg->GetMagic() == SKILL_NOVA_CHARGE || lpMsg->GetMagic() == SKILL_TREE_ADD_NOVA_START_IMPROVED) ? true : this->SkillComboCheck(skill_info->GetBaseSkill()));
}

void Player::DurationMagicAttack(uint8 * Packet)
{
	SafeAssert(Packet, "Packet == nullptr");

	POINTER_PCT_LOG(DURATION_MAGIC_ATTACK, lpMsg, Packet, 0);

	if (!this->IsActionAllowed(PlayerAction::PLAYER_ACTION_ATTACK_REQUEST))
	{
		return;
	}

	if (this->IsInSafeZone())
	{
		this->ProcessAttackSafe();
		this->PositionSend(this->GetX(), this->GetY());
		return;
	}

	Unit * target = sObjectMgr->GetUnit(lpMsg->GetTarget());

	Skill * mSkill = this->MagicGet(lpMsg->GetMagic());

	if (!mSkill)
	{
		//sLog->outError("root", "%s :: %s :: Wrong skill %u", __FUNCTION__, this->BuildLog().c_str(), lpMsg->GetMagic());
		return;
	}

	skill_template const* skill_info = sSkillMgr->GetSkill(mSkill->GetSkill());

	if (!skill_info)
	{
		//sLog->outError("root", "%s :: %s :: Wrong skill info %u", __FUNCTION__, this->BuildLog().c_str(), lpMsg->GetMagic());
		return;
	}

	if (!this->IsSkillCPS(skill_info->GetBaseSkill(), lpMsg->attack_time))
	{
		//sLog->outError("root", "%s :: %s :: Wrong skill time %u", __FUNCTION__, this->BuildLog().c_str(), lpMsg->GetMagic());

		sLog->outError("antihack", "%s -- %s Wrong skill time [Skill: %u][Time: %I64d][Count: %d]", __FUNCTION__, this->BuildLog().c_str(), lpMsg->GetMagic(), lpMsg->attack_time, this->GetSkillCPSCount(skill_info->GetBaseSkill()));
		return;
	}

	if (sGameServer->IsAntiHackSkillHeadcodeCheck())
	{
		if (skill_info->animation == 0)
		{
			sLog->outError("antihack", "%s -- %s Wrong Skill Headcode (%u)", __FUNCTION__, this->BuildLog().c_str(), lpMsg->GetMagic());
			return;
		}
	}

	if (!this->IsSkillUseAllowed(skill_info))
	{
		//sLog->outError("root", "%s :: %s :: Skill not allowed %u", __FUNCTION__, this->BuildLog().c_str(), lpMsg->GetMagic());
		return;
	}

	if (skill_info->GetMountCheck() == 2 && this->HasMount())
	{
		return;
	}

	if (lpMsg->x > 255)
	{
		lpMsg->x = lpMsg->x / 100;
	}

	if (lpMsg->y > 255)
	{
		lpMsg->y = lpMsg->y / 100;
	}

	coord_type x = lpMsg->x;
	coord_type y = lpMsg->y;

	/*if (target)
	{
		x = target->GetX();
		y = target->GetY();
	}*/

	if (sGameServer->IsMultiAttackSkillCheck())
	{
		if (!this->GetHelper()->IsStarted() || !this->GetHelper()->IsOffline())
		{
			int32 distance = Util::Distance(this->GetX(), this->GetY(), x, y);
			int32 max_distance = SkillHandler::GetSkillRange(this, skill_info->GetID());

			if (distance > (max_distance + sGameServer->GetAttackRangeTolerance()))
			{
				if (this->IsAdministrator())
				{
					this->SendNotice(CUSTOM_MESSAGE_ID_RED, "%s Wrong Attack Distance Detected: [%d / %d]", __FUNCTION__, distance, max_distance);
				}

				sLog->outError("antihack", "%s --- %s Wrong Attack Range Detected [%s / %d / %d]", __FUNCTION__, this->BuildLog().c_str(), skill_info->GetName().c_str(), distance, max_distance);

				sGameServer->LogAntiHack(this, ANTIHACK_ID_ATTACK_RANGE, "Wrong Attack Range Detected - Skill: %u - [%d / %d]", skill_info->GetID(), distance, max_distance);
				return;
			}
		}
	}

	this->Ping();

	SkillHandler(this, target, mSkill, skill_info).SkillUseProc(this->SkillComboCheck(skill_info->GetBaseSkill()), lpMsg->x, lpMsg->y, lpMsg->Dir, lpMsg->Dis, lpMsg->target_pos, true);

	if (lpMsg->MagicKey)
	{
		this->GetDurationMagicKey()->ApplyDurationTime(lpMsg->MagicKey, GetTickCount());
	}
}

void Player::MultiTargetMagicAttack(uint8 * Packet)
{
	if (!this->IsActionAllowed(PlayerAction::PLAYER_ACTION_ATTACK_REQUEST))
	{
		return;
	}

	if (this->IsInSafeZone())
	{
		//this->ProcessAttackSafe();
		//this->PositionSend(this->GetX(), this->GetY());
		return;
	}

	POINTER_PCT(MULTITARGET_MAGIC_ATTACK_HEAD, head, Packet, 0);

	if (head->Count > 5)
	{
		head->Count = 5;
	}

	if (head->h.get_size() < GET_PACKET_SIZE(MULTITARGET_MAGIC_ATTACK_HEAD, MULTITARGET_MAGIC_ATTACK_BODY, head->Count))
	{
		sLog->outError("antihack", "%s(PACKET SIZE ERROR) -- %s", __FUNCTION__, this->BuildLog().c_str());
		return;
	}

	POINTER_PCT(MULTITARGET_MAGIC_ATTACK_BODY, body, Packet, sizeof(MULTITARGET_MAGIC_ATTACK_HEAD));

	uint16 skill = head->GetMagic();

	Skill * mSkill = this->MagicGet(skill);

	if (!mSkill)
	{
		return;
	}

	skill_template const* skill_info = sSkillMgr->GetSkill(mSkill->GetSkill());

	if (!skill_info)
	{
		return;
	}

	if (!this->IsSkillUseAllowed(skill_info))
	{
		return;
	}

	/*if ( this->GetLastDurationSkill() != skill )
		return;*/

	int32 max_count = sGameServer->GetMultiAttackCount();
	coord_type x = this->GetLastDurationSkillX();
	coord_type y = this->GetLastDurationSkillY();

	if (skill_info->GetBaseSkill() == SKILL_PLASMA_BALL)
	{
		max_count = sGameServer->GetSkillPlasmaBallAttackCount();
	}
	else if (skill_info->GetBaseSkill() == SKILL_LIGHTNING_STORM)
	{
		max_count = 1;
	}

	if (!sGameServer->IsMultiAttackNew())
	{
		if (max_count > 0 && this->GetLastDurationSkillCount() >= max_count)
		{
			return;
		}

		if ((GetTickCount() - this->GetLastDurationSkillTime()) > sGameServer->GetMultiAttackTime())
		{
			return;
		}

		this->IncreaseLastDurationSkillCount(1);
	}
	else
	{
		SkillTime* pSkillTime = this->GetSkillTime(skill_info->GetBaseSkill());

		if (!pSkillTime)
		{
			return;
		}

		if (max_count > 0 && pSkillTime->GetCount() >= max_count)
		{
			return;
		}

		if (pSkillTime->GetTime()->GetElapsed() > sGameServer->GetMultiAttackTime())
		{
			return;
		}

		pSkillTime->IncreaseCount(1);
		x = pSkillTime->GetX();
		y = pSkillTime->GetY();
	}

	if (skill_info->GetBaseSkill() == SKILL_LIGHTNING_STORM)
	{
		if (head->Count > 0)
		{
			SkillHandler(this, nullptr, mSkill, nullptr, x, y).SkillLightningStormSplash(false, sObjectMgr->GetUnit(body[0].GetTarget()));
		}
		else
		{
			SkillHandler(this, nullptr, mSkill, nullptr, x, y).SkillLightningStormSplash(false);
		}
		
		return;
	}

	if (skill_info->GetBaseSkill() != SKILL_TWISTER && // Caster
		skill_info->GetBaseSkill() != SKILL_FLAME && // Target
		skill_info->GetBaseSkill() != SKILL_EVIL_SPIRIT && // Caster
		skill_info->GetBaseSkill() != SKILL_HELL_FIRE && // Caster
		skill_info->GetBaseSkill() != SKILL_AQUA_BEAM && // Caster
		skill_info->GetBaseSkill() != SKILL_BLAST && // Target
		skill_info->GetBaseSkill() != SKILL_INFERNO && // Caster
		skill_info->GetBaseSkill() != SKILL_TRIPLE_SHOT && // Caster
		skill_info->GetBaseSkill() != SKILL_IMPALE &&
		skill_info->GetBaseSkill() != SKILL_PENETRATION && // Caster
		skill_info->GetBaseSkill() != SKILL_FIRE_SLASH && // Caster
		skill_info->GetBaseSkill() != SKILL_FIRE_SCREAM && // Caster
		skill_info->GetBaseSkill() != SKILL_MULTI_SHOT &&
		skill_info->GetBaseSkill() != SKILL_MAGIC_ARROW &&
		skill_info->GetBaseSkill() != SKILL_PLASMA_BALL &&
		skill_info->GetBaseSkill() != SKILL_SWORD_INERTIA &&
		skill_info->GetBaseSkill() != SKILL_DARK_PLASMA)
	{
		return;
	}

	for (uint8 i = 0; i < head->Count; ++i)
	{
		uint16 target = body[i].GetTarget();

		for (uint8 h = 0; h < head->Count; ++h)
		{
			if (i != h)
			{
				if (target == body[h].GetTarget())
				{
					KICK_PLAYER(this, "Multi Attack Hack");

					sGameServer->LogAntiHack(this, ANTIHACK_ID_KICK, "Multi Attack Hack");
					return;
				}
			}
		}
	}

	int32 hit_count = 1;

	if (skill_info->GetBaseSkill() == SKILL_MAGIC_ARROW)
	{
		hit_count = 2;
	}

	hit_count += this->GetMajesticDataValueBySkill(mSkill->GetSkill(), MAJESTIC_LINKED_SKILL_ADDITIONAL_HIT);

	for (uint8 i = 0; i < head->Count; ++i)
	{
		uint16 target = body[i].GetTarget();

		if (skill_info->GetBaseSkill() == SKILL_EVIL_SPIRIT)
		{
			if (!this->GetDurationMagicKey()->IsValidDurationTime(body[i].MagicKey))
			{
				//sLog->outError("antihack", "[ MULTI ATTACK ] %s Evil Spirit Invalid DurationTime Key = %d ( Time: %d )",
				//	this->BuildLog().c_str(), body[i].MagicKey, this->GetDurationMagicKey()->GetValidDurationTime(body[i].MagicKey));
				continue;
			}

			if (!this->GetDurationMagicKey()->IsValidCount(body[i].MagicKey))
			{
				//sLog->outError("antihack", "[ MULTI ATTACK ] %s Evil Spirit Invalid ValidCount = %d ( Count: %d )",
				//	this->BuildLog().c_str(), body[i].MagicKey, this->GetDurationMagicKey()->GetValidCount(body[i].MagicKey));
				continue;
			}
		}

		Unit * mTarget = sObjectMgr->GetUnit(target);

		if (!mTarget)
		{
			continue;
		}

		if (sGameServer->IsMultiAttackSkillCheck() && skill_info->GetBaseSkill() != SKILL_PLASMA_BALL && skill_info->GetBaseSkill() != SKILL_DARK_PLASMA && skill_info->GetBaseSkill() != SKILL_TWISTER)
		{
			int32 distance = Util::Distance(mTarget->GetX(), mTarget->GetY(), x, y);
			int32 max_distance = SkillHandler::GetSkillRange(this, skill_info->GetID());

			if (max_distance <= 0 || skill_info->GetBaseSkill() == SKILL_FLAME)
			{
				max_distance = SkillHandler::GetSkillRadio(this, skill_info->GetID());
			}

			if (distance > (max_distance + sGameServer->GetAttackRangeTolerance()))
			{
				if (this->IsAdministrator())
				{
					this->SendNotice(CUSTOM_MESSAGE_ID_RED, "%s Wrong Attack Distance Detected: [%d / %d]", __FUNCTION__, distance, max_distance);
				}

				sLog->outError("antihack", "%s --- %s Wrong Attack Distance Detected [%s / %d / %d]", __FUNCTION__, this->BuildLog().c_str(), skill_info->GetName().c_str(), distance, max_distance);

				sGameServer->LogAntiHack(this, ANTIHACK_ID_ATTACK_RANGE, "Wrong Attack Range Detected - Skill: %u - [%d / %d]", skill_info->GetID(), distance, max_distance);
				continue;
			}
		}

		if (skill_info->GetBaseSkill() == SKILL_FIRE_SCREAM)
		{
			if (sGameServer->IsMultiAttackFireScream() && !this->MultiAttackCheckFireScreamSkill(target, head->Serial))
			{
				//sLog->outError("antihack", "[ MULTI ATTACK ] %s Fire Scream", this->BuildLog().c_str());
				return;
			}
		}
		else if (skill_info->GetBaseSkill() == SKILL_PENETRATION)
		{
			if (sGameServer->IsMultiAttackPenetration() && !this->MultiAttackCheckPenetrationSkill(target, head->Serial))
			{
				//sLog->outError("antihack", "[ MULTI ATTACK ] %s Penetration", this->BuildLog().c_str());
				return;
			}
		}

		for (int32 n = 1; n < (hit_count + 1); ++n)
		{
			if (skill_info->GetBaseSkill() == SKILL_MAGIC_ARROW)
			{
				this->AttackProc(mTarget, mSkill, false, 0, 0, 0, n);
			}
			else if (skill_info->GetBaseSkill() == SKILL_SHINING_PEAK)
			{
				this->AddDelayed(DELAYED_MAGIC_ATTACK, 100, mTarget, mSkill->GetSkill(), 0, 0, 0, n);
			}
			else if (skill_info->GetBaseSkill() == SKILL_FIRE_SLASH)
			{
				this->AttackProc(mTarget, mSkill, true, 0, 0, 0, n - 1);
			}
			else
			{
				this->AttackProc(mTarget, mSkill, false, 0, 0, 0, n - 1);
			}
		}
	}
}

void Player::NewMagicAttack(uint8 * Packet)
{
	SafeAssert(Packet, "Packet == nullptr");

	if ( !this->IsActionAllowed(PlayerAction::PLAYER_ACTION_MAGIC_REQUEST) )
	{
		return;
	}

	if ( this->IsInSafeZone() )
	{
		this->PositionSend(this->GetX(), this->GetY());
		return;
	}

	POINTER_PCT_LOG(NEW_MAGIC_ATTACK, lpMsg, Packet, 0);

	Skill * pSkill = this->MagicGet(lpMsg->GetMagic());

	if ( !pSkill )
	{
		return;
	}

	uint16 skill_base = sSkillMgr->GetSkillBaseSkill(pSkill->GetSkill());

	if( skill_base != SKILL_UPPER_BEAST && skill_base != SKILL_CHAIN_DRIVE && skill_base != SKILL_DARK_SIDE && skill_base != SKILL_DRAGON_SLAYER)
	{
		return;
	}

	this->Ping();

	this->SetRageFighterSkill(pSkill->GetSkill());
	this->SetRageFighterSkillCount(0);
	this->SetRageFighterSkillMaxCount(1);

	switch (skill_base)
	{
	case SKILL_UPPER_BEAST:
		{
			this->SetRageFighterSkillMaxCount(2);
		} break;
	}

	/*
	SKILL_UPPER_BEAST = 0x19 x2 (despues)
	SKILL_CHAIN_DRIVE = 0x19 x1 (despues)
	SKILL_DRAGON_SLAYER = 0x19 x1 (despues)

	SKILL_DARK_SIDE = 0x1E x1 (antes)
	*/
	
	if( skill_base == SKILL_DARK_SIDE )
	{
		//this->NewMagicAttackSend(this->GetRageFighterSkillTarget() == 0xFFFF ? this->GetEntry() : this->GetRageFighterSkillTarget(), pSkill->GetSkill());
		this->NewMagicAttackSend(lpMsg->GetTarget() == 0xFFFF ? this->GetEntry() : lpMsg->GetTarget(), pSkill->GetSkill());
	}
	else if( Unit* pUnit = sObjectMgr->GetUnit(lpMsg->GetTarget()) )
	{
		this->NewMagicAttackSend(lpMsg->GetTarget(), pSkill->GetSkill());
	}
}

void Player::DarkSideAttack(uint8 * Packet)
{
	
}

void Player::PositionAttack(uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(this, false))
	{
		return;
	}

	POINTER_PCT_LOG(PACKET_POSITION_ATTACK, lpMsg, Packet, 0);

	Skill * pSkill = this->MagicGet(lpMsg->GetMagic());

	if (!pSkill)
	{
		return;
	}

	Unit* pTarget = sObjectMgr->GetUnit(lpMsg->GetTarget());

	if (!pTarget)
	{
		return;
	}

	PACKET_POSITION_ATTACK_RESULT pMsg;
	pMsg.SetIndex(this->GetEntry());
	pMsg.SetSkill(pSkill->GetSkill());
	pMsg.SetTarget(pTarget->GetEntry());
	pMsg.x = pTarget->GetX();
	pMsg.y = pTarget->GetY();

	this->SEND_PCT(pMsg);
	this->SEND_PCT_VP(pMsg);
}

bool Player::IsSkillUseAllowed(skill_template const* skill_info)
{
	if ( this->IsSpectator() )
	{
		return false;
	}

	if ( !skill_info )
	{
		return false;
	}

	if ( !skill_info->IsRequiredClass(this->GetClass(), this->GetChangeUP(0), this->GetChangeUP(1), this->GetChangeUP(2)) )
	{
		return false;
	}

	if ( !skill_info->IsValidStatus(this->GuildGetRanking()) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not allowed to use this skill.");
		return false;
	}

	if ( !skill_info->IsValidKillCount(this->GetKillCount()) )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Required %d kill points to use this skill.", skill_info->GetRequiredKillCount());
		return false;
	}

	if ( !this->HasSkillRequiredStat(sSkillMgr->GetSkill(skill_info->GetBaseSkill())) )
	{
		return false;
	}

	return true;
}

void Player::MagicCancel(uint8 * Packet)
{
	SafeAssert(Packet, "Packet == nullptr");

	if ( !this->IsActionAllowed(PlayerAction::PLAYER_ACTION_CANCEL_MAGIC) )
		return;

	POINTER_PCT(MAGIC_CANCEL, lpMsg, Packet, 0);

	skill_template const* skill_info = sSkillMgr->GetSkill(lpMsg->GetMagic());

	if ( !skill_info )
		return;

	this->Ping();

	this->RemoveBuff(skill_info->GetBuffIcon());
}

void Player::GetPartyMembers(Unit* mMember[MAX_PARTY_MEMBERS], uint8 & count, uint8 distance)
{
	for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		mMember[i] = nullptr;
	}

	count = 0;

	mMember[count] = this;
	count++;

	if ( count > 1 )
	{
		count = 1;
	}

	Party* pParty = this->GetParty();

	if ( !pParty )
	{
		return;
	}

	for ( uint8 i = 0; i < MAX_PARTY_MEMBERS; ++i )
	{
		if ( pParty->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING )
		{
			continue;
		}

		Player* mPlayer = pParty->GetMember(i)->GetPlayer();

		if ( !mPlayer || mPlayer == this )
		{
			continue;
		}

		if ( !mPlayer->IsPlaying() )
		{
			continue;
		}

		if ( !this->SameDimension(mPlayer) )
		{
			continue;
		}

		if ( !IN_RANGE(this, mPlayer, distance) )
		{
			continue;
		}

		mMember[count] = mPlayer;
		count++;
	}

	if ( count > MAX_PARTY_MEMBERS )
	{
		count = 1;
	}
}

bool Player::SkillComboCheck(uint16 skill)
{
	if (this->QuestEvolutionGetState(3) == QUEST_EVOLUTION_STATE_COMPLETE && this->GetClass() == Character::DARK_KNIGHT)
	{
		int16 sp = this->SkillComboGetPos(skill);

		if (sp == 0)
		{
			this->GetCombo()->SetProgressIndex(0);
			this->GetCombo()->GetTime()->Start(3000);
			this->GetCombo()->SetSkill(0, skill);
		}
		else if (sp == 1)
		{
			if (this->GetCombo()->GetSkill(0) == 0)
			{
				this->GetCombo()->Init();
				return false;
			}

			if (this->GetCombo()->GetTime()->Elapsed())
			{
				this->GetCombo()->Init();
				return false;
			}

			if (this->GetCombo()->GetProgressIndex() == 0)
			{
				this->GetCombo()->SetProgressIndex(1);
				this->GetCombo()->GetTime()->Start(3000);
				this->GetCombo()->SetSkill(1, skill);
			}
			else if (this->GetCombo()->GetSkill(1) != skill)
			{
				this->GetCombo()->Init();
				return true;
			}
			else
				this->GetCombo()->Init();
		}
		else
			this->GetCombo()->Init();
	}

	return false;
}

int16 Player::SkillComboGetPos(uint16 skill)
{
	int16 pos = -1;

	switch ( skill )
	{
	case SKILL_FALLING_SLASH:
	case SKILL_LUNGE:
	case SKILL_UPPERCUT:
	case SKILL_CYCLONE:
	case SKILL_SLASH:
	case SKILL_RUSH:
	case SKILL_RUSH_KNIGHT:
		{
			pos = 0;
		} break;

	case SKILL_TWISTING_SLASH:
	case SKILL_RAGEFUL_BLOW:
	case SKILL_DEATH_STAB:
	case SKILL_STRIKE_OF_DESTRUCTION:
	case SKILL_BLOOD_STORM:
	case SKILL_FIRE_BLOW:
	case SKILL_SWORD_BLOW:
		{
			pos = 1;
		} break;
	}

	return pos;
}

void Player::SkillGetDamage(uint16 skill, int32 & damage_min, int32 & damage_max)
{
	skill_template const* pSkillInfo = sSkillMgr->GetSkill(skill);

	if (!pSkillInfo)
	{
		return;
	}

	uint16 base_skill = pSkillInfo->GetBaseSkill();

	base_skill = base_skill == SKILL_FORCE && this->IsForceUpgrade() ? SKILL_FORCE_WAVE : base_skill;

	skill_template const* pBaseInfo = sSkillMgr->GetSkill(base_skill);

	if (!pBaseInfo)
	{
		return;
	}

	int32 damage = pBaseInfo->GetDamage();

	switch (base_skill)
	{
	case SKILL_METEORITE:
	{
							damage += static_cast<int32>(this->MagicGetValue(390));
	} break;

	case SKILL_TRIPLE_SHOT:
	{
							  damage += static_cast<int32>(this->MagicGetValue(414));
							  damage += static_cast<int32>(this->MagicGetValue(1081, true));
	} break;

	case SKILL_ICE_ARROW:
	{
							damage += static_cast<int32>(this->MagicGetValue(424));
							damage += static_cast<int32>(this->MagicGetValue(1084, true));
	} break;

	case SKILL_PENETRATION:
	{
							  damage += static_cast<int32>(this->MagicGetValue(416));
	} break;

	case SKILL_FLAME:
	{
						damage += static_cast<int32>(this->MagicGetValue(378));
						damage += static_cast<int32>(this->MagicGetValue(483));
						damage += static_cast<int32>(this->MagicGetValue(1074, true));
	} break;

	case SKILL_LIGHTNING:
	{
							damage += static_cast<int32>(this->MagicGetValue(480));
							damage += static_cast<int32>(this->MagicGetValue(1086, true));
	} break;

	case SKILL_INFERNO:
	{
						  damage += static_cast<int32>(this->MagicGetValue(381));
						  damage += static_cast<int32>(this->MagicGetValue(486));
	} break;

	case SKILL_EVIL_SPIRIT:
	{
							  damage += static_cast<int32>(this->MagicGetValue(385));
							  damage += static_cast<int32>(this->MagicGetValue(487));
							  damage += static_cast<int32>(this->MagicGetValue(1078, true));
							  damage += static_cast<int32>(this->MagicGetValue(1088, true));
	} break;

	case SKILL_BLAST:
	{
						damage += static_cast<int32>(this->MagicGetValue(382));
						damage += static_cast<int32>(this->MagicGetValue(484));
	} break;

	case SKILL_HELL_FIRE:
	{
							damage += static_cast<int32>(this->MagicGetValue(388));
	} break;

	case SKILL_ICE:
	{
					  damage += static_cast<int32>(this->MagicGetValue(389));
					  damage += static_cast<int32>(this->MagicGetValue(489));
	} break;

	case SKILL_DECAY:
	{
						damage += static_cast<int32>(this->MagicGetValue(387));
	} break;

	case SKILL_CYCLONE:
	{
						  damage += static_cast<int32>(this->MagicGetValue(326));
						  damage += static_cast<int32>(this->MagicGetValue(479));
	} break;

	case SKILL_SLASH:
	{
						damage += static_cast<int32>(this->MagicGetValue(327));
	} break;

	case SKILL_FALLING_SLASH:
	{
								damage += static_cast<int32>(this->MagicGetValue(328));
	} break;

	case SKILL_LUNGE:
	{
						damage += static_cast<int32>(this->MagicGetValue(329));
	} break;

	case SKILL_TWISTING_SLASH:
	{
								 damage += static_cast<int32>(this->MagicGetValue(330));
								 damage += static_cast<int32>(this->MagicGetValue(481));
								 damage += static_cast<int32>(this->MagicGetValue(811));
	} break;

	case SKILL_RAGEFUL_BLOW:
	{
							   damage += static_cast<int32>(this->MagicGetValue(331));
							   damage += static_cast<int32>(this->MagicGetValue(812));
							   damage += static_cast<int32>(this->MagicGetValue(1069, true));
	} break;

	case SKILL_DEATH_STAB:
	{
							 damage += static_cast<int32>(this->MagicGetValue(336));
							 damage += static_cast<int32>(this->MagicGetValue(339));
							 damage += static_cast<int32>(this->MagicGetValue(1071, true));
	} break;

	case SKILL_STRIKE_OF_DESTRUCTION:
	{
										damage += static_cast<int32>(this->MagicGetValue(337));
										damage += static_cast<int32>(this->MagicGetValue(809));
										damage += static_cast<int32>(this->MagicGetValue(1202, true));
	} break;

	case SKILL_EXPLOSION:
	{
							damage += static_cast<int32>(this->MagicGetValue(774));
	} break;

	case SKILL_REQUIEM:
	{
						  damage += static_cast<int32>(this->MagicGetValue(775));
	} break;

	case SKILL_POLLUTION:
	{
							damage += static_cast<int32>(this->MagicGetValue(776));
							damage += static_cast<int32>(this->MagicGetValue(777));
							damage += static_cast<int32>(this->MagicGetValue(778));
	} break;

	case SKILL_DEATHSIDE:
	{
							damage += static_cast<int32>(this->MagicGetValue(1155, true));
	} break;

	case SKILL_CHAIN_LIGHTNING:
	{
								  damage += static_cast<int32>(this->MagicGetValue(455));
								  damage += static_cast<int32>(this->MagicGetValue(1104, true));
	} break;

	case SKILL_LIGHTNING_SHOCK:
	{
								  damage += static_cast<int32>(this->MagicGetValue(456));
								  damage += static_cast<int32>(this->MagicGetValue(1103, true));
	} break;

	case SKILL_DRAIN_LIFE:
	{
							 damage += static_cast<int32>(this->MagicGetValue(458));
							 damage += static_cast<int32>(this->MagicGetValue(1101, true));
	} break;

	case SKILL_LARGE_RING_BLOWER:
	{
									damage += static_cast<int32>(this->MagicGetValue(551));
	} break;

	case SKILL_UPPER_BEAST:
	{
							  damage += static_cast<int32>(this->MagicGetValue(552));
	} break;

	case SKILL_CHAIN_DRIVE:
	{
							  damage += static_cast<int32>(this->MagicGetValue(558));
							  damage += static_cast<int32>(this->MagicGetValue(1112, true));
	} break;

	case SKILL_DARK_SIDE:
	{
							damage += static_cast<int32>(this->MagicGetValue(559));
							damage += static_cast<int32>(this->MagicGetValue(1113, true));
	} break;

	case SKILL_DRAGON_LORE:
	{
							  damage += static_cast<int32>(this->MagicGetValue(560));
							  damage += static_cast<int32>(this->MagicGetValue(1111, true));
	} break;

	case SKILL_FIRE_BURST:
	{
							 damage += static_cast<int32>(this->MagicGetValue(508));
							 damage += static_cast<int32>(this->MagicGetValue(1096, true));
	} break;

	case SKILL_FORCE_WAVE:
	{
							 damage += static_cast<int32>(this->MagicGetValue(509));
	} break;

	case SKILL_EARTHSHAKE:
	{
							 damage += static_cast<int32>(this->MagicGetValue(512));
	} break;

	case SKILL_FIRE_SCREAM:
	{
							  damage += static_cast<int32>(this->MagicGetValue(518));
							  damage += static_cast<int32>(this->MagicGetValue(1097, true));
	} break;

	case SKILL_POWER_SLASH:
	{
							  damage += static_cast<int32>(this->MagicGetValue(482));
							  damage += static_cast<int32>(this->MagicGetValue(1091, true));
	} break;

	case SKILL_FIRE_SLASH:
	{
							 damage += static_cast<int32>(this->MagicGetValue(490));
							 damage += static_cast<int32>(this->MagicGetValue(1092, true));
	} break;

	case SKILL_BLOOD_STORM:
	{
							  damage += static_cast<int32>(this->MagicGetValue(346));
							  damage += static_cast<int32>(this->MagicGetValue(1070, true));
	} break;

	case SKILL_POISON_ARROW:
	{
							   damage += static_cast<int32>(this->MagicGetValue(434));
							   damage += static_cast<int32>(this->MagicGetValue(1082, true));
	} break;

	case SKILL_EARTH_PRISON:
	{
							   damage += static_cast<int32>(this->MagicGetValue(497));
	} break;

	case SKILL_ICE_STORM:
	{
							damage += static_cast<int32>(this->MagicGetValue(391));
	} break;

	case SKILL_NOVA:
	{
					   damage += static_cast<int32>(this->MagicGetValue(SKILL_TREE_ADD_NOVA_IMPROVED));
					   damage += static_cast<int32>(this->MagicGetValue(1077, true));
	} break;

	case SKILL_MULTI_SHOT:
	{
							 damage += static_cast<int32>(this->MagicGetValue(411));
							 damage += static_cast<int32>(this->MagicGetValue(1083, true));
	} break;

	case SKILL_FLAME_STRIKE:
	{
							   damage += static_cast<int32>(this->MagicGetValue(492));
							   damage += static_cast<int32>(this->MagicGetValue(1093, true));
	} break;

	case SKILL_GIGANTIC_STORM:
	{
								 damage += static_cast<int32>(this->MagicGetValue(496));
								 damage += static_cast<int32>(this->MagicGetValue(1087, true));
	} break;

	case SKILL_ELECTRIC_SPARK:
	{
								 damage += static_cast<int32>(this->MagicGetValue(519));
								 damage += static_cast<int32>(this->MagicGetValue(1100, true));
	} break;

	case SKILL_CHAOTIC_DESEIER:
	{
								  damage += static_cast<int32>(this->MagicGetValue(SKILL_TREE_ADD_BIRDS_IMPROVED));
								  damage += static_cast<int32>(this->MagicGetValue(1098, true));
	} break;

	case SKILL_DRAGON_SLAYER:
	{
								damage += static_cast<int32>(this->MagicGetValue(SKILL_TREE_ADD_DRAGON_SLAYER_IMPROVED));
								damage += static_cast<int32>(this->MagicGetValue(1108, true));
	} break;

	case SKILL_SPIN_STEP:
	{
							damage += static_cast<int32>(this->MagicGetValue(687));
							damage += static_cast<int32>(this->MagicGetValue(1115, true));
	} break;

	case SKILL_MAGIC_PIN:
	{
							damage += static_cast<int32>(this->MagicGetValue(692));
							damage += static_cast<int32>(this->MagicGetValue(1117, true));
	} break;

	case SKILL_HARSH_STRIKE:
	{
							   damage += static_cast<int32>(this->MagicGetValue(688));
							   damage += static_cast<int32>(this->MagicGetValue(1116, true));
	} break;

	case SKILL_SHINING_PEAK:
	{
							   damage += static_cast<int32>(this->MagicGetValue(699));
							   damage += static_cast<int32>(this->MagicGetValue(1119, true));
	} break;

	case SKILL_BRECHE:
	{
						 damage += static_cast<int32>(this->MagicGetValue(696));
						 damage += static_cast<int32>(this->MagicGetValue(1118, true));
	} break;

	case SKILL_PHOENIX_SHOT:
	{
							   damage += static_cast<int32>(this->MagicGetValue(744));
							   damage += static_cast<int32>(this->MagicGetValue(1109, true));
	} break;

	case SKILL_FIRE_BLOW:
	{
							damage += static_cast<int32>(this->MagicGetValue(1072, true));
	} break;

	case SKILL_METEOR_STRIKE:
	{
								damage += static_cast<int32>(this->MagicGetValue(1075, true));
	} break;

	case SKILL_METEOR_STORM:
	{
							   damage += static_cast<int32>(this->MagicGetValue(1076, true));
	} break;

	case SKILL_SOUL_SEEKER:
	{
							  damage += static_cast<int32>(this->MagicGetValue(1079, true));
	} break;

	case SKILL_FOCUS_SHOT:
	{
							 damage += static_cast<int32>(this->MagicGetValue(1085, true));
	} break;

	case SKILL_DARK_BLAST:
	{
							 damage += static_cast<int32>(this->MagicGetValue(1089, true));
	} break;

	case SKILL_METEOR_STRIKE_2:
	{
								  damage += static_cast<int32>(this->MagicGetValue(1090, true));
	} break;

	case SKILL_FIRE_BLOOD:
	{
							 damage += static_cast<int32>(this->MagicGetValue(1094, true));
	} break;

	case SKILL_ICE_BLOOD:
	{
							damage += static_cast<int32>(this->MagicGetValue(1095, true));
	} break;

	case SKILL_EVIL_WIND:
	{
							damage += static_cast<int32>(this->MagicGetValue(1099, true));
	} break;

	case SKILL_FIRE_BEAST:
	{
							 damage += static_cast<int32>(this->MagicGetValue(1102, true));
	} break;

	case SKILL_AQUA_BEAST:
	{
							 damage += static_cast<int32>(this->MagicGetValue(1105, true));
	} break;

	case SKILL_DARK_PHOENIX_SHOT:
	{
									damage += static_cast<int32>(this->MagicGetValue(1110, true));
	} break;

	case SKILL_SPIRIT_HOOK:
	{
							  damage += static_cast<int32>(this->MagicGetValue(1147, true));
	} break;

	case SKILL_POISON_STORM:
	{
							   damage += static_cast<int32>(this->MagicGetValue(1129, true));
							   damage += static_cast<int32>(this->MagicGetValue(1196, true));
	} break;

	case SKILL_FROZEN_SLAUGHTER:
	{
								   damage += static_cast<int32>(this->MagicGetValue(1130, true));
								   damage += static_cast<int32>(this->MagicGetValue(1197, true));
	} break;

	case SKILL_BLEEDING_HEAT:
	{
								damage += static_cast<int32>(this->MagicGetValue(1131, true));
								damage += static_cast<int32>(this->MagicGetValue(1198, true));
	} break;

	case SKILL_SHOCK_BOOM:
	{
							 damage += static_cast<int32>(this->MagicGetValue(1132, true));
	} break;

	case SKILL_MAGIC_ARROW:
	{
							  damage += static_cast<int32>(this->MagicGetValue(748));
							  damage += static_cast<int32>(this->MagicGetValue(749));
							  damage += static_cast<int32>(this->MagicGetValue(1148, true));
	} break;

	case SKILL_PLASMA_BALL:
	{
							  damage += static_cast<int32>(this->MagicGetValue(750));
							  damage += static_cast<int32>(this->MagicGetValue(751));
							  damage += static_cast<int32>(this->MagicGetValue(1149, true));
	} break;

	case SKILL_LIGHTNING_STORM:
	{
								  damage += static_cast<int32>(this->MagicGetValue(1150, true));
	} break;

	case SKILL_SWORD_INERTIA:
	{
								damage += static_cast<int32>(this->MagicGetValue(779));
								damage += static_cast<int32>(this->MagicGetValue(1158, true));
	} break;

	case SKILL_BAT_FLOCK:
	{
							damage += static_cast<int32>(this->MagicGetValue(781));
							damage += static_cast<int32>(this->MagicGetValue(1159, true));
	} break;

	case SKILL_PIERCE_ATTACK:
	{
								damage += static_cast<int32>(this->MagicGetValue(1160, true));
	} break;

	case SKILL_SWORD_BLOW:
	{
							 damage += static_cast<int32>(this->MagicGetValue(1203, true));
	} break;

	case SKILL_DARK_PLASMA:
		damage += static_cast<int32>(this->MagicGetValue(822));
		damage += static_cast<int32>(this->MagicGetValue(1211, true));
		break;

	case SKILL_ICE_BREAK:
		damage += static_cast<int32>(this->MagicGetValue(823));
		damage += static_cast<int32>(this->MagicGetValue(824));
		break;

	case SKILL_ICE_BLAST:
		damage += static_cast<int32>(this->MagicGetValue(1212, true));
		break;

	case SKILL_FIRE_DEATH:
		damage += static_cast<int32>(this->MagicGetValue(825));
		damage += static_cast<int32>(this->MagicGetValue(826));
		break;

	case SKILL_BURSTING_FLARE:
		damage += static_cast<int32>(this->MagicGetValue(1213, true));
		break;
	}

	if (pSkillInfo->GetStat1() != MAX_STAT_TYPE && pSkillInfo->GetStatValue1() > 0)
	{
		damage += this->GetTotalStat(pSkillInfo->GetStat1()) / pSkillInfo->GetStatValue1();
	}

	if (pSkillInfo->GetStat2() != MAX_STAT_TYPE && pSkillInfo->GetStatValue2() > 0)
	{
		damage += this->GetTotalStat(pSkillInfo->GetStat2()) / pSkillInfo->GetStatValue2();
	}

	if (sGameServer->IsMajesticAddDamageNormal())
	{
		damage += this->GetMajesticDataValueBySkill(skill, MAJESTIC_LINKED_SKILL_ADDITIONAL_DAMAGE);
	}

	damage_min += damage;
	damage_max += damage + (damage / 2);

	Item const* pPentagram = this->GetInventory()->GetItem(PENTAGRAM_SLOT);

	if (pPentagram->IsValid())
	{
		if (pSkillInfo->GetElementalAttribute() == this->GetElementalAttribute())
		{
			damage_min += (this->GetData(PLAYER_DATA_ELEMENTAL_IMPRINT) * pSkillInfo->GetElementalBonusDamage()) / 100;
			damage_max += (this->GetData(PLAYER_DATA_ELEMENTAL_IMPRINT) * pSkillInfo->GetElementalBonusDamage()) / 100;
		}
	}
}

void Player::SkillCastNovaUpdate()
{
	if ( !this->GetNova()->IsRunning() )
	{
		return;
	}

	skill_template const* skill_info = sSkillMgr->GetSkill(this->GetNova()->GetSkill());

	if ( !skill_info )
	{
		return;
	}

	if ( skill_info->GetBaseSkill() != SKILL_NOVA )
	{
		return;
	}

	this->GetNova()->UpdateCount();

	if(GetTickCount() < (this->GetNova()->GetTick() + 6000))
	{
		int32 consume_mana = skill_info->mana_cost.get() * 20 / 100;

		consume_mana -= consume_mana * this->GetDataFloat(PLAYER_FLOAT_MANA_REDUCTION) / 100;

		if ( this->SkillCastNovaConsumePotion(consume_mana) )
		{
			this->PowerReduce(POWER_MANA, consume_mana, true);

			SKILL_CAST pMsg(this->GetEntry(), this->GetNova()->GetSkill(), this->GetNova()->GetCount());
			this->sendPacket(MAKE_PCT(pMsg));
			this->sendPacket_viewport(MAKE_PCT(pMsg));
			return;
		}
	}

	this->RunNova();
}

bool Player::SkillCastNovaConsumePotion(int32 consume_mana)
{
	if ( this->PowerGet(POWER_MANA) >= consume_mana )
		return true;

	int32 counter = 0;
	uint8 mana_potion = 0;

	do
	{
		counter++;

		if ( counter > 100 )
			return false;

		mana_potion = this->GetManaPotionSlot();

		if ( mana_potion == uint8(-1) )
			return false;

		USE_ITEM pMsg(mana_potion, 0, 0);
		this->ItemUse((uint8*)&pMsg);
	}
	while ( consume_mana > this->PowerGet(POWER_MANA) );

	return true;
}

int32 Player::SkillManaUse(Skill * mSkill, skill_template const* skill_info)
{
	int32 mana_skill = skill_info->mana_cost.get();
	
	mana_skill += (mana_skill * mSkill->GetLevel()) / 200;

	mana_skill += mana_skill * this->GetDataFloat(PLAYER_DATA_INCREASES_MP_CONSUMPTION) / 100;

	mana_skill -= mana_skill * this->GetDataFloat(PLAYER_FLOAT_MANA_REDUCTION) / 100;

	limitmin(mana_skill, 0);

	return mana_skill;
}
	
int32 Player::SkillStaminaUse(Skill * mSkill, skill_template const* skill_info)
{
	int32 stamina_skill = skill_info->stamina_cost.get();

	stamina_skill += stamina_skill * this->GetDataFloat(PLAYER_DATA_INCREASES_AG_CONSUMPTION) / 100;

	stamina_skill -= stamina_skill * static_cast<int32>(this->GetStaminaReduction()) / 100;

	limitmin(stamina_skill, 0);

	return stamina_skill;
}

void Player::ApplyStaffDamage(uint16 skill, int32 & damage_min, int32 & damage_max)
{
	Item const* Right = this->GetInventory()->GetItem(WEAPON_01);

	if (Right->IsItem() && Right->IsValid() && Right->GetMagicPower())
	{
		int32 damage_percent = ((Right->GetMagicPower() / 2) + (Right->GetLevel() * 2));
		damage_percent -= damage_percent * Right->GetDurabilityCurrentStatus();

		float add_value = this->GetMajesticDataValueBySkill(skill, MAJESTIC_LINKED_SKILL_ADDITIONAL_WEAPON_MAGIC_DAMAGE);

		if (add_value > 0.0f)
		{
			damage_percent += damage_percent * add_value / 100.0f;
		}

		damage_min += damage_min * damage_percent / 100;
		damage_max += damage_max * damage_percent / 100;
	}

	/*if (this->GetClass() == Character::MAGIC_GLADIATOR)
	{
		Item const* Left = this->GetInventory()->GetItem(WEAPON_02);

		if (Left->IsItem() && Left->IsValid() && Left->GetMagicPower())
		{
			int32 damage_percent = ((Left->GetMagicPower() / 2) + (Left->GetLevel() * 2));
			damage_percent -= damage_percent * Left->GetDurabilityCurrentStatus();

			float add_value = this->GetMajesticDataValueBySkill(skill, MAJESTIC_LINKED_SKILL_ADDITIONAL_WEAPON_MAGIC_DAMAGE);

			if (add_value > 0.0f)
			{
				damage_percent += damage_percent * add_value / 100.0f;
			}

			damage_min += damage_min * damage_percent / 100;
			damage_max += damage_max * damage_percent / 100;
		}
	}*/
}

void Player::ApplyCurseDamage(uint16 skill, int32 & damage_min, int32 & damage_max)
{
	Item const* Left = this->GetInventory()->GetItem(WEAPON_02);

	if (!Left->IsItem() || !Left->IsValid())
	{
		return;
	}

	if (!Left->GetCursePower())
	{
		return;
	}

	int32 damage_percent = ((Left->GetCursePower() / 2) + (Left->GetLevel() * 2));
	damage_percent -= damage_percent * Left->GetDurabilityCurrentStatus();

	float add_value = this->GetMajesticDataValueBySkill(skill, MAJESTIC_LINKED_SKILL_ADDITIONAL_WEAPON_CURSE_DAMAGE);

	if (add_value > 0.0f)
	{
		damage_percent += damage_percent * add_value / 100.0f;
	}

	damage_min += damage_min * damage_percent / 100;
	damage_max += damage_max * damage_percent / 100;
}

void Player::ApplyWeaponDamage(uint16 skill, int32 & damage_min, int32 & damage_max)
{
	damage_min = this->GetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN);
	damage_max = this->GetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX);

	float add_value = this->GetMajesticDataValueBySkill(skill, MAJESTIC_LINKED_SKILL_ADDITIONAL_WEAPON_DAMAGE);

	if ( add_value > 0.0f )
	{
		damage_min += damage_min * add_value / 100.0f;
		damage_max += damage_max * add_value / 100.0f;
	}
}

void Player::ApplyDarkSpiritDamage(int32 & damage_min, int32 & damage_max)
{
	Item const* Right = this->GetInventory()->GetItem(WEAPON_01);

	if ( !Right->IsItem() || !Right->IsValid() )
	{
		return;
	}

	if ( !Right->GetPetPower() )
	{
		return;
	}

	int32 damage_percent = (Right->GetPetPower() / 2) + (Right->GetLevel() * 2);
	damage_percent -= damage_percent * Right->GetDurabilityCurrentStatus();
	
	damage_min += damage_min * damage_percent / 100;
	damage_max += damage_max * damage_percent / 100;
}

bool Player::ConsumeSkillItem()
{
	if (this->HasBuff(BUFF_INFINITY_ARROW) || this->HasBuff(BUFF_INFINITY_ARROW_MASTER))
	{
		return true;
	}

	Item * Right = this->GetInventory()->GetItem(WEAPON_01);
	Item * Left = this->GetInventory()->GetItem(WEAPON_02);

	if (Left->GetKind2() == ItemKind::QUIVER)
	{
		return true;
	}

	if (Right->GetKind2() == ItemKind::BOW || Right->GetKind2() == ItemKind::CROSSBOW)
	{
		if (Right->GetKind2() == ItemKind::BOW && Left->GetItem() != ITEMGET(4, 15))
		{
			return false;
		}

		if (Right->GetKind2() == ItemKind::CROSSBOW && Left->GetItem() != ITEMGET(4, 7))
		{
			return false;
		}

		if (Left->GetDurability() <= 0.0f)
		{
			return false;
		}

		this->DecreaseItemDurabilityByUse(WEAPON_02, 1.0f);
	}

	return true;
}

void Player::ApplyBuffValues(uint8 opt, float value)
{
	if (this->GetWorldId() == WORLD_VULCANUS_ROOM && sGameServer->IsDuelBuffDisabled())
	{
		return;
	}

	switch ( opt )
	{
	case BUFF_OPTION_INCREASE_ATTACK_SPEED:
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_SPEED, value);
			this->IncreaseIntData(UNIT_INT_MAGIC_SPEED, value);
		} break;

	case BUFF_OPTION_INCREASE_ATTACK_DAMAGE:
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value);
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
			this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, value);
			this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, value);
		} break;

	case BUFF_OPTION_INCREASE_DEFENSE:
		{
			this->IncreaseIntData(UNIT_INT_DEFENSE, value);
		} break;

	case BUFF_OPTION_INCREASE_MAXIMUM_LIFE:
		{
			this->PowerIncreseAdd(POWER_LIFE, static_cast<int32>(value), false);
		} break;

	case BUFF_OPTION_INCREASE_MAXIMUM_MANA:
		{
			this->PowerIncreseAdd(POWER_MANA, static_cast<int32>(value), false);
		} break;

	case BUFF_OPTION_INCREASE_EXPERIENCE:
		{
			this->IncreaseData(PLAYER_DATA_ADD_EXPERIENCE_RATE, static_cast<int32>(value));
		} break;

	case BUFF_OPTION_INCREASE_STRENGTH:
		{
			this->IncreaseAddStat(STRENGTH, value);
		} break;

	case BUFF_OPTION_INCREASE_AGILITY:
		{
			this->IncreaseAddStat(AGILITY, value);
		} break;

	case BUFF_OPTION_INCREASE_VITALITY:
		{
			this->IncreaseAddStat(VITALITY, value);
		} break;

	case BUFF_OPTION_INCREASE_ENERGY:
		{
			this->IncreaseAddStat(ENERGY, value);
		} break;

	case BUFF_OPTION_INCREASE_LEADERSHIP:
		{
			if (this->GetClass() == Character::DARK_LORD)
			{
				this->IncreaseAddStat(LEADERSHIP, value);
			}
		} break;

	case BUFF_OPTION_INCREASE_ATTACK_POWER:
		{
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value);
			this->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value);
		} break;

	case BUFF_OPTION_INCREASE_MAGIC_POWER:
		{
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
		} break;

	case BUFF_OPTION_INCREASE_CURSE_DAMAGE:
		this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, value);
		this->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, value);
		break;

	case BUFF_OPTION_BERSERKER_DAMAGE:
		{
			this->PowerIncreseAdd(POWER_MANA, static_cast<int32>(value * this->PowerGetMax(POWER_MANA) / 100.0f), false);
		} break;

	case BUFF_OPTION_BERSERKER_DEFENSE:
		{
			this->PowerReduceAdd(POWER_LIFE, static_cast<int32>(value * this->PowerGetMax(POWER_LIFE) / 100.0f), false);
		} break;

	case BUFF_OPTION_DARKNESS_DECREASE:
		{
			this->PowerReduceAdd(POWER_LIFE, static_cast<int32>(value * this->PowerGetMax(POWER_LIFE) / 100.0f), false);
		} break;

	case BUFF_OPTION_INCREASE_DEFENSE_PERCENT:
		{
			this->PercentIntData(UNIT_INT_DEFENSE, value);
		} break;

	case BUFF_OPTION_INCREASE_CRITICAL_DAMAGE_RATE:
		{
			this->IncreaseFloatData(UNIT_FLOAT_CRITICAL_DAMAGE_RATE, value);
		} break;

	case BUFF_OPTION_INCREASE_EXE_DAMAGE_RATE:
		{
			this->IncreaseFloatData(UNIT_FLOAT_EXCELLENT_DAMAGE_RATE, value);
		} break;

	case BUFF_OPTION_INCREASE_CRITICAL_DAMAGE:
		{
			this->IncreaseIntData(UNIT_INT_CRITICAL_DAMAGE_ADD, value);
		} break;

	case BUFF_OPTION_INCREASE_EXE_DAMAGE:
		{
			this->IncreaseIntData(UNIT_INT_EXCELLENT_DAMAGE_ADD, value);
		} break;

	case BUFF_OPTION_DAMAGE_REFLECTION_RATE:
		{
			this->IncreaseFloatData(UNIT_FLOAT_REFLECT_DAMAGE_RATE, value);
		} break;

	case BUFF_OPTION_IGNORE_RATE:
		{
			this->IncreaseFloatData(UNIT_FLOAT_IGNORE_DEFENSE_RATE, value);
		} break;

	case BUFF_OPTION_INCREASE_ZEN_DROP:
		{
			this->IncreaseData(PLAYER_DATA_MONSTER_DIE_GIVE_ZEN, 50);
		} break;

	case BUFF_OPTION_INCREASE_BOOST_STAMINA:
		{
			this->PowerIncreseAdd(POWER_STAMINA, this->GetTotalLevel() * value, false);
		} break;

	case BUFF_OPTION_INCREASE_BOOST_SD:
		{
			this->PowerIncreseAdd(POWER_SHIELD, this->GetTotalLevel() * value, false);
		} break;

	case BUFF_OPTION_INCREASE_DAMAGE_DECREASE:
		{
			this->IncreaseIntData(UNIT_INT_DAMAGE_DECREASE, value);
		} break;

	case BUFF_OPTION_INCREASE_MAXIMUM_SD:
		{
			this->PowerIncreseAdd(POWER_SHIELD, value, false);
		} break;

	case BUFF_OPTION_INCREASE_MAXIMUM_STAMINA:
		{
			this->PowerIncreseAdd(POWER_STAMINA, value, false);
		} break;

	case BUFF_OPTION_MIN_WIZARDY:
		{
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, value);
		} break;

	case BUFF_OPTION_MAX_WIZARDY:
		{
			this->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, value);
		} break;

	case BUFF_OPTION_INCREASE_ALL_STATS:
		{
			this->IncreaseAddStat(STRENGTH, value);
			this->IncreaseAddStat(AGILITY, value);
			this->IncreaseAddStat(VITALITY, value);
			this->IncreaseAddStat(ENERGY, value);
			
			if ( this->GetClass() == Character::DARK_LORD )
			{
				this->IncreaseAddStat(LEADERSHIP, value);
			}
		} break;

	case BUFF_OPTION_INCREASE_ATTACK_DAMAGE_SKILL:
		{
			this->IncreaseIntData(UNIT_INT_SKILL_ADD_DAMAGE, value);
		} break;

	case BUFF_OPTION_GREATER_DAMAGE:
		{
			this->IncreaseIntData(UNIT_INT_DAMAGE_ADD, value);
		} break;

	case BUFF_OPTION_INCREASE_DEFENSE_SUCCESS_RATE:
		{
			this->IncreaseIntData(UNIT_INT_DEFENSE_RATE, value);
		} break;

	case BUFF_OPTION_AUTOMATIC_HP_RECOVERY:
		{
			this->IncreaseDataFloat(PLAYER_FLOAT_HP_RECOVERY_RATE, value);
		} break;

	case BUFF_OPTION_AUTOMATIC_MP_RECOVERY:
		{
			this->IncreaseDataFloat(PLAYER_FLOAT_MP_RECOVERY_RATE, value);
		} break;

	case BUFF_OPTION_INCREASE_SD_RECOVERY_RATE:
		{
			this->IncreaseDataFloat(PLAYER_FLOAT_SD_RECOVERY_RATE, value);
		} break;

	case BUFF_OPTION_INCREASE_AG_RECOVERY_RATE:
		{
			this->IncreaseDataFloat(PLAYER_FLOAT_AG_RECOVERY_RATE, value);
		} break;

	case BUFF_OPTION_INCREASE_HP_RECOVERY_ADD:
		{
			this->IncreaseData(PLAYER_DATA_HP_RECOVERY_ADD, value);
		} break;

	case BUFF_OPTION_INCREASE_MP_RECOVERY_ADD:
		{
			this->IncreaseData(PLAYER_DATA_MP_RECOVERY_ADD, value);
		} break;

	case BUFF_OPTION_INCREASE_SD_RECOVERY_ADD:
		{
			this->IncreaseData(PLAYER_DATA_SD_RECOVERY_ADD, value);
		} break;

	case BUFF_OPTION_INCREASE_AG_RECOVERY_ADD:
		{
			this->IncreaseData(PLAYER_DATA_AG_RECOVERY_ADD, value);
		} break;

	case BUFF_OPTION_HP_MUL:
		{
			this->PowerIncreseAdd(POWER_LIFE, this->PowerGetMax(POWER_LIFE) * value / 100, false);
		} break;

	case BUFF_OPTION_MP_MUL:
		{
			this->PowerIncreseAdd(POWER_MANA, this->PowerGetMax(POWER_MANA) * value / 100, false);
		} break;

	case BUFF_OPTION_SD_MUL:
		{
			this->PowerIncreseAdd(POWER_SHIELD, this->PowerGetMax(POWER_SHIELD) * value / 100, false);
		} break;

	case BUFF_OPTION_AG_MUL:
		{
			this->PowerIncreseAdd(POWER_STAMINA, this->PowerGetMax(POWER_STAMINA) * value / 100, false);
		} break;

	case BUFF_OPTION_HP_DIV:
		{
			this->PowerReduceAdd(POWER_LIFE, this->PowerGetMax(POWER_LIFE) * value / 100, false);
		} break;

	case BUFF_OPTION_MP_DIV:
		{
			this->PowerReduceAdd(POWER_MANA, this->PowerGetMax(POWER_MANA) * value / 100, false);
		} break;

	case BUFF_OPTION_SD_DIV:
		{
			this->PowerReduceAdd(POWER_SHIELD, this->PowerGetMax(POWER_SHIELD) * value / 100, false);
		} break;

	case BUFF_OPTION_AG_DIV:
		{
			this->PowerReduceAdd(POWER_STAMINA, this->PowerGetMax(POWER_STAMINA) * value / 100, false);
		} break;

	case BUFF_OPTION_INCREASES_MP_CONSUMPTION:
		{
			this->IncreaseDataFloat(PLAYER_DATA_INCREASES_MP_CONSUMPTION, value);
		} break;

	case BUFF_OPTION_INCREASES_AG_CONSUMPTION:
		{
			this->IncreaseDataFloat(PLAYER_DATA_INCREASES_AG_CONSUMPTION, value);
		} break;

	case BUFF_OPTION_BASIC_DEFENSE:
	{
									  this->IncreaseIntData(UNIT_INT_DEFENSE_BASE, value);
	} break;

	case BUFF_OPTION_DECREASE_MAXIMUM_HP:
	{
		this->PowerReduceAdd(POWER_LIFE, value, false);
	} break;

	case BUFF_OPTION_WRATH_DAMAGE:
		this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, value);
		this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, value);
		this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, value);
		this->PercentIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, value);
		break;

	case BUFF_OPTION_WRATH_DEFENSE:
		this->ReduceIntData(UNIT_INT_DEFENSE, this->GetIntData(UNIT_INT_DEFENSE) * value / 100);
		break;
	}
}

void Player::ApplyBuffStatEffect()
{
	BUFF_LOOP(i)
	{
		BuffData * data = this->GetBuffData(i);

		if ( data->GetID() == 0 )
			continue;

		if ( data->IsDisabled() )
			continue;

		if ( !this->IsBuffAllowed(data->GetID()) )
		{
			continue;
		}

		BUFF_EFFECT_LOOP(h)
		{
			if ( data->GetEffect(h)->GetEffect() == BUFF_OPTION_INCREASE_STRENGTH ||
				 data->GetEffect(h)->GetEffect() == BUFF_OPTION_INCREASE_AGILITY ||
				 data->GetEffect(h)->GetEffect() == BUFF_OPTION_INCREASE_VITALITY ||
				 data->GetEffect(h)->GetEffect() == BUFF_OPTION_INCREASE_ENERGY ||
				 data->GetEffect(h)->GetEffect() == BUFF_OPTION_INCREASE_LEADERSHIP ||
				 data->GetEffect(h)->GetEffect() == BUFF_OPTION_INCREASE_ALL_STATS )
			{
				this->ApplyBuffValues(data->GetEffect(h)->GetEffect(), data->GetEffect(h)->GetValue());
			}
		}
	}
}

void Player::ApplyBuffNonStatEffect()
{
	BUFF_LOOP(i)
	{
		BuffData * data = this->GetBuffData(i);

		if ( data->GetID() == 0 )
			continue;

		if ( data->IsDisabled() )
			continue;

		if ( !this->IsBuffAllowed(data->GetID()) )
		{
			continue;
		}

		BUFF_EFFECT_LOOP(h)
		{
			if ( data->GetEffect(h)->GetEffect() != BUFF_OPTION_INCREASE_STRENGTH &&
				 data->GetEffect(h)->GetEffect() != BUFF_OPTION_INCREASE_AGILITY &&
				 data->GetEffect(h)->GetEffect() != BUFF_OPTION_INCREASE_VITALITY &&
				 data->GetEffect(h)->GetEffect() != BUFF_OPTION_INCREASE_ENERGY &&
				 data->GetEffect(h)->GetEffect() != BUFF_OPTION_INCREASE_LEADERSHIP &&
				 data->GetEffect(h)->GetEffect() != BUFF_OPTION_INCREASE_ALL_STATS )
			{
				 this->ApplyBuffValues(data->GetEffect(h)->GetEffect(), data->GetEffect(h)->GetValue());
			}
		}
	}
}

void Player::SendClearBuff()
{
	BUFF_LOOP(i)
	{
		BuffData * data = this->GetBuffData(i);

		if ( data->GetID() == 0 )
			continue;

		this->SendBuffEffect(data, 1);
	}
}

void Player::SendBuffEffect(BuffData * data, uint8 remove)
{
	buff_template const* buff_data = sSkillMgr->GetBuff(data->GetID());

	if (buff_data && buff_data->IsSend())
	{
		BUFF_SEND pMsg(data->GetID(), 0, remove);
		if (!remove)
		{
			pMsg.time = data->GetRemainTime();
			pMsg.group = buff_data->GetGroup();
			pMsg.effect_type = data->GetEffect(0)->GetEffect();
			pMsg.effect_value = static_cast<uint16>(data->GetEffect(0)->GetValue());
		}

		Item const* pItem = this->GetActiveInventoryItem(buff_data->GetItem());

		if (pItem)
		{
			pItem->ConvertToBuffer(pMsg.item_info);
			pMsg.item_info[6] = remove ? 0 : 1;
		}

		this->sendPacket(MAKE_PCT(pMsg));
	}
}

void Player::ClearAllBuff(uint8 flag)
{
	bool update = false;

	BUFF_LOOP(i)
	{
		BuffData * data = this->GetBuffData(i);

		if ( data->GetID() == 0 )
			continue;

		if ( data->IsFlag(BUFF_FLAG_CONSTANT) && !(flag & BUFF_CLEAR_FLAG_CONSTANT) )
			continue;

		if ( data->IsFlag(BUFF_FLAG_SHOP) && !(flag & BUFF_CLEAR_FLAG_SHOP) )
			continue;

		if ( data->IsFlag(BUFF_FLAG_COMBAT) && !(flag & BUFF_CLEAR_FLAG_COMBAT) )
			continue;

		this->BuffDataRemove(data);
		update = true;
	}

	if ( update )
		this->CalculateCharacter();
}

bool Player::MultiAttackCheckFireScreamSkill(uint16 target, uint8 serial)
{
	/*if ( g_SkillAdditionInfo.GetCheckFireScreamSkill() == FALSE )
		return TRUE;*/

	if ( serial > 100 )
	{
		for ( uint32 i = 0; i < MULTI_ATTACK_MAX_INFO; ++i )
			this->GetMultiAttack(i)->Reset();

		this->SetMultiAttackCount(0);
	}

	for ( uint32 i = 0; i < MULTI_ATTACK_MAX_INFO; ++i )
	{
		if ( this->GetMultiAttack(i)->GetIndex() == target )
		{
			this->GetMultiAttack(i)->IncreaseSerial(serial, 1);

			if ( this->GetMultiAttack(i)->GetSerial(serial) > 10 )
			{
				return false;
			}

			return true;
		}
	}

	this->GetMultiAttack(this->GetMultiAttackCount())->SetIndex(target);
	this->GetMultiAttack(this->GetMultiAttackCount())->IncreaseSerial(serial, 1);
	this->IncreaseMultiAttackCount(1);

	if ( this->GetMultiAttackCount() >= MULTI_ATTACK_MAX_INFO )
		this->SetMultiAttackCount(0);

	return true;
}
		
bool Player::MultiAttackCheckPenetrationSkill(uint16 target, uint8 serial)
{
	if ( serial == 10 )
	{
		for ( uint32 i = 0; i < MULTI_ATTACK_MAX_INFO; ++i )
			this->GetMultiAttack(i)->Reset();

		this->SetMultiAttackCount(0);
	}

	for ( uint32 i = 0; i < MULTI_ATTACK_MAX_INFO; ++i )
	{
		if ( this->GetMultiAttack(i)->GetIndex() == target )
		{
			if ( serial == 10 && this->GetMultiAttack(i)->GetSerial(serial) != 0 )
			{
				this->GetMultiAttack(i)->ResetSerial(0);
			}

			this->GetMultiAttack(i)->IncreaseSerial(serial, 1);

			if ( this->GetMultiAttack(i)->GetSerial(serial) > 1 )
				return false;
		}
	}

	this->GetMultiAttack(this->GetMultiAttackCount())->SetIndex(target);
	this->GetMultiAttack(this->GetMultiAttackCount())->IncreaseSerial(serial, 1);
	this->IncreaseMultiAttackCount(1);

	if ( this->GetMultiAttackCount() >= MULTI_ATTACK_MAX_INFO )
		this->SetMultiAttackCount(0);

	return true;
}

void Player::GetSkillDarkSideTargets(Unit* pTarget, skill_template const* skill_info, uint16 * target, int32 target_count)
{
	if (!skill_info)
	{
		return;
	}

	int32 distance = SkillHandler::GetSkillRange(this, skill_info->GetID());
	int32 count = 0;

	if (pTarget)
	{
		if (Util::Distance(this->GetX(), this->GetY(), pTarget->GetX(), pTarget->GetY()) <= distance)
		{
			target[count++] = pTarget->GetEntry();
		}
	}

	Object* pTargetSec = nullptr;

	VIEWPORT_LOOP(pTargetSec)

		if (!this->AttackAllowed(pTargetSec->ToUnit()))
		{
			continue;
		}

		if (Util::Distance(this->GetX(), this->GetY(), pTargetSec->GetX(), pTargetSec->GetY()) > distance)
		{
			continue;
		}

		target[count++] = pTargetSec->GetEntry();

		if (count >= target_count)
		{
			break;
		}

	VIEWPORT_CLOSE

	this->SetRageFighterSkillTarget(target[0]);
}

void Player::ApplySkillTime(uint16 skill, coord_type x, coord_type y)
{
	SkillTime* pSkillTime = this->GetSkillTime(skill);

	if (!pSkillTime)
	{
		this->m_SkillTimeMap[skill] = new SkillTime(x, y);
	}
	else
	{
		pSkillTime->SetCount(0);
		pSkillTime->GetTime()->Start();
		pSkillTime->SetX(x);
		pSkillTime->SetY(y);
	}
}

SkillTime* Player::GetSkillTime(uint16 skill)
{
	SkillTimeMap::const_iterator itr = this->m_SkillTimeMap.find(skill);

	if (itr != this->m_SkillTimeMap.end())
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}

int32 Player::GetBookCurseDamage() const
{
	Item const* pLeft = this->GetInventory()->GetItem(WEAPON_02);

	if (pLeft && pLeft->IsValid())
	{
		return pLeft->GetCursePower();
	}

	return 0;
}