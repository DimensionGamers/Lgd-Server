/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "UnitSkill.cpp"
*
*/

void Unit::SkillGetDamage(uint16 skill, int32 & damage_min, int32 & damage_max)
{
	damage_min = sSkillMgr->GetSkillDamage(skill);
	damage_max = damage_min + damage_min / 2;
}

void Unit::SkillAngleCalculate(float angle, float tx, float ty, float sx, float sy, bool convert, int32 add_x, int32 add_y)
{
	vec3_t p[4];
	Vector(-tx, ty, 0.f, p[0]);
	Vector( tx, ty, 0.f, p[1]);
	Vector( sx, sy, 0.f, p[2]);
	Vector(-sx, sy, 0.f, p[3]);

	vec3_t Angle;
	Vector(0.f, 0.f, convert ? angle * 360 / 255: angle, Angle);

	float Matrix[3][4];
	AngleMatrix(Angle, Matrix);

	vec3_t vFrustrum[4];

	for( uint8 i = 0; i < MAX_ARRAY_FRUSTRUM; i++ )
	{
		VectorRotate(p[i], Matrix, vFrustrum[i]);

		if (add_x == -1 && add_y == -1)
		{
			this->SetSkillAngleX(i, static_cast<int32>(vFrustrum[i][0]) + this->GetX());
			this->SetSkillAngleY(i, static_cast<int32>(vFrustrum[i][1]) + this->GetY());
		}
		else
		{
			this->SetSkillAngleX(i, static_cast<int32>(vFrustrum[i][0]) + add_x);
			this->SetSkillAngleY(i, static_cast<int32>(vFrustrum[i][1]) + add_y);
		}
	}

	/*float p[MAX_ARRAY_FRUSTRUM][3];

	p[0][0] = -tx;
	p[0][1] = ty;
	p[0][2] = 0;
	p[1][0] = tx;
	p[1][1] = ty;
	p[1][2] = 0;
	p[2][0] = sx;
	p[2][1] = sy;
	p[2][2] = 0;
	p[3][0] = -sx;
	p[3][1] = sy;
	p[3][2] = 0;

	float Angle[3];

	Angle[0] = 0;
	Angle[1] = 0;
	Angle[2] = convert ? angle * 360 / 255: angle;

	float Matrix[3][MAX_ARRAY_FRUSTRUM];
	float vFrustrum[MAX_ARRAY_FRUSTRUM][3];

	AngleMatrix(Angle,Matrix);

	for( int32 i = 0; i < MAX_ARRAY_FRUSTRUM; ++i )
	{
		VectorRotate((const float *)&p[i],(const float *)&Matrix[0],(float *)&vFrustrum[i]);

		this->SetSkillAngleX(i, static_cast<int32>(vFrustrum[i][0]) + this->GetX());
		this->SetSkillAngleY(i, static_cast<int32>(vFrustrum[i][1]) + this->GetY());
	}*/
}

bool Unit::SkillInAngle(int16 x, int16 y)
{
	uint8 j = 3;

	for ( uint8 i = 0; i < MAX_ARRAY_FRUSTRUM; j = i, i++ )
	{
		if ( (this->GetSkillAngleX(i) - x) * (this->GetSkillAngleY(j) - y) -
			 (this->GetSkillAngleX(j) - x) * (this->GetSkillAngleY(i) - y) < 0 )
			 return false;
	}

	return true;
}

//#define M_PI       3.14159265358979323846f

/*void AngleMatrix (const float *angles, float (*matrix)[4] )	// Good - Strange this0.0174532925199432955f
{
	float		angle;
	float		sr, sp, sy, cr, cp, cy;
	
	angle = angles[2] *	0.0174532925199432955f;		//0.0174532821447436905f;
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[1] *   0.0174532925199432955f;
	sp = sin(angle);   //ST=0.0174532923847436905
	cp = cos(angle);
	angle = angles[0] * 0.0174532925199432955f;		//0.0174532821247436905f;
	sr = sin(angle);
	cr = cos(angle);

	// matrix = (YAW * PITCH) * ROLL
	matrix[0][0] = cp*cy;
	matrix[1][0] = cp*sy;
	matrix[2][0] = -sp;
	matrix[0][1] = sr*sp*cy+cr*-sy;
	matrix[1][1] = sr*sp*sy+cr*cy;
	matrix[2][1] = sr*cp;
	matrix[0][2] = (cr*sp*cy+-sr*-sy);
	matrix[1][2] = (cr*sp*sy+-sr*cy);
	matrix[2][2] = cr*cp;
	matrix[0][3] = 0.0;
	matrix[1][3] = 0.0;
	matrix[2][3] = 0.0;
}*/

void AngleMatrix (const vec3_t angles, float (*matrix)[4] )
{
	float		angle;
	float		sr, sp, sy, cr, cp, cy;
	
	angle = angles[2] * (Q_PI*2 / 360);
	sy = (float)sin(angle);
	cy = (float)cos(angle);
	angle = angles[1] * (Q_PI*2 / 360);
	sp = (float)sin(angle);
	cp = (float)cos(angle);
	angle = angles[0] * (Q_PI*2 / 360);
	sr = (float)sin(angle);
	cr = (float)cos(angle);

	// matrix = (Z * Y) * X
	matrix[0][0] = cp*cy;
	matrix[1][0] = cp*sy;
	matrix[2][0] = -sp;
	matrix[0][1] = sr*sp*cy+cr*-sy;
	matrix[1][1] = sr*sp*sy+cr*cy;
	matrix[2][1] = sr*cp;
	matrix[0][2] = (cr*sp*cy+-sr*-sy);
	matrix[1][2] = (cr*sp*sy+-sr*cy);
	matrix[2][2] = cr*cp;
	matrix[0][3] = 0.0;
	matrix[1][3] = 0.0;
	matrix[2][3] = 0.0;
}

/*void VectorRotate (const float in1[3*4],const float in2[3*4],float out[3*4])	// Good
{
	out[0*4+0] = in1[0*4+0] * in2[0*4+0] + in1[0*4+1] * in2[0*4+1] + in1[0*4+2] * in2[0*4+2];
	out[0*4+1] = in1[0*4+0] * in2[0*4+4] + in1[0*4+1] * in2[0*4+5] + in1[0*4+2] * in2[0*4+6];
	out[0*4+2] = in1[0*4+0] * in2[0*4+8] + in1[0*4+1] * in2[0*4+9] + in1[0*4+2] * in2[0*4+10];
}*/

void VectorRotate (const vec3_t in1, const float in2[3][4], vec3_t out)
{
	out[0] = DotProduct(in1, in2[0]);
	out[1] = DotProduct(in1, in2[1]);
	out[2] = DotProduct(in1, in2[2]);
}

int32 Unit::GetAngle(int16 x, int16 y)
{
	double diffX = this->GetX() - x;
	double diffY = this->GetY() - y;

	double rad = atan2(diffY, diffX);

	int32 angle = rad * 180 / 3.141592741012573 + 90;
	//int32 angle = rad * 180 / 3.141592741012573;

	if( angle < 0 )
	{
		angle += 360;
	}
	return angle;
}

void Unit::GetPartyMembers(Unit* mMember[MAX_PARTY_MEMBERS], uint8 & count, uint8 distance)
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
}

void Unit::RunNova()
{
	SkillHandler(this, nullptr, this->MagicGet(this->GetNova()->GetSkill())).SkillUseProc();
}

void Unit::SkillTeleportUse(int16 x, int16 y)
{
	if ( this->IsTeleporting() )
		return;

	this->SetViewState(VIEW_STATE_TELEPORT);
	this->SetTeleportTime(GetTickCount());
	this->SetTeleportStatus(TELEPORT_STANDBY);
	this->ClearPathData();
	this->SetX(x);
	this->SetY(y);
	this->SetTX(x);
	this->SetTY(y);
	
	this->ViewportDeleteSend();
}

void Unit::UpdateTeleport()
{
	Player* pPlayer = this->ToPlayer();

	if ( !pPlayer )
	{
		return;
	}

	switch ( this->GetTeleportStatus() )
	{
	case TELEPORT_STANDBY:
		{
			if ( this->GetDeadStatus() != DEAD_NONE )
			{
				this->SetTeleportStatus(TELEPORT_NONE);
			}
			else if( (GetTickCount() - this->GetTeleportTime()) > sGameServer->GetSkillTeleportStateTime(this->GetTeleportStatus()) )
			{
				this->SetTeleportTime(GetTickCount());
				this->SetTeleportStatus(TELEPORT_READY);

				//this->ViewportDeleteSend();
			}
		} break;

	case TELEPORT_READY:
		{
			if( (GetTickCount() - this->GetTeleportTime()) > sGameServer->GetSkillTeleportStateTime(this->GetTeleportStatus()) )
			{
				this->SetTeleportTime(GetTickCount());
				this->SetTeleportStatus(TELEPORT_UNK);

				if (sGameServer->IsTeleportFix())
				{
					if (this->IsPlayer())
					{
						this->SetTeleportStatus(TELEPORT_NONE);
					}
				}

				this->SetTX(this->GetX());
				this->SetTY(this->GetY());
				this->CreateFrustrum();

				pPlayer->TeleportResult(0, false);
				pPlayer->ViewportCreate(VIEWPORT_CREATE_FLAG_ME | VIEWPORT_CREATE_FLAG_GEN | VIEWPORT_CREATE_FLAG_GUILD | VIEWPORT_CREATE_FLAG_SIEGE);
			}
		} break;

	case TELEPORT_UNK:
		{
			if( (GetTickCount() - this->GetTeleportTime()) > sGameServer->GetSkillTeleportStateTime(this->GetTeleportStatus()) )
			{
				this->SetTeleportTime(GetTickCount());
				this->SetTeleportStatus(TELEPORT_NONE);
			}
		} break;
	}

	if ( this->GetTeleportStatus() == TELEPORT_UNK )
	{
		//if( (GetTickCount() - this->GetTeleportTime()) > sGameServer->GetSkillTeleportStateTime(this->GetTeleportStatus()) )
		//{
			this->SetTeleportTime(GetTickCount());
			this->SetTeleportStatus(TELEPORT_NONE);
		//}
	}
}

bool Unit::IsDebuffInmune(bool elemental) const
{
	if ( this->HasBuff(BUFF_IRON_DEFENSE) || this->HasBuff(BUFF_IRON_DEFENSE_IMPROVED) )
	{
		return true;
	}

	if ( elemental )
	{
		if ( this->HasBuff(BUFF_ERRTEL_IMMUNE_II) )
		{
			return true;
		}
	}

	return false;
}

bool Unit::IsInTeleportInmuneState() const
{
	switch ( this->GetTeleportStatus() )
	{
	case TELEPORT_STANDBY:
	case TELEPORT_READY:
	case TELEPORT_UNK:
		{
			return sGameServer->IsSkillTeleportStateAttack(this->GetTeleportStatus());
		} break;
	}

	return false;
}

bool Unit::IsInTeleportRemoveViewportState() const
{
	switch ( this->GetTeleportStatus() )
	{
	case TELEPORT_STANDBY:
	case TELEPORT_READY:
	case TELEPORT_UNK:
		{
			return sGameServer->IsSkillTeleportStateRemoveVP(this->GetTeleportStatus());
		} break;
	}

	return false;
}