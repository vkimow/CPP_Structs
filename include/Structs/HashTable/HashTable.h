#pragma once
#include <stdexcept>
#include "../Collection/ICollection.h"
#include "../Collection/IIterable.h"
#include "KeySelectors.h"
#include <unordered_set>

namespace Structs
{
	template<typename T>
	class HashTableElement
	{
	public:
		HashTableElement()
			: hash(0), nextIndex(0), hasValue(false), hasNext(false)
		{}

	public:
		bool operator==(const T& rhs) const
		{
			return hasValue && value == rhs;
		}

		bool operator!=(const T& rhs) const
		{
			return !(*this == rhs);
		}

	public:
		void SetValue(const T& value, size_t hash) { this->value = value; this->hash = hash; hasValue = true; }
		void ResetValue() { hasValue = false; }
		T& GetValue() { return value; }

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

	template <typename T>
	class HashTableIterator : public IIterator<T, HashTableIterator<T>>
	{
	private:
		using Element = HashTableElement<T>;

	public:
		HashTableIterator()
			: HashTableIterator(nullptr, true)
		{}

		HashTableIterator(Element* element)
			: HashTableIterator(element, false)
		{}

		HashTableIterator(Element* element, bool isInvalid)
			: currentElement(element), isInvalid(isInvalid)
		{}

		virtual HashTableIterator& operator++() override
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

		virtual HashTableIterator& operator++(int) override
		{
			HashTableIterator temp = *this;
			++(*this);
			return temp;
		}

		HashTableIterator& operator--()
		{
			if (isInvalid)
			{
				return *this;
			}

			do
			{
				--currentElement;
			} while (!currentElement->HasValue());

			return *this;
		}

		HashTableIterator& operator--(int)
		{
			HashTableIterator temp = *this;
			--(*this);
			return temp;
		}

		virtual bool operator==(const HashTableIterator& rhs) const override
		{
			if (isInvalid)
			{
				return true;
			}

			isInvalid = currentElement == rhs.currentElement;
			return false;
		}

		virtual bool operator!=(const HashTableIterator& rhs) const override
		{
			return !(*this == rhs);
		}

		virtual T& operator*() const override
		{
			return currentElement->GetValue();
		}

		virtual T* operator->() const override
		{
			return &(currentElement->GetValue());
		}

	private:
		Element* currentElement;
		mutable bool isInvalid;
	};


	template<typename Key,
		typename Value = Key,
		typename KeySelector = Keys::NoSelector<Key>,
		typename Hasher = std::hash<Key>>
		class HashTable final : public IIterable<Value, HashTableIterator<Value>>, public ICollection
	{
	private:
		static_assert(std::is_base_of<Keys::Selector<Key, Value>, KeySelector>::value, "KeySelector mast be derivied from Structs::Keys::Selector");

	public:
		using Iterator = HashTableIterator<Value>;
		using Element = HashTableElement<Value>;
		using Bucket = BucketElement<Value>;

	public:
		HashTable()
			: elements(nullptr), buckets(nullptr), size(0), capacity(0), lastElementIndex(0)
		{
			ReAlloc(5);
		}

		HashTable(HashTable&& set)
			:
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

		HashTable& operator=(HashTable&& set) noexcept
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

		~HashTable()
		{
			Clear();
		}

	public:
		void Insert(const Value& value)
		{
			bool result = TryInsert(value);

			if (!result)
			{
				throw std::invalid_argument("Already contain value");
			}
		}

		bool TryInsert(const Value& value)
		{
			if (lastElementIndex >= capacity || size / capacity > fillValue)
			{
				ReAlloc();
			}

			Key key = keySelector(value);
			size_t hash = hasher(key);
			Bucket& bucket = GetBucketByHash(hash);

			if (!(bucket.HasElement()))
			{
				size_t elementIndex = lastElementIndex;
				elements[elementIndex].SetValue(value, hash);
				bucket.SetElementIndex(elementIndex);
				++lastElementIndex;
				++size;

				return true;
			}

			size_t next = bucket.GetElementIndex();
			Element* element = &elements[next];
			Key elementKey = keySelector(element->GetValue());

			while (element->HasNext() && element->HasValue() && elementKey != key)
			{
				next = element->GetNextIndex();
				element = &elements[next];
				elementKey = keySelector(element->GetValue());
			}

			if (!element->HasValue())
			{
				element->SetValue(value, hash);
				++size;

				return true;
			}

			if (elementKey == key)
			{
				return false;
			}

			size_t elementIndex = lastElementIndex;
			elements[elementIndex].SetValue(value, hash);
			element->SetNextIndex(elementIndex);
			++lastElementIndex;
			++size;

			return true;
		}

		void Remove(const Key& key)
		{
			bool result = TryRemove(key);

			if (!result)
			{
				throw ::std::invalid_argument("Doesn't contain value with key = " + key);
			}
		}

		bool TryRemove(const Key& key)
		{
			Element* element = FindElement(key);

			if (element == nullptr)
			{
				return false;
			}

			element->ResetValue();
			return true;
		}

		bool Contains(const Key& key)
		{
			Element* element = FindElement(key);
			return element != nullptr;
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
		Bucket& GetBucketByHash(size_t hash, Bucket* buckets, size_t capacity)
		{
			size_t bucketIndex = hash % capacity;
			return buckets[bucketIndex];
		}

		Bucket& GetBucketByElement(const Element& element, Bucket* buckets, size_t capacity)
		{
			size_t hash = element.GetHash();
			return GetBucketByHash(hash, buckets, capacity);
		}

		Bucket& GetBucket(const Key& key, Bucket* buckets, size_t capacity)
		{
			size_t hash = hasher(key);
			return GetBucketByHash(hash, buckets, capacity);
		}

		Bucket& GetBucketByHash(size_t hash)
		{
			return GetBucketByHash(hash, buckets, capacity);
		}

		Bucket& GetBucketByElement(const Element& element)
		{
			return GetBucketByElement(element, buckets, capacity);
		}

		Bucket& GetBucket(const Key& key)
		{
			return GetBucket(key, buckets, capacity);
		}


		void ReAlloc()
		{
			size_t newCapacity = capacity * 2;
			ReAlloc(newCapacity);
		}

		void ReAlloc(size_t newCapacity)
		{
			Bucket* newBuckets = new Bucket[newCapacity];
			Element* newElements = new Element[newCapacity];

			if (elements != nullptr)
			{
				size_t newLastElementIndex = 0;

				for (size_t i = 0; i < lastElementIndex; ++i)
				{
					if (TryMoveElement(i, newLastElementIndex, newElements))
					{
						++newLastElementIndex;
					}
				}

				for (size_t i = 0; i < size; ++i)
				{
					RehashElement(i, newBuckets, newElements, newCapacity);
				}

				lastElementIndex = newLastElementIndex;
				delete[] elements;
				delete[] buckets;
			}

			capacity = newCapacity;
			elements = newElements;
			buckets = newBuckets;
		}

		bool TryMoveElement(const size_t elementIndex, const size_t newElementIndex, Element* const newElements)
		{
			Element& element = elements[elementIndex];

			if (element.HasValue())
			{
				newElements[newElementIndex] = std::move(element);
				return true;
			}

			return false;
		}

		void RehashElement(const size_t elementIndex, Bucket* const buckets, Element* const elements, size_t capacity)
		{
			Element& elementToRehash = elements[elementIndex];
			elementToRehash.ResetNextIndex();

			Bucket& bucket = GetBucketByElement(elementToRehash, buckets, capacity);

			if (!bucket.HasElement())
			{
				bucket.SetElementIndex(elementIndex);
				return;
			}

			size_t next = bucket.GetElementIndex();
			Element* element = &elements[next];

			while (element->HasNext())
			{
				next = element->GetNextIndex();
				element = &elements[next];
			}

			element->SetNextIndex(elementIndex);
		}

	private:
		Element* const FindElement(const Key& key)
		{
			Bucket& bucket = GetBucket(key);

			if (!bucket.HasElement())
			{
				return nullptr;
			}

			size_t index = bucket.GetElementIndex();
			Element* element = &elements[index];
			Key elementKey = keySelector(element->GetValue());

			while (!(element->HasValue() && elementKey == key) && element->HasNext())
			{
				index = element->GetNextIndex();
				element = &elements[index];
				elementKey = keySelector(element->GetValue());
			}

			if (!(element->HasValue() && elementKey == key))
			{
				return nullptr;
			}

			return element;
		}

		Element* GetLastValidElement() const
		{
			for (size_t i = lastElementIndex - 1; i >= 0; --i)
			{
				if (elements[i].HasValue())
				{
					return &elements[i];
				}
			}

			return nullptr;
		}

	public:
		virtual size_t GetSize() const override { return size; }
		virtual bool IsEmpty() const override { return size == 0; }

	public:
		virtual Iterator begin() const override
		{
			if (IsEmpty())
			{
				return Iterator(nullptr, true);
			}

			return Iterator(&elements[0]);
		}

		virtual Iterator end() const override
		{
			if (IsEmpty())
			{
				return Iterator(nullptr, true);
			}

			return Iterator(GetLastValidElement());
		}

	private:
		const static float fillValue;

	private:
		Hasher hasher;
		KeySelector keySelector;

		Element* elements;
		Bucket* buckets;

		size_t size;
		size_t capacity;
		size_t lastElementIndex;
	};

	template<typename Key, typename Value, typename KeySelector, typename Hasher>
	const float HashTable<Key, Value, KeySelector, Hasher>::fillValue = 0.75;
}
