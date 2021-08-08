#include <gtest/gtest.h>
#include "List/List.h"

struct ListTest : testing::Test
{
public:
	Structs::List<int> list;

	void FillWith10Numbers()
	{
		for (int i = 0; i < 10; ++i)
		{
			list.AddLast(i);
		}
	}
};


TEST_F(ListTest, ListAddFirstThrowsNoException)
{
	ASSERT_NO_THROW(
		list.AddFirst(0);
	);
}

TEST_F(ListTest, ListAddLastThrowsNoException)
{
	ASSERT_NO_THROW(
		list.AddLast(0);
	);
}

TEST_F(ListTest, ListInsertThrowsNoException)
{
	FillWith10Numbers();

	ASSERT_NO_THROW(
		list.Insert(10, 10);
	);

	for (size_t i = 0; i < list.GetSize(); ++i)
	{
		ASSERT_EQ(list[i], i);
	}

	ASSERT_NO_THROW(
		list.Insert(-1, 0);
	);
}

TEST_F(ListTest, ListInsertThrowsException)
{
	FillWith10Numbers();

	ASSERT_ANY_THROW(
		list.Insert(0, 15);
	);

	ASSERT_ANY_THROW(
		list.Insert(0, -1);
	);
}

TEST_F(ListTest, ListRemoveLastThrowException)
{
	ASSERT_ANY_THROW(
		list.RemoveFirst();
	);
}

TEST_F(ListTest, ListRemoveFirstInEmptyListThrowException)
{
	ASSERT_ANY_THROW(
		list.RemoveFirst();
	);
}

TEST_F(ListTest, ListRemoveLastInEmptyListThrowException)
{
	ASSERT_ANY_THROW(
		list.RemoveLast();
	);
}

TEST_F(ListTest, ListIteratorThrowsNoException)
{
	FillWith10Numbers();

	ASSERT_NO_THROW(
		for (auto i : list)
		{
			int a = i;
		}
	);
}

TEST_F(ListTest, ListRemoveLastThrowsNoException)
{
	FillWith10Numbers();

	ASSERT_NO_THROW(
		list.RemoveLast();
	);

	ASSERT_EQ(list.GetSize(), 9);
}

TEST_F(ListTest, ListRemoveFirstThrowsNoException)
{
	FillWith10Numbers();

	ASSERT_NO_THROW(
		list.RemoveFirst();
	);

	ASSERT_EQ(list.GetSize(), 9);
}

TEST_F(ListTest, ListRemoveAtValidRangeThrowsNoException)
{
	FillWith10Numbers();

	ASSERT_NO_THROW(
		list.RemoveAt(5);
	);

	ASSERT_EQ(list.GetSize(), 9);
}

TEST_F(ListTest, ListRemoveAtInvalidRangeThrowsException)
{
	FillWith10Numbers();

	ASSERT_ANY_THROW(
		list.RemoveAt(10);
	);

	ASSERT_ANY_THROW(
		list.RemoveAt(-1);
	);
}
