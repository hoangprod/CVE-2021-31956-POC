#pragma once
#include <cstdio>
#include <source_location>

template<typename... Args>
constexpr void LogTrace(const std::source_location location, const char* format, Args... args) {
	printf("[ fs0x30 ] - %s: ", location.function_name());
	printf(format, args...);
	printf("\n");
}