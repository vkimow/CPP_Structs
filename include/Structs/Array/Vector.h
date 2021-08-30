#pragma once
#include "../Collection/ICollection.h"
#include "../Collection/IIterable.h"
#include <stdexcept>

namespace Structs
{
	template <typename T>
	class VectorIterator : IIterator<T, VectorIterator<T>>
	{
	public:
		VectorIterator() = delete;
		VectorIterator(T* element)
			: element(element)
		{}

		VectorIterator& operator++() override
		{
			++element;
			return *this;
		}

		VectorIterator& operator++(int) override
		{
			VectorIterator temp = *this;
			++(*this);
			return temp;
		}

		VectorIterator& operator--()
		{
			--element;
			return *this;
		}

		VectorIterator& operator--(int)
		{
			VectorIterator temp = this;
			--(*this);
			return temp;
		}

		bool operator==(const VectorIterator& rhs) const override
		{
			return element == rhs.element;
		}

		bool operator!=(const VectorIterator& rhs) const override
		{
			return !(*this == rhs);
		}

		T* operator->() const override
		{
			return element;
		}

		T& operator*() const override
		{
			return *element;
		}

	private:
		T* element;
	};

	template <typename T>
	class Vector final : public IIterable<T, VectorIterator<T>>, public ICollection
	{
	public:
		using Iterator = VectorIterator<T>;

	public:
		Vector()
			: elements(nullptr), size(0), capacity(0)
		{
			ReAlloc(5);
		}

		Vector(const Vector& vector) = delete;
		Vector& operator=(const Vector& vector) = delete;

		Vector(Vector&& vector) noexcept
			:
			elements(std::move(vector.elements)),
			capacity(vector.capacity),
			size(vector.size)
		{
			vector.elements = nullptr;
			vector.capacity = 0;
			vector.size = 0;
		}

		Vector& operator=(Vector&& vector) noexcept
		{
			elements = std::move(vector.elements);
			capacity = vector.capacity;
			size = vector.size;

			vector.elements = nullptr;
			vector.capacity = 0;
			vector.size = 0;
		}

		~Vector()
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
		T* elements;
	};
}
