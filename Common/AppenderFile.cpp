/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

AppenderFile::AppenderFile(uint8 id, std::string const& name, LogLevel level, const char* filename, const char* _logDir, const char* mode, AppenderFlags flags):
	Appender(id, name, level, flags), 
	logfile(NULL),
    _logDir(_logDir)
{
	open_time = Custom::SystemTimer();
	this->GetOpenTick()->Reset();

    _fileName = filename;

    _dynamicName = std::string::npos != _fileName.find("%s");
    _backup = (flags & APPENDER_FLAGS_MAKE_FILE_BACKUP) != 0;

    if (!_dynamicName)
        logfile = OpenFile(_fileName, mode, !strcmp(mode, "w") && _backup);
}

AppenderFile::~AppenderFile()
{
    CloseFile();
}

void AppenderFile::_write(LogMessage const* message)
{
    if (_dynamicName)
    {
        char namebuf[TRINITY_PATH_MAX];
        snprintf(namebuf, TRINITY_PATH_MAX, _fileName.c_str(), message->param1.c_str());
        // always use "a" with dynamic name otherwise it could delete the log we wrote in last _write() call
        FILE* file = OpenFile(namebuf, "a", _backup);
        if (!file)
            return;
        fprintf(file, "%s%s\n", message->prefix.c_str(), message->text.c_str());
        fflush(file);
        fclose(file);
        return;
    }
    else if ( this->GetOpenTick()->Elapsed(MINUTE * IN_MILLISECONDS) )
	{
		Custom::SystemTimer time_tmp = Custom::SystemTimer();

		if ( time_tmp.GetDay() != this->open_time.GetDay() ||
			 time_tmp.GetMonth() != this->open_time.GetMonth() ||
			 time_tmp.GetYear() != this->open_time.GetYear() )
		{
			open_time = time_tmp;
			logfile = OpenFile(_fileName, "w", true);
		}
	}

    if (!logfile)
        return;

    fprintf(logfile, "%s%s\n", message->prefix.c_str(), message->text.c_str());
    fflush(logfile);
}

FILE* AppenderFile::OpenFile(std::string const &filename, std::string const &mode, bool backup)
{
	std::string fullName(_logDir + filename);

	if ( !_dynamicName )
	{
		if (getFlags() & APPENDER_FLAGS_USE_TIMESTAMP)
		{
			size_t dot_pos = fullName.find_last_of(".");
			if (dot_pos != fullName.npos)
				fullName.insert(dot_pos, "_" + Log::GetTimestampStr());
			else
				fullName += "_" + Log::GetTimestampStr();
		}
	}
	
    if (backup)
    {
        CloseFile();
        std::string newName(fullName);
        newName.push_back('.');
        newName.append(LogMessage::getTimeStr(time(NULL)));
        //std::replace(newName.begin(), newName.end(), ':', '-');
        rename(fullName.c_str(), newName.c_str()); // no error handling... if we couldn't make a backup, just ignore
    }

    if (FILE* ret = fopen(fullName.c_str(), mode.c_str()))
    {
        return ret;
    }

    return NULL;
}

void AppenderFile::CloseFile()
{
    if (logfile)
    {
        fclose(logfile);
        logfile = NULL;
    }
}