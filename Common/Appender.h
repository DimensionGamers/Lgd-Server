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

#ifndef APPENDER_H
#define APPENDER_H

#define	LOG_DEFAULT			"root"
#define	LOG_TRADE			"trade"
#define	LOG_CASHSHOP		"cashshop"
#define	LOG_NETWORK			"network"
#define	LOG_CHAOS_MIX		"chaosmix"
#define	LOG_QUEST			"quest"
#define	LOG_BLOOD_CASTLE	"bloodcastle"
#define	LOG_DEVIL_SQUARE	"devilsquare"
#define	LOG_CHAOS_CASTLE	"chaoscastle"
#define	LOG_CRYWOLF			"crywolf"
#define	LOG_PARTY			"party"
#define	LOG_INVASION		"invasion"
#define	LOG_KANTURU			"kanturu"
#define	LOG_CASTLE_SIEGE	"castlesiege"
#define	LOG_SQL				"database"
#define	LOG_RAKLION			"raklion"
#define	LOG_PERSONAL_SHOP	"personalshop"
#define	LOG_DOUBLE_GOER		"doublegoer"
#define	LOG_GUILD			"guild"
#define	LOG_ITEMS			"items"
#define	LOG_GENS			"gens"
#define	LOG_SKILL			"skill"
#define	LOG_CHAT			"chat"
#define	LOG_PLAYER			"player"
#define	LOG_MONSTER			"monster"
#define	LOG_CRASH			"crash"
#define	LOG_SCRAMBLE		"scramble"
#define	LOG_RACE			"race"
#define	LOG_ILLUSION_TEMPLE	"illusiontemple"
#define	LOG_IMPERIAL_FORTRESS	"imperial_fortress"

enum LogLevel
{
    LOG_LEVEL_DISABLED                           = 0,
    LOG_LEVEL_TRACE                              = 1,
    LOG_LEVEL_DEBUG                              = 2,
    LOG_LEVEL_INFO                               = 3,
    LOG_LEVEL_WARN                               = 4,
    LOG_LEVEL_ERROR                              = 5,
    LOG_LEVEL_FATAL                              = 6
};

const uint8 MaxLogLevels = 6;

enum AppenderType
{
    APPENDER_NONE,
    APPENDER_CONSOLE,
    APPENDER_FILE
};

enum AppenderFlags
{
    APPENDER_FLAGS_NONE                 = 0x00,
    APPENDER_FLAGS_PREFIX_TIMESTAMP     = 0x01,
    APPENDER_FLAGS_PREFIX_LOGLEVEL      = 0x02,
    APPENDER_FLAGS_PREFIX_LOGFILTERTYPE = 0x04,
    APPENDER_FLAGS_USE_TIMESTAMP        = 0x08, // only used by FileAppender
    APPENDER_FLAGS_MAKE_FILE_BACKUP     = 0x10  // only used by FileAppender
};

struct LogMessage
{
    LogMessage(LogLevel _level, std::string const& _type, std::string const& _text)
        : level(_level), type(_type), text(_text), mtime(time(NULL))
    { }

	LogMessage(LogMessage const& /*other*/) = delete;
    LogMessage& operator=(LogMessage const& /*other*/) = delete;

    static std::string getTimeStr(time_t time);
    std::string getTimeStr();

    LogLevel const level;
    std::string const type;
    std::string const text;
    std::string prefix;
    std::string param1;
    time_t mtime;

    ///@ Returns size of the log message content in bytes
    uint32 Size() const
    {
        return static_cast<uint32>(prefix.size() + text.size());
    }
};

class Appender
{
    public:
        Appender(uint8 _id, std::string const& name, LogLevel level = LOG_LEVEL_DISABLED, AppenderFlags flags = APPENDER_FLAGS_NONE);
        virtual ~Appender();

        uint8 getId() const;
        std::string const& getName() const;
        virtual AppenderType getType() const = 0;
        LogLevel getLogLevel() const;
        AppenderFlags getFlags() const;

        void setLogLevel(LogLevel);
        void write(LogMessage * message);
        static const char* getLogLevelString(LogLevel level);

    private:
        virtual void _write(LogMessage const* message) = 0;

        uint8 id;
        std::string name;
        LogLevel level;
        AppenderFlags flags;
};

typedef std::map<uint8, Appender*> AppenderMap;

#endif