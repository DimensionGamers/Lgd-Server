#ifndef TRADE_SCRIPT_H
#define TRADE_SCRIPT_H

class TradeScript: public StoreScript
{
	public:
		explicit TradeScript(Player* pPlayer): StoreScript(pPlayer, trade_size, trade_size) {}
		void Clear()
		{
			StoreScript::Clear();
			this->SetButtonEnabled(false);
			this->SetZen(0);
			this->ResetTargetSlot(0);
		}

		DECLARE_BOOL(ButtonEnabled);
		DECLARE_PROPERTY(uint32, Zen);
		DECLARE_PROPERTY_ARRAY(uint8, TargetSlot, trade_size);
};

#endif