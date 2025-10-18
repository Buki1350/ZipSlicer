#pragma once
#include <string>

static long long ConvertToBytes(long long value, const std::string& unit) {
    if (unit == "-b")  return value;
    if (unit == "-kb") return value * 1024LL;
    if (unit == "-mb") return value * 1024LL * 1024LL;
    if (unit == "-gb") return value * 1024LL * 1024LL * 1024LL;
    return value;
}