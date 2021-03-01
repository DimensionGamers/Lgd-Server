LosttowerRace::LosttowerRace()
{
	this->SetRaceName("LostTower");
}

LosttowerRace::~LosttowerRace()
{

}

void LosttowerRace::LoadData()
{
	this->Clear();

	if ( !sGameServer->IsLosttowerRaceEnabled() )
		return;

	sLog->outInfo(LOG_DEFAULT, "Loading Losttower Race Data...");

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
			case 1:
				{
					int32 gate = file.GetInt32();
					file.GetToken(); int32 points = file.GetInt32();
					
					this->gate_list.push_back(new CommonRaceGate(gate, points));
				} break;
			}
		}
	}

	sLog->outInfo(LOG_DEFAULT, "Losttower Race Data Loaded");
	sLog->outInfo(LOG_DEFAULT, " ");
}

void LosttowerRace::Update()
{
	if ( !sGameServer->IsLosttowerRaceEnabled() )
		return;

	CommonRace::Update();
}