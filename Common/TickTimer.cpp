TickTimer::TickTimer()
{
	this->Reset();
}

TickTimer::~TickTimer()
{

}

void TickTimer::Reset()
{
	this->SetTimer(MyGetTickCount());
	this->SetDelay(0);
}

void TickTimer::Start()
{
	this->SetTimer(MyGetTickCount());
}

void TickTimer::Start(uint32 delay)
{
	this->SetTimer(MyGetTickCount());
	this->SetDelay(delay);
}

bool TickTimer::Elapsed()
{
	return this->Elapsed(this->GetDelay());
}

bool TickTimer::ExElapsed(uint32 delay)
{
	return this->Elapsed(this->GetDelay() + delay);
}

bool TickTimer::Elapsed(uint32 delay)
{
	if (MyGetTickCount() > (this->GetTimer() + delay))
	{
		this->SetTimer(MyGetTickCount());
		return true;
	}

	return false;
}

uint32 TickTimer::GetElapsed() const
{
	return uint32(MyGetTickCount() - this->GetTimer());
}

uint32 TickTimer::GetRemain() const
{
	uint32 diff = uint32(MyGetTickCount() - this->GetTimer());

	if (diff < this->GetDelay())
	{
		diff = this->GetDelay() - diff;
	}
	else
	{
		diff = 0;
	}

	return diff;
}

bool TickTimer::Finished() const
{
	return this->GetElapsed() >= this->GetDelay();
}