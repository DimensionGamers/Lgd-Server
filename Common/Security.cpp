Security::Security()
{

}

Security::~Security()
{

}

#include <Rpc.h>
#pragma comment(lib, "Rpcrt4")

void Security::GetMac(char* mac)
{
	memset(mac, 0, MAX_ACCOUNT_MAC_LENGTH);

	UUID uuid;
	UuidCreateSequential(&uuid);    // Ask OS to create UUID

	sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X",
		uuid.Data4[2],uuid.Data4[3],uuid.Data4[4],
		uuid.Data4[5],uuid.Data4[6],uuid.Data4[7]);
}

std::string Security::GetMac()
{
	char mac[MAX_ACCOUNT_MAC_LENGTH];
	memset(mac, 0, MAX_ACCOUNT_MAC_LENGTH);

	UUID uuid;
	UuidCreateSequential(&uuid);    // Ask OS to create UUID

	sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X",
		uuid.Data4[2],uuid.Data4[3],uuid.Data4[4],
		uuid.Data4[5],uuid.Data4[6],uuid.Data4[7]);

	return mac;
}