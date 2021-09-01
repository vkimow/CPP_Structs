#pragma once
#include "../Collection/ICollection.h"
#include "../Collection/IIterable.h"
#include <stdexcept>

namespace Structs
{
	template <typename T>
	class QueueIterator final : public IIterator<T, QueueIterator<T>>
	{
	public:
	public:
		QueueIterator() = delete;
		QueueIterator(T* element, T* arrayBegin, T* arrayEnd)
			: element(element),
			arrayBegin(arrayBegin),
			arrayEnd(arrayEnd)
		{}

		QueueIterator& operator++() override
		{
			++element;

			if (element == arrayEnd)
			{
				element = arrayBegin + 1;
			}

			return *this;
		}

		virtual QueueIterator& operator++(int) override
		{
			QueueIterator temp = *this;
			++(*this);
			return temp;
		}

		virtual QueueIterator& operator--()
		{
			--element;

			if (element == arrayBegin)
			{
				element = arrayEnd - 1;
			}

			return *this;
		}

		virtual QueueIterator& operator--(int)
		{
			QueueIterator temp = *this;
			--(*this);
			return temp;
		}

		virtual bool operator==(const QueueIterator& rhs) const override
		{
			return element == rhs.element;
		}

		virtual bool operator!=(const QueueIterator& rhs) const override
		{
			return !(*this == rhs);
		}

		virtual T* operator->() const override
		{
			return element;
		}

		virtual T& operator*() const override
		{
			return *element;
		}

	private:
		T* element;
		T* arrayBegin;
		T* arrayEnd;
	};

	template <typename T>
	class Queue final : public IIterable<T, QueueIterator<T>>, ICollection
	{
	public:
		using Iterator = QueueIterator<T>;

	public:
		Queue()
			:
			elements(nullptr),
			size(0),
			capacity(0),
			firstElementIndex(0),
			lastElementIndex(0)
		{
			ReAlloc(5);
		}

		Queue(const Queue& queue) = delete;
		Queue& operator=(const Queue& queue) = delete;

		Queue(Queue&& queue)
			:
			elements(std::move(queue.elements)),
			size(queue.size),
			capacity(queue.capacity),
			firstElementIndex(queue.firstElementIndex),
			lastElementIndex(queue.lastElementIndex)
		{
			queue.elements = nullptr;
			queue.size = 0;
			queue.capacity = 0;
			queue.firstElementIndex = 0;
			queue.lastElementIndex = 0;
		}

		Queue& operator=(Queue&& queue)
		{
			elements = std::move(queue.elements);
			size = queue.size;
			capacity = queue.capacity;
			firstElementIndex = queue.firstElementIndex;
			lastElementIndex = queue.lastElementIndex;

			queue.elements = nullptr;
			queue.size = 0;
			queue.capacity = 0;
			queue.firstElementIndex = 0;
			queue.lastElementIndex = 0;
		}

		~Queue()
		{
			if (elements == nullptr)
			{
				return;
			}

			delete[] elements;

			size = 0;
			capacity = 0;
			firstElementIndex = 0;
			lastElementIndex = 0;
		}

	public:
		void Enqueue(const T& value)
		{
			if (size >= capacity)
			{
				ReAlloc();
			}

			if (!IsEmpty())
			{
				++lastElementIndex;

				if (lastElementIndex >= capacity)
				{
					lastElementIndex -= capacity;
				}
			}

			elements[lastElementIndex] = value;
			++size;
		}

		T& Dequeue()
		{
			if (IsEmpty())
			{
				throw std::out_of_range("");
			}

			T& temp = elements[firstElementIndex];
			++firstElementIndex;

			if (firstElementIndex >= capacity)
			{
				firstElementIndex -= capacity;
			}

			--size;
			return temp;
		}

		T& Peek()
		{
			if (IsEmpty())
			{
				throw std::out_of_range("");
			}

			return elements[firstElementIndex];
		}

		bool Contains(const T& value)
		{
			size_t elementIndex = firstElementIndex;

			for (size_t i = 0; i < size; ++i)
			{
				if (elementIndex >= capacity)
				{
					elementIndex -= capacity;
				}
				
				if (elements[elementIndex] == value)
				{
					return true;
				}

				++elementIndex;
			}

			return false;
		}

		virtual void Clear() override
		{
			size = 0;
			firstElementIndex = 0;
			lastElementIndex = -1;
		}

	private:
		void AddElement(size_t index)
		{
			elements[index] = value;
			++size;
		}

		void ReAlloc()
		{
			size_t newCapacity = capacity * 2;
			ReAlloc(newCapacity);
		}

		void ReAlloc(size_t newCapacity)
		{
			T* newElements = new T[newCapacity];

			if (elements != nullptr)
			{
				size_t elementIndex = firstElementIndex;

				for (size_t i = 0; i < size; ++i)
				{
					if (elementIndex >= capacity)
					{
						elementIndex -= capacity;
					}

					newElements[i] = std::move(elements[elementIndex]);
					++elementIndex;
				}

				firstElementIndex = 0;
				lastElementIndex = size - 1;
				delete[] elements;
			}

			elements = newElements;
			capacity = newCapacity;
		}

	public:
		virtual Iterator begin() const override
		{
			return Iterator(&elements[firstElementIndex], elements - 1, elements + capacity);
		}

		virtual Iterator end() const override
		{
			return Iterator(&elements[lastElementIndex], elements - 1, elements + capacity);
		}

	public:
		virtual bool IsEmpty() const override { return size == 0; }
		virtual size_t GetSize() const override { return size; }

	private:
		T* elements;
		size_t size;
		size_t capacity;
		size_t firstElementIndex;
		size_t lastElementIndex;
	};
}
