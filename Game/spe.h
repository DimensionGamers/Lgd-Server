/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "spe.h"
*
*/

#ifndef SPE_HEADER
#define SPE_HEADER

static const uint8 byXorFilter[32] = { 0xAB, 0x11, 0xCD, 0xFE, 0x18, 0x23, 0xC5, 0xA3, 0xCA, 0x33, 0xC1, 0xCC, 0x66, 0x67, 0x21, 0xF3, 0x32, 0x12, 0x15, 
								 0x35, 0x29, 0xFF, 0xFE, 0x1D, 0x44, 0xEF, 0xCD, 0x41, 0x26, 0x3C, 0x4E, 0x4D };


class CStreamPacketEngine_Server
{
public:
	CStreamPacketEngine_Server()
	{
		this->Clear();
	};

	void Clear() 
	{
		this->m_wSize = 0;
		memset(this->m_byBuffer, 0, 8192);
	};

	bool Process(uint8* buffer, uint16 size, uint8 * buffer_final)
	{
		this->Clear();

		if ( !this->AddData(buffer, size) )
		{
			return false;
		}
		
		if ( this->ExtractPacket(buffer_final) != 0 )
		{
			return false;
		}

		return true;
	}
protected:

	void XorData(int32 iStart, int32 iEnd, int32 iDir)
	{
		if ( iStart < iEnd  )
		{
			return;
		}
		///- TODO: Cambiar != por >=
		for ( int i = iStart ; i != iEnd ; i+=iDir )
		{
			this->m_byBuffer[i] ^= this->m_byBuffer[i-1] ^ byXorFilter[ i%32 ];
		}
	}

public:



	bool AddData(void* pSrc, uint16 wSize ) 
	{
		if ( ((this->m_wSize + wSize) >=  8192) ||  (wSize == 0) )
		{
			return false;
		}

		memcpy((void*)&this->m_byBuffer[this->m_wSize], pSrc, wSize);
		this->m_wSize += wSize;
		return true;
	}

	BOOL ExtractPacket(void* pTar)
	{
		uint16 wSize;
		uint8 byTemp[8192];

		switch ( this->m_byBuffer[0] )
		{
			case 0xC1:
				wSize = this->m_byBuffer[1];
				break;

			case 0xC2:
				wSize = this->m_byBuffer[1] * 256 + this->m_byBuffer[2];
				break;

			default:
				return 1;
				break;
		}

		if ( this->m_wSize < wSize )
		{
			return 2;
		}

		this->XorData(wSize-1, (this->m_byBuffer[0] == 0xC1 ? 2 : 3), -1);
		memcpy(pTar, this->m_byBuffer, wSize);
		this->m_wSize-=wSize;
		memcpy(byTemp, &this->m_byBuffer[wSize], this->m_wSize);
		memcpy(this->m_byBuffer, byTemp, this->m_wSize);
		return 0;
	}

private:
	uint16 m_wSize;	// 4
	uint8 m_byBuffer[8192];	// 6
};

#endif