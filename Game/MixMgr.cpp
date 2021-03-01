MixMgr::MixMgr()
{

}

MixMgr::~MixMgr()
{
	this->Clear();
}

void MixMgr::Clear()
{
	for ( MixDisabledMap::iterator itr = this->mix_disabled.begin(); itr != this->mix_disabled.end(); ++itr )
	{
		itr->second.clear();
	}

	this->mix_disabled.clear();

	LIST_CLEAR(MixSpellStoneList::iterator, this->m_MixSpellStoneList);

	LIST_CLEAR(MixGuardianList::iterator, this->m_MixGuardianList);
}

void MixMgr::LoadDisabled()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Mix Disabled...");

	for ( MixDisabledMap::iterator itr = this->mix_disabled.begin(); itr != this->mix_disabled.end(); ++itr )
	{
		itr->second.clear();
	}

	this->mix_disabled.clear();

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM mix_disabled");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			this->mix_disabled[fields[0].GetUInt8()][fields[1].GetUInt16()] = 1;
			count++;
		}
		while(result->NextRow());
	}
		
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u mix disabled definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MixMgr::LoadSpellStone()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Mix SpellStone...");

	LIST_CLEAR(MixSpellStoneList::iterator, this->m_MixSpellStoneList);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM mix_spellstone AS i ORDER BY i.type ASC, i.count ASC");

	if (result)
	{
		do
		{
			FieldReader reader(result->Fetch());

			MixSpellStone * pData = new MixSpellStone;
			pData->SetType(reader.GetUInt8());
			pData->SetCount(reader.GetUInt16());

			for (int32 i = 0; i < MAX_SPELL_STONE_COUNT; ++i)
			{
				pData->SetRate(i, reader.GetUInt16());
			}

			this->m_MixSpellStoneList.push_back(pData);
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u mix spell stone definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MixMgr::LoadGuardian()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Mix Guardian...");

	LIST_CLEAR(MixGuardianList::iterator, this->m_MixGuardianList);

	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM mix_guardian");

	if (result)
	{
		do
		{
			FieldReader reader(result->Fetch());
			
			MixGuardian * pData = new MixGuardian;
			pData->SetType(reader.GetUInt8());
			pData->SetName(reader.GetString());
			pData->SetID(reader.GetUInt8());
			pData->SetMainItem(reader.ReadItem());
			pData->SetMainItemLevel(reader.GetUInt8());
			pData->SetMainItemDurability(reader.GetUInt8());

			for (int32 i = 0; i < MAX_GUARDIAN_MIX_MATERIAL; ++i)
			{
				pData->SetMaterialItem(i, reader.ReadItem());
				pData->SetMaterialAmount(i, reader.GetInt32());
			}

			pData->SetPrice(reader.GetInt32());
			pData->SetSuccessRate(reader.GetInt32());
			pData->SetResultItem(reader.ReadItem());
			pData->SetResultLevel(reader.GetUInt8());

			this->m_MixGuardianList.push_back(pData);
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u mix guardian definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

MixGuardian const* MixMgr::GetGuardianMix(uint8 type, uint8 id) const
{
	for (auto & pData : this->m_MixGuardianList)
	{
		if (!pData)
		{
			continue;
		}

		if (pData->GetType() == type && pData->GetID() == id)
		{
			return pData;
		}
	}

	return nullptr;
}

bool MixMgr::IsMixDisabled(uint8 type, uint16 id) const
{
	MixDisabledMap::const_iterator itr = this->mix_disabled.find(type);

	if ( itr != this->mix_disabled.end() )
	{
		MixDisabledList::const_iterator it = itr->second.find(id);

		return (it != itr->second.end());
	}
	else
	{
		return false;
	}
}

uint16 MixMgr::GetSpiritStoneCount(uint8 type, int64 price) const
{
	if (type >= MAX_SPIRIT_STONE)
	{
		return 0;
	}

	int32 division = price / sGameServer->GetMixSpellStonePriceDivision(type);
	uint16 count = -1;
	uint16 id = -1;

	for (auto & pData : this->m_MixSpellStoneList)
	{
		if (!pData)
		{
			continue;
		}

		if (pData->GetType() != type)
		{
			continue;
		}

		if (division >= pData->GetCount())
		{
			count = pData->GetCount();
		}
	}

	if (count == uint16(-1))
	{
		return 0;
	}

	RandomValue<uint16> m_RandomValue(0);

	for (auto & pData : this->m_MixSpellStoneList)
	{
		if (!pData)
		{
			continue;
		}

		if (pData->GetType() != type)
		{
			continue;
		}

		if (pData->GetCount() != count)
		{
			continue;
		}

		for (int32 i = 0; i < MAX_SPELL_STONE_COUNT; ++i)
		{
			if (pData->GetRate(i) > 0)
			{
				m_RandomValue.AddValue(i, pData->GetRate(i));
			}
		}
	}

	return m_RandomValue.GetRandomValue(RANDOM_POOL_WEIGHT);
}