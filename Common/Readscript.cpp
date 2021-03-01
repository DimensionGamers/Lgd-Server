SMDToken ReadScript::GetToken()
{
	char ch;
	char* p;
	char TempString[10000];

	this->TokenString[0]=0;

	do
	{
		if ((ch=fgetc(this->fp)) == -1)
		{
			return END;	// End of FILE (EOF)
		}
		else
		{	
			if (ch=='/')		// this is /
			{
				if((ch=fgetc(this->fp)) == '/')
				{
					while ((ch != '\n') && (ch != -1))	// End of Line (EOL)
						ch=fgetc(this->fp);
	
					if (ch == -1)
						return END;
				}
			}
		}
	}
	while (isspace(ch) != 0);
	
	switch(ch)
	{
	case 0x23: return this->CurrentToken = COMMAND; break; // #
	case 0x3B: return this->CurrentToken = SEMICOLON; break; // ;
	case 0x2C: return this->CurrentToken = COMMA; break; // ,
	case 0x7B: return this->CurrentToken = LP; break; // {
	case 0x7D: return this->CurrentToken = RP; break; // }
	// Numbers Case
	case 0x2D:	//-
	case 0x2E:	//.
	case '0':	//0
	case '1':	//1
	case '2':	//2
	case '3':	//3
	case '4':	//4		
	case '5':	//5
	case '6':	//6
	case '7':	//7
	case '8':	//8
	case '9':	//9
		ungetc(ch, this->fp);
		p = TempString;
		
		while (((ch=getc(this->fp)) != -1) && ((ch == 0x2E) || (isdigit(ch) != 0) || (ch == 0x2D)))  // 2e '.'  2D '-'
		{
			*p = ch;	// Construct a String
			p++;
		}		
		*p = 0;
		this->TokenNumber = static_cast<float>(atof(TempString));	// Select the first Byte as Main ID
		return this->CurrentToken  = NUMBER ;
		break;

	case '\"':	// "	String Case
		p=&this->TokenString[0];
		
		while (((ch=getc(this->fp)) != -1 ) && (ch != 0x22))	// nice
		{
			*p = ch;
			p++;
		}
		if (ch != 0x22 )
		{
			ungetc(ch, this->fp);
		}
		*p = 0;
		return this->CurrentToken = NAME;
		break;

	default:	// Others
		if (isalpha(ch))
		{
			p=&this->TokenString[0];
			*p=ch;
			p++;

			while ( ((ch=getc(this->fp)) != -1) && ( (ch == 0x2E) || (ch == 0x5F) || (isalnum(ch) != 0) ) )
			{
				*p=ch;
				p++;
				
			}

			ungetc(ch, this->fp);
			*p=0;
			this->CurrentToken = NAME;
			return this->CurrentToken;
		}
		else
		{
			return this->CurrentToken = SMD_ERROR; // '<'
		}
		break;

	}

	return NAME;
}

void ReadScript::LogOpenError()
{
	sLog->outError(LOG_DEFAULT, "%s Open Error", this->m_filename.c_str());
}