#include "gtest/gtest.h"
#include "Array/Queue.h"
#include <vector>

class QueueTest : public testing::Test
{
public:
	Structs::Queue<int> Queue;

	void FillWith10Numbers()
	{
		for (int i = 0; i < 10; ++i)
		{
			Queue.Enqueue(i);
		}
	}

	void FillWith10RandomNumbers()
	{
		srand(time(NULL));

		for (int i = 0; i < 10; ++i)
		{
			Queue.Enqueue(rand());
		}
	}
};

class QueueParametrizedTestWith10Values :
	public QueueTest,
	public testing::WithParamInterface<int>
{};

class QueueParametrizedTestWithMultipleValues :
	public QueueTest,
	public testing::WithParamInterface<std::vector<int>>
{};



INSTANTIATE_TEST_CASE_P(
	Queue10ValuesTests,
	QueueParametrizedTestWith10Values,
	testing::Values(
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	));

INSTANTIATE_TEST_CASE_P(
	QueueInsertMultipleValuesTests,
	QueueParametrizedTestWithMultipleValues,
	testing::Values(
		std::vector<int> {1, 2, 3, 4, 5},
		std::vector<int> {0, 1, 711, 1989, 2013},
		std::vector<int> {643, 2, 12, 456435, 1}
));


TEST_P(QueueParametrizedTestWith10Values, QueueEnqueueOneValueThrowsNoExcpetion)
{
	int value = GetParam();

	ASSERT_NO_THROW(
		Queue.Enqueue(value);
	);
};

TEST_P(QueueParametrizedTestWith10Values, QueueEnqueueOneValueInsertsValue)
{
	int value = GetParam();
	Queue.Enqueue(value);

	ASSERT_EQ(Queue.Contains(value), true);
};

TEST_F(QueueTest, QueuePeekNotEmptyThrowsNoExcpetion)
{
	FillWith10Numbers();

	ASSERT_NO_THROW(
		Queue.Peek();
	);
}

TEST_F(QueueTest, QueuePeekEmptyThrowsExcpetion)
{
	ASSERT_THROW(Queue.Peek(), std::out_of_range);
}

TEST_F(QueueTest, QueueDequeueNotEmptyThrowsNoExcpetion)
{
	FillWith10Numbers();

	ASSERT_NO_THROW(
		Queue.Dequeue();
	);
}



TEST_F(QueueTest, QueuePopEmptyThrowsExcpetion)
{
	ASSERT_THROW(Queue.Dequeue(), std::out_of_range);
}

TEST_P(QueueParametrizedTestWith10Values, QueueContainsValidValueThrowsNoExcpetion)
{
	FillWith10Numbers();
	int value = GetParam();

	ASSERT_NO_THROW(
		Queue.Contains(value);
	);
}

TEST_P(QueueParametrizedTestWith10Values, QueueContainsInvalidValueThrowsNoExcpetion)
{
	int value = GetParam();

	ASSERT_NO_THROW(
		Queue.Contains(value);
	);
}

TEST_P(QueueParametrizedTestWith10Values, QueueContainsValidValueReturnsTrue)
{
	FillWith10Numbers();
	int value = GetParam();
	ASSERT_EQ(Queue.Contains(value), true);
}

TEST_P(QueueParametrizedTestWith10Values, QueueContainsInvalidValueReturnsFalse)
{
	int value = GetParam();
	ASSERT_EQ(Queue.Contains(value), false);
}

TEST_F(QueueTest, QueueIteratorOnEmptyTreeThrowsNoExcpetion)
{
	ASSERT_NO_THROW(
		for (int value : Queue)
		{
		}
	);
}

TEST_F(QueueTest, QueueIteratorOnNotEmptyTreeThrowsNoExcpetion)
{
	FillWith10Numbers();

	ASSERT_NO_THROW(
		for (int value : Queue)
		{
		}
	);
}

TEST_P(QueueParametrizedTestWithMultipleValues, QueueDequeueAndPeekReturnsSameValue)
{
	std::vector<int> values = GetParam();

	for (int value : values)
	{
		Queue.Enqueue(value);
	}

	while (!(Queue.IsEmpty()))
	{
		auto peek = Queue.Peek();
		auto pop = Queue.Dequeue();
		ASSERT_EQ(peek, pop);
	}
}

TEST_P(QueueParametrizedTestWithMultipleValues, QueueIteratorReturnValuesInOrder)
{
	std::vector<int> values = GetParam();

	for (int value : values)
	{
		Queue.Enqueue(value);
	}

	size_t index = 0;

	for (int value : Queue)
	{
		ASSERT_EQ(value, values[index]);
		++index;
	}
}

TEST_P(QueueParametrizedTestWithMultipleValues, QueueDequeueReturnsValuesInOrder)
{
	std::vector<int> values = GetParam();

	for (int value : values)
	{
		Queue.Enqueue(value);
	}

	size_t index = 0;

	while (!(Queue.IsEmpty()))
	{
		auto pop = Queue.Dequeue();
		ASSERT_EQ(pop, values[index]);
		++index;
	}
}

TEST_P(QueueParametrizedTestWithMultipleValues, QueueEnqueueMultipleValuesThrowsNoExcpetion)
{
	auto& values = GetParam();

	for (int value : values)
	{
		ASSERT_NO_THROW(
			Queue.Enqueue(value);
		);
	}
}