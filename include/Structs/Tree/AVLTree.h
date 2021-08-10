#pragma once
#include "../Collection/ICollection.h"
#include "BinaryTree.h"
#include <stdexcept>
#include <exception>
#include <string>
#include <stack>

namespace Structs
{
	template <typename T>
	class AVLTree final : public IBinaryTree<T, BinaryTreeInorderIterator<T>>
	{
	public:
		using Node = BinaryTreeNode<T>;
		using Iterator = BinaryTreeInorderIterator<T>;

	public:
		AVLTree()
			: root(nullptr), size(0)
		{}

		AVLTree(const T& value)
			: root(new Node(value)), size(1)
		{}

		AVLTree(const AVLTree& tree) = delete;
		AVLTree& operator=(const AVLTree& tree) = delete;

		AVLTree(AVLTree&& tree)
			:
			root(::std::move(tree.root)),
			size(tree.size)
		{
			tree.root = nullptr;
			tree.size = 0;
		}

		AVLTree& operator=(AVLTree&& tree)
		{
			root = ::std::move(tree.root);
			size = tree.size;

			tree.root = nullptr;
			tree.size = 0;
		}

		~AVLTree()
		{
			Clear();
		}

	public:
		virtual void Insert(const T& value) override
		{
			Node* newRoot = nullptr;

			try
			{
				newRoot = InsertRecursive(root, value);
			}
			catch (std::invalid_argument error)
			{
				throw error;
			}

			root = newRoot;
		}

		virtual bool TryInsert(const T& value) override
		{
			Node* newRoot = nullptr;

			try
			{
				newRoot = InsertRecursive(root, value);
			}
			catch (std::invalid_argument error)
			{
				return false;
			}

			root = newRoot;
			return true;
		}

		virtual void Remove(const T& value) override
		{
			Node* newRoot = nullptr;

			try
			{
				newRoot = RemoveRecursive(root, value);
			}
			catch (std::invalid_argument error)
			{
				throw error;
			}

			root = newRoot;
		}

		virtual bool TryRemove(const T& value) override
		{
			Node* newRoot = nullptr;

			try
			{
				newRoot = RemoveRecursive(root, value);
			}
			catch (std::invalid_argument error)
			{
				return false;
			}

			root = newRoot;
			return true;
		}

		virtual bool Contains(const T& value) const override
		{
			Node* node = SearchRecursive(root, value);

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

	private:
		Node* InsertRecursive(Node* node, const T& value)
		{
			if (node == nullptr)
			{
				++size;
				return new Node(value);
			}
			else if (node->value > value)
			{
				Node* newLeft = InsertRecursive(node->left, value);
				node->left = newLeft;
			}
			else if (node->value < value)
			{
				Node* newRight = InsertRecursive(node->right, value);
				node->right = newRight;
			}
			else
			{
				throw std::invalid_argument("Already contains value = " + value);
			}

			return BalanceNode(node, value);
		}

		Node* RemoveRecursive(Node* node, const T& value)
		{
			if (node == nullptr)
			{
				throw ::std::invalid_argument("Doesn't contain value = " + value);
			}
			else if (node->value > value)
			{
				Node* newLeft = RemoveRecursive(node->left, value);
				node->left = newLeft;
			}
			else if (node->value < value)
			{
				Node* newRight = RemoveRecursive(node->right, value);
				node->right = newRight;
			}
			else
			{
				return RemoveNode(node);
			}

			return BalanceNode(node);
		}

		Node* SearchRecursive(Node* node, const T& value) const
		{
			if (node == nullptr || node->value == value)
			{
				return node;
			}

			if (node->value > value)
			{
				return SearchRecursive(node->left, value);
			}

			if (node->value < value)
			{
				return SearchRecursive(node->right, value);
			}

			return nullptr;
		}

		void ClearRecursive(Node* node)
		{
			if (node == nullptr)
				return;

			ClearRecursive(node->left);
			ClearRecursive(node->right);

			delete node;
		}

	private:
		Node* RemoveNode(Node* node)
		{
			if (node->left == nullptr)
			{
				Node* temp = node->right;
				delete node;
				--size;
				return temp;
			}
			else if (node->right == nullptr)
			{
				Node* temp = node->left;
				delete node;
				--size;
				return temp;
			}
			else
			{
				Node* temp = GetMostLeftChildOf(node->right);
				node->value = temp->value;
				node->right = RemoveRecursive(node->right, temp->value);
				return node;
			}
		}

		Node* BalanceNode(Node* node)
		{
			int bf = GetBalanceFactorOf(node);

			if (bf > 1)
			{
				size_t bfLeft = GetBalanceFactorOf(node->left);

				// Left Left Case  
				if (bfLeft >= 0)
				{
					return RotateRight(node);
				}

				// Left Right Case  
				else
				{
					node->left = RotateLeft(node->left);
					return RotateRight(node);
				}
			}

			if (bf < -1)
			{
				size_t bfRight = GetBalanceFactorOf(node->right);

				// Right Right Case  
				if (bfRight <= 0)
				{
					return RotateLeft(node);
				}

				// Right Left Case  
				else
				{
					node->right = RotateRight(node->right);
					return RotateLeft(node);
				}
			}

			return node;
		}

		Node* BalanceNode(Node* node, const T& newValue)
		{
			int bf = GetBalanceFactorOf(node);

			if (bf > 1)
			{
				const T& lValue = node->left->value;

				// Left Left Case  
				if (newValue <= lValue)
				{
					return RotateRight(node);
				}

				// Left Right Case  
				else
				{
					node->left = RotateLeft(node->left);
					return RotateRight(node);
				}
			}

			if (bf < -1)
			{
				const T& rValue = node->right->value;

				// Right Right Case  
				if (newValue >= rValue)
				{
					return RotateLeft(node);
				}

				// Right Left Case  
				else
				{
					node->right = RotateRight(node->right);
					return RotateLeft(node);
				}
			}

			return node;
		}

		Node* RotateRight(Node* parent)
		{
			Node* newParent = parent->left;
			Node* newLeftChild = newParent->right;

			parent->left = newLeftChild;
			newParent->right = parent;

			return newParent;
		}

		Node* RotateLeft(Node* parent)
		{
			Node* newParent = parent->right;
			Node* newRightChild = newParent->left;

			parent->right = newRightChild;
			newParent->left = parent;

			return newParent;
		}

		int GetBalanceFactorOf(Node* node) const
		{
			size_t hLeft = 0;
			size_t hRight = 0;

			if (node->HasLeft())
			{
				hLeft = GetHeightOf(node->left);
			}

			if (node->HasRight())
			{
				hRight = GetHeightOf(node->right);
			}

			return hLeft - hRight;
		}

	public:
		virtual bool IsEmpty() const override { return root == nullptr; }
		virtual size_t GetSize() const override { return size; }

	public:
		virtual Iterator begin() const override
		{
			return Iterator(root);
		}

		virtual Iterator end() const override
		{
			return Iterator();
		}

	private:
		Node* root;
		size_t size;

	};
}
