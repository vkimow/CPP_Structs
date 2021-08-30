#include <gtest/gtest.h>
#include "List/XORList.h"

struct XORListTest : testing::Test
{
public:
	Structs::XORList<int> list;

	void FillWith10Numbers()
	{
		for (int i = 0; i < 10; ++i)
		{
			list.AddLast(i);
		}
	}
};


TEST_F(XORListTest, XORListAddFirstThrowsNoException)
{
	ASSERT_NO_THROW(
		list.AddFirst(0);
	);
}

TEST_F(XORListTest, XORListAddLastThrowsNoException)
{
	ASSERT_NO_THROW(
		list.AddLast(0);
	);
}

TEST_F(XORListTest, XORListInsertThrowsNoException)
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

TEST_F(XORListTest, XORListInsertThrowsException)
{
	FillWith10Numbers();

	ASSERT_ANY_THROW(
		list.Insert(0, 15);
	);

	ASSERT_ANY_THROW(
		list.Insert(0, -1);
	);
}

TEST_F(XORListTest, XORListRemoveLastInEmptyThrowsException)
{
	ASSERT_ANY_THROW(
		list.RemoveLast();
	);
}

TEST_F(XORListTest, XORListRemoveFirstInEmptyThrowsException)
{
	ASSERT_ANY_THROW(
		list.RemoveFirst();
	);
}

TEST_F(XORListTest, XORListIteratorThrowsNoException)
{
	FillWith10Numbers();

	ASSERT_NO_THROW(
		for (auto i : list)
		{
		}
	);
}

TEST_F(XORListTest, XORListRemoveLastThrowsNoException)
{
	FillWith10Numbers();

	ASSERT_NO_THROW(
		list.RemoveLast();
	);

	ASSERT_EQ(list.GetSize(), 9);
}

TEST_F(XORListTest, XORListRemoveFirstThrowsNoException)
{
	FillWith10Numbers();

	ASSERT_NO_THROW(
		list.RemoveFirst();
	);

	ASSERT_EQ(list.GetSize(), 9);
}

TEST_F(XORListTest, XORListRemoveAtValidRangeThrowsNoException)
{
	FillWith10Numbers();

	ASSERT_NO_THROW(
		list.RemoveAt(5);
	);

	ASSERT_EQ(list.GetSize(), 9);
}

TEST_F(XORListTest, XORListRemoveAtInvalidRangeThrowsException)
{
	FillWith10Numbers();

	ASSERT_ANY_THROW(
		list.RemoveAt(10);
	);

	ASSERT_ANY_THROW(
		list.RemoveAt(-1);
	);
}
