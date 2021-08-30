#pragma once
#include "../HashTable/HashTable.h"
#include "IMap.h"
#include "../HashTable/KeySelectors.h"

namespace Structs
{
	template <typename Key, typename Value, typename Pair = std::pair<Key, Value>>
	class UnorderedMapIterator : public IIterator<Pair, UnorderedMapIterator<Key, Value>>
	{
	public:
		using KeySelector = Keys::PairSelector<Key, Value>;
		using HashTable = HashTable<Key, Pair, KeySelector>;
		using TreeIterator = HashTableIterator<Pair>;

	public:
		UnorderedMapIterator() = delete;

		UnorderedMapIterator(TreeIterator iterator)
			: i(iterator)
		{}

		virtual UnorderedMapIterator& operator++() override
		{
			++i;
			return *this;
		}

		virtual UnorderedMapIterator& operator++(int) override
		{
			return UnorderedMapIterator(i++);
		}

		virtual bool operator==(const UnorderedMapIterator& rhs) const override
		{
			return i == rhs.i;
		}
		virtual bool operator!=(const UnorderedMapIterator& rhs) const override
		{
			return i != rhs.i;
		}

		virtual Pair& operator*() const override
		{
			return i.operator*();
		}

		virtual Pair* operator->() const override
		{
			return i.operator->();
		}

	private:
		TreeIterator i;
	};

	template <typename Key, typename Value>
	class UnorderedMap final : public IMap<Key, Value, UnorderedMapIterator<Key, Value>>
	{
	public:
		using Pair = std::pair<Key, Value>;
		using KeySelector = Keys::PairSelector<Key, Value>;
		using HashTable = HashTable<Key, Pair, KeySelector>;
		using Iterator = UnorderedMapIterator<Key, Value>;

	public:
		UnorderedMap()
			: hashTable()
		{}

		~UnorderedMap()
		{
			Clear();
		}

	public:
		virtual void Insert(const Pair& keyValuePair) override
		{
			hashTable.Insert(keyValuePair);
		}

		virtual void Insert(const Key& key, const Value& value) override
		{
			Insert(Pair(key, value));
		}

		virtual bool TryInsert(const Pair& keyValuePair) override
		{
			return hashTable.TryInsert(keyValuePair);
		}

		virtual bool TryInsert(const Key& key, const Value& value) override
		{
			return TryInsert(Pair(key, value));
		}

		virtual void Remove(const Key& key) override
		{
			hashTable.Remove(key);
		}

		virtual bool TryRemove(const Key& key) override
		{
			return hashTable.TryRemove(key);
		}

		virtual bool Contains(const Key& key) override
		{
			return hashTable.Contains(key);
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
		HashTable hashTable;
	};
}
