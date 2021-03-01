#ifndef SINGLETON_INC_H
#define SINGLETON_INC_H

#define SingletonInstance(TYPE) \
	public: \
	static TYPE* instance() \
		{ \
			static TYPE instance; \
			return &instance; \
		} \
		private: \
		TYPE(TYPE const&); \
		TYPE& operator=(TYPE const&); \
		

#define SingNull(TYPE) TYPE::instance()

#endif