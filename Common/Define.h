#ifndef DEFINE_H
#define DEFINE_H

#pragma warning(disable:4018)

#define ATTR_PRINTF(F,V)

typedef __int64 int64;
typedef int int32;
typedef short int16;
typedef char int8;
typedef unsigned __int64 uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

#define coord_type int16
#define world_type uint16

#if UPDATE_GET_TICK_COUNT == 0
typedef uint32 TCType;
#define MyGetTickCount GetTickCount
#else
typedef uint64 TCType;
#define MyGetTickCount GetTickCount64
#endif

#define SET_NUMBERH(x) ( (uint8)((uint16)(x)>>(uint16)8) )
#define SET_NUMBERL(x) ( (uint8)((uint16)(x) & 0xFF) )
#define MAKE_NUMBERW(x,y)  ( (uint16)(((uint8)((y)&0xFF)) |   ((uint8)((x)&0xFF)<<8 ))  )
#define MAKE_NUMBERDW(x,y) ( (uint32)(((uint16)((y)&0xFFFF)) | ((uint16)((x)&0xFFFF)<<16))  )
#define LODWORD(l) ((uint32)(((uint64)(l)) & 0xffffffff))
#define HIDWORD(l) ((uint32)((((uint64)(l)) >> 32) & 0xffffffff))
#define SET_NUMBERHW(x) ( (uint16)((uint32)(x)>>(uint32)16) )
#define SET_NUMBERLW(x) ( (uint16)((uint32)(x) & 0xFFFF) )

#define CASTLE_SIEGE_SCORE_MACRO(value) ((int64)value & 0xFFFFFFFF)

#if PLATFORM == PLATFORM_WINDOWS
#  define TRINITY_PATH_MAX MAX_PATH
#  define _USE_MATH_DEFINES
#  ifndef DECLSPEC_NORETURN
#    define DECLSPEC_NORETURN __declspec(noreturn)
#  endif //DECLSPEC_NORETURN
#  ifndef DECLSPEC_DEPRECATED
#    define DECLSPEC_DEPRECATED __declspec(deprecated)
#  endif //DECLSPEC_DEPRECATED
#else //PLATFORM != PLATFORM_WINDOWS
#  define TRINITY_PATH_MAX PATH_MAX
#  define DECLSPEC_NORETURN
#  define DECLSPEC_DEPRECATED
#endif //PLATFORM

#if COMPILER == COMPILER_GNU
#  define ATTR_NORETURN __attribute__((noreturn))
#  define ATTR_PRINTF(F, V) __attribute__ ((format (printf, F, V)))
#  define ATTR_DEPRECATED __attribute__((deprecated))
#else //COMPILER != COMPILER_GNU
#  define ATTR_NORETURN
#  define ATTR_PRINTF(F, V)
#  define ATTR_DEPRECATED
#endif //COMPILER == COMPILER_GNU

#define PAIR std::pair

#define CLIENT_TO_SERVER				"[ Client to Server ]"
#define SERVER_TO_CLIENT				"[ Server to Client ]"

#define non_copyable(type) \
	private: \
	type(const type&) = delete; \
	type& operator=(type const&) = delete;

#define DECLARE_PROPERTY_STRING(name)	\
	protected: \
		std::string m_prop##name; \
	public: \
		std::string Get##name() { return this->m_prop##name; } \
		std::string Get##name() const { return this->m_prop##name; } \
		char* Get##name##Ptr() { return (char*)this->m_prop##name.c_str(); } \
		std::string& Get##name##Ref() { return this->m_prop##name; } \
		void Set##name(std::string const& value) { this->m_prop##name.assign(value); } \
		void Set##name(const char* value) { this->m_prop##name.assign(value); } \
		void Reset##name() { this->m_prop##name.clear(); } \
		void Convert##name##ToLower() { strToLower(this->m_prop##name); } \
		void Convert##name##ToUpper() { strToUpper(this->m_prop##name); }


#define DECLARE_BOOL(name)	\
	protected: \
		bool m_propb##name; \
	public: \
		bool Is##name() const { return this->m_propb##name; } \
		void Set##name(bool value) { this->m_propb##name = value; }

#define DECLARE_ARRAY_BOOL(name, size)	\
	protected: \
		bool m_propb##name[size]; \
	public: \
		bool Is##name(uint32 position) const { return position >= size ? false: this->m_propb##name[position]; } \
		void Set##name(uint32 position, bool value) { if ( position < size ) this->m_propb##name[position] = value; } \
		void Set##name(bool value) { for ( uint32 i = 0; i < size; i++ ) this->m_propb##name[i] = value; }


#define DECLARE_PROPERTY(type, name)	\
	protected: \
		##type m_prop##name; \
	public: \
		##type Get##name() const { return this->m_prop##name; } \
		void Set##name(##type value) { this->m_prop##name = value; } \
		void Increase##name(##type value) { this->m_prop##name = this->m_prop##name + value; } \
		void Reduce##name(##type value) { this->m_prop##name = this->m_prop##name - value; } \
		void LimitMax##name(##type value) { if ( this->m_prop##name > value ) this->m_prop##name = value; } \
		void LimitMin##name(##type value) { if ( this->m_prop##name < value ) this->m_prop##name = value; } \
		void And##name(##type value) { this->m_prop##name = this->m_prop##name & value; } \
		void Or##name(##type value) { this->m_prop##name = this->m_prop##name | value; }

#define DECLARE_FLAG(type, name)	\
	protected: \
		##type m_prop##name; \
	public: \
		##type Get##name() const { return this->m_prop##name; } \
		void Set##name(##type value) { this->m_prop##name = value; } \
		void Add##name(##type value) { this->m_prop##name = this->m_prop##name | value; } \
		void Remove##name(##type value) { this->m_prop##name = this->m_prop##name & ~value; } \
		bool Is##name(##type value) const { return (this->m_prop##name & value) != 0; }

		
#define DECLARE_FLOAT(name)	\
	protected: \
		float m_prop##name; \
	public: \
		float Get##name() const { return this->m_prop##name; } \
		void Set##name(float value) { this->m_prop##name = value; } \
		void Increase##name(float value) { this->m_prop##name = this->m_prop##name + value; } \
		void Reduce##name(float value) { this->m_prop##name = this->m_prop##name - value; } \
		void LimitMax##name(float value) { if ( this->m_prop##name > value ) this->m_prop##name = value; } \
		void LimitMin##name(float value) { if ( this->m_prop##name < value ) this->m_prop##name = value; }

#define DECLARE_ENUM(type, name)	\
	protected: \
		##type m_prop##name; \
	public: \
		##type Get##name() const { return this->m_prop##name; } \
		void Set##name(##type value) { this->m_prop##name = value; }

#define DECLARE_RANGE(type, name) \
	protected: \
		##type m_prop##name##Min; \
		##type m_prop##name##Max; \
	public: \
		##type Get##name##Min() const { return this->m_prop##name##Min; } \
		void Set##name##Min(##type value) { this->m_prop##name##Min = value; } \
		##type Get##name##Max() const { return this->m_prop##name##Max; } \
		void Set##name##Max(##type value) { this->m_prop##name##Max = value; } \
		type GetRandom##name() const { if ( this->Get##name##Min() >= this->Get##name##Max() ) { return this->Get##name##Min(); } \
			return Random<type>((this->Get##name##Max() - this->Get##name##Min()) + 1) + this->Get##name##Min(); }

#define DECLARE_PROPERTY_PTR(type, name)	\
	protected: \
		##type* m_prop##name; \
	public: \
		##type* Get##name() { return this->m_prop##name; } \
		##type* Get##name() const { return this->m_prop##name; } \
		void Set##name(##type* value) { this->m_prop##name = value; }

#define DECLARE_PROPERTY_ARRAY(type, name, size)	\
	protected: \
		##type m_prop##name[size]; \
	public: \
		##type* Get##name##s() { return &this->m_prop##name[0]; } \
		##type const* Get##name##s() const { return &this->m_prop##name[0]; } \
		##type* Get##name##Ptr(unsigned int dwPosition) { return &this->m_prop##name[dwPosition]; } \
		##type Get##name(unsigned int dwPosition) { return this->m_prop##name[dwPosition]; } \
		##type Get##name(unsigned int dwPosition) const { return this->m_prop##name[dwPosition]; } \
		unsigned int Get##name##Length() const { return size; } \
		void Set##name (unsigned int dwPosition, ##type Value) { this->m_prop##name[dwPosition] = Value; } \
		void Set##name (##type const* Value) { for ( uint32 i = 0; i < size; ++i ) this->m_prop##name[i] = Value[i]; } \
		void Reset##name(##type Value) { for ( uint32 i = 0; i < size; ++i ) this->m_prop##name[i] = Value; } \
		void Increase##name(uint32 dwPosition, ##type value) { this->m_prop##name[dwPosition] = this->m_prop##name[dwPosition] + value; } \
		void Reduce##name(uint32 dwPosition, ##type value) { this->m_prop##name[dwPosition] = this->m_prop##name[dwPosition] - value; } \
		void Copy##name(##type * value) { for ( uint32 i = 0; i < size; ++i ) value[i] = this->m_prop##name[i]; }

#define DECLARE_PROPERTY_ARRAY_PTR(type, name, size)	\
	protected: \
		##type m_prop##name[size]; \
	public: \
		##type* Get##name##s() { return &this->m_prop##name[0]; } \
		##type* Get##name##Ptr(unsigned int dwPosition) { return &this->m_prop##name[dwPosition]; } \
		##type Get##name(unsigned int dwPosition) { return this->m_prop##name[dwPosition]; } \
		##type Get##name(unsigned int dwPosition) const { return this->m_prop##name[dwPosition]; } \
		unsigned int Get##name##Length() { return size; } \
		void Set##name (unsigned int dwPosition, ##type Value) { this->m_prop##name[dwPosition] = Value; } \
		void Reset##name(##type Value) { for ( uint32 i = 0; i < size; ++i ) this->m_prop##name[i] = Value; } 

#define DECLARE_ARRAY_STRUCT(type, name, size)	\
	protected: \
		##type m_prop##name[size]; \
	public: \
		##type* Get##name(unsigned int dwPosition) { return &this->m_prop##name[dwPosition]; } \
		##type const* Get##name(unsigned int dwPosition) const { return &this->m_prop##name[dwPosition]; } \
		##type* Get##name##Ptr() { return &this->m_prop##name[0]; } \
		##type const* Get##name##Ptr() const { return &this->m_prop##name[0]; } \
		void Set##name(unsigned int dwPosition, type const& value) { if ( dwPosition < size ) { this->m_prop##name[dwPosition] = value; } }

#define DECLARE_ARRAY_STRUCT_SAFE(type, name, size)	\
	protected: \
		##type m_prop##name[size]; \
	public: \
		##type* Get##name(unsigned int dwPosition) { return &this->m_prop##name[dwPosition]; } \
		##type const* Get##name(unsigned int dwPosition) const { return &this->m_prop##name[dwPosition]; }

#define DECLARE_ARRAY_STRUCT_PTR(type, name, size)	\
	protected: \
		##type* m_prop##name[size]; \
	public: \
		##type* Get##name(unsigned int dwPosition) const { return this->m_prop##name[dwPosition]; } \
		void Set##name (unsigned int dwPosition, ##type* Value) { if ( dwPosition < size ) { this->m_prop##name[dwPosition] = Value; } } \
		void Erase##name() {  for ( uint32 i = 0; i < size; ++i ) SAFE_DELETE(this->m_prop##name[i]); } \
		void Init##name() {  for ( uint32 i = 0; i < size; ++i ) this->m_prop##name[i] = NULL; }

#define DECLARE_ARRAY_PTR(type, name)	\
	protected: \
		##type** m_prop##name; \
		uint32 m_prop##name##size; \
	public: \
		##type* Get##name(uint32 dwPosition) { return this->m_prop##name[dwPosition]; } \
		##type const* Get##name(uint32 dwPosition) const { return this->m_prop##name[dwPosition]; } \
		void Set##name (uint32 dwPosition, ##type* Value) { this->m_prop##name[dwPosition] = Value; } \
		void Erase##name() { SAFE_DELETE_ARRAY(this->m_prop##name); this->m_prop##name##size = 0; } \
		void Create##name(uint32 size) { this->m_prop##name = new (##type *[size]); for ( uint32 i = 0; i < size; ++i ) this->m_prop##name[i] = NULL; this->m_prop##name##size = size; } \
		void Init##name() { this->m_prop##name = NULL; this->m_prop##name##size = 0; } \
		uint32 Get##name##Size() const { return this->m_prop##name##size; }

#define DECLARE_STRUCT_PTR(type, name)	\
	protected: \
		##type* m_prop##name; \
		DECLARE_ENUM(uint32, ##name##Size); \
	public: \
		##type* Get##name(unsigned int dwPosition) { return &this->m_prop##name[dwPosition]; } \
		##type const* Get##name(unsigned int dwPosition) const { return &this->m_prop##name[dwPosition]; } \
		void Set##name (unsigned int dwPosition, ##type Value) { this->m_prop##name[dwPosition] = Value; } \
		void Set##name (##type * Value) { this->m_prop##name = Value; } \
		void Erase##name() { SAFE_DELETE_ARRAY(this->m_prop##name); this->Set##name##Size(0); } \
		void Create##name(uint32 size) { this->m_prop##name = new (##type[size]); this->Set##name##Size(size); } \
		void Init##name() { this->m_prop##name = nullptr; this->Set##name##Size(0); } \
		bool Is##name##Loaded() const { return this->m_prop##name != nullptr; }

#define DECLARE_STRUCT(type, name)	\
	protected: \
		##type m_prop##name; \
	public: \
		##type* Get##name() { return &this->m_prop##name; } \
		##type const* Get##name() const { return &this->m_prop##name; } \
		void Set##name(##type* value) { memcpy(&this->m_prop##name, value, sizeof(##type)); }
		

#define DECLARE_PROPERTY_ARRAY_ENUM(type, name, size, Enum)	\
	protected: \
		##type m_prop##name[size]; \
	public: \
		##type* Get##name##s() { return this->m_prop##name; } \
		##type* Get##name##Ptr(Enum dwPosition) { return &this->m_prop##name[dwPosition]; } \
		##type Get##name(Enum dwPosition) const { return this->m_prop##name[dwPosition]; } \
		void Set##name (Enum dwPosition, ##type Value) { this->m_prop##name[dwPosition] = Value; } \
		void Reset##name(##type Value) { for ( uint32 i = 0; i < size; ++i ) this->m_prop##name[i] = Value; } \
		void Increase##name(Enum dwPosition, ##type value) { this->m_prop##name[dwPosition] = this->m_prop##name[dwPosition] + value; } \
		void Reduce##name(Enum dwPosition, ##type value) { this->m_prop##name[dwPosition] = this->m_prop##name[dwPosition] - value; } \
		void Percent##name(Enum dwPosition, ##type value) { this->Increase##name(dwPosition, this->Get##name(dwPosition) * value / 100); } \
		void Copy##name(##type * value) { for ( uint32 i = 0; i < size; ++i ) value[i] = this->m_prop##name[i]; } \
		void LimitMax##name(Enum dwPosition, ##type value) { if ( this->m_prop##name[dwPosition] > value ) this->m_prop##name[dwPosition] = value; } \
		void LimitMin##name(Enum dwPosition, ##type value) { if ( this->m_prop##name[dwPosition] < value ) this->m_prop##name[dwPosition] = value; }

#define DECLARE_PTR(type, name)	\
	protected: \
		##type* m_prop##name; \
	public: \
		##type* Get##name() { return this->m_prop##name; } \
		##type* Get##name() const { return this->m_prop##name; } \
		void Set##name(##type* value) { this->m_prop##name = value; } \
		void Erase##name() { SAFE_DELETE(this->m_prop##name); }

#define DECLARE_STRING_FIXED(name, size)	\
	protected: \
		char m_prop##name[size]; \
	public: \
		const char * Get##name() const { return this->m_prop##name; } \
		void Set##name(const char * value, size_t len) { this->Reset##name(); if ( value ) memcpy(this->m_prop##name, value, len > size ? size: len); } \
		void Set##name(const char * value) { this->Reset##name(); if ( value ) memcpy(this->m_prop##name, value, strlen(value) > size ? size: strlen(value)); } \
		void Reset##name() { memset(this->m_prop##name, 0, size); } \
		void Set##name##ToLower() { StringToLower(this->m_prop##name); } \
		void Copy##name(const char * value) { this->Reset##name(); if ( value ) strcpy(this->m_prop##name, value); } \


#define DECLARE_ARRAY_ENUM(enum_type, type, name, size)	\
	protected: \
		##type m_prop##name[size]; \
	public: \
		##type Get##name(enum_type dwPosition) const { return this->m_prop##name[dwPosition]; } \
		void Set##name (enum_type dwPosition, ##type Value) { this->m_prop##name[dwPosition] = Value; } \
		void Reset##name(##type Value) { for ( uint32 i = 0; i < size; ++i ) this->m_prop##name[i] = Value; } \
		void Increase##name(enum_type dwPosition, ##type value) { this->m_prop##name[dwPosition] = this->m_prop##name[dwPosition] + value; } \
		void Reduce##name(enum_type dwPosition, ##type value) { this->m_prop##name[dwPosition] = this->m_prop##name[dwPosition] - value; }

template<typename T>
inline void memcpy(T* target, T const* source)
{
	memcpy(target, source, sizeof(T));
}

#endif
