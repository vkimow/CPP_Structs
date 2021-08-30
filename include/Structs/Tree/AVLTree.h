#pragma once
#include "BinaryTree.h"
#include "../Collection/ICollection.h"
#include "../Collection//IIterable.h"
#include "../HashTable/KeySelectors.h"
#include <stdexcept>
#include <string>
#include <type_traits>

namespace Structs
{
	template<typename Key, 
		typename Value = Key, 
		typename KeySelector = Keys::NoSelector<Value>>
	class AVLTree final : public IIterable<Value, BinaryTreeInorderIterator<Value>>, public ICollection
	{
	private:
		static_assert(std::is_base_of<Keys::Selector<Key, Value>, KeySelector>::value, "KeySelector mast be derivied from Structs::Keys::Selector");

	public:
		using Node = BinaryTreeNode<Value>;
		using Iterator = BinaryTreeInorderIterator<Value>;

	public:
		AVLTree()
			: root(nullptr), size(0), keySelector()
		{}

		AVLTree(const Value& value)
			: root(new Node(value)), size(1), keySelector()
		{}

		AVLTree(const AVLTree& tree) = delete;
		AVLTree& operator=(const AVLTree& tree) = delete;

		AVLTree(AVLTree&& tree)
			:
			root(::std::move(tree.root)),
			size(tree.size),
			keySelector()
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
		void Insert(const Value& value)
		{
			Node* newRoot = nullptr;
			Key key = keySelector(value);

			try
			{
				newRoot = InsertRecursive(root, key, value);
			}
			catch (std::invalid_argument error)
			{
				throw error;
			}

			root = newRoot;
		}

		bool TryInsert(const Value& value)
		{
			Node* newRoot = nullptr;
			Key key = keySelector(value);

			try
			{
				newRoot = InsertRecursive(root, key, value);
			}
			catch (std::invalid_argument error)
			{
				return false;
			}

			root = newRoot;
			return true;
		}

		void Remove(const Key& key)
		{
			Node* newRoot = nullptr;

			try
			{
				newRoot = RemoveRecursive(root, key);
			}
			catch (std::invalid_argument error)
			{
				throw error;
			}

			root = newRoot;
		}

		bool TryRemove(const Key& key)
		{
			Node* newRoot = nullptr;

			try
			{
				newRoot = RemoveRecursive(root, key);
			}
			catch (std::invalid_argument error)
			{
				return false;
			}

			root = newRoot;
			return true;
		}

		bool Contains(const Key& key) const
		{
			Node* node = SearchRecursive(root, key);

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
		Node* InsertRecursive(Node* node, const Key& key, const Value& value)
		{
			if (node == nullptr)
			{
				++size;
				return new Node(value);
			}

			Key nodeKey = keySelector(node->value);

			if (nodeKey > key)
			{
				Node* newLeft = InsertRecursive(node->left, key, value);
				node->left = newLeft;
			}
			else if (nodeKey < key)
			{
				Node* newRight = InsertRecursive(node->right, key, value);
				node->right = newRight;
			}
			else
			{
				throw std::invalid_argument("Already contains value with key = " + key);
			}

			return BalanceNode(node, key);
		}

		Node* RemoveRecursive(Node* node, const Key& key)
		{
			if (node == nullptr)
			{
				throw ::std::invalid_argument("Doesn't contain value with key = " + key);
			}

			Key nodeKey = keySelector(node->value);

			if (nodeKey > key)
			{
				Node* newLeft = RemoveRecursive(node->left, key);
				node->left = newLeft;
			}
			else if (nodeKey < key)
			{
				Node* newRight = RemoveRecursive(node->right, key);
				node->right = newRight;
			}
			else
			{
				return RemoveNode(node);
			}

			return BalanceNode(node);
		}

		Node* SearchRecursive(Node* node, const Key& key) const
		{
			if (node == nullptr)
			{
				return node;
			}

			Key nodeKey = keySelector(node->value);

			if (nodeKey > key)
			{
				return SearchRecursive(node->left, key);
			}

			if (nodeKey < key)
			{
				return SearchRecursive(node->right, key);
			}

			return node;
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
				node->right = RemoveRecursive(node->right, keySelector(temp->value));
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

		Node* BalanceNode(Node* node, const Key& newKey)
		{
			int bf = GetBalanceFactorOf(node);

			if (bf > 1)
			{
				Key lKey = keySelector(node->left->value);

				// Left Left Case  
				if (newKey <= lKey)
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
				Key rKey = keySelector(node->right->value);

				// Right Right Case  
				if (newKey >= rKey)
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
		KeySelector keySelector;
	};
}
