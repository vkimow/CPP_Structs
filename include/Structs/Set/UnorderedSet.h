#pragma once
#include "ISet.h"
#include <xhash>
#include <stdexcept>

namespace Structs
{
	template<typename T>
	class UnorderedSetElement
	{
	public:
		UnorderedSetElement()
			: value(), hash(0), bucketIndex(0), nextIndex(0), hasValue(false), hasNext(false)
		{}

	public:
		void SetValue(const T& value, size_T hash) { this->value = value; this->hash = hash; hasValue = true }
		void ResetValue() { hasValue = false; }
		const T& GetValue() const { return value; }

		size_t GetHash() const { return hash; }

		void SetNextIndex(size_t value) { nextIndex = value; hasNext = true; }
		void ResetNextIndex() { nextIndex = 0; hasNext = false; }
		size_t GetNextIndex() const { return nextIndex; }

		bool HasValue() const { return hasValue; }
		bool HasNext() const { return hasNext; }

	private:
		T value;
		size_t hash;
		size_t nextIndex;
		bool hasValue;
		bool hasNext;
	};

	template<typename T>
	bool operator==(UnorderedSetElement<T> lhs, const T& rhs)
	{
		return lhs.HasValue() && lhs.GetValue() == rhs;
	}

	template<typename T>
	bool operator==(const T& lhs, UnorderedSetElement<T> rhs)
	{
		return rhs.HasValue() && lhs == rhs.GetValue();
	}

	template<typename T>
	bool operator!=(UnorderedSetElement<T> lhs, const T& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename T>
	bool operator!=(const T& lhs, UnorderedSetElement<T> rhs)
	{
		return !(lhs == rhs);
	}

	template <typename T>
	class UnorderedSetIterator : IIterator<T, UnorderedSetIterator<T>>
	{
	private:
		using Element = UnorderedSetElement<T>;
		using Iterator = UnorderedSetIterator;

	public:
		Iterator()
			: Iterator(nullptr, true)
		{}

		Iterator(const Element* element)
			: Iterator(element, false)
		{}

		Iterator(const Element* element, bool isInvalid)
			: currentElement(element), isInvalid(isInvalid)
		{}

		virtual Iterator& operator++() override
		{
			if (isInvalid)
			{
				return *this;
			}

			do
			{
				++currentElement;
			} while (!currentElement->HasValue());

			return *this;
		}

		virtual Iterator& operator++(int) override
		{
			Iterator temp = *this;
			++(*this);
			return temp;
		}

		Iterator& operator--()
		{
			if (isInvalid)
			{
				return *this;
			}

			{
				--currentElement;
			}
			while (!currentElement->HasValue());

			return *this;
		}

		Iterator& operator--(int)
		{
			Iterator temp = *this;
			--(*this);
			return temp;
		}

		virtual bool operator==(const Iterator& rhs) const override
		{
			if (isInvalid)
			{
				return true;
			}

			isInvalid = currentElement == rhs.currentElement;
			return false;
		}

		virtual bool operator!=(const Iterator& rhs) const override
		{
			return !(this == rhs);
		}

		virtual T& operator*() const override
		{
			if (isInvalid)
			{
				throw std::out_of_range("Can't take value from invalid iterator");
			}

			return *currentElement->GetValue();
		}

		virtual T* operator->() const override
		{
			if (isInvalid)
			{
				throw std::out_of_range("Can't take value from invalid iterator");
			}

			return &(*currentElement->GetValue());
		}

	private:
		const Element* currentElement;
		bool isInvalid;
	};

	template<typename T>
	class BucketElement
	{
	public:
		BucketElement()
			: elementIndex(0), hasElement(false)
		{}

	public:
		void SetElementIndex(size_t value) { elementIndex = value; hasElement = true; }
		void ResetElementIndex() { elementIndex = 0; hasElement = false; }
		size_t GetElementIndex() const { return elementIndex; }

		bool HasElement() const { return hasElement; }

	private:
		size_t elementIndex;
		bool hasElement;
	};


	template <typename T, typename Hasher = std::hash<T>>
	class UnorderedSet final : public ISet<T, UnorderedSetIterator<T>>
	{
	public:
		using Iterator = UnorderedSetIterator<T>;
		using Element = UnorderedSetElement<T>;
		using Bucket = BucketElement<T>;

	public:
		UnorderedSet()
			: hasher(), elements(nullptr), buckets(nullptr), size(0), capacity(0)
		{}

		UnorderedSet(UnorderedSet&& set)
			: hasher(),
			elements(std::move(set.elements)),
			buckets(std::move(set.buckets)),
			size(set.size),
			capacity(set.capacity),
			lastElementIndex(set.lastElementIndex)
		{
			set.elements = nullptr;
			set.buckets = nullptr;
			set.size = 0;
			set.capacity = 0;
			set.lastElementIndex = 0;
		}

		UnorderedSet& operator=(UnorderedSet&& set) noexcept
		{
			elements = std::move(set.elements);
			buckets = std::move(set.buckets);
			size = set.size;
			capacity = set.capacity;
			lastElementIndex = set.lastElementIndex;

			set.elements = nullptr;
			set.buckets = nullptr;
			set.size = 0;
			set.capacity = 0;
			set.lastElementIndex = 0;
		}

		~UnorderedSet()
		{
			Clear();
		}

	public:
		virtual void Insert(const T& value) override
		{
			bool result = TryInsert(value);

			if (!result)
			{
				throw ::std::invalid_argument("Already contain value = " + value);
			}
		}

		virtual bool TryInsert(const T& value) override
		{
			if (lastElementIndex >= capacity - 1 || size / capacity < fillValue)
			{
				ReAlloc();
			}

			size_t hash = hasher(value);
			const Bucket& bucket = GetBucketByHash(hash);

			if (!bucket.HasElement())
			{
				size_t elementIndex = ++lastElementIndex;
				elements[elementIndex].SetValue(value, hash);
				bucket.SetElementIndex(elementIndex);
				++size;

				return true;
			}

			size_t next = bucket.GetElementIndex();
			Element* element = &elements[next];

			while (element->HasNext() && element->HasValue() && element->GetValue() != value)
			{
				next = element->GetNextIndex();
				element = &elements[next];
			}

			if (!element->HasValue())
			{
				element->SetValue(value, hash);
				++size;

				return true;
			}

			if (element->GetValue() == value)
			{
				return false;
			}

			size_t elementIndex = ++lastElementIndex;
			elements[elementIndex].SetValue(value);
			element->SetNextIndex(elementIndex);
			++size;

			return true;
		}

		virtual void Remove(const T& value) override
		{
			bool result = TryRemove(value);

			if (!result)
			{
				throw ::std::invalid_argument("Doesn't contain value = " + value);
			}
		}

		virtual bool TryRemove(const T& value) override
		{
			const Bucket& bucket = GetBucket(value);

			if (!bucket.HasElement())
			{
				return false;
			}

			size_t index = bucket.GetElementIndex();
			Element* element = &elements[index];

			while (element != value && element->HasNext())
			{
				index = element->GetNextIndex();
				element = &elements[index];
			}

			if (element == value)
			{
				element->ResetValue();
				return true;
			}

			return false;
		}

		virtual bool Contains(const T& value) const override
		{
			const Bucket& bucket = GetBucket(value);

			if (!bucket.HasElement())
			{
				return false;
			}

			size_t index = bucket.GetElementIndex();
			Element* element = &elements[index];

			while (element != value && element->HasNext())
			{
				index = element->GetNextIndex();
				element = &elements[index];
			}

			return element == value;
		}

		virtual void Clear() override
		{
			if (elements == nullptr)
				return;

			delete[] elements;
			delete[] buckets;

			elements = nullptr;
			buckets = nullptr;

			lastElementIndex = 0;
			capacity = 0;
			size = 0;
		}

	private:
		const Bucket& GetBucket(const T& value) const
		{
			return GetBucketByHash(value, buckets, capacity);
		}

		const Bucket& GetBucketByHash(size_t hash) const
		{
			return GetBucketByHash(hash, buckets, capacity);
		}

		const Bucket& GetBucketByElement(const Element& element) const
		{
			size_t hash = element.GetHash();
			return GetBucketByHash(hash, buckets, capacity);
		}

		const Bucket& GetBucket(const T& value, Bucket* buckets, size_t capacity) const
		{
			size_t hash = hasher(value);
			return GetBucketByHash(hash, buckets, capacity);
		}

		const Bucket& GetBucketByHash(size_t hash, Bucket* buckets, size_t capacity) const
		{
			size_t bucketIndex = hash % capacity;
			return buckets[bucketIndex];
		}

		const Bucket& GetBucketByElement(const Element& element, Bucket* buckets, size_t capacity) const
		{
			size_t hash = element.GetHash();
			return GetBucketByHash(hash, buckets, capacity);
		}

		void ReAlloc()
		{
			size_t newCapacity = capacity * 2;

			Bucket* newBuckets = new Bucket[capacity];
			Element* newElements = new Element[capacity];

			std::copy(elements[0], elements[size - 1], newElements);

			for (size_t i = 0; i < size; ++i)
			{
				RehashElement(i, newBuckets, newElements, newCapacity);
			}

			delete[] elements;
			delete[] buckets;

			elements = newElements;
			buckets = newBuckets;
		}

		void RehashElement(const size_t elementIndex, Bucket* const newBuckets, Element* const newElements, size_t newCapacity)
		{
			Element& elementToRehash = newElements[elementIndex];
			elementToRehash.ResetNextIndex();

			Bucket& bucket = GetBucketByElement(elementToRehash, newBuckets, newCapacity);

			if (!bucket.HasElement())
			{
				bucket.SetElementIndex(elementIndex);
				return;
			}

			size_t next = bucket.GetElementIndex();
			Element* element = &newElements[next];

			while (element->HasNext())
			{
				next = element->GetNextIndex();
				element = &newElements[next];
			}

			element->SetNextIndex(elementIndex);
		}

	private:
		const Element* GetLastValidElement()
		{
			for (size_t i = lastElementIndex; i >= 0; --i)
			{
				if (elements[i].HasValue())
				{
					return &(*elements[i]);
				}
			}

			return nullptr;
		}

	public:
		virtual size_t GetSize() const override { return size; }
		virtual bool IsEmpty() const override { return size == 0; }

	public:
		virtual Iterator begin()
		{
			if (IsEmpty())
			{
				return Iterator(nullptr, true);
			}

			return Iterator(elements[0]);
		}

		virtual Iterator end()
		{
			if (IsEmpty())
			{
				return Iterator(nullptr, true);
			}

			return Iterator(GetLastValidElement());
		}

	private:
		const static float fillValue = 0.75;

	private:
		Hasher hasher;

		Element* elements;
		Bucket* buckets;

		size_t size;
		size_t capacity;
		size_t lastElementIndex;
	};
}
