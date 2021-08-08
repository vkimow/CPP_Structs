#pragma once
#include "../Collection/ICollection.h"
#include "IBinaryTree.h"
#include <stdexcept>
#include <exception>
#include <string>
#include <stack>

namespace Structs
{
	template <typename T>
	class BinaryTreeNode : public IBinaryTreeNode<T>
	{
	public:
		BinaryTreeNode(const T& value)
			:BinaryTreeNode(value, nullptr, nullptr)
		{}

		BinaryTreeNode(const T& value, BinaryTreeNode* right, BinaryTreeNode* left)
			:value(value), right(right), left(left)
		{}

		virtual void SetValue(const T& value) override { this->value = value; }
		virtual T& GetValue() override { return value; }

		virtual BinaryTreeNode* const GetLeft() const override { return left; }
		virtual BinaryTreeNode* const GetRight() const override { return right; }
		virtual BinaryTreeNode* const GetChild(bool isRight) const override { return isRight ? right : left; }

		virtual bool HasRight() const override { return right != nullptr; }
		bool HasLeft() const override { return left != nullptr; }
		bool HasChild(bool isRight) const override
		{
			return isRight ? HasRight() : HasLeft();
		}

		void SetChild(BinaryTreeNode* child, bool isRight)
		{
			isRight
				? right = child
				: left = child;
		}

	public:
		T value;
		BinaryTreeNode* right;
		BinaryTreeNode* left;
	};

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
	class BinaryTree : public IBinaryTree<T, BinaryTreeInorderIterator<T>>
	{
	public:
		using Node = BinaryTreeNode<T>;
		using Iterator = BinaryTreeInorderIterator<T>;

	public:
		BinaryTree()
			: root(nullptr), size(0)
		{}

		BinaryTree(const T& value)
			: root(new Node(value)), size(1)
		{}

		BinaryTree(const BinaryTree& tree) = delete;
		BinaryTree& operator=(const BinaryTree& tree) = delete;

		BinaryTree(BinaryTree&& tree)
			:
			root(::std::move(tree.root)),
			size(tree.size)
		{
			tree.root = nullptr;
			tree.size = 0;
		}

		BinaryTree& operator=(BinaryTree&& tree)
		{
			root = ::std::move(tree.root);
			size = tree.size;

			tree.root = nullptr;
			tree.size = 0;
		}

		virtual ~BinaryTree()
		{
			Clear();
		}

	public:
		virtual void Insert(const T& value) override
		{
			Node* parent = FindParent(value);
			Node* newNode = CreateNode(value);

			if (parent == nullptr)
			{
				root = newNode;
				return;
			}

			bool isRightChild = parent->value < value;
			bool alreadyHaveChild = parent->HasChild(isRightChild);
			parent->SetChild(newNode, isRightChild);

			if (alreadyHaveChild)
			{
				throw std::invalid_argument("Already contains value = " + value);
			}
		}

		virtual void Remove(const T& value) override
		{
			Node* parent = FindParent(value);
			Node* node = root;

			if (parent != nullptr)
			{
				node = FindNode(parent, value);
			}

			RemoveNode(parent, node);
		}

		virtual bool Contains(const T& value) const override
		{
			Node* node = FindNode(value);

			if (node == nullptr)
			{
				return false;
			}

			return true;
		}

		virtual void Clear() override
		{
			ClearRecursive(root);
			root = nullptr;
			size = 0;
		}


		void ClearRecursive(Node* node)
		{
			if (node == nullptr)
				return;

			ClearRecursive(node->GetLeft());
			ClearRecursive(node->right);

			delete node;
		}

	private:
		Node* FindNode(const T& value) const
		{
			return FindNode(root, value);
		}

		Node* FindNode(Node* root, const T& value) const
		{
			Node* node = root;

			while (node != nullptr)
			{
				if (value < node->value)
				{
					node = node->left;
				}
				else if (value > node->value)
				{
					node = node->right;
				}
				else
				{
					return node;
				}
			}

			return node;
		}

		Node* FindParent(const T& value) const
		{
			FindParent(root, value);
		}

		Node* FindParent(Node* root, const T& value) const
		{
			Node* parent = nullptr;
			Node* node = root;

			while (node != nullptr)
			{
				if (value < node->value)
				{
					parent = node;
					node = node->left;
				}
				else if (value > node->value)
				{
					parent = node;
					node = node->right;
				}
				else
				{
					return parent;
				}
			}

			return parent;
		}

	private:
		void RemoveNode(Node* parent, Node* node)
		{
			if (parent == nullptr)
			{
				root = nullptr;
				DeleteNode(node);
				return;
			}

			while (node != nullptr)
			{
				bool isRightChild = parent->right == node;

				if (!node->HasLeft())
				{
					parent->SetChild(node->right, isRightChild);
					DeleteNode(node);
					return;
				}
				else if (!node->HasRight())
				{
					parent->SetChild(node->left, isRightChild);
					DeleteNode(node);
					return;
				}
				else
				{
					parent = GetMostLeftParentOf(node->right);
					Node* nodeToDelete = node->right;

					parent == nullptr
						? parent = node
						: nodeToDelete = parent->left;

					node->value = nodeToDelete->value;
					node = nodeToDelete;
				}
			}
		}

	private:
		Node* CreateNode(const T& value)
		{
			Node* newNode = new Node(value);
			++size;
			return newNode;
		}

		void DeleteNode(Node* node)
		{
			delete node;
			--size;
		}

	public:
		virtual Iterator begin() const override
		{
			return Iterator(root);
		}

		virtual Iterator end() const override
		{
			return Iterator();
		}

	public:
		virtual bool IsEmpty() const override { return root == nullptr; }
		virtual size_t GetSize() const override { return size; }

	protected:
		Node* GetRoot() const { return root; }
		void SetRoot(Node* node) { root = node; }

	private:
		Node* root;
		size_t size;
	};
}
