#ifndef MOSS_MERCHANT_H
#define MOSS_MERCHANT_H

#define MOSS_MERCHANT_SECTION_MAX		7
#pragma pack(1)
struct MOSS_MERCHANT_OPEN
{
	PBMSG_HEAD2 h;
	uint8 count;
};

struct MOSS_MERCHANT_OPEN_BOX
{
	PBMSG_HEAD2 h;
	uint8 type;
};

struct MOSS_MERCHANT_REWARD
{
	MOSS_MERCHANT_REWARD()
	{
		this->h.set(0x70, 0x12, sizeof(MOSS_MERCHANT_REWARD));
		memset(this->item_info, 0, MAX_ITEM_INFO);
	}

	PBMSG_HEAD2 h;
	uint8 item_info[MAX_ITEM_INFO];
};
#pragma pack()
struct MossMerchantSection
{
	DECLARE_ENUM(uint8, ID);
	DECLARE_PROPERTY_STRING(Name);
	DECLARE_ENUM(uint16, RequiredItem);
	DECLARE_ENUM(uint32, RequiredZen);
	DECLARE_PROPERTY_STRING(ItemBag);
	DECLARE_ENUM(uint8, Enabled);
};

typedef std::map<uint8, MossMerchantSection*> MossMerchantSectionMap;

class MossMerchant
{
	SingletonInstance(MossMerchant);

	public:
		MossMerchant();
		virtual ~MossMerchant();

		void Load();

		void Open(Player* pPlayer);
		void OpenBox(Player* pPlayer, uint8 * Packet);
		void OpenBoxResult(Player* pPlayer, uint8 result);

		MossMerchantSection const* GetMossMerchantSection(uint8 id) const;
	private:
		MossMerchantSectionMap section_map;
};

#endif