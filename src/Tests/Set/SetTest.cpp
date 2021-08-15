#include "gtest/gtest.h"
#include "Set/Set.h"
#include <vector>

class SetTest : public testing::Test
{
public:
	Structs::Set<int> set;

	void FillWith10Numbers()
	{
		for (int i = 0; i < 10; ++i)
		{
			set.Insert(i);
		}
	}

	void FillWith10RandomNumbers()
	{
		srand(time(NULL));

		for (int i = 0; i < 10; ++i)
		{
			set.Insert(rand());
		}
	}
};

class SetParametrizedTestWith10Values :
	public SetTest,
	public testing::WithParamInterface<int>
{};

class SetParametrizedTestWithMultipleValues :
	public SetTest,
	public testing::WithParamInterface<std::vector<int>>
{};



INSTANTIATE_TEST_CASE_P(
	Set10ValuesTests,
	SetParametrizedTestWith10Values,
	testing::Values(
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	));

INSTANTIATE_TEST_CASE_P(
	SetInsertMultipleValuesTests,
	SetParametrizedTestWithMultipleValues,
	testing::Values(
		std::vector<int> {1, 2, 3, 4, 5},
		std::vector<int> {0, 1, 711, 1989, 2013},
		std::vector<int> {643, 2, 12, 456435, 1}
));


TEST_P(SetParametrizedTestWith10Values, SetInsertOneValueThrowsNoExcpetion)
{
	int value = GetParam();

	ASSERT_NO_THROW(
		set.Insert(value);
	);
};

TEST_P(SetParametrizedTestWith10Values, SetInsertOneValueInsertsValue)
{
	int value = GetParam();
	set.Insert(value);

	ASSERT_EQ(set.Contains(value), true);
};

TEST_P(SetParametrizedTestWith10Values, SetInsertAlreadyContainingElementThrowsException)
{
	FillWith10Numbers();
	int value = GetParam();

	ASSERT_THROW(set.Insert(value), std::invalid_argument);
};

TEST_P(SetParametrizedTestWith10Values, SetTryInsertAlreadyContainingElementThrowsNoException)
{
	FillWith10Numbers();
	int value = GetParam();

	ASSERT_NO_THROW(
		set.TryInsert(value)
	);
};

TEST_P(SetParametrizedTestWith10Values, SetTryInsertAlreadyContainingElementReturnsFalse)
{
	FillWith10Numbers();
	int value = GetParam();

	ASSERT_EQ(set.TryInsert(value), false);
};

TEST_P(SetParametrizedTestWith10Values, SetTryInsertElementReturnsTrue)
{
	int value = GetParam();

	ASSERT_EQ(set.TryInsert(value), true);
};

TEST_P(SetParametrizedTestWith10Values, SetTryInsertElementThrowsNoException)
{
	int value = GetParam();

	ASSERT_NO_THROW(
		set.TryInsert(value)
	);
};

TEST_P(SetParametrizedTestWith10Values, SetRemoveValidValueThrowsNoExcpetion)
{
	FillWith10Numbers();
	int value = GetParam();

	ASSERT_NO_THROW(
		set.Remove(value);
	);
}

TEST_P(SetParametrizedTestWith10Values, SetRemoveValidValueRemovesValue)
{
	FillWith10Numbers();
	int value = GetParam();
	set.Remove(value);

	ASSERT_EQ(set.Contains(value), false);
}

TEST_P(SetParametrizedTestWith10Values, SetRemoveValidValueDoesntRemoveOtherValues)
{
	FillWith10Numbers();
	int value = GetParam();

	set.Remove(value);

	for (int i = 0; i < 10; ++i)
	{
		if (i == value)
		{
			continue;
		}

		ASSERT_EQ(set.Contains(value), false);
	}
}

TEST_P(SetParametrizedTestWith10Values, SetContainsValidValueThrowsNoExcpetion)
{
	FillWith10Numbers();
	int value = GetParam();

	ASSERT_NO_THROW(
		set.Contains(value);
	);
}

TEST_P(SetParametrizedTestWith10Values, SetContainsInvalidValueThrowsNoExcpetion)
{
	int value = GetParam();

	ASSERT_NO_THROW(
		set.Contains(value);
	);
}

TEST_P(SetParametrizedTestWith10Values, SetContainsValidValueReturnsTrue)
{
	FillWith10Numbers();
	int value = GetParam();

	ASSERT_EQ(set.Contains(value), true);
}

TEST_P(SetParametrizedTestWith10Values, SetContainsInvalidValueReturnsFalse)
{
	int value = GetParam();

	ASSERT_EQ(set.Contains(value), false);
}

TEST_F(SetTest, SetIteratorOnEmptyTreeThrowsNoExcpetion)
{
	ASSERT_NO_THROW(
		for (int value : set)
		{
		}
	);
}

TEST_F(SetTest, SetIteratorOnNotEmptyTreeThrowsNoExcpetion)
{
	FillWith10Numbers();

	ASSERT_NO_THROW(
		for (int value : set)
		{
		}
	);
}

TEST_P(SetParametrizedTestWithMultipleValues, SetIteratorReturnValuesInOrder)
{
	std::vector<int> values = GetParam();

	for (int value : values)
	{
		set.Insert(value);
	}

	int previousValue = std::numeric_limits<int>::min();

	for (int value : set)
	{
		ASSERT_TRUE(previousValue < value);
		previousValue = value;
	}
}

TEST_P(SetParametrizedTestWithMultipleValues, SetInsertMultipleValuesThrowsNoExcpetion)
{
	auto& values = GetParam();

	for (int value : values)
	{
		ASSERT_NO_THROW(
			set.Insert(value);
		);
	}
}