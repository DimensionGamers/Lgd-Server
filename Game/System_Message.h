#ifndef SYSTEM_MESSAGE_H
#define SYSTEM_MESSAGE_H

struct SystemMessageData
{
	DECLARE_ENUM(uint32, ID);
	DECLARE_ENUM(uint8, Language);
	DECLARE_PROPERTY_STRING(Text);
};

typedef std::map<uint32, SystemMessageData*> SystemMessageMap;

class SystemMessage
{
	SingletonInstance(SystemMessage);

	public:
		SystemMessage();
		virtual ~SystemMessage();

		void Init();
		std::string Get(uint32 id) const;
	private:
		SystemMessageMap message_map;
};

#endif