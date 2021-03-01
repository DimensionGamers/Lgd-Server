/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#ifndef TRINITYCORE_LOG_H
#define TRINITYCORE_LOG_H

#define LOGGER_ROOT "root"

class Log
{
    SingletonInstance(Log);

    typedef std::unordered_map<std::string, Logger> LoggerMap;
	DECLARE_ENUM(TCType, OutTime);

    private:
        Log();
        ~Log();

    public:
		void LoadFolder();
		void LoadAppender(std::string const&, AppenderType type, LogLevel level, AppenderFlags flags, std::string const& path, std::string const& filename, std::string const& mode);
        void LoadLogger(std::string const&, LogLevel level, std::string const& appenders);

        void Close();
        bool ShouldLog(std::string const& type, LogLevel level) const;

        void outTrace(std::string const& filter, char const* str, ...) ATTR_PRINTF(3,4);
        void outDebug(std::string const& filter, char const* str, ...) ATTR_PRINTF(3,4);
        void outInfo(std::string const& filter, char const* str, ...) ATTR_PRINTF(3,4);
        void outWarn(std::string const& filter, char const* str, ...) ATTR_PRINTF(3,4);
        void outError(std::string const& filter, char const* str, ...) ATTR_PRINTF(3,4);
        void outFatal(std::string const& filter, char const* str, ...) ATTR_PRINTF(3,4);
		void outPacket(std::string const& filter, uint8 const* packet, uint16 len, const char * str, ...);
		void outLoad(bool init, char const* str, ...) ATTR_PRINTF(3, 4);

        static std::string GetTimestampStr();
    private:
        void vlog(std::string const& filter, LogLevel level, char const* str, va_list argptr);
		void vlog_load(std::string const& filter, LogLevel level, char const* str, va_list argptr);
        void write(std::unique_ptr<LogMessage>&& msg) const;

        Logger const* GetLoggerByType(std::string const& type) const;
        Appender* GetAppenderByName(std::string const& name);
        uint8 NextAppenderId();
        
        AppenderMap appenders;
        LoggerMap loggers;
        uint8 AppenderId;

        std::string m_logsDir;
        std::string m_logsTimestamp;
};

inline Logger const* Log::GetLoggerByType(std::string const& type) const
{
    LoggerMap::const_iterator it = loggers.find(type);
    if (it != loggers.end())
        return &(it->second);

    if (type == LOGGER_ROOT)
        return NULL;

    std::string parentLogger = LOGGER_ROOT;
    size_t found = type.find_last_of(".");
    if (found != std::string::npos)
        parentLogger = type.substr(0,found);

    return GetLoggerByType(parentLogger);
}

inline bool Log::ShouldLog(std::string const& type, LogLevel level) const
{
    // TODO: Use cache to store "Type.sub1.sub2": "Type" equivalence, should
    // Speed up in cases where requesting "Type.sub1.sub2" but only configured
    // Logger "Type"

    Logger const* logger = GetLoggerByType(type);
    if (!logger)
        return false;

    LogLevel logLevel = logger->getLogLevel();
    return logLevel != LOG_LEVEL_DISABLED && logLevel <= level;
}

#endif