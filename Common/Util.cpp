#include <boost/thread/tss.hpp>

static boost::thread_specific_ptr<SFMTRand> sfmtRand;

static SFMTRand* GetRng()
{
    SFMTRand* rand = sfmtRand.get();

    if (!rand)
    {
        rand = new SFMTRand();
        sfmtRand.reset(rand);
    }

    return rand;
}

namespace Util
{
	void ConvertToByte(uint16 source, uint8 * dest, bool normal)
	{
		if ( normal )
		{
			dest[0] = LOBYTE(source);
			dest[1] = HIBYTE(source);
		}
		else
		{
			dest[0] = HIBYTE(source);
			dest[1] = LOBYTE(source);
		}
	}

	void ConvertToByte(uint32 source, uint8 * dest, bool normal)
	{
		if ( normal )
		{
			dest[0] = LOBYTE(LOWORD(source));
			dest[1] = HIBYTE(LOWORD(source));
			dest[2] = LOBYTE(HIWORD(source));
			dest[3] = HIBYTE(HIWORD(source));
		}
		else
		{
			dest[0] = HIBYTE(HIWORD(source));
			dest[1] = LOBYTE(HIWORD(source));
			dest[2] = HIBYTE(LOWORD(source));
			dest[3] = LOBYTE(LOWORD(source));
		}
	}

	void ConvertToByte(uint64 source, uint8 * dest, bool normal)
	{
		if ( normal )
		{
			dest[0] = LOBYTE(LOWORD(LODWORD(source)));
			dest[1] = HIBYTE(LOWORD(LODWORD(source)));
			dest[2] = LOBYTE(HIWORD(LODWORD(source)));
			dest[3] = HIBYTE(HIWORD(LODWORD(source)));
			dest[4] = LOBYTE(LOWORD(HIDWORD(source)));
			dest[5] = HIBYTE(LOWORD(HIDWORD(source)));
			dest[6] = LOBYTE(HIWORD(HIDWORD(source)));
			dest[7] = HIBYTE(HIWORD(HIDWORD(source)));
		}
		else
		{
			dest[0] = HIBYTE(HIWORD(HIDWORD(source)));
			dest[1] = LOBYTE(HIWORD(HIDWORD(source)));
			dest[2] = HIBYTE(LOWORD(HIDWORD(source)));
			dest[3] = LOBYTE(LOWORD(HIDWORD(source)));
			dest[4] = HIBYTE(HIWORD(LODWORD(source)));
			dest[5] = LOBYTE(HIWORD(LODWORD(source)));
			dest[6] = HIBYTE(LOWORD(LODWORD(source)));
			dest[7] = LOBYTE(LOWORD(LODWORD(source)));
		}
	}

	void ConvertToByte(int16 source, uint8 * dest, bool normal)
	{
		if ( normal )
		{
			dest[0] = LOBYTE(source);
			dest[1] = HIBYTE(source);
		}
		else
		{
			dest[0] = HIBYTE(source);
			dest[1] = LOBYTE(source);
		}
	}

	void ConvertToByte(int32 source, uint8 * dest, bool normal)
	{
		if ( normal )
		{
			dest[0] = LOBYTE(LOWORD(source));
			dest[1] = HIBYTE(LOWORD(source));
			dest[2] = LOBYTE(HIWORD(source));
			dest[3] = HIBYTE(HIWORD(source));
		}
		else
		{
			dest[0] = HIBYTE(HIWORD(source));
			dest[1] = LOBYTE(HIWORD(source));
			dest[2] = HIBYTE(LOWORD(source));
			dest[3] = LOBYTE(LOWORD(source));
		}
	}

	void ConvertToByte(int64 source, uint8 * dest, bool normal)
	{
		if ( normal )
		{
			dest[0] = LOBYTE(LOWORD(LODWORD(source)));
			dest[1] = HIBYTE(LOWORD(LODWORD(source)));
			dest[2] = LOBYTE(HIWORD(LODWORD(source)));
			dest[3] = HIBYTE(HIWORD(LODWORD(source)));
			dest[4] = LOBYTE(LOWORD(HIDWORD(source)));
			dest[5] = HIBYTE(LOWORD(HIDWORD(source)));
			dest[6] = LOBYTE(HIWORD(HIDWORD(source)));
			dest[7] = HIBYTE(HIWORD(HIDWORD(source)));
		}
		else
		{
			dest[0] = HIBYTE(HIWORD(HIDWORD(source)));
			dest[1] = LOBYTE(HIWORD(HIDWORD(source)));
			dest[2] = HIBYTE(LOWORD(HIDWORD(source)));
			dest[3] = LOBYTE(LOWORD(HIDWORD(source)));
			dest[4] = HIBYTE(HIWORD(LODWORD(source)));
			dest[5] = LOBYTE(HIWORD(LODWORD(source)));
			dest[6] = HIBYTE(LOWORD(LODWORD(source)));
			dest[7] = LOBYTE(LOWORD(LODWORD(source)));
		}
	}
	
	int32 Distance(int32 x1, int32 y1, int32 x2, int32 y2)
	{
		if(x1 == x2 && y1 == y2)
		{
			return 0;
		}

		int32 tx = x1 - x2;
		int32 ty = y1 - y2;

		return static_cast<int32>(sqrt(double(tx*tx + ty*ty)));
	}

	void ReduceUnsigned(uint32 & data, uint32 value)
	{
		if ( data > value )
		{
			data -= value;
		}
		else
		{
			data = 0;
		}
	}

	void ReplaceString(std::string &src, std::string const& search, std::string const& replace)
	{
		size_t pos = -1;

		while ( true )
		{
			pos = FindString(src, search);

			if ( pos == size_t(-1) )
				break;

			src.replace(pos, search.length(), replace);
		}
	}

	std::string ReplaceStringArg(const char * src, ...)
	{
		ARG(buffer, src);
		return buffer;
	}
}

Tokens::Tokens(const std::string &src, const char sep, uint32 vectorReserve)
{
    m_str = new char[src.length() + 1];
    memcpy(m_str, src.c_str(), src.length() + 1);

    if (vectorReserve)
        reserve(vectorReserve);

    char* posold = m_str;
    char* posnew = m_str;

    for (;;)
    {
        if (*posnew == sep)
        {
            push_back(posold);
            posold = posnew + 1;

            *posnew = 0x00;
        }
        else if (*posnew == 0x00)
        {
            // Hack like, but the old code accepted these kind of broken strings,
            // so changing it would break other things
            if (posold != posnew)
                push_back(posold);

            break;
        }

        ++posnew;
    }
}

//////////////////////////////////////////////////////////////////////////
int32 irand(int32 min, int32 max)
{
    WPAssert(max >= min);
    return int32(GetRng()->IRandom(min, max));
}

uint32 urand(uint32 min, uint32 max)
{
    WPAssert(max >= min);
    return GetRng()->URandom(min, max);
}

float frand(float min, float max)
{
    WPAssert(max >= min);
    return float(GetRng()->Random() * (max - min) + min);
}

uint32 rand32()
{
    return GetRng()->BRandom();
}

double rand_norm()
{
    return GetRng()->Random();
}

double rand_chance()
{
    return GetRng()->Random() * 100.0;
}

void stripLineInvisibleChars(std::string& str)
{
    static std::string invChars = " \t\7\n";

    size_t wpos = 0;

    bool space = false;
    for (size_t pos = 0; pos < str.size(); ++pos)
    {
        if (invChars.find(str[pos]) != std::string::npos)
        {
            if (!space)
            {
                str[wpos++] = ' ';
                space = true;
            }
        }
        else
        {
            if (wpos != pos)
                str[wpos++] = str[pos];
            else
                ++wpos;
            space = false;
        }
    }

    if (wpos < str.size())
        str.erase(wpos, str.size());
}

std::string secsToTimeString(time_t timeInSecs, bool shortText, bool hoursOnly)
{
    time_t secs    = timeInSecs % MINUTE;
    time_t minutes = timeInSecs % HOUR / MINUTE;
    time_t hours   = timeInSecs % DAY  / HOUR;
    time_t days    = timeInSecs / DAY;

    std::ostringstream ss;
    if (days)
        ss << days << (shortText ? "d " : " Day(s) ");
    if (hours || hoursOnly)
        ss << hours << (shortText ? "h " : " Hour(s) ");
    if (!hoursOnly)
    {
        if (minutes)
            ss << minutes << (shortText ? "m " : " Minute(s) ");
        if (secs || (!days && !hours && !minutes))
            ss << secs << (shortText ? "s." : " Second(s).");
    }

    return ss.str();
}

uint32 TimeStringToSecs(const std::string& timestring)
{
    uint32 secs       = 0;
    uint32 buffer     = 0;
    uint32 multiplier = 0;

    for (std::string::const_iterator itr = timestring.begin(); itr != timestring.end(); ++itr)
    {
        if (isdigit(*itr))
        {
            buffer *= 10;
            buffer += (*itr) - '0';
        }
        else
        {
            switch (*itr)
            {
                case 'd': multiplier = DAY;     break;
                case 'h': multiplier = HOUR;    break;
                case 'm': multiplier = MINUTE;  break;
                case 's': multiplier = 1;       break;
                default : return 0;                         // bad format
            }
            buffer *= multiplier;
            secs += buffer;
            buffer = 0;
        }
    }

    return secs;
}

std::string TimeToTimestampStr(time_t t)
{
    tm* aTm = localtime(&t);
    //       YYYY   year
    //       MM     month (2 digits 01-12)
    //       DD     day (2 digits 01-31)
    //       HH     hour (2 digits 00-23)
    //       MM     minutes (2 digits 00-59)
    //       SS     seconds (2 digits 00-59)
    char buf[20];
    snprintf(buf, 20, "%04d-%02d-%02d_%02d-%02d-%02d", aTm->tm_year + 1900, aTm->tm_mon + 1, aTm->tm_mday, aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
    return std::string(buf);
}

time_t timeBitFieldsToSecs( uint32 packedDate )
{
    tm lt;
    memset(&lt, 0, sizeof(lt));

    lt.tm_min = packedDate & 0x3F;
    lt.tm_hour = (packedDate >> 6) & 0x1F;
    lt.tm_wday = (packedDate >> 11) & 7;
    lt.tm_mday = ((packedDate >> 14) & 0x3F) + 1;
    lt.tm_mon = (packedDate >> 20) & 0xF;
    lt.tm_year = ((packedDate >> 24) & 0x1F) + 100;

    return time_t(mktime(&lt));
}

bool Utf8toWStr(char const* utf8str, size_t csize, wchar_t* wstr, size_t& wsize)
{
    try
    {
        size_t len = utf8::distance(utf8str, utf8str + csize);
        if (len > wsize)
        {
            if (wsize > 0)
                wstr[0] = L'\0';
            wsize = 0;
            return false;
        }

        wsize = len;
        utf8::utf8to16(utf8str, utf8str + csize, wstr);
        wstr[len] = L'\0';
    }
    catch (std::exception)
    {
        if (wsize > 0)
            wstr[0] = L'\0';
        wsize = 0;
        return false;
    }

    return true;
}

bool Utf8toWStr(const std::string& utf8str, std::wstring& wstr)
{
    try
    {
        size_t len = utf8::distance(utf8str.c_str(), utf8str.c_str() + utf8str.size());
        wstr.resize(len);

        if (len)
            utf8::utf8to16(utf8str.c_str(), utf8str.c_str() + utf8str.size(), &wstr[0]);
    }
    catch (std::exception)
    {
        wstr = L"";
        return false;
    }

    return true;
}

bool WStrToUtf8(wchar_t* wstr, size_t size, std::string& utf8str)
{
    try
    {
        std::string utf8str2;
        utf8str2.resize(size * 4);                          // allocate for most long case

        char* oend = utf8::utf16to8(wstr, wstr + size, &utf8str2[0]);
        utf8str2.resize(oend - (&utf8str2[0]));             // remove unused tail
        utf8str = utf8str2;
    }
    catch (std::exception)
    {
        utf8str = "";
        return false;
    }

    return true;
}

bool WStrToUtf8(std::wstring wstr, std::string& utf8str)
{
    try
    {
        std::string utf8str2;
        utf8str2.resize(wstr.size() * 4);                   // allocate for most long case

        char* oend = utf8::utf16to8(wstr.c_str(), wstr.c_str() + wstr.size(), &utf8str2[0]);
        utf8str2.resize(oend - (&utf8str2[0]));             // remove unused tail
        utf8str = utf8str2;
    }
    catch (std::exception)
    {
        utf8str = "";
        return false;
    }

    return true;
}

typedef wchar_t const* const* wstrlist;

void utf8printf(FILE* out, const char* str, ...)
{
    va_list ap;
    va_start(ap, str);
    vutf8printf(stdout, str, &ap);
    va_end(ap);
}

void vutf8printf(FILE* out, const char* str, va_list* ap)
{
#if PLATFORM == PLATFORM_WINDOWS
    char temp_buf[32*1024];
    wchar_t wtemp_buf[32*1024];

    size_t temp_len = vsnprintf(temp_buf, 32*1024, str, *ap);

    size_t wtemp_len = 32*1024-1;
    Utf8toWStr(temp_buf, temp_len, wtemp_buf, wtemp_len);

    CharToOemBuffW(&wtemp_buf[0], &temp_buf[0], wtemp_len+1);
    fprintf(out, "%s", temp_buf);
#else
    vfprintf(out, str, *ap);
#endif
}

void hexEncodeByteArray(uint8* bytes, uint32 arrayLen, std::string& result)
{
    std::ostringstream ss;
    for (uint32 i = 0; i < arrayLen; ++i)
    {
        for (uint8 j = 0; j < 2; ++j)
        {
            unsigned char nibble = 0x0F & (bytes[i] >> ((1 - j) * 4));
            char encodedNibble;
            if (nibble < 0x0A)
                encodedNibble = '0' + nibble;
            else
                encodedNibble = 'A' + nibble - 0x0A;
            ss << encodedNibble;
        }
    }
    result = ss.str();
}

bool minutesElapsed(int32 sec, uint8 minutes)
{
	if ( sec <= 0 )
		return false;

	if ( sec < (minutes * MINUTE) )
		return false;

	if ( (sec % (minutes * MINUTE) ) != 0 )
		return false;

	return true;
}

void StringToLower(char * str)
{
	for ( size_t i = 0; i < strlen(str); ++i )
	{
		str[i] = static_cast<char>(tolower(str[i]));
	}
}

void StringToUpper(char * str)
{
	for ( size_t i = 0; i < strlen(str); ++i )
	{
		str[i] = static_cast<char>(toupper(str[i]));
	}
}


bool CreatePath(std::string const& PathToCreate)
{
	char Path[_MAX_PATH];

	strcpy(Path, PathToCreate.c_str());

	// First add an ending slash

	char* Pos = strrchr(Path,'\\');

	if ( !Pos || Pos[1] != 0 )
		strcat(Path,"\\");

	// From the first slash to the last check and create path

	// Exception : Jump initial duble slashed UNC host name and include first directory

	if ( Path[0] == '\\' && Path[1] == '\\' )
	{
		// Find slash ending the host name
		Pos = strchr(Path+2,'\\');
		// Find slash after the first directory (after the host name)
		if (Pos)
			Pos = strchr(Pos+1,'\\');

	}
	else
	{
		Pos = strchr(Path,'\\');
	}

	bool AllOk = true;

	while (Pos && AllOk)
	{
		// Cut the string
		*Pos = 0;
		// Check if exist this piece of path
		if ( GetFileAttributes(Path) == -1 )
			if ( GetLastError() == ERROR_FILE_NOT_FOUND )
				AllOk = CreateDirectory(Path, NULL) == TRUE;
			else
				AllOk = false;

		// Recover the character deleted
		*Pos = '\\';

		// Find next slash
		Pos = strchr(Pos+1,'\\');
	}

	// Return true if finally the path was created or exist
	// (if not was created see LastError)
	return AllOk;
}

#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

std::string GetFileName()
{
	//GetModuleFileName(NULL, "", 0);

	char buffer[MAX_PATH] = {0};
	std::string out;
	uint32 size = sizeof(buffer) / sizeof(*buffer);

	if( GetModuleFileName(NULL, buffer, size) != size )
		return PathFindFileName(buffer);

	return "";
}

std::string GetConfigFileName()
{
	std::string out = GetFileName();

	Util::ReplaceString(out, ".exe", ".conf");

	return out;
}

std::string GetLogFolder()
{
	std::string out = GetFileName();

	Util::ReplaceString(out, ".exe", "_Log");

	return out;
}

void Shuffle(std::string &s)
{
    int32 size = s.size();
    srand(time(NULL));
    for(int32 a = 0; a < size-1; a++ )
    {
        int32 i = rand()%(size-a)+a;
        while(i==a)
            i = rand()%(size-a)+a;
        std::swap(s[a],s[rand()%(size-a)+a]);
    }
}

bool hasStringAbbr(const char* name, const char* part)
{
    // non "" command
    if (*name)
    {
        // "" part from non-"" command
        if (!*part)
            return false;

        for (;;)
        {
            if (!*part)
                return true;
            else if (!*name)
                return false;
            else if (tolower(*name) != tolower(*part))
                return false;
            ++name; ++part;
        }
    }
    // allow with any for ""

    return true;
}

Tokenizer::Tokenizer(const std::string &src, const char sep, uint32 vectorReserve)
{
    m_str = new char[src.length() + 1];
    memcpy(m_str, src.c_str(), src.length() + 1);

    if (vectorReserve)
        m_storage.reserve(vectorReserve);

    char* posold = m_str;
    char* posnew = m_str;

    for (;;)
    {
        if (*posnew == sep)
        {
            m_storage.push_back(posold);
            posold = posnew + 1;

            *posnew = '\0';
        }
        else if (*posnew == '\0')
        {
            // Hack like, but the old code accepted these kind of broken strings,
            // so changing it would break other things
            if (posold != posnew)
                m_storage.push_back(posold);

            break;
        }

        ++posnew;
    }
}

char* AllocMemory(const char * value)
{
	char* c = (char*)malloc(strlen(value) + 1);
	strcpy(c, value);
	return c;
}

std::string EncodePassword(std::string account, std::string password)
{
	/*Sha1Hash sha;
    sha.Initialize();
    sha.UpdateData(account);
    sha.UpdateData(":");
    sha.UpdateData(password);
    sha.Finalize();*/

    std::string encoded;
    //hexEncodeByteArray(sha.GetDigest(), sha.GetLength(), encoded);

    return encoded;
}

void Xor::BMD(uint8 * info, int32 Size)
{
	static const uint8 Key_1[KEY_SIZE_1]				= { 0xFC, 0xCF, 0xAB };

	for ( int32 i = 0; i < Size; ++i )
	{
		info[i] ^= Key_1[i % KEY_SIZE_1];
	}
}

void Xor::Byte3(char * info, int32 Size)
{
	static const uint8 Key_1[KEY_SIZE_1]				= { 0xFC, 0xCF, 0xAB };

	for ( int32 i = 0; i < Size; ++i )
	{
		info[i] ^= Key_1[i % KEY_SIZE_1];
	}
}

void Xor::LangXor(char * info, int32 Size, uint16 key)
{
	static const uint8 Key_1[KEY_SIZE_1] = { 0xFC, 0xCF, 0xAB };

	for (int32 i = 0; i < Size; ++i)
	{
		info[i] ^= Key_1[i % KEY_SIZE_1];
		info[i] ^= key;
	}
}

int32 Xor::Byte16(char * Source, char * Dest, int32 size)
{
	static const uint8 Key_2[KEY_SIZE_2]				= { 0xD1, 0x73, 0x52, 0xF6, 0xD2, 0x9A, 0xCB, 0x27, 0x3E, 0xAF, 0x59, 0x31, 0x37, 0xB3, 0xE7, 0xA2 };
	static const uint32 KEY_1							= 94;

	int32 key = KEY_1;

	for ( int32 i = 0; i < size; ++i )
	{
		Dest[i] = (Key_2[i % KEY_SIZE_2] ^ Source[i]) - key;
		key = Source[i] + 61;
	}

	return size;
}

void Xor::Byte20(char * src, char * dst, int32 size)
{
	static const uint8 Key_3[KEY_SIZE_3]				= { 0xD1, 0x73, 0x52, 0xF6, 0xD2, 0x9A, 0xCB, 0x27, 0x3E, 0xAF, 0x59, 0x31, 0x37, 0xB3, 0xE7, 0xA2 };

	for ( int32 i = 0; i < size; ++i )
	{
		dst[i] = (Key_3[i % KEY_SIZE_3] ^ src[i]);
	}
}

void Xor::ByteCustom(char * data, size_t size)
{
	static const uint8 Key_Custom[KEY_SIZE_CUSTOM]		= { 0xF1, 0x10, 0x23, 0x66, 0xAB };

	for ( size_t i = 0; i < size; ++i )
	{
		data[i] ^= Key_Custom[i % KEY_SIZE_CUSTOM];
	}
}

int32 Xor::Byte16New(char * Source, char * Dest, int32 size)
{
	static const uint8 Key_2[KEY_SIZE_2]				= { 0xD1, 0x73, 0x52, 0xF6, 0xD2, 0x9A, 0xCB, 0x27, 0x3E, 0xAF, 0x59, 0x31, 0x37, 0xB3, 0xE7, 0xA2 };
	static const uint32 KEY_1							= 94;

	int32 key = KEY_1;

	for ( int32 i = 0; i < size; ++i )
	{
		Dest[i] = (Key_2[i % KEY_SIZE_2] ^ Source[i]) - key;
		key = Source[i] + 61;
	}

	return size;
}

std::string ConvertTimeToString(time_t _time)
{
	tm * local_time = localtime(&_time);

	if ( !local_time )
		return "";

	char chDate[20];
	strftime(chDate, 20, "%Y-%m-%d %H:%M:%S", local_time);

	//sprintf(chDate,"%d-%.2d-%.2d %.2d:%.2d:%.2d",local_time->tm_year+1900,
	//	local_time->tm_mon,local_time->tm_mday,local_time->tm_hour,local_time->tm_min,local_time->tm_sec);
	std::string tmp = chDate;

	return tmp;
}

std::string TrimBlankSpaces(std::string const& source)
{
	std::string converted = source;

	do
	{
		if ( converted.empty() )
			break;

		if ( isspace(converted[0]) )
			converted.erase(0, 1);
		else
			break;
	}
	while(true);

	return converted;
}

std::string RemoveSpecialCharacters(std::string const& source)
{
	std::string converted = source;
	/*int32 count = 100;

	while ( count-- > 0 )
	{
		if ( converted.empty() )
		{
			break;
		}

		if ( converted[converted.length() - 1] == '%' )
		{
			converted = converted.substr(0, converted.length() - 1);
		}
	}
	*/
	return converted;
	//string str2 = str1.substr (0,str1.length()-1);
}

int32 GetTimeFromString(std::string const& source)
{
	std::istringstream stream(source);
	int32 time = 0;
	std::string data;

	if ( stream )
	{
		stream >> data;
		time = atoi(data.c_str());

		if ( stream )
		{
			stream >> data;
			strToLower(data);

			if ( data == "min" || data == "mins" )
			{
				time *= MINUTE;
			}
			else if ( data == "hour" || data == "hours" )
			{
				time *= HOUR;
			}
			else if ( data == "day" || data == "days" )
			{
				time *= DAY;
			}
		}
	}

	return time;
}