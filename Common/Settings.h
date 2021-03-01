#ifndef SETTINGS_H
#define SETTINGS_H

typedef std::map<std::string, std::string> SettingMap;

class SettingsMgr
{
	public:
		SettingsMgr()
		{
			
		}

		virtual ~SettingsMgr()
		{
			this->Init();
		}

		virtual void Load() = 0;

		void Init()
		{
			this->setting_map.clear();
		}

		void Add(std::string const& key, std::string const& value)
		{
			this->setting_map[key] = value;
		}

		virtual void CreateSetting(std::string const& key, std::string const& default) = 0;

		bool GetBool(std::string const& key, bool default)
        {
            return this->GetUInt8(key, default ? 1 : 0) == 1 ? true : false;
        }

        uint8 GetUInt8(std::string const& key, uint8 default)
        {
			SettingMap::const_iterator it = this->setting_map.find(key);

            if ( it == this->setting_map.end() )
			{
				this->CreateSetting(key, std::to_string(default));
                return default;
			}

			if (it->second.empty())
				return default;

			return static_cast<uint8>(strtoul((char*)it->second.c_str(), nullptr, 10));
        }

        int8 GetInt8(std::string const& key, int8 default)
        {
            SettingMap::const_iterator it = this->setting_map.find(key);

            if ( it == this->setting_map.end() )
			{
				this->CreateSetting(key, std::to_string(default));
                return default;
			}

			if (it->second.empty())
				return default;

			return static_cast<int8>(strtol((char*)it->second.c_str(), NULL, 10));
        }

        uint16 GetUInt16(std::string const& key, uint16 default)
        {
			SettingMap::const_iterator it = this->setting_map.find(key);

            if ( it == this->setting_map.end() )
			{
				this->CreateSetting(key, std::to_string(default));
                return default;
			}

			if (it->second.empty())
				return default;

            return static_cast<uint16>(strtoul((char*)it->second.c_str(), nullptr, 10));
        }

        int16 GetInt16(std::string const& key, int16 default)
        {
            SettingMap::const_iterator it = this->setting_map.find(key);

            if ( it == this->setting_map.end() )
			{
				this->CreateSetting(key, std::to_string(default));
                return default;
			}

			if (it->second.empty())
				return default;

            return static_cast<int16>(strtol((char*)it->second.c_str(), NULL, 10));
        }

        uint32 GetUInt32(std::string const& key, uint32 default)
        {
            SettingMap::const_iterator it = this->setting_map.find(key);

            if ( it == this->setting_map.end() )
			{
				this->CreateSetting(key, std::to_string(default));
                return default;
			}

			if (it->second.empty())
				return default;

            return static_cast<uint32>(strtoul((char*)it->second.c_str(), nullptr, 10));
        }

        int32 GetInt32(std::string const& key, int32 default)
        {
            SettingMap::const_iterator it = this->setting_map.find(key);

            if ( it == this->setting_map.end() )
			{
				this->CreateSetting(key, std::to_string(default));
                return default;
			}

			if (it->second.empty())
				return default;

            return static_cast<int32>(strtol((char*)it->second.c_str(), NULL, 10));
        }

        uint64 GetUInt64(std::string const& key, uint64 default)
        {
            SettingMap::const_iterator it = this->setting_map.find(key);

            if ( it == this->setting_map.end() )
			{
				this->CreateSetting(key, std::to_string(default));
                return default;
			}

			if (it->second.empty())
				return default;

            return static_cast<uint64>(strtoull((char*)it->second.c_str(), nullptr, 10));
        }

        int64 GetInt64(std::string const& key, int64 default)
        {
            SettingMap::const_iterator it = this->setting_map.find(key);

            if ( it == this->setting_map.end() )
			{
				this->CreateSetting(key, std::to_string(default));
                return default;
			}

			if (it->second.empty())
				return default;

			return static_cast<int64>(strtoll((char*)it->second.c_str(), NULL, 10));
        }

        float GetFloat(std::string const& key, float default)
        {
            SettingMap::const_iterator it = this->setting_map.find(key);

            if ( it == this->setting_map.end() )
			{
				this->CreateSetting(key, std::to_string(default));
                return default;
			}

			if (it->second.empty())
				return default;

            return static_cast<float>(atof((char*)it->second.c_str()));
        }

        double GetDouble(std::string const& key, double default)
        {
            SettingMap::const_iterator it = this->setting_map.find(key);

            if ( it == this->setting_map.end() )
			{
				this->CreateSetting(key, std::to_string(default));
                return default;
			}

			if (it->second.empty())
				return default;

            return static_cast<double>(atof((char*)it->second.c_str()));
        }

		uint32 GetTime(std::string const& key, uint32 default)
		{
			SettingMap::const_iterator it = this->setting_map.find(key);

            if ( it == this->setting_map.end() )
			{
				this->CreateSetting(key, std::to_string(default));
                return default;
			}

			if (it->second.empty())
				return default;

            return GetTimeFromString(it->second);
		}

		uint32 GetTimeMS(std::string const& key, uint32 default)
		{
            return this->GetTime(key, default) * IN_MILLISECONDS;
		}

		std::string GetString(std::string const& key, std::string const& default)
		{
			SettingMap::const_iterator it = this->setting_map.find(key);

            if ( it == this->setting_map.end() )
			{
				this->CreateSetting(key, default);
                return default;
			}

			if (it->second.empty())
				return default;

			return it->second;
		}

	private:
		SettingMap setting_map;
};

#endif