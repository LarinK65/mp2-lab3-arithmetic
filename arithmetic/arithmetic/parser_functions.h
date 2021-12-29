#pragma once

#include <string_view>
#include <algorithm>
#include <cctype>
#include <limits>
#include <exception>
#include <string>
#include "operations_table.h"

inline double _parse_integer(std::string_view& str)
{
	double res = 0;

	while (!str.empty() && std::isdigit(str.front()))
	{
		res = res * 10. + (str.front() - '0');
		str.remove_prefix(1);
	}

	return res;
}

inline double _parse_fraction(std::string_view& str)
{
	double res = 0;
	double pw = 0.1;
	
	if (str.empty() || str.front() != '.') {
		return 0;
	}

	str.remove_prefix(1);

	while (!str.empty() && std::isdigit(str.front()))
	{
		res = res + (str.front() - '0') * pw;
		pw /= 10.;
		str.remove_prefix(1);
	}

	return res;
}

inline double _parse_exp(std::string_view& str)
{
	double sign = 1;


	if (str.size() <= 1 || str.front() != 'e') {
		return 0;
	}

	str.remove_prefix(1);


	if (str.front() == '+' || str.front() == '-') {
		if (str.front() == '-') {
			sign = -1;
		}

		str.remove_prefix(1);
	}

	double res = _parse_integer(str);

	return sign * res;
}

inline double parse_number(std::string_view& str)
{
	constexpr double err_value = std::numeric_limits<double>::quiet_NaN();

	if (str.empty() || (str.front() != '.' && !std::isdigit(str.front()))) {
		return err_value;
	}

	double sign = 1;
	double result = 0;


	result = _parse_integer(str);
	result += _parse_fraction(str);

	if (!str.empty() && str.front() == '.') {
		throw std::exception("unexpected symbol \".\"");
	}

	result *= std::pow(10, _parse_exp(str));

	if (!str.empty() && str.front() == '.') {
		throw std::exception("unexpected symbol \".\"");
	}


	return result;
}



inline bool is_part_of_name(char ch)
{
	return std::isalpha(ch) || std::isdigit(ch) || ch == '_';
}

inline std::string parse_name(std::string_view& str)
{
	std::string res;

	while (!str.empty() && is_part_of_name(str.front()))
	{
		res.push_back(str.front());
		str.remove_prefix(1);
	}

	return res;
}



inline bool parse_operator(std::string_view& str)
{
	for (size_t i = 0; i < sizeof(operators) / sizeof(*operators); i++)
	{
		if (str.front() == operators[i]) {
			return true;
		}
	}

	return false;
}

inline bool parse_special_operator(std::string_view& str)
{
	for (size_t i = 0; i < sizeof(special_operators) / sizeof(*special_operators); i++)
	{
		if (str.front() == special_operators[i]) {
			return true;
		}
	}

	return false;
}


