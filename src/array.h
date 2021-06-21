#ifndef _BC_DS_ARRAY_H
#define _BC_DS_ARRAY_H

#include <exception>
#include <cstddef>

namespace bc::ds
{
	//=============================
	// Array const iterator
	//=============================
	template <class T, size_t Size>
	class array_const_iterator
	{
	public:
		using value_type = T;
		using size_type = size_t;
		using diff_type = ptrdiff_t;

		using pointer = const T*;
		using reference = const T&;

	public: //API
		pointer operator->() const 
		{
			if (!ptr_) { throw std::exception("Underlying pointer can not be null."); }
			if (index_ >= Size) { throw std::exception("can not deference out of range."); }
			return ptr_ + index_;
		}

		reference operator*() const { return *operator->(); }
		
		// pre-fix increment
		array_const_iterator& operator++()
		{
			if (!ptr_) { throw std::exception("Underlying pointer can not be null."); }
			if (index_ >= Size) { throw std::exception("can not deference out of range."); }
			++index_;
			return *this;
		}

		// post-fix increment
		array_const_iterator operator++(int)
		{
			auto temp = *this;
			++*this;
			return temp;
		}

		// prefix decrement
		array_const_iterator& operator--()
		{
			if (!ptr_) { throw std::exception("Underlying pointer can not be null."); }
			if (index_ == 0) { throw std::exception("can not deference out of range."); }
			--index_;
			return *this;
		}
		// postfix decrement
		array_const_iterator operator--(int)
		{
			auto temp = *this;
			--* this;
			return temp;
		}

		//
		array_const_iterator& operator+=(const diff_type offset)
		{
			verify_offset(offset);
			index_ += offset;
			return *this;
		}

		array_const_iterator operator+(const diff_type offset) const
		{
			auto temp = *this;
			return temp += offset;
		}

		array_const_iterator& operator-=(const diff_type offset)
		{
			return *this += -offset;
		}

		array_const_iterator operator-(const diff_type offset) const
		{
			auto temp = *this;
			return temp -= offset;
		}

		diff_type operator-(const array_const_iterator& rhs) const
		{
			verify_compatibility(rhs.ptr_);
			return static_cast<diff_type>(index_ - rhs.index_);
		}

		//============
		// Access
		//============
		reference operator[](const ptrdiff_t offset) const
		{
			return *(*this + offset);
		}

		//============
		// Comparison
		//============
		bool operator==(const array_const_iterator& rhs) const
		{
			verify_compatibility(rhs.ptr_);
			return index_ == rhs.index_;
		}

		bool operator!=(const array_const_iterator& rhs) const
		{
			return !(*this == rhs);
		}

		bool operator<(const array_const_iterator& rhs) const
		{
			verify_compatibility(rhs.ptr_);
			return index_ < rhs.index_;
		}

		bool operator>(const array_const_iterator& rhs) const
		{
			return rhs < *this;
		}

		bool operator<=(const array_const_iterator& rhs) const
		{
			return !(rhs < *this);
		}

		bool operator>=(const array_const_iterator& rhs) const
		{
			return !(*this < rhs);
		}

	public:
		array_const_iterator() : ptr_(), index_(0) {}
		array_const_iterator(pointer ptr, size_t offset = 0) : ptr_(ptr), index_(offset) {}

	private:
		void verify_offset(const diff_type offset) const
		{
			if (!ptr_)
				throw std::exception("Underlying pointer can not be null.");

			if (offset < 0 && index_ < -static_cast<size_t>(offset))
				throw std::exception("can not reach an array iterator before begin");

			if (offset > 0 && Size - index_ > static_cast<size_t>(offset))
				throw std::exception("can not reach an array iterator after end.");
		}

		void verify_compatibility(value_type* ptr) const
		{
			if (ptr_ != ptr)
				throw std::exception("internal pointer types are different");
		}

	private:
		pointer ptr_;
		size_type index_;
	};

	template<class T, size_t Size>
	array_const_iterator<T, Size> operator+(const ptrdiff_t offset, array_const_iterator<T, Size> next)
	{
		return next += offset;
	}

	//=============================
	// Array iterator
	//=============================
	template<class T, size_t Size>
	class array_iterator : public array_const_iterator<T, Size>
	{
	public:
		using value_type = T;
		using size_type = size_t;
		using diff_type = ptrdiff_t;
		using pointer = T*;
		using reference = T&;
	private:
		using theBase = array_const_iterator<T, Size>;

	public: //API
		
		pointer operator->() const { return const_cast<pointer>(theBase::operator->()); }
		reference operator*() const { return const_cast<reference>(theBase::operator*()); }
		
		//=========
		// Increment - decrement
		//=========

		// prefix
		array_iterator& operator++()
		{
			theBase::operator++();
			return *this;
		}

		// postfix
		array_iterator operator++(int)
		{
			auto temp = *this;
			theBase::operator++();
			return temp;
		}

		// prefix
		array_iterator& operator--()
		{
			theBase::operator--();
			return *this;
		}

		//postfix
		array_iterator operator--(int)
		{
			auto temp = *this;
			theBase::operator--();
			return temp;
		}

		// =========
		// Unary increment / decrement
		// =========

		array_iterator& operator+=(const diff_type offset)
		{
			theBase::operator+=(offset);
			return *this;
		}

		array_iterator operator+(const diff_type offset)
		{
			auto temp = *this;
			return temp += offset;
		}

		array_iterator& operator-=(const diff_type offset)
		{
			theBase::operator-=(offset);
			return *this;
		}

		array_iterator operator-(const diff_type offset)
		{
			auto temp = *this;
			return temp -= offset;
		}

		// =========
		// Access
		// =========
		reference operator[](const diff_type index) const
		{
			return const_cast<reference>(theBase::operator[](index));
		}

	public:
		array_iterator() : theBase() {}
		array_iterator(pointer ptr, size_type offset = 0) : theBase(ptr, offset) {}	
	};

	template <class T, size_t Size>
	array_iterator<T, Size> operator+(const ptrdiff_t offset, array_iterator<T, Size> next)
	{
		return next += offset;
	}


	//=============================
	// Fixed-size array
	//=============================
	template <class T, size_t Size>
	struct array
	{
	public:
		using value_type = T;
		using size_type = size_t;
		using reference = T&;
		using const_reference = const T&;
		using pointer = T*;
		using const_pointer = const T*;

		using iterator = array_iterator<T, Size>;
		using const_iterator = array_const_iterator<T, Size>;

	public://API
		constexpr size_type size() const noexcept { return Size; }
		constexpr bool empty() const noexcept { return false; } // Array can not be empty

	public:
		const_iterator begin() const { return const_iterator(elements_, 0);	}
		iterator begin() { return iterator(elements_, 0);}

		const_iterator cbegin() const { return begin(); }
		const_iterator cend() const { return end(); }

		const_iterator end() const { return const_iterator(elements_, Size); }
		iterator end() { return iterator(elements_, Size); }

		reference at(size_type index) 
		{
			if (Size <= index)
				throw std::exception("index is out of range");

			return elements_[index]; 
		}

		const_reference at(size_type index) const 
		{
			if (Size <= index)
				throw std::exception("index is out of range");

			return elements_[index]; 
		}

		reference operator[](size_type index)
		{
			if (Size <= index)
				throw std::exception("index is out of range");

			return elements_[index];
		}

		const_reference operator[](size_type index) const
		{
			if (Size <= index)
				throw std::exception("index is out of range");

			return elements_[index];
		}

		reference front() { return elements_[0]; }
		const_reference front() const { return elements_[0]; }
		reference back() { return elements_[Size - 1]; }
		const_reference back() const { return elements_[Size - 1]; }

		value_type* data() { return elements_; }
		const value_type* data() const { return elements_; }


	public:
		T elements_[Size];
	};
}


#endif // !_BC_DS_ARRAY_H
