#pragma once
#include <vector>

// Utility functions for string parsing
//
// trim() removes whitespace before and after relevant data
//
// getTokens() parses input stream into vector of strings
//
// PopTagFromTokens() is used to compare vector element in pos
// to input string. If equal, element is erased from vector and
// output is that element. If input string is empty, output will be
// the element in vector at pos.

std::string trim(const std::string& str);
std::vector<std::string> getTokens(std::istringstream&, const char delim);
std::string PopTagFromTokens(std::vector<std::string>&, const size_t&, const std::string&);
