#pragma once

#include <utility>
#include <type_traits>

class my_any
{
public:
	template <typename T>
	my_any(T&& value)
	{
		this->data = new std::decay_t<T>(std::forward<T>(value));
	}

	template <typename T>
	friend T my_any_cast(const my_any& obj)
	{
		return *reinterpret_cast<T*>(obj.data);
	}


private:
	void* data;
};
