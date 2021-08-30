#pragma once
#include "../Tree/AVLTree.h"
#include "IMap.h"
#include "../HashTable/KeySelectors.h"

namespace Structs
{
	template <typename Key, typename Value, typename Pair = std::pair<Key, Value>>
	class MapIterator : public IIterator<Pair, MapIterator<Key, Value>>
	{
	public:
		using KeySelector = Keys::PairSelector<Key, Value>;
		using Tree = AVLTree<Key, Pair, KeySelector>;
		using TreeIterator = BinaryTreeInorderIterator<Pair>;

	public:
		MapIterator() = delete;

		MapIterator(TreeIterator iterator)
			: i(iterator)
		{}

		virtual MapIterator& operator++() override
		{
			++i;
			return *this;
		}

		virtual MapIterator& operator++(int) override
		{
			return MapIterator(i++);
		}

		virtual bool operator==(const MapIterator& rhs) const override
		{
			return i == rhs.i;
		}
		virtual bool operator!=(const MapIterator& rhs) const override
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
	class Map final : public IMap<Key, Value, MapIterator<Key, Value>>
	{
	public:
		using Pair = std::pair<Key, Value>;
		using KeySelector = Keys::PairSelector<Key, Value>;
		using Tree = AVLTree<Key, Pair, KeySelector>;
		using Iterator = MapIterator<Key, Value>;

	public:
		Map()
			: tree()
		{}

		~Map()
		{
			Clear();
		}

	public:
		virtual void Insert(const Pair& keyValuePair) override
		{
			tree.Insert(keyValuePair);
		}

		virtual void Insert(const Key& key, const Value& value) override
		{
			Insert(Pair(key, value));
		}

		virtual bool TryInsert(const Pair& keyValuePair) override
		{
			return tree.TryInsert(keyValuePair);
		}

		virtual bool TryInsert(const Key& key, const Value& value) override
		{
			return TryInsert(Pair(key, value));
		}

		virtual void Remove(const Key& key) override
		{
			tree.Remove(key);
		}

		virtual bool TryRemove(const Key& key) override
		{
			return tree.TryRemove(key);
		}

		virtual bool Contains(const Key& key) override
		{
			return tree.Contains(key);
		}

		virtual void Clear() override
		{
			tree.Clear();
		}

	public:
		virtual size_t GetSize() const override { return tree.GetSize(); }
		virtual bool IsEmpty() const override { return tree.IsEmpty(); }

	public:
		virtual Iterator begin() const override
		{
			return Iterator(tree.begin());
		}

		virtual Iterator end() const override
		{
			return Iterator(tree.end());
		}

	private:
		Tree tree;
	};
}
