#ifndef _UTIL_H
#define _UTIL_H

using namespace std::chrono;

template<typename charT>
struct my_equal
{
    my_equal( const std::locale& loc ) : loc_(loc) {}
    bool operator()(charT ch1, charT ch2) {
        return std::toupper(ch1, loc_) == std::toupper(ch2, loc_);
    }
private:
    const std::locale& loc_;
};

template<typename T>
size_t FindString( const T& str1, const T& str2, const std::locale& loc = std::locale() )
{
    T::const_iterator it = std::search( str1.begin(), str1.end(), 
        str2.begin(), str2.end(), my_equal<T::value_type>(loc) );
    if ( it != str1.end() ) return it - str1.begin();
    else return -1; // not found
}

namespace Util
{
	void ConvertToByte(uint16 source, uint8 * dest, bool normal = false);
	void ConvertToByte(uint32 source, uint8 * dest, bool normal = false);
	void ConvertToByte(uint64 source, uint8 * dest, bool normal = false);

	void ConvertToByte(int16 source, uint8 * dest, bool normal = false);
	void ConvertToByte(int32 source, uint8 * dest, bool normal = false);
	void ConvertToByte(int64 source, uint8 * dest, bool normal = false);
	int32 Distance(int32 x1, int32 y1, int32 x2, int32 y2);

	void ReduceUnsigned(uint32 & data, uint32 value);

	void ReplaceString(std::string &src, std::string const& search, std::string const& replace);
	std::string ReplaceStringArg(const char * src, ...);

	template<typename Iter, typename RandomGenerator>
	Iter select_randomly(Iter start, Iter end, RandomGenerator& g)
	{
		std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
		std::advance(start, dis(g));
		return start;
	}

	template<typename Iter>
	Iter select_randomly(Iter start, Iter end)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		return select_randomly(start, end, gen);
	}

	template<typename type>
	type ContainerRandom(type start, type end, int32 position)
	{
		int32 count = 0;
		type it = start;

		for ( ; it != end; ++it )
		{
			if ( position == count )
			{
				break;
			}

			++count;
		}

		return it == end ? start: it;
	}
}

class Compare
{
	public:
		template <typename t1, typename t2>
		static bool Less(t1 const& a, t2 const& b)
		{
			return a < static_cast<t1>(b);
		}

		template <typename t1, typename t2>
		static bool LessEqual(t1 const& a, t2 const& b)
		{
			return a <= static_cast<t1>(b);
		}

		template <typename t1, typename t2>
		static bool More(t1 const& a, t2 const& b)
		{
			return a > static_cast<t1>(b);
		}

		template <typename t1, typename t2>
		static bool MoreEqual(t1 const& a, t2 const& b)
		{
			return a >= static_cast<t1>(b);
		}

		template <typename t1, typename t2>
		static bool Equal(t1 const& a, t2 const& b)
		{
			return a == static_cast<t1>(b);
		}

		template <typename t1, typename t2>
		static bool Diff(t1 const& a, t2 const& b)
		{
			return a != static_cast<t1>(b);
		}
};

struct Tokens: public std::vector<char*>
{
    Tokens(const std::string &src, const char sep, uint32 vectorReserve = 0);
    ~Tokens() { delete[] m_str; }

    char* m_str;
};

void stripLineInvisibleChars(std::string& src);

std::string secsToTimeString(time_t timeInSecs, bool shortText = false, bool hoursOnly = false);
uint32 TimeStringToSecs(const std::string& timestring);
std::string TimeToTimestampStr(time_t t);
time_t timeBitFieldsToSecs(uint32 packedDate);

inline uint32 secsToTimeBitFields(time_t secs)
{
    tm* lt = localtime(&secs);
    return (lt->tm_year - 100) << 24 | lt->tm_mon  << 20 | (lt->tm_mday - 1) << 14 | lt->tm_wday << 11 | lt->tm_hour << 6 | lt->tm_min;
}

/* Return a random number in the range min..max. */
int32 irand(int32 min, int32 max);

/* Return a random number in the range min..max (inclusive). */
uint32 urand(uint32 min, uint32 max);

/* Return a random number in the range 0 .. UINT32_MAX. */
uint32 rand32();

/* Return a random number in the range min..max */
float frand(float min, float max);

/* Return a random double from 0.0 to 1.0 (exclusive). */
double rand_norm();

/* Return a random double from 0.0 to 100.0 (exclusive). */
double rand_chance();

/* Return true if a random roll fits in the specified chance (range 0-100). */
inline bool roll_chance_f(float chance)
{
	 return chance > frand(0.0f, 99.0f);
    //return chance > rand_chance();
}

/* Return true if a random roll fits in the specified chance (range 0-100). */
inline bool roll_chance_i(int32 chance)
{
    return chance > irand(0, 99);
}

inline bool roll_chance_i(int32 chance, int32 max)
{
    return chance > irand(0, max-1);
}

bool Utf8toWStr(const std::string& utf8str, std::wstring& wstr);
// in wsize==max size of buffer, out wsize==real string size
bool Utf8toWStr(char const* utf8str, size_t csize, wchar_t* wstr, size_t& wsize);
inline bool Utf8toWStr(const std::string& utf8str, wchar_t* wstr, size_t& wsize)
{
    return Utf8toWStr(utf8str.c_str(), utf8str.size(), wstr, wsize);
}

bool WStrToUtf8(std::wstring wstr, std::string& utf8str);
// size==real string size
bool WStrToUtf8(wchar_t* wstr, size_t size, std::string& utf8str);

#define StringLower(str, src) \
	std::string str = src; \
	strToLower(str);

#define StringUpper(str, src) \
	std::string str = src; \
	strToUpper(str);

inline void strToUpper(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), toupper);
}

inline void strToLower(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), tolower);
}

void utf8printf(FILE* out, const char* str, ...);
void vutf8printf(FILE* out, const char* str, va_list* ap);

void hexEncodeByteArray(uint8* bytes, uint32 arrayLen, std::string& result);

bool minutesElapsed(int32 sec, uint8 minutes);

void StringToLower(char * str);
void StringToUpper(char * str);

bool CreatePath(std::string const& PathToCreate);

std::string GetFileName();
std::string GetConfigFileName();
std::string GetLogFolder();

inline bool is_digits(const std::string &str)
{
    return std::all_of(str.begin(), str.end(), ::isdigit); // C++11
}

inline void GetStartAndEndDate(tm & start_date, tm & end_date)
{
/*	Custom::SystemTimer m_time;

	tm date;
	date.tm_year = m_time.GetYear() - 1900;
	date.tm_mon = m_time.GetMonth() - 1;
	date.tm_mday = m_time.GetDay();
	mktime(&date);

	date.tm_mday -= date.tm_mday + 1;
	mktime(&date);
	start_date = date;

	date.tm_mday += 5;
	mktime(&date);
	end_date = date;*/
}

void Shuffle(std::string &s);
bool hasStringAbbr(const char* name, const char* part);

//Json::Value ExecutePHPScript(std::string const& url, std::string const& param);

enum ComparisionType
{
    COMP_TYPE_EQ = 0,
    COMP_TYPE_HIGH,
    COMP_TYPE_LOW,
    COMP_TYPE_HIGH_EQ,
    COMP_TYPE_LOW_EQ,
    COMP_TYPE_MAX
};

template <class T>
bool CompareValues(ComparisionType type, T val1, T val2)
{
    switch (type)
    {
        case COMP_TYPE_EQ:
            return val1 == val2;
        case COMP_TYPE_HIGH:
            return val1 > val2;
        case COMP_TYPE_LOW:
            return val1 < val2;
        case COMP_TYPE_HIGH_EQ:
            return val1 >= val2;
        case COMP_TYPE_LOW_EQ:
            return val1 <= val2;
        default:
            return false;
    }
}

template <class T>
inline void SplitToVector(std::vector<T> & list, std::string const& value)
{
	list.clear();

	if (value.empty())
	{
		return;
	}

	T data;
	std::istringstream stream(value.c_str());

	while (stream)
	{
		stream >> data;
		list.push_back(data);
	}
}

char* AllocMemory(const char * value);

class Tokenizer
{
public:
    typedef std::vector<char const*> StorageType;

    typedef StorageType::size_type size_type;

    typedef StorageType::const_iterator const_iterator;
    typedef StorageType::reference reference;
    typedef StorageType::const_reference const_reference;

public:
    Tokenizer(const std::string &src, char const sep, uint32 vectorReserve = 0);
    ~Tokenizer() { delete[] m_str; }

    const_iterator begin() const { return m_storage.begin(); }
    const_iterator end() const { return m_storage.end(); }

    size_type size() const { return m_storage.size(); }

    reference operator [] (size_type i) { return m_storage[i]; }
    const_reference operator [] (size_type i) const { return m_storage[i]; }

private:
    char* m_str;
    StorageType m_storage;
};

std::string EncodePassword(std::string account, std::string password);

class Xor
{
	public:
		static void BMD(uint8 * info, int32 Size);
		static void Byte3(char * info, int32 Size);
		static void LangXor(char * info, int32 Size, uint16 key);
		static int32 Byte16(char * Source, char * Dest, int32 size);
		static void Byte20(char * src, char * dst, int32 size);
		static void ByteCustom(char * data, size_t size);

		static int32 Byte16New(char * Source, char * Dest, int32 size);

	private:
		static const uint32 KEY_SIZE_1					= 3;
		static const uint32 KEY_SIZE_2					= 16;
		static const uint32 KEY_SIZE_3					= 20;
		static const uint32 KEY_SIZE_CUSTOM				= 5;
};

std::string ConvertTimeToString(time_t _time);

std::string TrimBlankSpaces(std::string const& source);

std::string RemoveSpecialCharacters(std::string const& source);

int32 GetTimeFromString(std::string const& source);

#endif
