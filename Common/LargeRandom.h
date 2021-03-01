#ifndef LARGE_RANDOM_H
#define LARGE_RANDOM_H

#define MAX_LARGE_RANDOM_STATE	625

class CLargeRandom
{
	SingletonInstance(CLargeRandom);

	public:
		CLargeRandom();
		~CLargeRandom();

		void seed(uint32 _seed);
		uint32 regenerete();
		uint32 generate();

	private:
		uint32 state[MAX_LARGE_RANDOM_STATE];
		uint32 * next;
		int32 left;
};

#define sLargeRandom CLargeRandom::instance()

struct RandomSafe
{
	template <typename type>
    uint32 operator ()(type const& a) const
	{
		return 0;
    }

	template <typename type>
    uint32 operator %(type const& a) const
	{
		type b = a;

		if ( b <= 0 )
			b = 1;

        return sLargeRandom->generate() % b;
    }
};

#define RAND RandomSafe()

template <typename type>
inline type Random(type value) { return cast(type, RAND% (value < 1 ? 1: value)); }

inline uint8 RANDOM(uint8 value) { return cast(uint8, RAND% (value < 1 ? 1: value)); }
inline uint16 RANDOM(uint16 value) { return cast(uint16, RAND% (value < 1 ? 1: value)); }
inline uint32 RANDOM(uint32 value) { return cast(uint32, RAND% (value < 1 ? 1: value)); }
inline uint64 RANDOM(uint64 value) { return cast(uint64, RAND% (value < 1 ? 1: value)); }

inline int8 RANDOM(int8 value) { return cast(int8, RAND% (value < 1 ? 1: value)); }
inline int16 RANDOM(int16 value) { return cast(int16, RAND% (value < 1 ? 1: value)); }
inline int32 RANDOM(int32 value) { return cast(int32, RAND% (value < 1 ? 1: value)); }
inline int64 RANDOM(int64 value) { return cast(int64, RAND% (value < 1 ? 1: value)); }

#endif