#ifndef CASTLE_SIEGE_H
#define CASTLE_SIEGE_H

class CastleSiege
{
	SingletonInstance(CastleSiege);

	public:
		CastleSiege();
		virtual ~CastleSiege();

		void LoadData(std::shared_ptr<ServerSocket> socket);
		void LoadNpc(std::shared_ptr<ServerSocket> socket);
		void LoadGuildRegister(std::shared_ptr<ServerSocket> socket);
		void SaveNpc(uint8 * packet);
		void ClearGuild();
		void InsertFinalGuild(uint8 * packet);
		void SaveTaxRate(uint8 * packet);
		void UpdateMoney(uint8 * packet);
		void SaveOwnerStatus(uint8 * packet);
		void InsertRegisteredGuild(uint8 * packet);
		void DeleteRegisteredGuild(uint8 * packet);
		void UpdateRegisteredGuild(uint8 * packet);
		void RegisteredGuild(uint8 * packet, std::shared_ptr<ServerSocket> socket);
		void RegisteredGuildAll(uint8 * packet, std::shared_ptr<ServerSocket> socket);
};

#endif