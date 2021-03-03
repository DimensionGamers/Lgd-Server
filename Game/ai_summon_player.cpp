bool SummonPlayerAI::ViewportListAddConditions(Unit* pAdd)
{
	if ( pAdd->IsPlayer() )
		return true;

	if ( pAdd->IsCreature() && (pAdd->ToCreature()->IsMonster() || pAdd->ToCreature()->IsNeutral()) )
	{
		return true;
	}

	return false;
}

bool SummonPlayerAI::MoveAttempt()
{
	me()->FollowOwner();
	return true;
}

bool SummonPlayerAI::MoveAllowed(int16 x, int16 y)
{
	return me()->GetWorld()->GetGrid(x, y).IsSummonedMoveAllowed();
}

bool SummonPlayerAI::SearchEnemy()
{
	if ( me()->GetSummoner()->IsInSafeZone() )
	{
		me()->SetTarget(nullptr);
		return true;
	}

	Object * pObject = nullptr;
	int32 min_distance = 999;

	VIEWPORT_LOOP_OBJECT(me(), pObject)

		if ( !pObject->IsCreature() || !pObject->ToCreature()->IsLive() )
			continue;

		if ( !pObject->ToCreature()->IsMonster() )
			continue;

		if ( pObject->ToCreature()->IsSummoned() && pObject->ToCreature()->GetSummoner()->IsPlayer() )
			continue;

		int32 dis = DISTANCE(me()->GetSummoner(), pObject);

		if ( dis < min_distance )
		{
			me()->SetTarget(pObject->ToUnit());
			min_distance = dis;
		}

	VIEWPORT_CLOSE

	return true;
}

void SummonPlayerAI::OnDie()
{
	if ( me()->GetSummoner() && me()->GetSummoner()->GetSummoned() == me() )
	{
		me()->GetSummoner()->SendSummonedHP(0, 60);
		me()->GetSummoner()->KillSummoned(false);
	}
}