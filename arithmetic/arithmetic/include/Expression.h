#pragma once

#include "Lexem.h"
#include <vector>
#include <string>
#include <string_view>
#include <initializer_list>
#include <iterator>
#include <type_traits>
#include <iterator>
#include <type_traits>
#include <map>

#include "../../containers/include/my_stack.h"


class Expression
{
public:
	template <typename InputIt>
	Expression(InputIt first, InputIt last, const std::string& _expression)
		: variables(first, last)
	{
		this->expr = this->_convert_postfix(this->_run_parser(_expression));
	}


	template <typename InputItF, typename InputItS> 
	double calculate(InputItF first, InputItS last) const
	{
		std::map<std::string, double> vars;

		size_t i = 0;
		while (first != last)
		{
			vars[this->variables[i]] = *first;

			++i;
			++first;
		}



		my_stack<Token, std::vector<Token>> funcs;
		std::vector<double> values;

		for (size_t i = 0; i < expr.size(); i++)
		{
			auto tmp = expr[i].value().second;
			if (expr[i].get_tag() == Token::lexem_tag::var) {
				values.push_back(vars[std::any_cast<std::string>(tmp)]);
			}
			else if (expr[i].get_tag() == Token::lexem_tag::real) {
				values.push_back(std::any_cast<double>(tmp));
			}
			else {
				if (values.size() < std::get<2>(std::any_cast<std::tuple<std::any, int, int>>(tmp))) {
					throw std::exception("expression is incorrect");
				}

				double res = apply_double_function(std::get<0>(std::any_cast<std::tuple<std::any, int, int>>(tmp)),
					std::get<2>(std::any_cast<std::tuple<std::any, int, int>>(tmp)),
					values.end() - std::get<2>(std::any_cast<std::tuple<std::any, int, int>>(tmp)));

				values.resize(values.size() - std::get<2>(std::any_cast<std::tuple<std::any, int, int>>(tmp)));
				values.push_back(res);
			}
		}

		if (values.size() != 1 || !funcs.empty()) {
			throw std::exception("expression is incorrect");
		}

		return values.front();
	}

private:
	std::vector<Token> _run_parser(std::string_view);
	std::vector<Token> _convert_postfix(std::vector<Token>);

	std::vector<Token> expr;
	std::vector<std::string> variables;
};




#include <iostream>
inline void print_expression(const std::vector<Token>& expr)
{
	for (auto& elem : expr)
	{
		auto tmp = elem.value();
		if (tmp.first == Token::lexem_tag::real) {
			std::cout << std::any_cast<double>(tmp.second);
		}
		else if (tmp.first == Token::lexem_tag::var) {
			std::cout << std::any_cast<std::string>(tmp.second);
		}
		else if (tmp.first == Token::lexem_tag::binary_operator || tmp.first == Token::lexem_tag::special_operator) {
			std::cout << std::any_cast<char>(tmp.second);
		}
		else if (tmp.first == Token::lexem_tag::function) {
			std::cout << "func:";
		}
	}

	std::cout << '\n';
}

inline std::vector<Token> Expression::_convert_postfix(std::vector<Token> expr)
{
	std::vector<Token> result;
	my_stack<Token, std::vector<Token>> st;


	for (auto& elem : expr)
	{
		if (elem.get_tag() == Token::lexem_tag::var ||
			elem.get_tag() == Token::lexem_tag::real) {
			result.push_back(elem);
		}
		else if (elem.get_tag() == Token::lexem_tag::special_operator) {
			if (std::any_cast<char>(elem.value().second) == ')') {
				while (st.top().get_tag() != Token::lexem_tag::special_operator ||
					std::any_cast<char>(st.top().value().second) != '(')
				{
					result.push_back(st.top());
					st.pop();
				}
				st.pop();
				continue;
			}
			if (std::any_cast<char>(elem.value().second) == ',') {
				while (st.top().get_tag() != Token::lexem_tag::special_operator ||
					std::any_cast<char>(st.top().value().second) != '(')
				{
					result.push_back(st.top());
					st.pop();
				}
				continue;
			}
			st.push(elem);
		}
		else if (elem.get_tag() == Token::lexem_tag::binary_operator) {
			auto tmp = find_in_table_by_name(std::string_view(std::string(1, std::any_cast<char>(elem.value().second))), functions_parser_table);
			while (!st.empty() && st.top().get_tag() != Token::lexem_tag::special_operator && 
				std::get<1>(std::any_cast<std::tuple<std::any, int, int>>(st.top().value().second)) <= std::get<1>(tmp))
			{
				result.push_back(st.top());
				st.pop();
			}
			st.push(Token(Token::lexem_tag::function, tmp));
		}
		else if (elem.get_tag() == Token::lexem_tag::function) {
			while (!st.empty() && st.top().get_tag() != Token::lexem_tag::special_operator &&
				std::get<1>(std::any_cast<std::tuple<std::any, int, int>>(st.top().value().second)) <= 
				std::get<1>(std::any_cast<std::tuple<std::any, int, int>>(elem.value().second)))
			{
				result.push_back(st.top());
				st.pop();
			}
			st.push(elem);
		}
	}
	while (!st.empty())
	{
		result.push_back(st.top());
		st.pop();
	}

	return result;
}

inline std::vector<Token> Expression::_run_parser(std::string_view _expr)
{
	my_stack<Token, std::vector<Token>> operations;

	auto str_start = _expr.begin();

	bool last_is_operation = false;
	bool last_is_operand = false;
	bool last_is_value = false;

	int deep = 0;

	std::vector<Token>expression;

	while (!_expr.empty())
	{
		auto str_start_parse = _expr.begin();

		auto token = Token::parse_lexem(_expr, this->variables);

		auto tmp = token.value();
		if (tmp.first == Token::lexem_tag::var) {

			if (last_is_value) {
				expression.push_back(Token(Token::lexem_tag::binary_operator, '*'));
			}

			if (!expression.empty() && expression.back().value().first == Token::lexem_tag::special_operator 
				&& std::any_cast<char>(expression.back().value().second) == ')') {

				expression.push_back(Token(Token::lexem_tag::binary_operator, '*'));
			}

			last_is_value = true;

			expression.push_back(token);
			continue;
		}
		if (tmp.first == Token::lexem_tag::real) {

			if (last_is_value) {
				throw std::exception((std::string("unexpected value at ") + std::to_string(str_start_parse - str_start)).c_str());
			}

			if (!expression.empty() && expression.back().get_tag() == Token::lexem_tag::special_operator
				&& std::any_cast<char>(expression.back().value().second) == ')') {

				expression.push_back(Token(Token::lexem_tag::binary_operator, '*'));
			}

			last_is_value = true;

			expression.push_back(token);
			continue;
		}


		if (tmp.first == Token::lexem_tag::special_operator) {
			if (std::any_cast<char>(tmp.second) == ')') {
				if (deep == 0) {
					throw std::exception((std::string("unexpected ')' at ") + std::to_string(str_start_parse - str_start)).c_str());
				}

				deep--;
			}
			else if (std::any_cast<char>(tmp.second) == '(') {
				if (last_is_value) {
					expression.push_back(Token(Token::lexem_tag::binary_operator, '*'));
				}
				deep++;
			}
			else if (std::any_cast<char>(tmp.second) == '-') {
				if (!last_is_value) {
					token = Token(Token::lexem_tag::function, find_in_table_by_name("@", functions_parser_table));
				}
				else {
					token = Token(Token::lexem_tag::binary_operator, '-');
				}
			}

			expression.push_back(token);
		}
		else if (tmp.first == Token::lexem_tag::binary_operator) {
			expression.push_back(token);
		}
		else if (tmp.first == Token::lexem_tag::function) {
			// count arg
			if (last_is_value) {
				expression.push_back(Token(Token::lexem_tag::binary_operator, '*'));
			}
			expression.push_back(token);
		}
		last_is_value = false;
	}

	if (deep != 0) {
		throw std::exception("expected )");
	}

	return expression;
}

