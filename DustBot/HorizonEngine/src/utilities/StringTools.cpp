#include "StringTools.h"
#include <algorithm>
#include <string>
#include <sstream>

std::string Horizon::StringTools::ReplaceAll(const std::string& str, const std::string& a, const std::string& b)
{
	std::string newStr = str;

	size_t startPosition = 0;
	while ((startPosition = newStr.find(a, startPosition)) != std::string::npos)
	{
		newStr.replace(startPosition, a.length(), b);
		startPosition += b.length();
	}

	return newStr;
}

std::string Horizon::StringTools::ToLower(std::string& str)
{
	transform(str.begin(), str.end(), str.begin(), ::tolower);

	return str;
}

std::string Horizon::StringTools::ToUpper(std::string& str)
{
	transform(str.begin(), str.end(), str.begin(), ::toupper);

	return str;
}

bool Horizon::StringTools::StartsWith(const std::string& str, const std::string& a)
{
	bool startsWith = false;

	if (str.substr(0, a.length()) == a)
	{
		startsWith = true;
	}

	return startsWith;
}

bool Horizon::StringTools::EndsWith(const std::string& str, const std::string& a)
{
	bool endsWith = false;

	if (str.substr(str.length() - a.length(), a.length()) == a)
	{
		endsWith = true;
	}

	return endsWith;
}

bool Horizon::StringTools::Contains(const std::string& string, const std::string& search)
{
	if(string.find(search) != std::string::npos) {
		return true;
	}
	else
	{
		return false;
	}
}

std::vector<std::string> Horizon::StringTools::Split(const std::string & str, const std::string & delimiter)
{
	std::vector<std::string> elements;

	elements.push_back(str);

	size_t splitPosition;
	size_t delimiterLength = delimiter.size();

	std::string currentFragment;

	while (true)
	{
		currentFragment = elements.back();

		splitPosition = currentFragment.find(delimiter);

		if (splitPosition == std::string::npos)
		{
			break;
		}

		elements.back() = currentFragment.substr(0, splitPosition);
		elements.push_back(currentFragment.substr(splitPosition + delimiterLength, currentFragment.size() - (splitPosition + delimiterLength)));
	}

	if (elements.back() == str)
	{
		return std::vector<std::string>();
	}
	else
	{
		return elements;
	}
}
