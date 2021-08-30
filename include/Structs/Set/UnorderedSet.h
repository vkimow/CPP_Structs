#pragma once
#include "ISet.h"
#include <stdexcept>
#include "../HashTable/HashTable.h"

namespace Structs
{
	template <typename T>
	class UnorderedSetIterator : public IIterator<T, UnorderedSetIterator<T>>
	{
	public:
		using HashTable = HashTable<T>;
		using HashTableIterator = HashTableIterator<T>;

	public:
		UnorderedSetIterator() = delete;

		UnorderedSetIterator(HashTableIterator iterator)
			: i(iterator)
		{}

		virtual UnorderedSetIterator& operator++() override
		{
			++i;
			return *this;
		}

		virtual UnorderedSetIterator& operator++(int) override
		{
			return UnorderedSetIterator(i++);
		}

		virtual bool operator==(const UnorderedSetIterator& rhs) const override
		{
			return i == rhs.i;
		}
		virtual bool operator!=(const UnorderedSetIterator& rhs) const override
		{
			return i != rhs.i;
		}

		virtual T& operator*() const override
		{
			return i.operator*();
		}

		virtual T* operator->() const override
		{
			return i.operator->();
		}

	private:
		HashTableIterator i;
	};



	template <typename T>
	class UnorderedSet final : public ISet<T, UnorderedSetIterator<T>>
	{
	public:
		using Iterator = UnorderedSetIterator<T>;
		using HashTable = HashTable<T>;

	public:
		UnorderedSet()
			: hashTable()
		{}

		UnorderedSet(UnorderedSet&& set) noexcept
			: hashTable(std::move(set.hashTable))
		{}

		UnorderedSet& operator=(UnorderedSet&& set) noexcept
		{
			hashTable = std::move(set.hashTable);
		}

		~UnorderedSet()
		{
			Clear();
		}

	public:
		virtual void Insert(const T& value) override
		{
			hashTable.Insert(value);
		}

		virtual bool TryInsert(const T& value) override
		{
			return hashTable.TryInsert(value);
		}

		virtual void Remove(const T& value) override
		{
			hashTable.Remove(value);
		}

		virtual bool TryRemove(const T& value) override
		{
			return hashTable.TryRemove(value);
		}

		virtual bool Contains(const T& value) override
		{
			return hashTable.Contains(value);
		}

		virtual void Clear() override
		{
			hashTable.Clear();
		}

	public:
		virtual size_t GetSize() const override { return hashTable.GetSize(); }
		virtual bool IsEmpty() const override { return hashTable.IsEmpty(); }

	public:
		virtual Iterator begin() const override
		{
			return Iterator(hashTable.begin());
		}

		virtual Iterator end() const override
		{
			return Iterator(hashTable.end());
		}

	private:
		const static float fillValue;

	private:
		HashTable hashTable;
	};
}
