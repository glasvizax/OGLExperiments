#pragma once

#include "Aliases.h"

// FNV-1a
constexpr uint32 hashString(const char* str, size_t len)
{
	uint32 hash = 2166136261u;
	for (size_t i = 0; i < len; ++i)
	{
		hash ^= static_cast<uint32>(*str++);
		hash *= 16777619u;
	}
	return hash;
}

// FNV-1a
constexpr uint32 hashString(std::string_view view)
{
	return hashString(view.data(), view.length());
}