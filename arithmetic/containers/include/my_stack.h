#pragma once

#include <cstddef>
#include <utility>

#include <stack>

template <typename T, typename Container>
class my_stack
{
public:
	using container_type = Container;
	using value_type = T;
	using size_type = std::size_t;
	using reference = T&;
	using const_reference = const T&;

	my_stack() = default;
	my_stack(const my_stack&) = default;
	my_stack(my_stack&&) = default;

	my_stack(const container_type& cont)
		: _data(cont)
	{}
	my_stack(container_type&& cont)
		: _data(std::move(cont))
	{}

	template <typename InputIt>
	my_stack(InputIt first, InputIt last)
		: _data(first, last)
	{}

	my_stack& operator=(const my_stack&) = default;
	my_stack& operator=(my_stack&&) = default;

	reference top() noexcept
	{
		return _data.back();
	}

	const_reference top() const noexcept
	{
		return _data.back();
	}

	bool empty() const noexcept
	{
		return _data.empty();
	}

	size_type size() const noexcept
	{
		return _data.size();
	}



	void push(const value_type& val)
	{
		this->_data.push_back(val);
	}
	void push(value_type&& val)
	{
		this->_data.push_back(std::move(val));
	}

	template <typename... types>
	void emplace(types&&... args)
	{
		this->_data.emplace(std::forward<types>(args)...);
	}

	void pop()
	{
		this->_data.pop_back();
	}

	bool operator==(const my_stack& other) const noexcept(noexcept(this->_data == other._data))
	{
		return this->_data == other._data;
	}

private:
	container_type _data;
};

