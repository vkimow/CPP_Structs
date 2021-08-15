#pragma once
#include "IBinaryTree.h"
#include "../Collection/IIterator.h"
#include <stdexcept>
#include <string>
#include <stack>

namespace Structs
{
	template <typename T>
	class BinaryTreeInorderIterator : public IIterator<T, BinaryTreeInorderIterator<T>>
	{
	public:
		using Node = IBinaryTreeNode<T>;

	public:
		BinaryTreeInorderIterator()
			:nodesOrder(), currentNode(nullptr)
		{}

		BinaryTreeInorderIterator(Node* root)
			:nodesOrder(), currentNode(root)
		{
			if (currentNode == nullptr)
				return;

			while (currentNode->HasLeft())
			{
				nodesOrder.push(currentNode);
				currentNode = currentNode->GetLeft();
			}
		}

		virtual BinaryTreeInorderIterator& operator++() override
		{
			bool iteratorMoved = false;

			if (currentNode->HasRight())
			{
				currentNode = currentNode->GetRight();
				iteratorMoved = true;
			}

			if (iteratorMoved)
			{
				while (currentNode->HasLeft())
				{
					nodesOrder.push(currentNode);
					currentNode = currentNode->GetLeft();
				}
			}
			else
			{
				if (nodesOrder.empty())
				{
					currentNode = nullptr;
					return *this;
				}

				currentNode = nodesOrder.top();
				nodesOrder.pop();
			}

			return *this;
		}

		virtual BinaryTreeInorderIterator& operator++(int) override
		{
			BinaryTreeInorderIterator temp = *this;
			++(*this);
			return temp;
		}


		virtual bool operator==(const BinaryTreeInorderIterator& other) const override
		{
			return currentNode == other.currentNode;
		}

		virtual bool operator!=(const BinaryTreeInorderIterator& other) const override
		{
			return currentNode != other.currentNode;
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
		std::stack<Node*> nodesOrder;
	};

	template <typename T>
	class BinaryTreePreorderIterator : public IIterator<T, BinaryTreeInorderIterator<T>>
	{
	public:
		using Node = IBinaryTreeNode<T>;

	public:
		BinaryTreePreorderIterator()
			:nodesOrder(), currentNode(nullptr)
		{}

		BinaryTreePreorderIterator(Node* root)
			:nodesOrder(), currentNode(root)
		{
			if (currentNode == nullptr)
				return;
		}

		virtual BinaryTreePreorderIterator& operator++() override
		{
			if (currentNode->HasLeft())
			{
				nodesOrder.push(currentNode);
				currentNode = currentNode->GetLeft();
				return *this;
			}

			while (!currentNode->HasRight())
			{
				if (nodesOrder.empty())
				{
					currentNode = nullptr;
					return *this;
				}

				currentNode = nodesOrder.top();
				nodesOrder.pop();
			}

			if (currentNode->HasRight())
			{
				currentNode = currentNode->GetRight();
				return *this;
			}
		}

		virtual BinaryTreePreorderIterator& operator++(int) override
		{
			BinaryTreeInorderIterator temp = *this;
			++(*this);
			return temp;
		}


		virtual bool operator==(const BinaryTreePreorderIterator& other) const override
		{
			return currentNode == other.currentNode;
		}

		virtual bool operator!=(const BinaryTreePreorderIterator& other) const override
		{
			return currentNode != other.currentNode;
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
		std::stack<Node*> nodesOrder;
	};
}
