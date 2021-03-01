#ifndef GAME_PACKET_H
#define GAME_PACKET_H

#pragma once

class GamePacket : public ByteBuffer
{
public:
	GamePacket(uint8 code, uint8 sub_code, size_t res = 4) : ByteBuffer(res)
	{
		this->append<uint8>(0xC2);
		this->append<uint16>(0);
		this->append<uint8>(code);
		this->append<uint8>(sub_code);
	}

	uint8* End()
	{
		this->put<uint8>(1, SET_NUMBERH(this->size()));
		this->put<uint8>(2, SET_NUMBERL(this->size()));

		return this->contents();
	}
};

#endif
