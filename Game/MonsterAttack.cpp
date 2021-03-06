/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "MonsterAttack.cpp"
*
*/

void Monster::Attack()
{
	if ( this->GetAction()->Attack == 0 )
		return;

	if (this->GetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN) <= 0 ||
		this->GetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX) <= 0)
	{
		this->SetTarget(nullptr);
		this->GetAction()->Emotion = EMOTION_REST;
		this->GetAction()->Attack = 0;
		this->GetAction()->Move = 0;
		this->SetNextActionTime(1000);
		return;
	}

	if ( this->GetPathData()->IsStartEnd() )
	{
		this->SetNextActionTime(1000);
		return;
	}

	if ((MyGetTickCount() - this->GetLastMoveTime()) < 500)
	{
		return;
	}

	this->IncreaseTargetHitCount(1);

	if ( this->GetAI() && this->GetAI()->OnAttack(this->GetTarget()) )
		return;

	if ( !this->GetTarget() || 
		 !this->GetTarget()->IsLive() || 
		 !this->GetTarget()->IsPlaying() || 
		 this->GetTarget()->IsInTeleportInmuneState() || 
		 this->GetTarget()->IsInSafeZone() ||
		 !this->SameDimension(this->GetTarget()) )
	{
		this->SetTarget(nullptr);
		this->GetAction()->Emotion = EMOTION_REST;
		this->GetAction()->Attack = 0;
		this->GetAction()->Move = 0;
		this->SetNextActionTime(1000);
		return;
	}

	if ( Random(4) && this->SpecialMagicAttack() )
	{
		this->SetNextActionTime(this->GetIntData(UNIT_INT_MAGIC_SPEED));
	}
	else if ( Random(3) && this->MagicAttack() )
	{
		this->SetNextActionTime(this->GetIntData(UNIT_INT_MAGIC_SPEED));
	}
	else
	{
		this->NormalAttack();
	}

	this->GetAction()->Attack = 0;
	this->SetLastActionTime(MyGetTickCount());
}

bool Monster::MagicAttack()
{
	if ( this->IsDelayed() )
		return false;

	Skill * mSkill = this->GetRandomSkill();

	if ( !mSkill )
		return false;

	skill_template const* skill_info = sSkillMgr->GetSkill(mSkill->GetSkill());

	if ( !skill_info )
		return false;

	return SkillHandler(this, this->GetTarget(), mSkill, skill_info).SkillUseProc(false, this->GetX(), this->GetY(), this->GetDirection(), 0, 0, skill_info->animation.get());
}
	
void Monster::NormalAttack()
{
	if ( !this->GetAI() || !this->GetAI()->NormalAttack() )
	{
		this->AttackProc(this->GetTarget(), nullptr);
	}
	
	this->ActionSend(this->GetTarget()->GetEntry(), 120, this->GetDirection());
	this->SetNextActionTime(this->GetIntData(UNIT_INT_ATTACK_SPEED));
}

bool Monster::SpecialMagicAttack()
{
	return this->SpecialMagicAttack(this->GetRandomSpecialSkill(), -1, this->GetTarget());
}

bool Monster::SpecialMagicAttack(uint16 skill, int16 type, Unit* pTarget)
{
	if ( !skill && type != -1 )
	{
		skill = sMonsterManager->GetRandomSkillSpecial(this, type);
	}

	if ( !this->HasSpecialSkill(skill) )
		return false;

	skill_special const* skill_info = sSkillMgr->GetSkillSpecial(skill);

	if ( !skill_info )
		return false;

	SpecialSkillHandler(this, skill, skill_info, pTarget).Run();
	return true;
}

void Monster::SpecialMagicSend(Unit* mTarget, uint16 skill)
{
	SPECIAL_SKILL_SEND pMsg(this->GetEntry(), mTarget ? mTarget->GetEntry() : this->GetEntry(), skill);
	this->sendPacket_viewport(MAKE_PCT(pMsg));
}

Skill * Monster::GetRandomSkill()
{
	RandomValue<int32> m_randomizer(-1);

	SKILL_LOOP(i)
	{
		if ( this->GetSkill(i)->GetSkill() == 0 )
			continue;
		
		m_randomizer.AddValue(i, 0);
	}

	int32 position = m_randomizer.GetRandomValue(RANDOM_POOL_RANDOM);

	return position == -1 ? nullptr: this->GetSkill(position);
}