SocketSystem::SocketSystem()
{

}

SocketSystem::~SocketSystem()
{

}

void SocketSystem::MixRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT(SOCKET_MIX_REQUEST, lpMsg, Packet, 0);

	switch(lpMsg->mix_id)
	{
	case 0:
		{
		} break;

	case 1:
		{
			MixHandler(pPlayer, 4).SocketMix(lpMsg->mix_id, lpMsg->slot, lpMsg->data, 36);
		} break;

	case 2:
		{
			MixHandler(pPlayer, 4).SocketMix(lpMsg->mix_id, lpMsg->slot, lpMsg->data, 28);
		} break;

	case 3:
		{
			uint8 socket = lpMsg->slot[1];
			lpMsg->slot[1] = 0xFF;
			MixHandler(pPlayer, 4).SocketMix(lpMsg->mix_id, lpMsg->slot, socket, 21);
		} break;

	case 4:
		{
			uint8 socket = lpMsg->slot[1];
			lpMsg->slot[1] = 0xFF;
			MixHandler(pPlayer, 4).SocketMix(lpMsg->mix_id, lpMsg->slot, socket, 29);
		} break;

	case 5:
		{
			MixHandler(pPlayer, 4).SocketMix(lpMsg->mix_id, lpMsg->slot, 0xFF, 4);
		} break;

	case 6:
		{
			MixHandler(pPlayer, 4).SocketMix(lpMsg->mix_id, lpMsg->slot, 0xFF, 3);
		} break;

	case 7:
		{
			MixHandler(pPlayer, 4).SocketMix(lpMsg->mix_id, lpMsg->slot, 0xFF, 3);
		} break;
	}
}