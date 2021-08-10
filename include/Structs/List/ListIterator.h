#pragma once
#include "IList.h"
#include <stdexcept>
#include <string>

namespace Structs
{
	template <typename T>
	class ListIterator final : IIterator<T, ListIterator<T>>
	{
	private:
		using Node = IListNode<T>;

	public:
		ListIterator()
			: currentNode(nullptr)
		{}

		ListIterator(Node* const node)
			: currentNode(node)
		{}

	public:
		virtual ListIterator& operator++() override
		{
			currentNode = currentNode->GetNext();
			return *this;
		}

		virtual ListIterator& operator++(int) override
		{
			ListIterator<T> tempIterator = *this;
			++(*this);
			return tempIterator;
		}

		ListIterator& operator--()
		{
			currentNode = currentNode->GetPrevious();
			return *this;
		}

		ListIterator operator--(int)
		{
			ListIterator tempIterator = *this;
			--(*this);
			return tempIterator;
		}

		virtual bool operator==(const ListIterator& rhs) const override
		{
			return currentNode == rhs.currentNode;
		}

		virtual bool operator!=(const ListIterator& rhs) const override
		{
			return currentNode != rhs.currentNode;
		}

		virtual T& operator*() const override
		{
			return currentNode->GetValue();

		}

		virtual T* operator->() const override
		{
			return &currentNode->GetValue();
		}

	private:
		Node* currentNode;
	};
}