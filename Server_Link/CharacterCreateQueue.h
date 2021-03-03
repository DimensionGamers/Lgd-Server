#ifndef CHARACTER_CREATE_QUEUE_H
#define CHARACTER_CREATE_QUEUE_H

struct CharacterCreateData
{
	explicit CharacterCreateData(uint16 entry, uint8 character_class, const char * character_name): 
		entry(entry),
		character_class(character_class),
		server(-1),
		ms_time(0),
		account_id(0)
	{
		memcpy(this->character_name, character_name, MAX_CHARACTER_LENGTH + 1);
	}

	uint16 server;
	uint16 entry;
	uint32 ms_time;
	uint32 account_id;
	uint8 character_class;
	char character_name[MAX_CHARACTER_LENGTH + 1];
	uint32 strength;
	uint32 agility;
	uint32 vitality;
	uint32 energy;
	uint32 leadership;
	int32 life;
	int32 mana;
	uint32 points;
	uint32 level;
	uint16 world;
	int16 x;
	int16 y;
	uint8 slot;
	std::shared_ptr<ServerSocket> socket;

	uint16 GetServerGroup() const { return this->server / 20; }
};

typedef std::shared_ptr<CharacterCreateData> CharacterCreateDataPtr;

class CharacterCreateQueue
{
	SingletonInstance(CharacterCreateQueue);

	public:
		CharacterCreateQueue();
		virtual ~CharacterCreateQueue();

		void Queue(CharacterCreateDataPtr data);
		void Update();
		void Process(CharacterCreateDataPtr data);

	private:
		LockedQueue<CharacterCreateDataPtr> CharacterCreateDataQueue;
};

#endif