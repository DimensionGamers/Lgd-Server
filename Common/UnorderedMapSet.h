#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H

#ifdef _STLPORT_VERSION
#define UNORDERED_MAP std::hash_map
using std::hash_map;
#elif COMPILER == COMPILER_MICROSOFT && _MSC_VER >= 1500 && _HAS_TR1
#define UNORDERED_MAP std::tr1::unordered_map
#elif COMPILER == COMPILER_MICROSOFT && _MSC_VER >= 1300
#define UNORDERED_MAP stdext::hash_map
using stdext::hash_map;
#elif COMPILER == COMPILER_GNU && (__GNUC__ > 4 || __GNUC__ == 4 && __GNUC_MINOR__ >= 3)
#define UNORDERED_MAP std::tr1::unordered_map
#elif (COMPILER == COMPILER_GNU && __GNUC__ >= 3) || COMPILER == COMPILER_INTEL
#define UNORDERED_MAP __gnu_cxx::hash_map

namespace __gnu_cxx
{
    template<> struct hash<unsigned long long>
    {
        size_t operator()(const unsigned long long &__x) const { return (size_t)__x; }
    };
    template<typename T> struct hash<T *>
    {
        size_t operator()(T * const &__x) const { return (size_t)__x; }
    };
    template<> struct hash<std::string>
    {
        size_t operator()(const std::string &__x) const
        {
            return hash<const char *>()(__x.c_str());
        }
    };
};

#else
#define UNORDERED_MAP std::hash_map
using std::hash_map;
#endif

#endif
