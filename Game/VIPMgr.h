/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "VIPMgr.h"
*
*/

#ifndef VIP_MGR_H
#define VIP_MGR_H

struct vip_template
{
	DECLARE_ENUM(uint32, ID);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_FLOAT(Experience);
	DECLARE_ENUM(int32, Instance);
	DECLARE_ENUM(uint32, Duration);
	DECLARE_PROPERTY_STRING(Comment);
};

typedef std::map<uint32, vip_template*> VipTemplateMap;

class CVipMgr
{
	SingletonInstance(CVipMgr);

	public:
		CVipMgr();
		virtual ~CVipMgr();

		void LoadVipTemplate();
		vip_template * GetVipData(uint32 guid) const;

		void ApplyVipExperience(Player* pPlayer, int64 & experience);
		void SendVipData(Player* pPlayer);
	private:
		VipTemplateMap vip_template_map;
};

#endif