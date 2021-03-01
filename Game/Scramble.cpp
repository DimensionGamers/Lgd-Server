Scramble::Scramble()
{
	this->GetWordTime()->Reset();
	this->SetState_None();
}

Scramble::~Scramble()
{

}

void Scramble::LoadWordList()
{
	this->word_list.clear();

	if ( !sGameServer->IsScrambleEnabled() )
		return;

	sLog->outInfo(LOG_DEFAULT, "Loading Scramble Words...");
	
	uint32 count = 0;

	QueryResult result = GameServerDatabase.Query("SELECT * FROM scramble_word");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			this->word_list.push_back(fields[0].GetString());
			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u scramble words", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void Scramble::Start(int32 duration, int32 notify)
{
	if ( !sGameServer->IsScrambleEnabled() )
	{
		return;
	}

	if ( this->GetState() != SCRAMBLE_STATE_NONE )
	{
		return;
	}

	if ( this->word_list.empty() )
	{
		return;
	}

	this->SetDuration(duration);
	this->SetNotify(notify);

	if ( notify > 0 )
	{
		this->SetState_Notify();
	}
	else
	{
		this->SetState_Playing();
	}
}

void Scramble::Update()
{
	if ( !sGameServer->IsScrambleEnabled() )
		return;

	switch ( this->GetState() )
	{
	case SCRAMBLE_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case SCRAMBLE_STATE_NOTIFY:
		{
			this->ProcState_Notify();
		} break;

	case SCRAMBLE_STATE_START:
		{
			this->ProcState_Playing();
		} break;
	}
}

void Scramble::SetState_None()
{
	this->SetState(SCRAMBLE_STATE_NONE);
	this->RunTime(0);
	this->SetTotalWordCount(0);
	this->SetCurrentWordCount(0);
	this->GetWordTime()->Reset();
	this->ResetWord();
	this->ResetOriginalWord();
	this->SetWordChange(false);

	sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "[ Scramble ] Finished.");

	sLog->outInfo(LOG_SCRAMBLE, "SetState_None()");
}

void Scramble::SetState_Notify()
{
	this->SetState(SCRAMBLE_STATE_NOTIFY);
	this->RunTime(this->GetNotify() * MINUTE * IN_MILLISECONDS);

	sLog->outInfo(LOG_SCRAMBLE, "SetState_Notify()");
}

void Scramble::SetState_Playing()
{
	this->SetState(SCRAMBLE_STATE_START);
	this->RunTime(this->GetDuration() * MINUTE * IN_MILLISECONDS);
	this->SetWordChange(true);
	this->GetWordTime()->Start(0);

	sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "[ Scramble ] Started.");

	sLog->outInfo(LOG_SCRAMBLE, "SetState_Playing()");
}

void Scramble::ProcState_None()
{

}

void Scramble::ProcState_Notify()
{
	if ( this->TimePassed() )
		this->SetState_Playing();
}

void Scramble::ProcState_Playing()
{
	if ( this->GetTotalWordCount() >= sGameServer->GetScrambleTotalWordCount() )
	{
		this->SetState_None();
		return;
	}

	if ( this->IsWordChange() )
	{
		if ( this->GetWordTime()->Elapsed() )
		{
			this->MakeRandomWord();
		}
	}
	else
	{
		if ( this->GetWordTime()->Elapsed() )
		{
			this->IncreaseCurrentWordCount(1);

			if ( this->GetCurrentWordCount() >= sGameServer->GetScrambleRepeatWordCount() )
			{
				this->SetWordChange(true);
			}
			else
			{
				sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "[ Scramble ] Solve word: %s", this->GetWord().c_str());
				sObjectMgr->SendNoticeToAll(CUSTOM_MESSAGE_ID_ADMIN_NORMAL, "[ Scramble ] Solve word: %s", this->GetWord().c_str());
			}

			this->GetWordTime()->Start(30 * IN_MILLISECONDS);
		}
	}

	if ( this->TimePassed() )
		this->SetState_None();
}

void Scramble::MakeRandomWord()
{
	if ( this->word_list.empty() )
		return;

	this->SetCurrentWordCount(0);
	this->ResetWord();
	this->ResetOriginalWord();

	std::string shuffled_string(this->word_list[Random(this->word_list.size())]);
	this->SetOriginalWord(shuffled_string);
	std::random_shuffle(shuffled_string.begin(), shuffled_string.end());
	this->SetWord(shuffled_string);

	this->SetWordChange(false);

	sLog->outInfo(LOG_SCRAMBLE, "MakeRandomWord() Word: %s - Scrambled Word: %s",
		this->GetOriginalWord().c_str(), this->GetWord().c_str());
}

bool Scramble::CompareWord(Player* pPlayer, std::string const& word)
{
	if ( !sGameServer->IsScrambleEnabled() )
	{
		return false;
	}

	if ( this->GetState() != SCRAMBLE_STATE_START )
	{
		return false;
	}

	if ( !pPlayer )
	{
		return false;
	}

	if ( this->GetOriginalWord() != word )
	{
		return false;
	}

	this->MakeReward(pPlayer);
	return true;
}

void Scramble::MakeReward(Player* pPlayer)
{
	if ( this->GetWord().empty() )
	{
		return;
	}

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_EVENT_RANKING_SCRAMBLE);
	stmt->setUInt32(0, pPlayer->GetGUID());
	stmt->setString(1, this->GetOriginalWord());
	stmt->setString(2, this->GetWord());
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);

	this->GetWordTime()->Reset();
	this->SetWordChange(true);
	this->IncreaseTotalWordCount(1);
	this->ResetWord();

	sItemBagMgr->RunItemBag(pPlayer, sGameServer->GetScrambleReward(), Item(), true, GremoryCaseReward::EVENT);

	sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "[ Scramble ] %s is the winner. Word was %s",
		pPlayer->GetName(), this->GetOriginalWord().c_str());

	sLog->outInfo(LOG_SCRAMBLE, "MakeReward() %s solved Word: %s",
		pPlayer->BuildLog().c_str(), this->GetOriginalWord().c_str());
}