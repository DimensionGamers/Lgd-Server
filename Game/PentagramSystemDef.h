#ifndef PENTRAGRAM_SYSTEM_DEF_H
#define PENTRAGRAM_SYSTEM_DEF_H

struct ErrtelRankData
{
	DECLARE_ENUM(uint8, Index);
	DECLARE_ENUM(uint8, Level);

	void Reset()
	{
		this->SetIndex(-1);
		this->SetLevel(-1);
	}
};

struct PentagramJewelInfo
{
	void Clear()
	{
		this->SetType(-1);
		this->SetIndex(-1);
		this->SetAttribute(-1);
		this->SetItemSection(-1);
		this->SetItemType(511);
		this->SetItemLevel(0);

		SOCKET_SLOT_LOOP(i)
		{
			this->GetRankData(i)->Reset();
		}

		this->SetSerialServer(0);
		this->SetSerial(0);
		this->SetDBFlag(0);
		this->ResetData(0);
	}

	std::string BuildLogDB() const;

	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint8, Index);
	DECLARE_ENUM(uint8, Attribute);
	DECLARE_ENUM(uint8, ItemSection);
	DECLARE_ENUM(uint16, ItemType);
	DECLARE_ENUM(uint8, ItemLevel);
	DECLARE_ARRAY_STRUCT(ErrtelRankData, RankData, MAX_SOCKET_SLOT);
	DECLARE_ENUM(uint16, SerialServer);
	DECLARE_ENUM(uint32, Serial);
	DECLARE_FLAG(uint32, DBFlag);
	DECLARE_PROPERTY_ARRAY(int32, Data, 3);

	uint8 GetRank() const
	{
		uint8 count = 0;
		INIT_SOCKET_DATA(SocketOption);
		SOCKET_SLOT_LOOP(i)
		{
			SocketOption[i] = this->GetRankData(i)->GetIndex() | (this->GetRankData(i)->GetLevel() * 16);
		}

		SOCKET_SLOT_LOOP(i)
		{
			if (SocketOption[i] == 0xFF)
			{
				SocketOption[i] = SOCKET_SLOT_NONE;
			}
			else if (SocketOption[i] == 0xFE)
			{
				SocketOption[i] = SOCKET_SLOT_EMPTY;
			}
			else
			{
				++count;
			}
		}

		return count;
	}
};

#endif