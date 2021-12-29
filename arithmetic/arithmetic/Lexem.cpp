#include "Lexem.h"

#include <string_view>
#include <numeric>
#include <array>
#include <cstddef>
#include <cmath>
#include <algorithm>



Token Token::parse_lexem(std::string_view& expr, const std::vector<std::string>& vars)
{
	expr.remove_prefix(std::find_if_not(expr.begin(), expr.end(), std::isspace) - expr.begin());


	if (parse_special_operator(expr)) {
		char op = expr.front();
		expr.remove_prefix(1);

		return Token(lexem_tag::special_operator, op);
	}
	else if (parse_operator(expr)) {
		char op = expr.front();
		expr.remove_prefix(1);

		return Token(lexem_tag::binary_operator, op);
	}
	else {
		auto tmp = parse_number(expr);
		
		if (std::isnan(tmp)) {
			std::string name = parse_name(expr);

			if (find(vars.begin(), vars.end(), name) != vars.end()) {
				return Token(lexem_tag::var, name);
			}
			else {
				auto func = find_in_table_by_name(name, functions_parser_table);

				if (get<2>(func) == -1) {
					throw std::exception((std::string("unknown name ") + name).c_str());
				}

				return Token(lexem_tag::function, func);
			}

		}
		else {
			return Token(lexem_tag::real, tmp);
		}
	}
	
}


std::pair<Token::lexem_tag, std::any> Token::value() const noexcept
{
	std::any res;

	switch (this->lexem_type)
	{
	case lexem_tag::binary_operator:
		res = this->bin_op;

		break;
	case lexem_tag::special_operator:
		res = this->special_op;

		break;
	case lexem_tag::integer:
		res = this->integer_num;

		break;
	case lexem_tag::real:
		res = this->real_num;

		break;
	case lexem_tag::var:
		res = this->variable;

		break;
	case lexem_tag::function:
		res = this->func;

		break;
	}

	return { this->lexem_type, res };
}


Token::lexem_tag Token::get_tag() const noexcept
{
	return this->lexem_type;
}
