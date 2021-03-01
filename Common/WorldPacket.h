#ifndef WORLD_PACKET_H
#define WORLD_PACKET_H

class WorldPacket: public MessageBuffer
{
	public:
		explicit WorldPacket(uint8 headcode, uint8 * packet, size_t size): MessageBuffer(size), headcode(headcode)
		{
			this->Write(packet, size);
		}

		uint8 GetHeadcode() const { return this->headcode; }

	private:
		uint8 headcode;
};

typedef std::shared_ptr<WorldPacket> WorldPacketPtr;

#endif