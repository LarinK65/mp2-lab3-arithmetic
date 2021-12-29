#include "pch.h"

#include "../arithmetic/Expression.h"
#include "../arithmetic/Lexem.h"
#include <algorithm>

TEST(calculator, test1)
{
	std::vector<std::string> vars;
	std::vector<double> vals;
	Expression e(vars.begin(), vars.end(), "10/2");

	ASSERT_TRUE(e.calculate(vals.begin(), vals.end()) == 5);
}

TEST(calculator, test2)
{
	std::vector<std::string> vars{ "x", "y" };
	std::vector<double> vals{ 2, 5 };
	Expression e(vars.begin(), vars.end(), "-10/-2(24)x(abs(-4y))");

	ASSERT_TRUE(e.calculate(vals.begin(), vals.end()) == 4800);
}

TEST(calculator, test3)
{
	std::vector<std::string> vars;
	std::vector<double> vals;


	ASSERT_ANY_THROW(
		Expression e(vars.begin(), vars.end(), "1-+5");
		e.calculate(vals.begin(), vals.end());
	);
}

TEST(calculator, test4)
{
	std::vector<std::string> vars{ "x", "y", "a", "b" };
	std::vector<double> vals{ 3, 4, 1, 10 };
	Expression e(vars.begin(), vars.end(), "x^2+y^3-a/b^2+4e+2-range(x, y, a, b)");

	ASSERT_TRUE(abs(e.calculate(vals.begin(), vals.end()) - 466.665) < 1e-8);
}

TEST(calculator, test5)
{
	std::vector<std::string> vars{ "x", "y", "a", "b" };
	std::vector<double> vals{ 3, 4, 1, 10 };
	Expression e(vars.begin(), vars.end(), "range(x, y, a, b)");

	ASSERT_TRUE(abs(e.calculate(vals.begin(), vals.end()) - 6.32456) < 1e-8);
}

TEST(calculator, test6)
{
	std::vector<std::string> vars;
	std::vector<double> vals;


	ASSERT_ANY_THROW(
		Expression e(vars.begin(), vars.end(), "1+(5+4");
	e.calculate(vals.begin(), vals.end());
	);
}

TEST(calculator, test6)
{
	std::vector<std::string> vars;
	std::vector<double> vals;


	ASSERT_ANY_THROW(
		Expression e(vars.begin(), vars.end(), "1+(5+4+sin(3)");
	e.calculate(vals.begin(), vals.end());
	);
}
