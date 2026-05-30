#pragma once

#include "Aliases.h"

#include <string_view>
#include <span>

// FNV-1a

constexpr uint32 fnv1a_init = 2166136261u;
constexpr uint32 fnv1a_prime = 16777619u;

constexpr uint32 hashString(std::string_view view, uint32 hash = fnv1a_init)
{
	for (char c : view)
	{
		hash ^= static_cast<uint32>(static_cast<unsigned char>(c));
		hash *= fnv1a_prime;
	}
	return hash;
}

constexpr uint32 hashString(std::span<std::string_view> views)
{
	uint32 hash = fnv1a_init;
	for (size_t i = 0; i < views.size(); ++i)
	{
		hash = hashString(views[i], hash);
	}

	return hash;
}

constexpr uint32_t operator"" _id(const char* str, size_t len)
{
	return hashString(std::string_view(str, len));
}