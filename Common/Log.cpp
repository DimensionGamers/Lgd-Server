/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2008 MaNGOS <http://getmangos.com/>
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

Log::Log(): AppenderId(0)
{
    m_logsTimestamp = "_" + GetTimestampStr();
}

Log::~Log()
{
    Close();
}

uint8 Log::NextAppenderId()
{
    return AppenderId++;
}

Appender* Log::GetAppenderByName(std::string const& name)
{
    AppenderMap::iterator it = appenders.begin();
    while (it != appenders.end() && it->second && it->second->getName() != name)
        ++it;

    return it == appenders.end() ? NULL : it->second;
}

void Log::LoadAppender(std::string const& appenderName, AppenderType type, LogLevel level, AppenderFlags flags, std::string const& path, std::string const& filename, std::string const& mode)
{
	std::string mPath = m_logsDir;
	if ( type == APPENDER_FILE )
	{
		if ( !path.empty() )
		{
			mPath.append(path);
			CreatePath(mPath);

			if (!mPath.empty())
				if ((mPath.at(mPath.length() - 1) != '/') && (mPath.at(mPath.length() - 1) != '\\'))
					mPath.push_back('/');
		}
	}

    switch (type)
    {
	case APPENDER_CONSOLE:
        {
			AppenderConsole* appender = new AppenderConsole(NextAppenderId(), appenderName, level, flags);
			appenders[appender->getId()] = appender;
		} break;

	case APPENDER_FILE:
        {
			uint8 id = NextAppenderId();
			appenders[id] = new AppenderFile(id, appenderName, level, filename.c_str(), mPath.c_str(), mode.c_str(), flags);
        } break;
    }
}

void Log::LoadLogger(std::string const& name, LogLevel level, std::string const& appenders)
{
	Logger& logger = loggers[name];
    if (!logger.getName().empty())
    {
        fprintf(stderr, "Error while configuring Logger %s. Already defined\n", name.c_str());
        return;
    }

    if (level > LOG_LEVEL_FATAL)
    {
        fprintf(stderr, "Log::CreateLoggerFromConfig: Wrong Log Level %u for logger %s\n", uint32(level), name.c_str());
        return;
    }

    logger.Create(name, level);

    std::istringstream ss(appenders);
    std::string str;

    ss >> str;
    while (ss)
    {
        if (Appender* appender = GetAppenderByName(str))
        {
            logger.addAppender(appender->getId(), appender);
        }
        else
            fprintf(stderr, "Error while configuring Appender %s in Logger %s. Appender does not exist", str.c_str(), name.c_str());
        ss >> str;
    }
}

void Log::vlog(std::string const& filter, LogLevel level, char const* str, va_list argptr)
{
    char text[MAX_QUERY_LEN];
    vsnprintf(text, MAX_QUERY_LEN, str, argptr);
    std::unique_ptr<LogMessage> msg(new LogMessage(level, filter, text));
	write(std::move(msg));
}

void Log::vlog_load(std::string const& filter, LogLevel level, char const* str, va_list argptr)
{
	char text[MAX_QUERY_LEN];
	vsnprintf(text, MAX_QUERY_LEN, str, argptr);

	std::string lg = text;
	lg += " in " + std::to_string(MyGetTickCount() - this->GetOutTime()) + " msec.";

	std::unique_ptr<LogMessage> msg(new LogMessage(level, filter, lg));
	write(std::move(msg));
}

void Log::write(std::unique_ptr<LogMessage>&& msg) const
{
    //msg->text.append("\n");
    Logger const* logger = GetLoggerByType(msg->type);

	if ( logger )
		logger->write(msg.get());

	//std::unique_ptr<LogOperation> pLarge(new LogOperation(logger, msg));
	//pLarge->call();
}

std::string Log::GetTimestampStr()
{
    time_t t = time(NULL);
    tm* aTm = localtime(&t);
    //       YYYY   year
    //       MM     month (2 digits 01-12)
    //       DD     day (2 digits 01-31)
    //       HH     hour (2 digits 00-23)
    //       MM     minutes (2 digits 00-59)
    //       SS     seconds (2 digits 00-59)
    char buf[100];
    snprintf(buf, 100, "[%04d-%02d-%02d_%02d-%02d-%02d]", aTm->tm_year+1900, aTm->tm_mon+1, aTm->tm_mday, aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
    return std::string(buf);
}

void Log::outTrace(std::string const& filter, const char * str, ...)
{
    if (!str || !ShouldLog(filter, LOG_LEVEL_TRACE))
        return;

    va_list ap;
    va_start(ap, str);

    vlog(filter, LOG_LEVEL_TRACE, str, ap);

    va_end(ap);
}

void Log::outDebug(std::string const& filter, const char * str, ...)
{
    if (!str || !ShouldLog(filter, LOG_LEVEL_DEBUG))
        return;

    va_list ap;
    va_start(ap, str);

    vlog(filter, LOG_LEVEL_DEBUG, str, ap);

    va_end(ap);
}

void Log::outInfo(std::string const& filter, const char * str, ...)
{
    if (!str || !ShouldLog(filter, LOG_LEVEL_INFO))
        return;

    va_list ap;
    va_start(ap, str);

    vlog(filter, LOG_LEVEL_INFO, str, ap);

    va_end(ap);
}

void Log::outWarn(std::string const& filter, const char * str, ...)
{
    if (!str || !ShouldLog(filter, LOG_LEVEL_WARN))
        return;

    va_list ap;
    va_start(ap, str);

    vlog(filter, LOG_LEVEL_WARN, str, ap);

    va_end(ap);
}

void Log::outError(std::string const& filter, const char * str, ...)
{
    if (!str || !ShouldLog(filter, LOG_LEVEL_ERROR))
        return;

    va_list ap;
    va_start(ap, str);

    vlog(filter, LOG_LEVEL_ERROR, str, ap);

    va_end(ap);
}

void Log::outFatal(std::string const& filter, const char * str, ...)
{
    if (!str || !ShouldLog(filter, LOG_LEVEL_FATAL))
        return;

    va_list ap;
    va_start(ap, str);

    vlog(filter, LOG_LEVEL_FATAL, str, ap);

    va_end(ap);
}

void Log::outPacket(std::string const& filter, uint8 const* packet, uint16 len, const char * str, ...)
{
	ARG(buffer, str);

	std::string	X;
	std::string	T;
	char cBuff[8192];

	for ( uint16 i = 0; i < len; ++i )
	{
		wsprintf(cBuff, "%02X,", packet[i]);
		X	= cBuff;
		T  += X;
	}

	std::string data = std::string(buffer) + T;

	this->outInfo(filter, data.c_str());
}

void Log::outLoad(bool init, const char * str, ...)
{
	if (!str || !ShouldLog("root", LOG_LEVEL_INFO))
		return;

	if (init)
	{
		this->SetOutTime(MyGetTickCount());
	}

	va_list ap;
	va_start(ap, str);

	if (init)
	{
		vlog("root", LOG_LEVEL_INFO, str, ap);
	}
	else
	{
		vlog_load("root", LOG_LEVEL_INFO, str, ap);
	}

	va_end(ap);

	if (!init)
	{
		this->outInfo("root", " ");
	}
}

void Log::Close()
{
    loggers.clear();
    for (AppenderMap::iterator it = appenders.begin(); it != appenders.end(); ++it)
    {
        delete it->second;
        it->second = NULL;
    }
    appenders.clear();
}

void Log::LoadFolder()
{
	m_logsDir = GetLogFolder();

	CreatePath(m_logsDir);

	if (!m_logsDir.empty())
		if ((m_logsDir.at(m_logsDir.length() - 1) != '/') && (m_logsDir.at(m_logsDir.length() - 1) != '\\'))
			m_logsDir.push_back('/');
}