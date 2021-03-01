/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "VIPMgr.cpp"
*
*/

CVipMgr::CVipMgr()
{

}

CVipMgr::~CVipMgr()
{
	MAP_CLEAR(VipTemplateMap::iterator, this->vip_template_map);
}

void CVipMgr::LoadVipTemplate()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Vip Template...");

	MAP_CLEAR(VipTemplateMap::iterator, this->vip_template_map);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM vip_template");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			vip_template * add_vip = new vip_template;

			add_vip->SetID(fields[0].GetUInt32());
			add_vip->SetName(fields[1].GetString());
			add_vip->SetExperience(fields[2].GetFloat());
			add_vip->SetInstance(fields[3].GetInt32());
			add_vip->SetDuration(fields[4].GetUInt32());
			add_vip->SetComment(fields[5].GetString());

			this->vip_template_map[add_vip->GetID()] = add_vip;
		
			count++;
		}
		while(result->NextRow());
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u vip template", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

vip_template * CVipMgr::GetVipData(uint32 guid) const
{
	VipTemplateMap::const_iterator it = this->vip_template_map.find(guid);

	if ( it != this->vip_template_map.end() )
		return it->second;

	return nullptr;
}

void CVipMgr::ApplyVipExperience(Player* pPlayer, int64 & experience)
{
	vip_template const* vip_data = this->GetVipData(pPlayer->GetAccountData()->GetVIPStatus());

	if ( !vip_data )
		return;

	experience *= vip_data->GetExperience();
}

void CVipMgr::SendVipData(Player* pPlayer)
{
	vip_template const* vip_data = this->GetVipData(pPlayer->GetAccountData()->GetVIPStatus());

	if ( !vip_data )
		return;

	pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "[ VIP ]: %s.", vip_data->GetName().c_str());
}