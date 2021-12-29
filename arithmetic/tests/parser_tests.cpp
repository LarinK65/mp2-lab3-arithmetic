#include "pch.h"

#include "../arithmetic/include/parser_functions.h"
#include <limits>
#include <algorithm>

static bool dbl_cmp(double a, double b)
{
	return std::abs(a - b) < std::numeric_limits<double>::epsilon() * 1e+3;
}

TEST(number_parser, test1)
{
	std::string_view s("5.5");

	ASSERT_TRUE(dbl_cmp(parse_number(s), 5.5));
}

TEST(number_parser, test2)
{
	std::string_view s(".5");

	ASSERT_TRUE(dbl_cmp(parse_number(s), 0.5));
}

TEST(number_parser, test3)
{
	std::string_view s("5.");

	ASSERT_TRUE(dbl_cmp(parse_number(s), 5.));
}

TEST(number_parser, test4)
{
	std::string_view s("0.5");

	ASSERT_TRUE(dbl_cmp(parse_number(s), 0.5));
}

TEST(number_parser, test5)
{
	std::string_view s("5.0");

	ASSERT_TRUE(dbl_cmp(parse_number(s), 5.0));
}

TEST(number_parser, test6)
{
	std::string_view s("5e+2");

	ASSERT_TRUE(dbl_cmp(parse_number(s), 500));
}

TEST(number_parser, test7)
{
	std::string_view s("5e-2");

	ASSERT_TRUE(dbl_cmp(parse_number(s), 0.05));
}

TEST(number_parser, test8)
{
	std::string_view s("5.42e-2");

	ASSERT_TRUE(dbl_cmp(parse_number(s), 0.0542));
}

TEST(number_parser, test9)
{
	std::string_view s("5.42e1");

	ASSERT_TRUE(dbl_cmp(parse_number(s), 54.2));
}

TEST(number_parser, test10)
{
	std::string_view s("5.42.1");

	ASSERT_ANY_THROW(parse_number(s));
}

TEST(number_parser, test11)
{
	std::string_view s("5.4e2.1");

	ASSERT_ANY_THROW(parse_number(s));
}



TEST(name_parser, test1)
{
	std::string_view s("var");

	ASSERT_TRUE(parse_name(s) == "var");
}

TEST(name_parser, test2)
{
	std::string_view s("var1");

	ASSERT_TRUE(parse_name(s) == "var1");
}

TEST(name_parser, test3)
{
	std::string_view s("int2str");

	ASSERT_TRUE(parse_name(s) == "int2str");
}

TEST(name_parser, test4)
{
	std::string_view s("_1");

	ASSERT_TRUE(parse_name(s) == "_1");
}

TEST(name_parser, test5)
{
	std::string_view s("var+15");

	ASSERT_TRUE(parse_name(s) == "var");
	ASSERT_TRUE(s == "+15");
}



TEST(operator_parser, basic_operators)
{
	std::string_view s("+15");

	ASSERT_TRUE(parse_operator(s));
}

TEST(operator_parser, special_operators)
{
	std::string_view s("(15");

	ASSERT_TRUE(parse_special_operator(s));
}

