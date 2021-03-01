MonsterAIHandler::MonsterAIHandler(Monster* pMonster)
{
	this->SetMonster(pMonster);
	this->SetAIAutomata(nullptr);
	this->SetAIElement(nullptr);
	this->SetAIState(nullptr);
	this->SetAIUnit(nullptr);
	this->SetTarget(nullptr);
}
		
void MonsterAIHandler::Run()
{
	this->SetAIUnit(sMonsterMgr->GetMonsterAIUnit(this->GetMonster()->GetCurrentAI()));

	if ( !this->GetAIUnit() )
		return;

	if ( (GetTickCount() - this->GetMonster()->GetLastAIRunTime()) < this->GetAIUnit()->GetDelay() )
		return;

	if ( !this->RunAIUnit() )
		return;

	this->GetMonster()->SetLastAIRunTime(GetTickCount());
}

bool MonsterAIHandler::RunAIUnit()
{
	this->SetAIAutomata(this->GetAIUnit()->GetAutomata());

	if ( !this->GetAIAutomata() )
		return false;

	if ( (GetTickCount() - this->GetMonster()->GetLastAIAutomataRunTime()) < this->GetMonster()->GetLastAIAutomataDelay() )
		return false;

	if ( !this->RunAIAutomata() )
		return false;

	this->GetMonster()->SetLastAIAutomataRunTime(GetTickCount());

	this->SetAIElement(this->GetAIUnit()->GetAIClass(this->GetAIState()->GetNextState()));

	if ( !this->GetAIElement() )
		return false;

	if ( !this->ForceAIElement() )
		return false;

	this->GetMonster()->SetCurrentAIState(this->GetAIState()->GetNextState());
	return true;
}

bool MonsterAIHandler::RunAIAutomata()
{
	if ( !MAX_AI_STATE_RANGE(this->GetMonster()->GetCurrentAIState()) )
		return nullptr;

	Player* pPlayer = this->GetMonster()->GetMaxAggro();
	int64 MaxAggro = this->GetMonster()->GetThreatManager()->GetHighestThreat();

	uint8 iCurrentState = this->GetMonster()->GetCurrentAIState();
	int32 iMaxStateTransCount = this->GetAIAutomata()->m_AIStatCount[iCurrentState];

	for ( int32 iPriority = 0; iPriority < iMaxStateTransCount; iPriority++ )
	{
		monster_ai_state const* pAIState = &this->GetAIAutomata()->m_AIState[iCurrentState][iPriority];
		BOOL bTransition = MAI_STATE_TRANS_VALUE_;	// If there is an apropiate Value it is set to TRUE

		// Choose the right action according Transation Type 
		switch ( pAIState->GetTransitionType() )
		{
			case MAI_STATE_TRANS_NO_ENEMY:
				if ( pPlayer == nullptr )
				{
					bTransition = true;
				}
				break;

			case MAI_STATE_TRANS_IN_ENEMY:
				if ( pPlayer )
				{
					if ( IN_RANGE(this->GetMonster(), pPlayer, this->GetMonster()->GetAttackRange()) )
					{
						bTransition = true;
					}
				}
				break;

			case MAI_STATE_TRANS_OUT_ENEMY:
				if ( pPlayer )
				{
					if ( !IN_RANGE(this->GetMonster(), pPlayer, this->GetMonster()->GetAttackRange()) )
					{
						bTransition = true;
					}
				}
				break;

			//case MAI_STATE_TRANS_DIE_ENEMY:

			case MAI_STATE_TRANS_DEC_HP:
				{
					if ( pAIState->GetTransitionValue() > this->GetMonster()->PowerGet(POWER_LIFE) )
					{
						bTransition = true;
					}
				} break;

			case MAI_STATE_TRANS_IMMEDIATELY:
				{
					bTransition = true;
				} break;

			case MAI_STATE_TRANS_DEC_HP_PER:
				{
					int32 transition_value = pAIState->GetTransitionValue();
					transition_value = (transition_value > 0)? transition_value : 0;
					transition_value = (transition_value < 100 )? transition_value : 100;

					int32 iLife = this->GetMonster()->PowerGetTotal(POWER_LIFE) * transition_value / 100;

					if ( iLife > this->GetMonster()->PowerGet(POWER_LIFE) )
					{
						bTransition = true;
					}
				} break;

			case MAI_STATE_TRANS_AGRO_UP:
				{
					if ( pPlayer )
					{
						if ( MaxAggro >= pAIState->GetTransitionValue() )
						{
							bTransition = true;
						}
					}
				} break;

			case MAI_STATE_TRANS_AGRO_DOWN:
				{
					if ( pPlayer )
					{
						if ( MaxAggro <= pAIState->GetTransitionValue() )
						{
							bTransition = true;
						}
					}
				} break;

			case MAI_STATE_TRANS_GROUP_SOMMON:
				{
					if ( this->GetMonster()->GetGroupAI() )
					{
						if ( this->GetMonster()->FindGroupMemberToSummon() )
						{
							bTransition = true;
						}
					}
				} break;

			case MAI_STATE_TRANS_GROUP_HEAL:
				{
					if ( this->GetMonster()->GetGroupAI() )
					{
						if ( this->GetMonster()->FindGroupMemberToHeal() )
						{
							bTransition = true;
						}
					}
				} break;
		}

		// If is Wrong State
		if ( !bTransition )	
			continue;

		if ( Random<uint8>(100) < pAIState->GetTransitionRate() )
		{
			this->GetMonster()->SetLastAIAutomataDelay(pAIState->GetDelay());
			this->SetAIState(pAIState);
			return true;
		}
	}

	return false;
}

bool MonsterAIHandler::ForceAIElement()
{
	if ( Random<uint8>(100) > this->GetAIElement()->GetRate() )
		return false;

	switch ( this->GetAIElement()->GetClass() )
	{
	case MAE_TYPE_COMMON_NORMAL:
		this->ApplyElementCommon();
		break;
	case MAE_TYPE_MOVE_NORMAL:
		this->ApplyElementMove();
		break;
	case MAE_TYPE_MOVE_TARGET:
		this->ApplyElementMoveTarget();
		break;
	case MAE_TYPE_GROUP_MOVE:
		this->ApplyElementGroupMove();
		break;
	/*case MAE_TYPE_GROUP_MOVE_TARGET:
		this->();
		break;*/
	case MAE_TYPE_ATTACK_NORMAL:
		this->ApplyElementAttack();
		break;
	case MAE_TYPE_ATTACK_AREA:
		this->ApplyElementAttackArea();
		break;
	case MAE_TYPE_ATTACK_PENETRATION:
		this->ApplyElementAttackPenetration();
		break;
	case MAE_TYPE_HEAL_SELF:
		this->ApplyElementHealSelf();
		break;
	case MAE_TYPE_HEAL_GROUP:
		this->ApplyElementHealGroup();
		break;
	case MAE_TYPE_AVOID_NORMAL:
		this->ApplyElementAvoid();
		break;
	/*case MAE_TYPE_HELP_HP:
		this->ApplyElement();
		break;*/
	/*case MAE_TYPE_HELP_BUFF:
		this->ApplyElement();
		break;*/
	/*case MAE_TYPE_HELP_TARGET:
		this->ApplyElement();
		break;*/
	/*case MAE_TYPE_SPECIAL:
		this->ApplyElement();
		break;*/
	case MAE_TYPE_SPECIAL_SOMMON:
		this->ApplyElementSpecialSommon();
		break;
	case MAE_TYPE_SPECIAL_NIGHTMARE_SUMMON:
		this->ApplyElementNightmareSummon();
		break;
	case MAE_TYPE_SPECIAL_WARP:
		this->ApplyElementNightmareWarp();
		break;
	case MAE_TYPE_SPECIAL_SKILLATTACK:
		this->ApplyElementSkillAttack();
		break;
	case MAE_TYPE_SPECIAL_CHANGEAI:
		this->ApplyElementAIChange();
		break;
	/*case MAE_TYPE_EVENT:
		this->ApplyElement();
		break;*/
	case MAE_TYPE_SPECIAL_IMMUNE:
		this->ApplyElementSpecialImmune();
		break;
	}
	return true;
}

void MonsterAIHandler::ApplyElementCommon()
{

}
	
void MonsterAIHandler::ApplyElementMove()
{
	if ( this->GetMonster()->GetPathData()->IsStartEnd() )
		return;

	bool bFindXY = false;

	if ( this->GetAIState()->GetTransitionType() == 2 )
		bFindXY = this->GetMonster()->GetXYToChase();
	else
		bFindXY = this->GetMonster()->GetXYToPatrol();

	if ( bFindXY )
		this->GetMonster()->PathFindToMove();
}
	
void MonsterAIHandler::ApplyElementMoveTarget()
{
	if ( this->GetMonster()->GetPathData()->IsStartEnd() )
		return;

	if ( this->GetMonster()->GetX() == this->GetAIElement()->GetX() &&
		 this->GetMonster()->GetY() == this->GetAIElement()->GetY() )
	{
		this->ApplyElementMove();
		return;
	}

	World* pWorld = this->GetMonster()->GetWorld();

	if ( !pWorld )
		return;

	bool bFindXY = true;
	coord_type iTargetX = this->GetAIElement()->GetX();
	coord_type iTargetY = this->GetAIElement()->GetY();
	int32 iTargetDistance = Util::Distance(this->GetMonster()->GetX(), this->GetMonster()->GetY(), iTargetX, iTargetY);

	if ( !pWorld->m_path_list.empty() )
	{
		if ( iTargetDistance > 10 )
		{
			int32 iMinCost = 1000000;
			coord_type iMidX = -1;
			coord_type iMidY = -1;

			for ( WorldAIPathList::const_iterator it = pWorld->m_path_list.begin(); it != pWorld->m_path_list.end(); ++it )
			{
				WorldAIPath const* PathInfo = (*it);

				if ( !PathInfo )
					continue;

				float fDistX = float(this->GetMonster()->GetX() - PathInfo->GetX());
				float fDistY = float(this->GetMonster()->GetY() - PathInfo->GetY());
				int32 iPathSpotDist =  int32(sqrt((fDistX*fDistX) + (fDistY*fDistY)));

				if ( iPathSpotDist < 20 )
				{
					fDistX = float(iTargetX - PathInfo->GetX());
					fDistY = float(iTargetY - PathInfo->GetY());
					int32 iMidDist = int32(sqrt((fDistX*fDistX) + (fDistY*fDistY)));

					if ( iMinCost > iMidDist )
					{
						if ( iMidDist )
						{
							iMinCost = iMidDist;
							iMidX = PathInfo->GetX();
							iMidY = PathInfo->GetY();
						}
					}
				}
			}

			if ( iMinCost != 1000000 )
			{
				iTargetX = iMidX;
				iTargetY = iMidY;
				bFindXY = true;
			}
		}
	}

	if ( bFindXY )
	{
		this->GetMonster()->SetTempX(iTargetX);
		this->GetMonster()->SetTempY(iTargetY);
		this->GetMonster()->GetPathData()->SetStartEnd(this->GetMonster()->PathFindToMove());
	}
}
	
void MonsterAIHandler::ApplyElementGroupMove()
{
	if ( this->GetMonster()->GetPathData()->IsStartEnd() )
		return;

	bool bFindXY = false;
	bool bFoundLeader = true;
	Monster* pLeader = sObjectMgr->FindAIGroupLeader(this->GetMonster()->GetGroupAI());

	if ( !pLeader )
		bFoundLeader = false;

	if ( bFoundLeader && !IN_RANGE(pLeader, this->GetMonster(), 6) )
	{
		this->GetMonster()->SetTarget(pLeader);
		bFindXY = this->GetMonster()->GetXYToChase();
	}
	else if ( this->GetAIState()->GetTransitionType() == 2 )
	{
		bFindXY = this->GetMonster()->GetXYToChase();
	}
	else
	{
		bFindXY = this->GetMonster()->GetXYToPatrol();
	}

	if ( bFindXY )
	{
		if ( !this->GetMonster()->PathFindToMove() )
		{
			this->GetMonster()->GetXYToPatrol();
			this->GetMonster()->PathFindToMove();
		}
	}
}

void MonsterAIHandler::ApplyElementAttack()
{
	this->GetMonster()->GetAction()->Attack = 1;
	this->GetMonster()->Attack();
}
	
void MonsterAIHandler::ApplyElementAttackArea()
{
	coord_type iTargetX = this->GetAIElement()->GetX() + Random(5) * ((Random(2) == 0) ? 1 : -1);
	coord_type iTargetY = this->GetAIElement()->GetY() + Random(5) * ((Random(2) == 0) ? 1 : -1);

	Player* pPlayer = nullptr;

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		pPlayer = it->second;

		if ( !pPlayer )
			continue;

		if ( !pPlayer->IsLive() )
			continue;

		if ( !pPlayer->SameDimension(this->GetMonster()) )
			continue;

		int32 distance = Util::Distance(pPlayer->GetX(), pPlayer->GetY(), iTargetX, iTargetY);

		if ( distance >= 10 )
			continue;

		MONSTER_ATTACK_REGION pMsg(this->GetMonster()->GetClass(), this->GetMonster()->GetX(), this->GetMonster()->GetY(), iTargetX, iTargetY);
		pPlayer->sendPacket(MAKE_PCT(pMsg));

		if ( distance < 6 )
		{
			this->GetMonster()->AttackProc(pPlayer, nullptr);
		}
	}
}
	
void MonsterAIHandler::ApplyElementAttackPenetration()
{
	Unit* pTarget = this->GetMonster()->GetTarget();

	if ( !pTarget || !pTarget->IsLive() )
		return;

	this->GetMonster()->SpecialMagicAttack(0, 2);
}
	
void MonsterAIHandler::ApplyElementAvoid()
{
	if ( this->GetMonster()->GetXYToEscape() )
	{
		this->GetMonster()->PathFindToMove();
	}
}

void MonsterAIHandler::ApplyElementHealSelf()
{
	this->GetMonster()->PowerIncrease(POWER_LIFE, this->GetMonster()->PowerGet(POWER_LIFE) * 20 / 100);
	this->GetMonster()->AddBuff(BUFF_GREATER_DEFENSE, -10, BUFF_FLAG_CONSTANT, this->GetMonster());
}
	
void MonsterAIHandler::ApplyElementHealGroup()
{
	Monster* pMonster = this->GetMonster()->FindGroupMemberToHeal();
	
	if ( pMonster && pMonster->IsLive() )
	{
		this->GetMonster()->SpecialMagicAttack(21, -1, pMonster);
	}
}
	
void MonsterAIHandler::ApplyElementSpecialSommon()
{
	Monster* pMonster = this->GetMonster()->FindGroupMemberToSummon();
	
	if ( pMonster && pMonster->IsLive() )
	{
		this->GetMonster()->SpecialMagicAttack(30, -1, pMonster);
	}
}
	
void MonsterAIHandler::ApplyElementSpecialImmune()
{
	int32 time_magic = this->GetAIElement()->GetX();
	int32 time_attack = this->GetAIElement()->GetY();

	if ( time_magic < 1 )
		time_magic = 10;

	if ( time_attack < 1 )
		time_attack = 10;

	this->GetMonster()->AddBuff(BUFF_INMUNE_SKILL, time_magic, 0, this->GetMonster());
	this->GetMonster()->AddBuff(BUFF_INMUNE_ATTACK, time_attack, 0, this->GetMonster());
}
	
void MonsterAIHandler::ApplyElementNightmareSummon()
{
	this->GetMonster()->SpecialMagicAttack(30, -1, this->GetMonster()->GetTarget());

	sKanturuMgr->AddSummonedMonster();
}
	
void MonsterAIHandler::ApplyElementNightmareWarp()
{
	this->GetMonster()->MagicAttackSend(ENTRY(this->GetMonster()), SKILL_TELEPORT);
	this->GetMonster()->SkillTeleportUse(this->GetAIElement()->GetX(), this->GetAIElement()->GetY());
	this->GetMonster()->SetTarget(nullptr);
}
	
void MonsterAIHandler::ApplyElementSkillAttack()
{
	if ( !this->GetTarget() )
		return;

	int32 iRate1 = this->GetAIElement()->GetTarget();
	int32 iRate2 = this->GetAIElement()->GetX();
	int32 iRate3 = this->GetAIElement()->GetY();
	int32 iRandom = Random(100);

	if ( iRandom < iRate1 )
		this->GetMonster()->SpecialMagicAttack(0, 0, this->GetMonster()->GetTarget());
	else if ( iRandom < (iRate1+iRate2) )
		this->GetMonster()->SpecialMagicAttack(0, 1, this->GetMonster()->GetTarget());
	else if ( iRandom < (iRate1+iRate2+iRate3) )
		this->GetMonster()->SpecialMagicAttack(0, 2, this->GetMonster()->GetTarget());

	this->GetMonster()->GetAction()->Attack = 0;
}
	
void MonsterAIHandler::ApplyElementAIChange()
{
	sObjectMgr->ChangeAIOrder(this->GetAIElement()->GetTarget(), this->GetAIElement()->GetX());
}