#include "gtest/gtest.h"
#include "Set/UnorderedSet.h"
#include <vector>

class UnorderedSetTest : public testing::Test
{
public:
	Structs::UnorderedSet<int> set;
	void FillWith10Numbers()
	{
		for (int i = 0; i < 10; ++i)
		{
			set.Insert(i);
		}
	}
};

class UnorderedSetParametrizedTestWith10Values :
	public UnorderedSetTest,
	public testing::WithParamInterface<int>
{};

class UnorderedSetParametrizedTestWithMultipleValues :
	public UnorderedSetTest,
	public testing::WithParamInterface<std::vector<int>>
{};



INSTANTIATE_TEST_CASE_P(
	UnorderedSet10ValuesTests,
	UnorderedSetParametrizedTestWith10Values,
	testing::Values(
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	));

INSTANTIATE_TEST_CASE_P(
	UnorderedSetInsertMultipleValuesTests,
	UnorderedSetParametrizedTestWithMultipleValues,
	testing::Values(
		std::vector<int> {1, 2, 3, 4, 5},
		std::vector<int> {0, 1, 711, 1989, 2013},
		std::vector<int> {643, 2, 12, 456435, 1}
));


TEST_P(UnorderedSetParametrizedTestWith10Values, UnorderedSetInsertOneValueThrowsNoExcpetion)
{
	int value = GetParam();

	ASSERT_NO_THROW(
		set.Insert(value);
	);
};

TEST_P(UnorderedSetParametrizedTestWith10Values, UnorderedSetInsertOneValueInsertsValue)
{
	int value = GetParam();
	set.Insert(value);

	ASSERT_EQ(set.Contains(value), true);
};

TEST_P(UnorderedSetParametrizedTestWith10Values, UnorderedSetInsertAlreadyContainingElementThrowsException)
{
	FillWith10Numbers();
	int value = GetParam();

	ASSERT_THROW(set.Insert(value), std::invalid_argument);
};

TEST_P(UnorderedSetParametrizedTestWith10Values, UnorderedSetTryInsertAlreadyContainingElementThrowsNoException)
{
	FillWith10Numbers();
	int value = GetParam();

	ASSERT_NO_THROW(
		set.TryInsert(value)
	);
};

TEST_P(UnorderedSetParametrizedTestWith10Values, UnorderedSetTryInsertAlreadyContainingElementReturnsFalse)
{
	FillWith10Numbers();
	int value = GetParam();

	ASSERT_EQ(set.TryInsert(value), false);
};

TEST_P(UnorderedSetParametrizedTestWith10Values, UnorderedSetTryInsertElementReturnsTrue)
{
	int value = GetParam();

	ASSERT_EQ(set.TryInsert(value), true);
};

TEST_P(UnorderedSetParametrizedTestWith10Values, UnorderedSetTryInsertElementThrowsNoException)
{
	int value = GetParam();

	ASSERT_NO_THROW(
		set.TryInsert(value)
	);
};

TEST_P(UnorderedSetParametrizedTestWith10Values, UnorderedSetRemoveValidValueThrowsNoExcpetion)
{
	FillWith10Numbers();
	int value = GetParam();

	ASSERT_NO_THROW(
		set.Remove(value);
	);
}

TEST_P(UnorderedSetParametrizedTestWith10Values, UnorderedSetRemoveValidValueRemovesValue)
{
	FillWith10Numbers();
	int value = GetParam();
	set.Remove(value);

	ASSERT_EQ(set.Contains(value), false);
}

TEST_P(UnorderedSetParametrizedTestWith10Values, UnorderedSetRemoveValidValueDoesntRemoveOtherValues)
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

TEST_P(UnorderedSetParametrizedTestWith10Values, UnorderedSetContainsValidValueThrowsNoExcpetion)
{
	FillWith10Numbers();
	int value = GetParam();

	ASSERT_NO_THROW(
		set.Contains(value);
	);
}

TEST_P(UnorderedSetParametrizedTestWith10Values, UnorderedSetContainsInvalidValueThrowsNoExcpetion)
{
	int value = GetParam();

	ASSERT_NO_THROW(
		set.Contains(value);
	);
}

TEST_P(UnorderedSetParametrizedTestWith10Values, UnorderedSetContainsValidValueReturnsTrue)
{
	FillWith10Numbers();
	int value = GetParam();

	ASSERT_EQ(set.Contains(value), true);
}

TEST_P(UnorderedSetParametrizedTestWith10Values, UnorderedSetContainsInvalidValueReturnsFalse)
{
	int value = GetParam();

	ASSERT_EQ(set.Contains(value), false);
}

TEST_F(UnorderedSetTest, UnorderedSetIteratorOnEmptyTreeThrowsNoExcpetion)
{
	ASSERT_NO_THROW(
		for (int value : set)
		{
		}
	);
}

TEST_F(UnorderedSetTest, UnorderedSetIteratorOnNotEmptyTreeThrowsNoExcpetion)
{
	FillWith10Numbers();

	ASSERT_NO_THROW(
		for (int value : set)
		{
		}
	);
}

TEST_P(UnorderedSetParametrizedTestWithMultipleValues, UnorderedSetInsertMultipleValuesThrowsNoExcpetion)
{
	auto& values = GetParam();

	for (int value : values)
	{
		ASSERT_NO_THROW(
			set.Insert(value);
		);
	}
}