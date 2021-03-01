#ifndef EVENT_MASSIVE_H
#define EVENT_MASSIVE_H

class EventMassive
{
	public:
		EventMassive()
		{
			this->InitCharacterCount();
		}

		virtual ~EventMassive()
		{

		}

		void InitCharacterCount()
		{
			this->SetCharacterCount(0);
		}

		virtual void InsertRanking() = 0;
		virtual void UpdateCharacterCount() = 0;

		DECLARE_PROPERTY(int32, CharacterCount);
};

#endif