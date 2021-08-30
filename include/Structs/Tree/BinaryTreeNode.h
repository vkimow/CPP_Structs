#pragma once

namespace Structs
{
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

	template <typename T>
	class BinaryTreeNode : public IBinaryTreeNode<T>
	{
	public:
		BinaryTreeNode(const T& value)
			:BinaryTreeNode(value, nullptr, nullptr)
		{}

		BinaryTreeNode(const T& value, BinaryTreeNode* right, BinaryTreeNode* left)
			: value(value), right(right), left(left)
		{}

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

	template<typename Node, typename = std::is_base_of<IBinaryTreeRelationsNode, Node>>
	Node* GetMostRightChildOf(Node* node)
	{
		while (node->HasRight())
		{
			node = node->GetRight();
		}

		return node;
	}

	template<typename Node, typename = std::is_base_of<IBinaryTreeRelationsNode, Node>>
	Node* GetMostLeftChildOf(Node* node)
	{
		while (node->HasLeft())
		{
			node = node->GetLeft();
		}

		return node;
	}

	template<typename Node, typename = std::is_base_of<IBinaryTreeRelationsNode, Node>>
	Node* GetMostLeftParentOf(Node* node)
	{
		if (!node->HasLeft())
		{
			return nullptr;
		}

		while (node->GetLeft()->HasLeft())
		{
			node = node->GetLeft();
		}

		return node;
	}

	template<typename Node, typename = std::is_base_of<IBinaryTreeRelationsNode, Node>>
	Node* GetMostRightParentOf(Node* node)
	{
		if (!node->HasRight())
		{
			return nullptr;
		}

		while (node->GetRight()->HasRight())
		{
			node = node->GetRight();
		}

		return node;
	}

	template<typename Node, typename = std::is_base_of<IBinaryTreeRelationsNode, Node>>
	size_t GetHeightOf(const Node* const node)
	{
		size_t hLeft = 0;
		size_t hRight = 0;

		if (node->HasLeft())
		{
			hLeft = GetHeightOf(node->GetLeft());
		}

		if (node->HasRight())
		{
			hRight = GetHeightOf(node->GetRight());
		}

		if (hLeft >= hRight)
		{
			return ++hLeft;
		}
		else
		{
			return ++hRight;
		}
	}
}