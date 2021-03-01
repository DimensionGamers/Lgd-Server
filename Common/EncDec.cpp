bool EncodeDecode::Encode(const uint8 * source, uint8 * encoded, uint16 & size)
{
	if ( !source || !encoded || size < 3 )
	{
		return false;
	}

	int32 rand_1 = -1;
	int32 rand_2 = -1;

	do
	{
		rand_1 = rand()%MAX_KEY_1;
		rand_2 = rand()%MAX_KEY_2;
	}
	while(LastKey1 == rand_1 || LastKey2 == rand_2);

	LastKey1 = rand_1;
	LastKey2 = rand_2;

	encoded[ENCODE_DECODE_KEY_1] = key_1[rand_1];
	encoded[ENCODE_DECODE_KEY_2] = key_2[rand_2];
	encoded[ENCODE_DECODE_SERIAL] = SerialSEND++;

	encoded[ENCODE_DECODE_SERIAL] -= key_final[rand_1][rand_2];

	for ( uint16 i = 0; i < size; ++i )
	{
		encoded[i + ENCODE_DECODE_SIZE] = source[i] - key_final[rand_1][rand_2];
	}

	size += ENCODE_DECODE_SIZE;

	return true;
}

bool EncodeDecode::EncodeDS(const uint8 * source, uint8 * encoded, uint16 & size)
{
	if ( !source || !encoded || size < 3 )
	{
		return false;
	}

	int32 rand_1 = RAND%MAX_KEY_1;
	int32 rand_2 = RAND%MAX_KEY_2;
	int32 add = 0;

	if ( source[0] == 0xC1 || source[0] == 0xC3 )
	{
		add = 2;
	}
	else if ( source[0] == 0xC2 || source[0] == 0xC4 )
	{
		add = 3;
	}

	encoded[ENCODE_DECODE_KEY_1 + add] = key_1[rand_1];
	encoded[ENCODE_DECODE_KEY_2 + add] = key_2[rand_2];
	encoded[ENCODE_DECODE_SERIAL] = 0;

	for ( uint16 i = 0; i < size; ++i )
	{
		encoded[i + ENCODE_DECODE_SIZE + add] = source[i + add] - key_final[rand_1][rand_2];
	}

	size += ENCODE_DECODE_SIZE;

	return true;
}
	
bool EncodeDecode::Decode(uint8 * source, uint8 * decoded, uint16 & size, uint8 & serial)
{
	size = 0;

	if ( !source || !decoded )
	{
		return false;
	}

	uint8 tmp_key_1 = source[ENCODE_DECODE_KEY_1];
	uint8 tmp_key_2 = source[ENCODE_DECODE_KEY_2];
	uint8 tmp_serial = source[ENCODE_DECODE_SERIAL];
	uint8 position_1 = 0;
	uint8 position_2 = 0;

	if ( this->GetLastKey1() == tmp_key_1 || this->GetLastKey2() == tmp_key_2 )
	{
		return false;
	}

	this->SetLastKey1(tmp_key_1);
	this->SetLastKey2(tmp_key_2);

	if ( !this->GetKey1(tmp_key_1, position_1) || !this->GetKey2(tmp_key_2, position_2) )
	{
		return false;
	}

	serial = tmp_serial + key_final[position_1][position_2];

	uint8 header = decoded[0] = (source[ENCODE_DECODE_SIZE] + key_final[position_1][position_2]);
	size = 0;

	if ( header == 0xC1 || header == 0xC3 )
	{
		size = decoded[1] = (source[ENCODE_DECODE_SIZE + 1] + key_final[position_1][position_2]);

		if ( size >= MAX_BUFFER_SIZE )
		{
			return false;
		}

		for ( uint16 i = 2; i < size; ++i )
		{
			decoded[i] = source[ENCODE_DECODE_SIZE + i] + key_final[position_1][position_2];
		}
	}
	else if ( header == 0xC2 || header == 0xC4 )
	{
		decoded[1] = (source[ENCODE_DECODE_SIZE + 1] + key_final[position_1][position_2]);
		decoded[2] = (source[ENCODE_DECODE_SIZE + 2] + key_final[position_1][position_2]);

		size = MAKE_NUMBERW(decoded[1], decoded[2]);

		if ( size >= MAX_BUFFER_SIZE )
		{
			return false;
		}

		for ( uint16 i = 3; i < size; ++i )
		{
			decoded[i] = source[ENCODE_DECODE_SIZE + i] + key_final[position_1][position_2];
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool EncodeDecode::GetKey1(uint8 key, uint8 & position)
{
	for ( uint8 i = 0; i < MAX_KEY_1; ++i )
	{
		if ( key_1[i] == key )
		{
			position = i;
			return true;
		}
	}
	
	return false;
}

bool EncodeDecode::GetKey2(uint8 key, uint8 & position)
{
	for ( uint8 i = 0; i < MAX_KEY_2; ++i )
	{
		if ( key_2[i] == key )
		{
			position = i;
			return true;
		}
	}
	
	return false;
}