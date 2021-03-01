#ifndef	_READ_SCRIPT_H_
#define	_READ_SCRIPT_H_

enum SMDToken 
{
	NAME, 
	NUMBER, 
	END, 
	COMMAND = '#',
	LP = '{',
	RP = '}',
	COMMA = ',',
	SEMICOLON = ';',
	SMD_ERROR
};

class ReadScript
{
	public:
		ReadScript(const char* filename, const char* mode)
		{
			this->m_filename = filename;
			this->fp = fopen(filename,mode);
		}
		~ReadScript()
		{
			this->close();
		}

		bool open() const { return this->fp != NULL; }
		void close() { if ( this->fp ) fclose(this->fp); }

		SMDToken GetToken();

		bool IsEnd(const char * end)
		{
			return !strcmp(end,this->TokenString);
		}

		void LogOpenError();

		uint8 GetUInt8() const { return static_cast<uint8>(this->TokenNumber); }
		uint16 GetUInt16() const { return static_cast<uint16>(this->TokenNumber); }
		uint32 GetUInt32() const { return static_cast<uint32>(this->TokenNumber); }
		bool GetBool() const { return this->GetUInt8() == 1 ? true: false; }

		int8 GetInt8() const { return static_cast<int8>(this->TokenNumber); }
		int16 GetInt16() const { return static_cast<int16>(this->TokenNumber); }
		int32 GetInt32() const { return static_cast<int32>(this->TokenNumber); }

		float GetFloat() const { return this->TokenNumber; }
	public:
		FILE* fp;
		float TokenNumber;
		char TokenString[10000];
		SMDToken CurrentToken;
		std::string m_filename;
};

#endif