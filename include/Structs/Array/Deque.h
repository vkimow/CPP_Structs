#pragma once
#include "../Collection/ICollection.h"
#include "../Collection/IIterable.h"
#include <stdexcept>

namespace Structs
{
	template <typename T>
	class DequeIterator : IIterator<T, DequeIterator<T>>
	{
	};

	template <typename T>
	class DequeElement final
	{
	public:
		DequeElement()
			: elements(new T[COMMON_CAPACITY]),
			size(0),
			capacity(COMMON_CAPACITY),
			next(nullptr)
		{}

	public:
		void Add(const T& element)
		{
			elements[size] = element;
			++size;
		}

		void Insert(const T& element, size_t index)
		{
			for (size_t i = size; i > index; --i)
			{
				elements[i] = std::move(elements[i - 1]);
			}

			elements[index] = element;
			++size;
		}

		void Remove(const T& element)
		{
			size_t index = 0;

			for (; index < size; ++index)
			{
				if (elements[index] == element)
				{
					RemoveAt(index);
					return;
				}
			}

			throw std::out_of_range("Value not in range");
		}

		void RemoveAt(size_t index)
		{
			for (size_t i = index; i < size - 1; ++i)
			{
				elements[i] = std::move(elements[i + 1]);
			}

			--size;
		}

	public:
		T& At(size_t index)
		{
			if (index < 0 || index >= size)
			{
				throw std::out_of_range("Index not in range");
			}

			return elements[index];
		}

		T& operator[](size_t index)
		{
			return elements[index];
		}

		bool IsFull() const { return size == capacity; }
		bool IsEmpty() const { return size == 0; }
		bool HasNext() const { return next != nullptr; }
		void SetNext(DequeElement* element) { return next = element; }
		size_t GetSize() const { return size; }

	public:
		T* elements;
		size_t size;
		size_t capacity;
		DequeElement* next;

	private:
		static const size_t COMMON_CAPACITY;
	};

	template <typename T>
	class Deque final : public IIterable<T, DequeIterator<T>>, public ICollection
	{
	public:
		using Iterator = DequeIterator<T>;

	public:
		Deque()
			: elements(nullptr), size(0), capacity(0)
		{
			ReAlloc(5);
		}

		Deque(const Deque& vector) = delete;
		Deque& operator=(const Deque& vector) = delete;

		Deque(Deque&& vector) noexcept
			:
			elements(std::move(vector.elements)),
			capacity(vector.capacity),
			size(vector.size)
		{
			vector.elements = nullptr;
			vector.capacity = 0;
			vector.size = 0;
		}

		Deque& operator=(Deque&& vector) noexcept
		{
			elements = std::move(vector.elements);
			capacity = vector.capacity;
			size = vector.size;

			vector.elements = nullptr;
			vector.capacity = 0;
			vector.size = 0;
		}

		~Deque()
		{
			if (elements == nullptr)
				return;

			delete[] elements;

			elements = nullptr;
			capacity = 0;
			size = 0;
		}

	public:
		void Add(const T& value)
		{
			if (size >= capacity)
			{
				ReAlloc();
			}

			elements[size] = value;
			++size;
		}

		void Insert(const T& value, size_t index)
		{
			if (index < 0 || index > size)
			{
				throw std::out_of_range(std::to_string(index));
			}

			if (size >= capacity)
			{
				ReAlloc();
			}

			for (size_t i = size - 1; i >= index; --i)
			{
				elements[i + 1] = std::move(elements[i]);
			}

			elements[index] = value;
			++size;
		}

		void Remove(const T& value)
		{
			for (size_t i = 0; i < size; ++i)
			{
				if (elements[i] == value)
				{
					RemoveAt(i);
					return;
				}
			}

			throw std::out_of_range("Value not in range");
		}

		void RemoveAt(size_t index)
		{
			if (index < 0 || index >= size)
			{
				throw std::out_of_range(std::to_string(index));
			}

			for (size_t i = index; i < size - 1; ++i)
			{
				elements[i] = std::move(elements[i + 1]);
			}

			--size;
		}

		bool Contains(const T& value)
		{
			for (size_t i = 0; i < size; ++i)
			{
				if (elements[i] == value)
				{
					return true;
				}
			}

			return false;
		}

		virtual void Clear() override
		{
			size = 0;
		}

	private:

	public:
		T& operator[](size_t index)
		{
			return elements[index];
		}

	public:
		virtual size_t GetSize() const override { return size; }
		virtual bool IsEmpty() const override { return size == 0; }

	public:
		virtual Iterator begin() const override
		{
			return Iterator(elements);
		};

		virtual Iterator end() const override
		{
			return Iterator(&elements[size]);
		};

	private:
		size_t size;
		size_t capacity;
		DequeElements* elements;
	};
}
