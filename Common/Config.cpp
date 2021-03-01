/*
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
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

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

using namespace boost::property_tree;

bool Config::LoadInitial(std::string const& file, std::string& error, std::string const& section)
{
#ifndef USING_OLD_CONFIG
    std::lock_guard<std::mutex> lock(_configLock);
#endif

    _filename = file;

    try
    {
        ptree fullTree;
        ini_parser::read_ini(file, fullTree);

        if (fullTree.empty())
        {
            error = "empty file (" + file + ")";
            return false;
        }

        // Since we're using only one section per config file, we skip the section and have direct property access
        _config = fullTree.begin()->second;
    }
    catch (ini_parser::ini_parser_error const& e)
    {
        if (e.line() == 0)
            error = e.message() + " (" + e.filename() + ")";
        else
            error = e.message() + " (" + e.filename() + ":" + std::to_string(e.line()) + ")";
        return false;
    }

#ifdef USING_OLD_CONFIG
	_section = section;
#endif

    return true;

}

bool Config::Reload(std::string& error)
{
#ifndef USING_OLD_CONFIG
    return LoadInitial(_filename, error);
#else
	return true;
#endif
}

std::string Config::GetStringDefault(std::string const& name, const std::string& def)
{
#ifndef USING_OLD_CONFIG
    std::string value = _config.get<std::string>(ptree::path_type(name, '/'), def);

	value.erase(std::remove(value.begin(), value.end(), '"'), value.end());

	return value;
#else
	char returned_string[1000];
	memset(returned_string, 0, 1000);

    GetPrivateProfileString(_section.c_str(), name.c_str(), def.c_str(), returned_string, 1000, std::string(".\\" + _filename).c_str());

	//sLog->outInfo("config_temp", "\"%s\", \"%s\"", name.c_str(), returned_string);

	return returned_string;
#endif
}

bool Config::GetBoolDefault(std::string const& name, bool def)
{
#ifndef USING_OLD_CONFIG
    try
    {
        std::string val = _config.get<std::string>(ptree::path_type(name, '/'));
        val.erase(std::remove(val.begin(), val.end(), '"'), val.end());
        return (val == "true" || val == "TRUE" || val == "yes" || val == "YES" || val == "1");
    }
    catch (std::exception const&  /*ex*/)
    {
        return def;
    }
#else
	int result = GetPrivateProfileInt(_section.c_str(), name.c_str(), def, std::string(".\\" + _filename).c_str());

	//sLog->outInfo("config_temp", "\"%s\", \"%d\"", name.c_str(), result);

	return result == 0 ? false: true;
#endif
}

int Config::GetIntDefault(std::string const& name, int def)
{
#ifndef USING_OLD_CONFIG
    return _config.get<int>(ptree::path_type(name, '/'), def);
#else
	int result = GetPrivateProfileInt(_section.c_str(), name.c_str(), def, std::string(".\\" + _filename).c_str());

	//sLog->outInfo("config_temp", "\"%s\", \"%d\"", name.c_str(), result);

	return result;
#endif
}

int Config::GetTimeDefault(std::string const& name, int def)
{
	int32 result = GetTimeFromString(this->GetStringDefault(name, std::to_string(def)));

	//sLog->outInfo("config_temp", "\"%s\", \"%d\"", name.c_str(), result);

	return result;
}

float Config::GetFloatDefault(std::string const& name, float def)
{
	std::string result = this->GetStringDefault(name, std::to_string(def));

	return static_cast<float>(atof((char*)result.c_str()));
}

#ifndef USING_OLD_CONFIG
std::string const& Config::GetFilename()
{
    std::lock_guard<std::mutex> lock(_configLock);
    return _filename;
}
#endif

void Config::WriteString(std::string const& name, std::string const& value)
{
	WritePrivateProfileString(_section.c_str(), name.c_str(), value.c_str(), std::string(".\\" + _filename).c_str());
}
	
void Config::WriteInt(std::string const& name, int32 value)
{
	WritePrivateProfileString(_section.c_str(), name.c_str(), std::to_string(value).c_str(), std::string(".\\" + _filename).c_str());
}