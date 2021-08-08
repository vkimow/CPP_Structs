#pragma once
#include "../Collection/ICollection.h"
#include <stdexcept>
#include <exception>
#include <string>
#include <stack>

namespace Structs
{
	template <typename T>
	class IBinaryTreeNode : public IBinaryTreeRelationsNode
	{
	public:
		virtual void SetValue(const T& value) = 0;
		virtual T& GetValue() = 0;

		virtual IBinaryTreeNode* const GetLeft() const override = 0;
		virtual IBinaryTreeNode* const GetRight() const override = 0;
		virtual IBinaryTreeNode* const GetChild(bool isRight) const override = 0;
	};


	class IBinaryTreeRelationsNode
	{
	public:
		IBinaryTreeRelationsNode() = default;
		IBinaryTreeRelationsNode(const IBinaryTreeRelationsNode& node) = delete;
		IBinaryTreeRelationsNode& operator=(const IBinaryTreeRelationsNode& node) = delete;

		~IBinaryTreeRelationsNode() = default;

		virtual bool HasLeft() const = 0;
		virtual bool HasRight() const = 0;
		virtual bool HasChild(bool isRight) const = 0;

		virtual IBinaryTreeRelationsNode* const GetLeft() const = 0;
		virtual IBinaryTreeRelationsNode* const GetRight() const = 0;
		virtual IBinaryTreeRelationsNode* const GetChild(bool isRight) const = 0;
	};

	template <typename T, typename Iterator>
	class IBinaryTree : public IIterable<T, Iterator>, public ICollection
	{
	public:
		IBinaryTree() = default;
		IBinaryTree(const IBinaryTree& tree) = delete;
		IBinaryTree& operator=(const IBinaryTree& tree) = delete;

		virtual ~IBinaryTree() = default;

	public:
		virtual void Insert(const T& value) = 0;
		virtual void Remove(const T& value) = 0;
		virtual bool Contains(const T& value) const = 0;
	};
}
