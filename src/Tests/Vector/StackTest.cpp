#include "gtest/gtest.h"
#include "Array/Stack.h"
#include <vector>

class StackTest : public testing::Test
{
public:
	Structs::Stack<int> Stack;

	void FillWith10Numbers()
	{
		for (int i = 0; i < 10; ++i)
		{
			Stack.Push(i);
		}
	}

	void FillWith10RandomNumbers()
	{
		srand(time(NULL));

		for (int i = 0; i < 10; ++i)
		{
			Stack.Push(rand());
		}
	}
};

class StackParametrizedTestWith10Values :
	public StackTest,
	public testing::WithParamInterface<int>
{};

class StackParametrizedTestWithMultipleValues :
	public StackTest,
	public testing::WithParamInterface<std::vector<int>>
{};



INSTANTIATE_TEST_CASE_P(
	Stack10ValuesTests,
	StackParametrizedTestWith10Values,
	testing::Values(
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	));

INSTANTIATE_TEST_CASE_P(
	StackInsertMultipleValuesTests,
	StackParametrizedTestWithMultipleValues,
	testing::Values(
		std::vector<int> {1, 2, 3, 4, 5},
		std::vector<int> {0, 1, 711, 1989, 2013},
		std::vector<int> {643, 2, 12, 456435, 1}
));


TEST_P(StackParametrizedTestWith10Values, StackPushOneValueThrowsNoExcpetion)
{
	int value = GetParam();

	ASSERT_NO_THROW(
		Stack.Push(value);
	);
};

TEST_P(StackParametrizedTestWith10Values, StackPushOneValueInsertsValue)
{
	int value = GetParam();
	Stack.Push(value);

	ASSERT_EQ(Stack.Contains(value), true);
};

TEST_F(StackTest, StackPeekNotEmptyThrowsNoExcpetion)
{
	FillWith10Numbers();

	ASSERT_NO_THROW(
		Stack.Peek();
	);
}

TEST_F(StackTest, StackPeekEmptyThrowsExcpetion)
{
	ASSERT_THROW(Stack.Peek(), std::out_of_range);
}

TEST_F(StackTest, StackPopNotEmptyThrowsNoExcpetion)
{
	FillWith10Numbers();

	ASSERT_NO_THROW(
		Stack.Pop();
	);
}



TEST_F(StackTest, StackPopEmptyThrowsExcpetion)
{
	ASSERT_THROW(Stack.Pop(), std::out_of_range);
}

TEST_P(StackParametrizedTestWith10Values, StackContainsValidValueThrowsNoExcpetion)
{
	FillWith10Numbers();
	int value = GetParam();

	ASSERT_NO_THROW(
		Stack.Contains(value);
	);
}

TEST_P(StackParametrizedTestWith10Values, StackContainsInvalidValueThrowsNoExcpetion)
{
	int value = GetParam();

	ASSERT_NO_THROW(
		Stack.Contains(value);
	);
}

TEST_P(StackParametrizedTestWith10Values, StackContainsValidValueReturnsTrue)
{
	FillWith10Numbers();
	int value = GetParam();
	ASSERT_EQ(Stack.Contains(value), true);
}

TEST_P(StackParametrizedTestWith10Values, StackContainsInvalidValueReturnsFalse)
{
	int value = GetParam();
	ASSERT_EQ(Stack.Contains(value), false);
}

TEST_F(StackTest, StackIteratorOnEmptyTreeThrowsNoExcpetion)
{
	ASSERT_NO_THROW(
		for (int value : Stack)
		{
		}
	);
}

TEST_F(StackTest, StackIteratorOnNotEmptyTreeThrowsNoExcpetion)
{
	FillWith10Numbers();

	ASSERT_NO_THROW(
		for (int value : Stack)
		{
		}
	);
}

TEST_P(StackParametrizedTestWithMultipleValues, StackPopAndPeekReturnsSameValue)
{
	std::vector<int> values = GetParam();

	for (int value : values)
	{
		Stack.Push(value);
	}

	while (!(Stack.IsEmpty()))
	{
		auto peek = Stack.Peek();
		auto pop = Stack.Pop();
		ASSERT_EQ(peek, pop);
	}
}

TEST_P(StackParametrizedTestWithMultipleValues, StackIteratorReturnValuesInOrder)
{
	std::vector<int> values = GetParam();

	for (int value : values)
	{
		Stack.Push(value);
	}

	size_t index = 0;

	for (int value : Stack)
	{
		ASSERT_EQ(value, values[index]);
		++index;
	}
}

TEST_P(StackParametrizedTestWithMultipleValues, StackPopReturnsValuesInOrder)
{
	std::vector<int> values = GetParam();

	for (int value : values)
	{
		Stack.Push(value);
	}

	size_t index = 0;

	while (!(Stack.IsEmpty()))
	{
		auto pop = Stack.Pop();
		ASSERT_EQ(pop, values[values.size() - index - 1]);
		++index;
	}
}

TEST_P(StackParametrizedTestWithMultipleValues, StackInsertMultipleValuesThrowsNoExcpetion)
{
	auto& values = GetParam();

	for (int value : values)
	{
		ASSERT_NO_THROW(
			Stack.Push(value);
		);
	}
}