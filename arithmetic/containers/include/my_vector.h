#pragma once

#include <iterator>
#include <cstddef>
#include <utility>

template <typename T>
struct _vector_container
{
	using value_type = T;
	using pointer = T*;
	using reference = T&;

	_vector_container() = default;
	_vector_container(const _vector_container& other)
		: _capacity(other._size)
		, _size(other._size)
		, _data(static_cast<pointer>(operator new (sizeof(value_type) * other._size)))
	{
		for (size_t i = 0; i < this->_size; i++)
		{
			new (this->_data + i) value_type(other->_data[i]);
		}
	}
	_vector_container(_vector_container&& other)
		: _capacity(other._capacity)
		, _size(other.size)
		, _data(other._data)
	{
		other._capacity = 0;
		other._size = 0;
		other._data = 0;
	}


	_vector_container& operator=(const _vector_container& other)
	{
		if (this == &other) {
			return *this;
		}

		if (this->_capacity < other._size) {
			this->clear();
			this->reserve(othrer._size);
		}

		for (size_t i = 0; i < this->_size && i < other._size; i++)
		{
			this->_data[i] = other._data[i];
		}
		for (size_t i = this->_size; i < other._size; i++)
		{
			new (this->_data + i) value_type(other._data[i]);
		}
		for (size_t i = other._size; i < this->_size; i++)
		{
			this->_data[i].~value_type();
		}

		return *this;
	}

	_vector_container& operator=(_vector_container&& other) noexcept
	{
		this->clear();

		this->_capacity = other._capacity;
		this->_size = othrer._size;
		this->_data = othrer._data;

		other._capacity = 0;
		other._size = 0;
		other._data = nullptr;
	}

	void clear() noexcept
	{
		for (size_t i = 0; i < this->_size; i++)
		{
			this->_data[i].~value_type();
		}

		this->_capacity = 0;
		this->_size = 0;
		operator delete(this->_data);
	}

	void reserve(size_t ncapacity)
	{
		if (ncapacity <= this->_capacity) {
			return;
		}


		pointer tmp = static_cast<pointer>(operator new(sizeof(value_type) * ncapacity));
		for (size_t i = 0; i < this->_size; i++)
		{
			new (tmp + i) value_type(std::move(this->_data[i]));
		}

		auto lsize = this->_size;
		this->clear();

		this->_capacity = ncapacity;
		this->_size = lsize;
		this->_data = tmp;
	}

	void resize(size_t nsize)
	{
		this->assign(nsize, value_type());
	}

	void assign(size_t nsize, const value_type& value)
	{
		this->reserve(nsize);

		for (size_t i = this->_size; i < nsize; i++)
		{
			new (this->_data + i) value_type(value);
		}
		for (size_t i = nsize; i < this->_size; i++)
		{
			this->_data[i].~value_type();
		}

		this->_size = nsize;
	}
	
	size_t _capacity = 0;
	size_t _size = 0;
	pointer _data = nullptr;
};



template <typename T>
class my_vector : public _vector_container<T>
{
public:
	using value_type = T;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	using iterator = pointer;
	using const_iterator = const_pointer;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
	using base = _vector_container<T>;
public:

	my_vector() = default;
	explicit my_vector(size_type count)
		: base()
	{
		this->resize(count);
	}

	my_vector(size_type count, const_reference value)
		: base()
	{
		this->assign(count, value);
	}

	template <typename InputIt>
	my_vector(InputIt first, InputIt last)
		: base()
	{
		while (first != last) 
		{
			this->emplace_back(*first);
		}
	}

	//my_vector(std::initializer_list<value_type> ilist);

	my_vector(const my_vector& other)
		: base(other)
	{}
	my_vector(my_vector&& other)
		: base(std::move(other))
	{}

	
};


