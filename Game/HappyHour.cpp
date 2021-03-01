HappyHour::HappyHour()
{
	this->SetState_None();
}

HappyHour::~HappyHour()
{

}

void HappyHour::Start(int32 duration, int32 notify)
{
	if ( this->GetState() != HAPPY_HOUR_STATE_NONE )
		return;

	this->RunTime(notify * MINUTE * IN_MILLISECONDS);
	this->SetDuration(duration * MINUTE * IN_MILLISECONDS);
	this->SetState_Notify();
}

void HappyHour::Update()
{
	switch ( this->GetState() )
	{
	case HAPPY_HOUR_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case HAPPY_HOUR_STATE_NOTIFY:
		{
			this->ProcState_Notify();
		} break;

	case HAPPY_HOUR_STATE_START:
		{
			this->ProcState_Playing();
		} break;
	}
}

void HappyHour::SetState_None()
{
	this->SetState(HAPPY_HOUR_STATE_NONE);
	this->SetDuration(0);
	this->RunTime(0);

	sLog->outInfo(LOG_INVASION,"[HappyHour] SetState_None()");
}
	
void HappyHour::SetState_Notify()
{
	this->SetState(HAPPY_HOUR_STATE_NOTIFY);

	sLog->outInfo(LOG_INVASION,"[HappyHour] SetState_Notify()");
}
	
void HappyHour::SetState_Playing()
{
	this->SetState(HAPPY_HOUR_STATE_START);
	this->RunTime(this->GetDuration());

	sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "Happy Hour Started");

	sLog->outInfo(LOG_INVASION,"[HappyHour] SetState_Start()");
}

void HappyHour::ProcState_None()
{

}
	
void HappyHour::ProcState_Notify()
{
	this->SendRemainMinutes("Happy Hour will start in %d minute(s)", 5);
	
	if ( this->TimePassed() )
		this->SetState_Playing();
}
	
void HappyHour::ProcState_Playing()
{
	if ( this->TimePassed() )
	{
		sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "Happy Hour Finished");

		this->SetState_None();
	}
}