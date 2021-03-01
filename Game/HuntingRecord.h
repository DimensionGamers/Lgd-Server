#ifndef HUNTING_RECORD_H
#define HUNTING_RECORD_H

class HuntingRecord
{
	SingletonInstance(HuntingRecord);

	public:
		HuntingRecord();
		virtual ~HuntingRecord();

		void SetVisible(Player* pPlayer, uint8 * Packet);
		void RecordRequest(Player* pPlayer, uint8 * Packet);
		void Close(Player* pPlayer);

		void Start(Player* pPlayer);
		void Update(Player* pPlayer, uint64 damage, uint64 elemental_damage, uint32 count, uint32 healing, uint64 experience);
		void Update(Player* pPlayer);

		void SendCurrentDay(Player* pPlayer);

		HuntingRecordData * GetCurrentRecord(Player* pPlayer, bool create = false);
};

#endif