#include "gtest/gtest.h"
#include "Tree/AVLTree.h"
#include <vector>

class AVLTreeTest : public testing::Test
{
public:
	Structs::AVLTree<int> tree;

	void FillWith10Numbers()
	{
		for (int i = 0; i < 10; ++i)
		{
			tree.Insert(i);
		}
	}

	void FillWith10RandomNumbers()
	{
		srand(time(NULL));

		for (int i = 0; i < 10; ++i)
		{
			tree.Insert(rand());
		}
	}
};

class AVLTreeParametrizedTestWith10Values :
	public AVLTreeTest,
	public testing::WithParamInterface<int>
{};

class AVLTreeParametrizedTestWithMultipleValues :
	public AVLTreeTest,
	public testing::WithParamInterface<std::vector<int>>
{};



INSTANTIATE_TEST_CASE_P(
	Tree10ValuesTests,
	AVLTreeParametrizedTestWith10Values,
	testing::Values(
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	));

INSTANTIATE_TEST_CASE_P(
	TreeInsertMultipleValuesThrowsNoExcpetion,
	AVLTreeParametrizedTestWithMultipleValues,
	testing::Values(
		std::vector<int> {1, 2, 3, 4, 5},
		std::vector<int> {0, 1, 711, 1989, 2013},
		std::vector<int> {643, 2, 12, 456435, 1}
));


TEST_P(AVLTreeParametrizedTestWith10Values, TreeInsertOneValueThrowsNoExcpetion)
{
	int value = GetParam();

	ASSERT_NO_THROW(
		tree.Insert(value);
	);
};

TEST_P(AVLTreeParametrizedTestWith10Values, TreeInsertOneValueInsertsValue)
{
	int value = GetParam();
	tree.Insert(value);

	ASSERT_EQ(tree.Contains(value), true);
};

TEST_P(AVLTreeParametrizedTestWith10Values, TreeInsertAlreadyContainingElementThrowsException)
{
	FillWith10Numbers();
	int value = GetParam();

	ASSERT_THROW(tree.Insert(value), std::invalid_argument);
};

TEST_P(AVLTreeParametrizedTestWith10Values, TreeRemoveValidValueThrowsNoExcpetion)
{
	FillWith10Numbers();
	int value = GetParam();

	ASSERT_NO_THROW(
		tree.Remove(value);
	);
}

TEST_P(AVLTreeParametrizedTestWith10Values, TreeRemoveValidValueRemovesValue)
{
	FillWith10Numbers();
	int value = GetParam();
	tree.Remove(value);

	ASSERT_EQ(tree.Contains(value), false);
}

TEST_P(AVLTreeParametrizedTestWith10Values, TreeRemoveValidValueDoesntRemoveOtherValues)
{
	FillWith10Numbers();
	int value = GetParam();

	tree.Remove(value);

	for (int i = 0; i < 10; ++i)
	{
		if (i == value)
		{
			continue;
		}

		ASSERT_EQ(tree.Contains(value), false);
	}
}

TEST_P(AVLTreeParametrizedTestWith10Values, TreeContainsValidValueThrowsNoExcpetion)
{
	FillWith10Numbers();
	int value = GetParam();

	ASSERT_NO_THROW(
		tree.Contains(value);
	);
}

TEST_P(AVLTreeParametrizedTestWith10Values, TreeContainsInvalidValueThrowsNoExcpetion)
{
	int value = GetParam();

	ASSERT_NO_THROW(
		tree.Contains(value);
	);
}

TEST_P(AVLTreeParametrizedTestWith10Values, TreeContainsValidValueReturnsTrue)
{
	FillWith10Numbers();
	int value = GetParam();

	ASSERT_EQ(tree.Contains(value), true);
}

TEST_P(AVLTreeParametrizedTestWith10Values, TreeContainsInvalidValueReturnsFalse)
{
	int value = GetParam();

	ASSERT_EQ(tree.Contains(value), false);
}

TEST_F(AVLTreeTest, TreeIteratorOnEmptyTreeThrowsNoExcpetion)
{
	ASSERT_NO_THROW(
		for (int value : tree)
		{
		}
	);
}

TEST_F(AVLTreeTest, TreeIteratorOnNotEmptyTreeThrowsNoExcpetion)
{
	FillWith10Numbers();

	ASSERT_NO_THROW(
		for (int value : tree)
		{
		}
	);
}

TEST_P(AVLTreeParametrizedTestWithMultipleValues, TreeIteratorReturnValuesInOrder)
{
	std::vector<int> values = GetParam();

	for (int value : values)
	{
		tree.Insert(value);
	}

	int previousValue = std::numeric_limits<int>::min();

	for (int value : tree)
	{
		ASSERT_TRUE(previousValue < value);
		previousValue = value;
	}
}

TEST_P(AVLTreeParametrizedTestWithMultipleValues, TreeInsertMultipleValuesThrowsNoExcpetion)
{
	auto& values = GetParam();

	for (int value : values)
	{
		ASSERT_NO_THROW(
			tree.Insert(value);
		);
	}
}