/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Skill.h"
*
*/

#ifndef SKILL_H
#define SKILL_H

class Skill
{
	public:
		Skill()
		{
			this->Clear();
		}

		Skill(uint16 skill, uint8 level)
		{
			this->SetSkill(skill);
			this->SetLevel(level);
			this->SetWeapon(false);
			this->SetType(0);
		}

		void Clear();

	private:
		DECLARE_PROPERTY(uint16, Skill);
		DECLARE_PROPERTY(uint8, Level);
		DECLARE_BOOL(Weapon);
		DECLARE_ENUM(uint8, Type);
};

#endif