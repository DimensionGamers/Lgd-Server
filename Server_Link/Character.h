#ifndef CHARACTER_H
#define CHARACTER_H

struct CharacterData
{
	CharacterData()
	{
		this->SetID(0);
		this->ResetName();
		this->SetServer(-1);
		this->SetGuild(0);
		this->SetAuthority(0);
		this->SetAdminPanelFlags(0);
	}

	DECLARE_ENUM(uint32, ID);
	DECLARE_STRING_FIXED(Name, MAX_CHARACTER_LENGTH + 1);
	DECLARE_ENUM(uint16, Server);
	DECLARE_ENUM(uint32, Guild);
	DECLARE_ENUM(uint8, Authority);
	DECLARE_ENUM(uint32, AdminPanelFlags);

	std::shared_ptr<ServerSocket> socket;
};

typedef std::shared_ptr<CharacterData> CharacterDataPtr;
typedef std::unordered_map<uint32, CharacterDataPtr> CharacterDataMap;

class CharacterMgr
{
	SingletonInstance(CharacterMgr);

	public:
		CharacterMgr();
		virtual ~CharacterMgr();

		void Update();
		void LoginLogout(uint8 * Packet, std::shared_ptr<ServerSocket> socket);

		CharacterDataPtr GetCharacterData(uint32 id) const;
		CharacterDataPtr GetCharacterData(const char * name) const;

		void CharacterUpdateStatus(uint32 id, uint16 server, bool online);
	private:
		CharacterDataMap character_data_map;
};

#endif