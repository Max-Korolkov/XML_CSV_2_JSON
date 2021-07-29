#pragma once
#include <vector>

std::string trim(const std::string& str);
std::vector<std::string> getTokens(std::istringstream&, const char delim);
std::string PopTagFromTokens(std::vector<std::string>&, const size_t&, const std::string&);
