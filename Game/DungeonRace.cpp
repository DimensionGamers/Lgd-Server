DungeonRace::DungeonRace()
{
	this->SetRaceName("Dungeon");
}

DungeonRace::~DungeonRace()
{

}

void DungeonRace::LoadData()
{
	this->Clear();

	if ( !sGameServer->IsDungeonRaceEnabled() )
		return;

	sLog->outInfo(LOG_DEFAULT, "Loading Dungeon Race Data...");

	std::string filename("EventData/Race.txt");

	ReadScript file(filename.c_str(), "r");
	
	if ( !file.open() )
		return;

	while ( true )
	{
		if ( file.GetToken() == END )
			break;

		uint8 section = file.TokenNumber;

		while ( true )
		{
			file.GetToken();

			if ( file.IsEnd("end") )
				break;

			switch ( section )
			{
			case 0:
				{
					int32 gate = file.GetInt32();
					file.GetToken(); int32 points = file.GetInt32();
					
					this->gate_list.push_back(new CommonRaceGate(gate, points));
				} break;
			}
		}
	}

	sLog->outInfo(LOG_DEFAULT, "Dungeon Race Data Loaded");
	sLog->outInfo(LOG_DEFAULT, " ");
}

void DungeonRace::Update()
{
	if ( !sGameServer->IsDungeonRaceEnabled() )
		return;

	CommonRace::Update();
}