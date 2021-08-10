#pragma once
#include "IBinaryTree.h"
#include "BinaryTreeNode.h"
#include "BinaryTreeIterators.h"
#include <stdexcept>
#include <exception>
#include <string>
#include <stack>

namespace Structs
{
	template <typename T>
	class BinaryTree final : public IBinaryTree<T, BinaryTreeInorderIterator<T>>
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

		~BinaryTree()
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

			if (alreadyHaveChild)
			{
				throw std::invalid_argument("Already contains value = " + value);
			}

			parent->SetChild(newNode, isRightChild);
		}

		virtual bool TryInsert(const T& value) override
		{
			Node* parent = FindParent(value);
			Node* newNode = CreateNode(value);

			if (parent == nullptr)
			{
				root = newNode;
				return;
			}

			bool isRightChild = parent->value <= value;
			bool alreadyHaveChild = parent->HasChild(isRightChild);

			if (alreadyHaveChild)
			{
				return false;
			}

			parent->SetChild(newNode, isRightChild);
			return true;
		}

		virtual void Remove(const T& value) override
		{
			Node* parent = FindParent(value);
			Node* node = root;

			if (parent != nullptr)
			{
				node = FindNode(parent, value);
			}

			if (node == nullptr)
			{
				throw ::std::invalid_argument("Doesn't contain value = " + value);
			}

			RemoveNode(parent, node);
		}

		virtual bool TryRemove(const T& value) override
		{
			Node* parent = FindParent(value);
			Node* node = root;

			if (parent != nullptr)
			{
				node = FindNode(parent, value);
			}

			if (node == nullptr)
			{
				return false;
			}

			RemoveNode(parent, node);
			return true;
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
			if (node == nullptr)

			{

			}
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
