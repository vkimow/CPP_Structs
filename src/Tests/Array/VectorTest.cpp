#include "gtest/gtest.h"
#include "Array/Vector.h"
#include <vector>

class VectorTest : public testing::Test
{
public:
	Structs::Vector<int> Vector;

	void FillWith10Numbers()
	{
		for (int i = 0; i < 10; ++i)
		{
			Vector.Add(i);
		}
	}

	void FillWith10RandomNumbers()
	{
		srand(time(NULL));

		for (int i = 0; i < 10; ++i)
		{
			Vector.Add(rand());
		}
	}
};

class VectorParametrizedTestWith10Values :
	public VectorTest,
	public testing::WithParamInterface<int>
{};

class VectorParametrizedTestWithMultipleValues :
	public VectorTest,
	public testing::WithParamInterface<std::vector<int>>
{};



INSTANTIATE_TEST_CASE_P(
	Vector10ValuesTests,
	VectorParametrizedTestWith10Values,
	testing::Values(
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	));

INSTANTIATE_TEST_CASE_P(
	VectorInsertMultipleValuesTests,
	VectorParametrizedTestWithMultipleValues,
	testing::Values(
		std::vector<int> {1, 2, 3, 4, 5},
		std::vector<int> {0, 1, 711, 1989, 2013},
		std::vector<int> {643, 2, 12, 456435, 1}
));


TEST_P(VectorParametrizedTestWith10Values, VectorAddOneValueThrowsNoExcpetion)
{
	int value = GetParam();

	ASSERT_NO_THROW(
		Vector.Add(value);
	);
};

TEST_P(VectorParametrizedTestWith10Values, VectorInsertOneValueInsertsValue)
{
	int value = GetParam();
	Vector.Add(value);

	ASSERT_EQ(Vector.Contains(value), true);
};

TEST_P(VectorParametrizedTestWith10Values, VectorRemoveValidValueThrowsNoExcpetion)
{
	FillWith10Numbers();
	int value = GetParam();

	ASSERT_NO_THROW(
		Vector.Remove(value);
	);
}

TEST_P(VectorParametrizedTestWith10Values, VectorRemoveValidValueRemovesValue)
{
	FillWith10Numbers();
	int value = GetParam();
	Vector.Remove(value);

	ASSERT_EQ(Vector.Contains(value), false);
}

TEST_P(VectorParametrizedTestWith10Values, VectorRemoveValidValueDoesntRemoveOtherValues)
{
	FillWith10Numbers();
	int value = GetParam();

	Vector.Remove(value);

	for (int i = 0; i < 10; ++i)
	{
		if (i == value)
		{
			continue;
		}

		ASSERT_EQ(Vector.Contains(value), false);
	}
}

TEST_P(VectorParametrizedTestWith10Values, VectorContainsValidValueThrowsNoExcpetion)
{
	FillWith10Numbers();
	int value = GetParam();

	ASSERT_NO_THROW(
		Vector.Contains(value);
	);
}

TEST_P(VectorParametrizedTestWith10Values, VectorContainsInvalidValueThrowsNoExcpetion)
{
	int value = GetParam();

	ASSERT_NO_THROW(
		Vector.Contains(value);
	);
}

TEST_P(VectorParametrizedTestWith10Values, VectorContainsValidValueReturnsTrue)
{
	FillWith10Numbers();
	int value = GetParam();

	ASSERT_EQ(Vector.Contains(value), true);
}

TEST_P(VectorParametrizedTestWith10Values, VectorContainsInvalidValueReturnsFalse)
{
	int value = GetParam();

	ASSERT_EQ(Vector.Contains(value), false);
}

TEST_F(VectorTest, VectorIteratorOnEmptyTreeThrowsNoExcpetion)
{
	ASSERT_NO_THROW(
		for (int value : Vector)
		{
		}
	);
}

TEST_F(VectorTest, VectorIteratorOnNotEmptyTreeThrowsNoExcpetion)
{
	FillWith10Numbers();

	ASSERT_NO_THROW(
		for (int value : Vector)
		{
		}
	);
}

TEST_P(VectorParametrizedTestWithMultipleValues, VectorIteratorReturnValuesInOrder)
{
	std::vector<int> values = GetParam();

	for (int value : values)
	{
		Vector.Add(value);
	}

	size_t index = 0;

	for (int value : Vector)
	{
		ASSERT_EQ(value, values[index]);
		++index;
	}
}

TEST_P(VectorParametrizedTestWithMultipleValues, VectorInsertMultipleValuesThrowsNoExcpetion)
{
	auto& values = GetParam();

	for (int value : values)
	{
		ASSERT_NO_THROW(
			Vector.Add(value);
		);
	}
}