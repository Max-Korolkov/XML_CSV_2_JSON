#include <string>
#include <vector>
#include <sstream>
#include "StringUtils.h"

std::string trim(const std::string& str) // remove whitespace
{
	// useless symbols
	const std::string WHITESPACE = " \t\n\v\f\r";

	std::string trimmedStr = "";

	// removing unnecessary symbols before and after the relevant part
	size_t start = str.find_first_not_of(WHITESPACE);
	trimmedStr = (start == std::string::npos) ? "" : str.substr(start);
	size_t end = trimmedStr.find_last_not_of(WHITESPACE);
	trimmedStr = (end == std::string::npos) ? "" : trimmedStr.substr(0, end + 1);

	return trimmedStr;
}

std::vector<std::string> getTokens(std::istringstream& stream, const char delim) // parse to vector
{
	std::vector<std::string> tokens;
	std::string token;

	// parsing input string into vector of strings
	while (std::getline(stream, token, delim))
	{
		token = trim(token);
		// empty strings are ignored
		if (!token.empty())
			tokens.push_back(token);
	}

	return tokens;
}

std::string PopTagFromTokens(std::vector<std::string>& tokens, const size_t& pos, const std::string& tag) // get elem from pos
{
	std::string res;
	// if pos is out of reach
	if (pos >= tokens.size()) 
		return "";

	// if input string doesn't match elem in pos
	if (tag != "" && tokens[pos] != tag) 
		return "";
	res = tokens[pos];

	// remove elem from vector
	tokens.erase(tokens.begin() + pos);
	return res;
}
