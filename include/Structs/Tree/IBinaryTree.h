#pragma once
#include "../Collection/ICollection.h"
#include "../Collection/IIterable.h"

namespace Structs
{
	template <typename T>
	class IBinaryTreeNode : public IBinaryTreeRelationsNode
	{
	protected:
		IBinaryTreeNode() = default;

	public:
		virtual ~IBinaryTreeNode() = default;

	public:
		virtual T& GetValue() = 0;

		virtual IBinaryTreeNode* const GetLeft() const override = 0;
		virtual IBinaryTreeNode* const GetRight() const override = 0;
		virtual IBinaryTreeNode* const GetChild(bool isRight) const override = 0;
	};


	class IBinaryTreeRelationsNode
	{
	protected:
		IBinaryTreeRelationsNode() = default;
		IBinaryTreeRelationsNode(const IBinaryTreeRelationsNode& node) = delete;
		IBinaryTreeRelationsNode& operator=(const IBinaryTreeRelationsNode& node) = delete;

		~IBinaryTreeRelationsNode() = default;

	public:
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
	protected:
		IBinaryTree() = default;
		IBinaryTree(const IBinaryTree & tree) = delete;
		IBinaryTree& operator=(const IBinaryTree & tree) = delete;

	public:
		virtual ~IBinaryTree() = default;

	public:
		virtual void Insert(const T& value) = 0;
		virtual void Remove(const T& value) = 0;
		virtual bool Contains(const T& value) const = 0;

		virtual bool TryInsert(const T& value) = 0;
		virtual bool TryRemove(const T& value) = 0;
	};
}
