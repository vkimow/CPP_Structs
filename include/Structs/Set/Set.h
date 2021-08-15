#pragma once
#include "../Tree/AVLTree.h"
#include "ISet.h"

namespace Structs
{
	template <typename T>
	class SetIterator : public IIterator<T, SetIterator<T>>
	{
	private:
		using Tree = AVLTree<T>;
		using TreeIterator = BinaryTreeInorderIterator<T>;

	public:
		SetIterator() = delete;

		SetIterator(TreeIterator iterator)
			: i(iterator)
		{}

		virtual SetIterator& operator++() override
		{
			++i;
			return *this;
		}

		virtual SetIterator& operator++(int) override
		{
			return SetIterator(i++);
		}

		virtual bool operator==(const SetIterator& rhs) const override
		{
			return i == rhs.i;
		}
		virtual bool operator!=(const SetIterator& rhs) const override
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
		TreeIterator i;
	};

	template <typename T>
	class Set final : public ISet<T, SetIterator<T>>
	{
	public:
		using Iterator = SetIterator<T>;

	public:
		Set()
			: tree()
		{}

		Set(const T& value)
			: tree(value)
		{}

		~Set()
		{
			Clear();
		}

	public:
		virtual void Insert(const T& value) override
		{
			tree.Insert(value);
		}

		virtual bool TryInsert(const T& value) override
		{
			return tree.TryInsert(value);
		}

		virtual void Remove(const T& value) override
		{
			tree.Remove(value);
		}

		virtual bool TryRemove(const T& value) override
		{
			return tree.TryRemove(value);
		}

		virtual bool Contains(const T& value) override
		{
			return tree.Contains(value);
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
		AVLTree<T> tree;
	};
}
