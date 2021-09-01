#pragma once
#include "../Collection/ICollection.h"
#include "../Collection/IIterable.h"
#include "Vector.h"
#include <stdexcept>

namespace Structs
{
	template <typename T>
	class Stack final : public IIterable<T, VectorIterator<T>>, public ICollection
	{
	public:
		using Iterator = VectorIterator<T>;

	public:
		Stack()
			: elements(nullptr), size(0), capacity(0)
		{
			ReAlloc(5);
		}

		Stack(const Stack& vector) = delete;
		Stack& operator=(const Stack& vector) = delete;

		Stack(Stack&& vector) noexcept
			:
			elements(std::move(vector.elements)),
			capacity(vector.capacity),
			size(vector.size)
		{
			vector.elements = nullptr;
			vector.capacity = 0;
			vector.size = 0;
		}

		Stack& operator=(Stack&& vector) noexcept
		{
			elements = std::move(vector.elements);
			capacity = vector.capacity;
			size = vector.size;

			vector.elements = nullptr;
			vector.capacity = 0;
			vector.size = 0;
		}

		~Stack()
		{
			if (elements == nullptr)
				return;

			delete[] elements;

			elements = nullptr;
			capacity = 0;
			size = 0;
		}

	public:
		void Push(const T& value)
		{
			if (size >= capacity)
			{
				ReAlloc();
			}

			elements[size] = value;
			++size;
		}

		T& Peek()
		{
			if (IsEmpty())
			{
				throw std::out_of_range("Stack is empty");
			}

			return elements[size - 1];
		}

		T& Pop()
		{
			if (IsEmpty())
			{
				throw std::out_of_range("Stack is empty");
			}

			T temp = elements[size - 1];
			--size;

			return temp;
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
		void ReAlloc()
		{
			size_t newCapacity = capacity * 2;
			ReAlloc(newCapacity);
		}

		void ReAlloc(size_t newCapacity)
		{
			T* newElements = new T[newCapacity];

			if (newCapacity < size)
			{
				size = newCapacity;
			}

			if (elements != nullptr)
			{
				std::copy(&elements[0], &elements[size], newElements);
				delete[] elements;
			}

			elements = newElements;
			capacity = newCapacity;
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
		T* elements;
	};
}
