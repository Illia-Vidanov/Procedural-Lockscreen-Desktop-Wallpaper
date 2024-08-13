#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstring>
#include <filesystem>


template<typename CharT>
inline constexpr const CharT *slash = "/";
template<>
inline constexpr const wchar_t *slash<wchar_t> = L"/";



template<typename CharT>
inline auto AddCurrentPathToString(const std::basic_string<CharT> &str) -> std::basic_string<CharT> { return std::filesystem::current_path().generic_string<CharT>() + slash<CharT> + str; }
template<typename CharT>
inline auto RemoveCurrentPathFromString(const std::basic_string<CharT> &str) -> std::basic_string<CharT>;

// Split string by delimetr in raw pointer arrays
// std::pair with begin and end is returned
// User takes care of the memory!
template<typename CharT>
auto SplitStringRaw(const std::basic_string<CharT> &str, const std::basic_string<CharT> &delim) -> std::pair<CharT**, CharT**>;
// Split string by delimetr in vector of strings
template<typename CharT>
inline auto SplitString(const std::basic_string<CharT> &str, const std::basic_string<CharT> &delim) -> std::vector<std::basic_string<CharT>>;
// Split string by delimetr in vector of strings
template<typename CharT>
inline auto SplitString(const std::basic_string<CharT> &str, CharT delim) -> std::vector<std::basic_string<CharT>>
{ return SplitString(str, std::basic_string<CharT>(1, delim)); }

// Check if string starts with certain prefix
// Doesn't perform bounds checking so if prefix.size() > string.size() behaviour is undefined
template<typename CharT>
inline auto StringStartsWith(const std::basic_string<CharT> &prefix, const std::basic_string<CharT> &str) -> bool;

// Count amount of substrings in string
template<typename CharT>
inline auto SubstringCount(const std::basic_string<CharT> &str, const std::basic_string<CharT> &substr) -> std::size_t;

// Get char count of raw string array
template<typename CharT>
inline constexpr auto RawStringArrayLength(const CharT **first, const CharT **last) -> std::size_t; 

// Connect raw string array into single string with delimetr
template<typename CharT>
inline auto ConcatStringArray(const CharT **first, const CharT **last, const std::basic_string<CharT> &delim = "") -> std::basic_string<CharT>;

// strlen but templated for different char types
template<typename CharT>
inline constexpr auto StrLen(const CharT *str) -> std::size_t;



template<typename CharT>
inline auto RemoveCurrentPathFromString(const std::basic_string<CharT> &str) -> std::basic_string<CharT>
{
    const std::basic_string<CharT> &path = std::filesystem::current_path().generic_string<CharT>();
    if(!StringStartsWith(path, str))
        return str;
    return std::basic_string<CharT>(str).replace(0, path.size() + 1, ""); // + 1 for slash
}

template<typename CharT>
auto SplitStringRaw(const std::basic_string<CharT> &str, const std::basic_string<CharT> &delim) -> std::pair<CharT**, CharT**>
{
	const std::size_t result_size = SubstringCount(str, delim);
	CharT **result = new char*[result_size];
	std::size_t index = 0;
	
	std::size_t fromPos = 0;
	for(std::size_t delimPos = str.find(delim); delimPos != std::basic_string<CharT>::npos; delimPos = str.find(delim, delimPos + 1))
	{
		const std::basic_string<CharT> &element = str.substr(fromPos, delimPos - fromPos);
		if(!element.empty())
		{
			result[index] = new char[element.size() + 1]; // + 1 for null terminator
			std::copy(element.begin(), element.end() + 1, result[index]); // + 1 to get null terminator
			index++;
		}
		fromPos = delimPos + delim.size();
	}
	
	const std::basic_string<CharT> &element = str.substr(fromPos, str.size());
	if(!element.empty())
	{
		result[index] = new char[element.size() + 1];
		std::copy(element.begin(), element.end() + 1, result[index]);
		index++;
	}

	index++; // we add one to avoid writing index + 1 for every case where actual past the end element is required

	// Shrink to fit. We reserve much more memory in some cases. On avarage it is same as we use, so really, it can be commented out if memory use is not a concern or use cases aren't this demanding
	//if(index < result_size)
	//{
	//	char **unshrinked_result = result;
	//	result = new CharT*[index];
	//	std::copy(unshrinked_result, unshrinked_result + index, result);
	//	delete[] unshrinked_result;
	//}
	
	return std::make_pair(result, result + index);
}

template<typename CharT>
inline auto SplitString(const std::basic_string<CharT> &str, const std::basic_string<CharT> &delim) -> std::vector<std::basic_string<CharT>>
{
	const std::pair<CharT**, CharT**> &raw_str_arr = SplitStringRaw(str, delim);
	std::vector<std::basic_string<CharT>> result(raw_str_arr.second - raw_str_arr.first);
	for(std::size_t i = 0; i < result.size(); i++)
		result[i] = std::string(std::move(*(raw_str_arr.first + i)));

    // no need to delete string inside because they are moved
	delete[] raw_str_arr.first;

	return result;
}

template<typename CharT>
inline auto StringStartsWith(const std::basic_string<CharT> &prefix, const std::basic_string<CharT> &str) -> bool
{
    for(std::size_t i = 0; i < prefix.size(); i++)
    {
        if(str[i] != prefix[i])
            return false;
    }

    return true;
}

template<typename CharT>
inline auto SubstringCount(const std::basic_string<CharT> &str, const std::basic_string<CharT> &substr) -> std::size_t
{
	std::size_t count = 0;
	std::size_t substrPos = str.find(substr);
    while(substrPos != std::basic_string<CharT>::npos)
	{
		count++;
		substrPos = str.find(substr, substrPos + 1);
	}

	return count;
}

template<typename CharT>
inline constexpr auto RawStringArrayLength(const CharT **first, const CharT **last) -> std::size_t
{
    std::size_t result = 0;
	for(; first != last; first++)
		result += StrLen(*first);

	return result;
}

template<typename CharT>
inline auto ConcatStringArray(const CharT **first, const CharT **last, const std::basic_string<CharT> &delim) -> std::basic_string<CharT>
{
	std::string result;
	result.reserve(RawStringArrayLength(first, last) + (last - first - 1) * delim.size());

    for(; first != last; first++)
	{
		result += *first;
		result += delim;
	}

	return result;
}

template <typename CharT>
inline constexpr auto StrLen(const CharT *str) -> std::size_t
{
    std::size_t result = 0;
    while(*str != static_cast<CharT>('\0'))
	{
        result++;
		str++;
	}
    return result;
}

#endif // STRING_UTILS_HPP