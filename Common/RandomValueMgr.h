#ifndef RANDOM_VALUE_MGR_H
#define RANDOM_VALUE_MGR_H

enum RandomPoolType
{
	RANDOM_POOL_RANDOM,
	RANDOM_POOL_WEIGHT,
	RANDOM_POOL_RATE,
	RANDOM_POOL_MAX
};

template <typename T>
class RandomValue
{
	typedef std::map<T, int32> RandomPoolMap;

	public:
		RandomValue(T null_value)
		{
			this->total_weight = 0;
			this->m_null_value = null_value;
		}

		/*RandomValue()
		{
			this->total_weight = 0;
		}*/

		virtual ~RandomValue()
		{
			this->m_list.clear();
		}

		bool AddValue(T index, int32 weight)
		{
			RandomPoolMap::iterator it = this->m_list.find(index);

			if ( it == this->m_list.end() )
			{
				this->m_list[index] = weight;
				this->total_weight += weight;
				return true;
			}

			return false;
		}

		void SetTotalWeight(int32 weight)
		{
			this->total_weight = weight;
		}

		T GetRandomValue(RandomPoolType type)
		{
			int32 pool_size = this->m_list.size();

			if ( pool_size == 0 )
			{
				return this->m_null_value;
			}

			if ( this->total_weight <= 0 )
			{
				type = RANDOM_POOL_RANDOM;
			}

			if ( type == RANDOM_POOL_RANDOM )
			{
				int32 position = RANDOM(pool_size);
				int32 count = 0;

				for ( RandomPoolMap::iterator it = this->m_list.begin(); it != this->m_list.end(); ++it )
				{
					if ( position == count )
					{
						return it->first;
					}

					count++;
				}
			}
			else if ( type == RANDOM_POOL_WEIGHT )
			{
				int32 random_weight = RANDOM(this->total_weight);
				int32 total_rate = 0;

				for ( RandomPoolMap::iterator it = this->m_list.begin(); it != this->m_list.end(); ++it )
				{
					total_rate += it->second;

					if ( total_rate >= random_weight )
					{
						return it->first;
					}
				}
			}
			else if ( type == RANDOM_POOL_RATE )
			{
				for ( RandomPoolMap::iterator it = this->m_list.begin(); it != this->m_list.end(); )
				{
					if ( !it->second )
					{
						it = this->m_list.erase(it);
					}
					else
					{
						++it;
					}
				}

				pool_size = this->m_list.size();

				if ( this->m_list.empty() )
				{
					return this->m_null_value;
				}

				if ( this->total_weight <= 0 )
				{
					this->total_weight = 1;
				}

				while(true)
				{
					int32 position = RANDOM(pool_size);
					int32 count = 0;

					for ( RandomPoolMap::iterator it = this->m_list.begin(); it != this->m_list.end(); ++it )
					{
						if ( position == count )
						{
							if (roll_chance_i(it->second, this->total_weight))
							{
								return it->first;
							}

							break;
						}

						count++;
					}
				}
			}

			return this->m_null_value;
		}
		
	private:
		int32 total_weight;
		RandomPoolMap m_list;
		T m_null_value;
};

#endif