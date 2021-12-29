#pragma once

#include <cinttypes>
#include <string>
#include <functional>
#include <any>
#include <variant>
#include <array>
#include <tuple>
#include <set>

#include "operations_table.h"
#include "parser_functions.h"
#include "../containers/include/my_any.h"


struct Token
{
	enum class lexem_tag { integer, real, var, function, void_lexem, binary_operator, special_operator };
	
	Token() = delete;
	
	Token(lexem_tag tag, my_any arg)
		: lexem_type(tag)
	{
		switch (tag)
		{
		case lexem_tag::binary_operator:
			new (&this->bin_op) char(my_any_cast<char>(arg));

			break;
		case lexem_tag::special_operator:
			new (&this->special_op) char(my_any_cast<char>(arg));

			break;
		case lexem_tag::integer:
			new (&this->integer_num) int64_t(my_any_cast<int64_t>(arg));
			
			break;
		case lexem_tag::real:
			new (&this->real_num) double(my_any_cast<double>(arg));

			break;
		case lexem_tag::var:
			new (&this->variable) std::string(my_any_cast<std::string>(arg));

			break;
		case lexem_tag::function:
			new (&this->func) decltype(func)(my_any_cast<decltype(func)>(arg));

			break;
		default:
			throw std::exception("unknown lexem");

			break;
		}
	}
	
	Token(const Token& other)
	{
		this->lexem_type = other.lexem_type;
		switch (lexem_type)
		{
		case lexem_tag::binary_operator:
			new (&this->bin_op) char(other.bin_op);

			break;
		case lexem_tag::special_operator:
			new (&this->special_op) char(other.special_op);

			break;
		case lexem_tag::integer:
			new (&this->integer_num) int64_t(other.integer_num);

			break;
		case lexem_tag::real:
			new (&this->real_num) double(other.real_num);

			break;
		case lexem_tag::var:
			new (&this->variable) std::string(other.variable);

			break;
		case lexem_tag::function:
			new (&this->func) decltype(func)(other.func);

			break;
		}
	}

	Token& operator=(const Token& other)
	{
		if (this == &other) return *this;

		this->lexem_type = other.lexem_type;
		switch (lexem_type)
		{
		case lexem_tag::binary_operator:
			this->bin_op = other.bin_op;

			break;
		case lexem_tag::special_operator:
			this->special_op = other.special_op;

			break;
		case lexem_tag::integer:
			this->integer_num = other.integer_num;

			break;
		case lexem_tag::real:
			this->real_num = other.real_num;

			break;
		case lexem_tag::var:
			this->variable = other.variable;

			break;
		case lexem_tag::function:
			this->func = other.func;

			break;
		}
	}

	~Token()
	{
		if (lexem_type == lexem_tag::var) {
			this->variable.~basic_string();
		}
	}

	lexem_tag get_tag() const noexcept;
	std::pair<lexem_tag, std::any> value() const noexcept;

	static Token parse_lexem(std::string_view& expr, const std::vector<std::string>& vars);

private:
	
	lexem_tag lexem_type;

	
	union
	{
		int64_t integer_num;
		double real_num;
		char special_op;
		char bin_op;
		std::string variable;
		std::tuple<std::any, int, int> func;
	};
};

