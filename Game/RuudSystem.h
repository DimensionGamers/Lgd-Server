#ifndef RUUD_SYSTEM_H
#define RUUD_SYSTEM_H

class RuudSystem
{
	SingletonInstance(RuudSystem);

	public:
		RuudSystem();
		virtual ~RuudSystem();

		void ItemBuyRequest(Player* pPlayer, uint8 * Packet);
		void ItemBuyResult(Player* pPlayer, uint8 result = uint8(-1));
};

#endif