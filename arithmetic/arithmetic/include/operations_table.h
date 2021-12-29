#pragma once

#include <any>
#include <tuple>
#include <string_view>
#include <utility>
#include <functional>
#include <exception>
#include <type_traits>
#include <iterator>
#include <cstdarg>


template <int I, typename TupleLike>
std::tuple<std::any, int, int> _find_in_table_by_name_impl(std::string_view name, const TupleLike& table) noexcept
{
	if constexpr (I < 0) {
		return { 0, 0, -1 };
	}
	else {
		if (std::get<0>(std::get<I>(table)) == name) {
			return { std::function(std::get<1>(std::get<I>(table))), std::get<2>(std::get<I>(table)), std::get<3>(std::get<I>(table)) };
		}
		else {
			return _find_in_table_by_name_impl<I - 1, TupleLike>(name, table);
		}
	}
}

template <typename TupleLike>
std::tuple<std::any, int, int> find_in_table_by_name(std::string_view name, const TupleLike& table) noexcept
{
	return _find_in_table_by_name_impl<int(std::tuple_size_v<TupleLike>) - 1, TupleLike>(name, table);
}



template <typename... Types>
std::enable_if_t<std::conjunction_v<std::is_same<double, Types>...>, double>
apply_double_function(std::any func, size_t arg_cnt, Types... args)
{
	return std::any_cast<std::function<double(Types...)>>(func)(args...);
}

inline constexpr size_t max_double_function_number_of_arguments = 20;

template <typename InputIt, typename... Types>
std::enable_if_t<std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>, double> 
apply_double_function(std::any func, size_t arg_cnt, InputIt it, Types... args)
{
	if constexpr (sizeof...(args) < max_double_function_number_of_arguments) {
		if (sizeof...(args) == arg_cnt) {
			return apply_double_function(func, arg_cnt, args...);
		}
		else {
			double tmp = *it;
			++it;
			return apply_double_function(std::move(func), arg_cnt, it, args..., tmp);
		}
	}

	return 0;
}


inline constexpr std::tuple functions_parser_table {
	std::tuple{std::string_view(","), [](double, double) {}, 10, 2}
	, std::tuple{std::string_view("+"), [](double l, double r) { return l + r; } , 3, 2}
	, std::tuple{std::string_view("-"), [](double l, double r) { return l - r; }, 3, 2}
	, std::tuple{std::string_view("*"), [](double l, double r) { return l * r; }, 2, 2}
	, std::tuple{std::string_view("/"), [](double l, double r) { return l / r; }, 2, 2}
	, std::tuple{std::string_view("^"), [](double l, double r) { return pow(l, r); }, 1, 2}
	, std::tuple{std::string_view("@"), [](double v) { return -v; }, 1, 1}
	, std::tuple{std::string_view("sin"), [](double v) { return sin(v); }, 1, 1}
	, std::tuple{std::string_view("cos"), [](double v) { return cos(v); }, 1, 1}
	, std::tuple{std::string_view("ln"), [](double v) { return log(v); }, 1, 1}
	, std::tuple{std::string_view("range"), [](double x1, double y1, double x2, double y2) { return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)); }, 1, 4}
	, std::tuple{std::string_view("scale"), [](double l1, double r1, double l2, double r2, double x) { return (x - l1) / (r1 - l1) * (r2 - l2) + l2; }, 1, 5}
};

inline constexpr const char special_operators[] = { '(', ')', ',', '-'};
inline constexpr const char operators[] = { '+', '-', '*', '/', '^' };

