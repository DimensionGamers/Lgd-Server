#ifndef SECURITY_H
#define SECURITY_H

class Security
{
	SingletonInstance(Security);

	public:
		Security();
		virtual ~Security();

		void GetMac(char* mac);
		std::string GetMac();
};

#endif