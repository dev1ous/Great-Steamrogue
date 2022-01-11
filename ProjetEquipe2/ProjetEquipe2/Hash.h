#ifndef HASH_H
#define HASH_H
#include <cstdio>
#include <string_view>

using HashID = unsigned;

//algorithm took on stack overflow temporarly. i will make my own later but i dont have time at the moment
constexpr HashID HASH(const char* hash, std::size_t _count) {
	return ((_count ? HASH(hash, _count - 1) : 2166136261u) ^ hash[_count]) * 16777619u;
}

//Run time ID
constexpr HashID HASH(std::string_view hash) {
	return HASH(hash.data(), hash.size());
}

//Compile time ID
constexpr HashID operator "" _hash(const char* hash, std::size_t _count) {
	return HASH(hash, _count);
}
#endif

