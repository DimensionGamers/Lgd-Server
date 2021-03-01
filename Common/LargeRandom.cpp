CLargeRandom::CLargeRandom()
{
	this->left = -1;
}

CLargeRandom::~CLargeRandom()
{

}

void CLargeRandom::seed(uint32 _seed)
{
	uint32 x = _seed | 1;
	uint32 * s = this->state;
	this->left = 0;
	*s = x;
	s++;

	for ( int32 j = MAX_LARGE_RANDOM_STATE-1 ; --j != 0 ;  x *= 69069, *s = x, s++);
}

uint32 CLargeRandom::regenerete()
{
	uint32 * p0 = this->state;
	uint32 * p2 = &this->state[2];
	uint32 * pM = &this->state[397];

	if ( this->left < -1 )
		this->seed(GetTickCount());
		//this->seedMT(0x1105);

	this->left = MAX_LARGE_RANDOM_STATE-2;
	this->next = &this->state[1];
	uint32 s0 = this->state[0];	
	uint32 s1 = this->state[1];

	for ( int32 j = 228; --j != 0 ; s0 = s1, s1 = *p2, p2++)
	{
		*p0++ = (*pM++ ^ (((s0 & 0x80000000) | (s1 & 0x7FFFFFFF)) >> 1)) ^ ((((s1&1)!=0)?-1:0)&0x9908B0DF);
	}

	pM = this->state;

	for( int32 j = 0x18D ; --j != 0 ; s0=s1 , s1 = *p2 , p2++ )
	{
		*p0++ = (*pM++ ^ (((s0 & 0x80000000) | (s1 & 0x7FFFFFFF)) >> 1)) ^ ((((s1&1)!=0)?-1:0)&0x9908B0DF);
	}

	s1 = this->state[0];
	*p0 = (*pM ^ (((s0 & 0x80000000) | (s1 & 0x7FFFFFFF)) >> 1)) ^ ((((s1&1)!=0)?-1:0)&0x9908B0DF);
	s1 ^= s1 >> 11;
	s1 ^= (s1 << 7) & 0x9D2C5680;
	s1 ^= (s1 << 15) & 0xEFC60000;

	return s1 ^ ( s1 >> 18 );
}

uint32 CLargeRandom::generate()
{
	this->left--;

	if ( this->left < 0 )
	{
		return this->regenerete();
	}

	uint32 y = *next;
	next++;
	y ^= ( (uint32)y >> (uint32)11 );
	y ^= ( y << 7 ) & 2636928640;
	y ^= ( y << 15 ) & 4022730752;

	return y ^ ( y >> 18 );
}