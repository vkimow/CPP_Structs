#include "gtest/gtest.h"
#include "Map/UnorderedMap.h"
#include <vector>
#include <string>

class UnorderedMapTest : public testing::Test
{
public:
	Structs::UnorderedMap<int, std::string> map;

	void FillWith10Numbers()
	{
		for (int i = 0; i < 10; ++i)
		{
			map.Insert(i, std::to_string(i));
		}
	}

	void FillWith10RandomNumbers()
	{
		srand(time(NULL));

		for (int i = 0; i < 10; ++i)
		{
			int random = rand();
			map.Insert(random, std::to_string(random));
		}
	}
};

class UnorderedMapParametrizedTestWith10Values :
	public UnorderedMapTest,
	public testing::WithParamInterface<std::pair<int, std::string>>
{};

class UnorderedMapParametrizedTestWithMultipleValues :
	public UnorderedMapTest,
	public testing::WithParamInterface<std::vector<std::pair<int, std::string>>>
{};



INSTANTIATE_TEST_CASE_P(
	UnorderedMap10ValuesTests,
	UnorderedMapParametrizedTestWith10Values,
	testing::Values(
		std::pair<int, std::string>{ 0, "0"},
		std::pair<int, std::string>{ 1, "1" },
		std::pair<int, std::string>{ 2, "2" },
		std::pair<int, std::string>{ 3, "3" },
		std::pair<int, std::string>{ 4, "4" },
		std::pair<int, std::string>{ 5, "5" },
		std::pair<int, std::string>{ 6, "6" },
		std::pair<int, std::string>{ 7, "7" },
		std::pair<int, std::string>{ 8, "8" },
		std::pair<int, std::string>{ 9, "9" }
));

INSTANTIATE_TEST_CASE_P(
	UnorderedMapInsertMultipleValuesTests,
	UnorderedMapParametrizedTestWithMultipleValues,
	testing::Values(
		std::vector<std::pair<int, std::string>> { {1, "1"}, { 2, "2" }, { 3, "3" }, { 4, "4" }, { 5, "5" }},
		std::vector<std::pair<int, std::string>> { {0, "0"}, { 1, "1" }, { 711, "711" }, { 1989, "1989" }, { 2013, "2013" }},
		std::vector<std::pair<int, std::string>> { { 643, "643" }, { 2, "2" }, { 12, "12" }, { 456435, "456435" }, { 1, "1" }}
));


TEST_P(UnorderedMapParametrizedTestWith10Values, UnorderedMapInsertOneValueThrowsNoExcpetion)
{
	std::pair<int, std::string> value = GetParam();

	ASSERT_NO_THROW(
		map.Insert(value);
	);
};

TEST_P(UnorderedMapParametrizedTestWith10Values, UnorderedMapInsertOneValueInsertsValue)
{
	std::pair<int, std::string> value = GetParam();
	map.Insert(value);

	ASSERT_EQ(map.Contains(value.first), true);
};

TEST_P(UnorderedMapParametrizedTestWith10Values, UnorderedMapInsertAlreadyContainingElementThrowsException)
{
	FillWith10Numbers();
	auto value = GetParam();

	ASSERT_THROW(map.Insert(value), std::invalid_argument);
};

TEST_P(UnorderedMapParametrizedTestWith10Values, UnorderedMapTryInsertAlreadyContainingElementThrowsNoException)
{
	FillWith10Numbers();
	auto value = GetParam();

	ASSERT_NO_THROW(
		map.TryInsert(value)
	);
};

TEST_P(UnorderedMapParametrizedTestWith10Values, UnorderedMapTryInsertAlreadyContainingElementReturnsFalse)
{
	FillWith10Numbers();
	auto value = GetParam();

	ASSERT_EQ(map.TryInsert(value), false);
};

TEST_P(UnorderedMapParametrizedTestWith10Values, UnorderedMapTryInsertElementReturnsTrue)
{
	auto value = GetParam();

	ASSERT_EQ(map.TryInsert(value), true);
};

TEST_P(UnorderedMapParametrizedTestWith10Values, UnorderedMapTryInsertElementThrowsNoException)
{
	auto value = GetParam();

	ASSERT_NO_THROW(
		map.TryInsert(value)
	);
};

TEST_P(UnorderedMapParametrizedTestWith10Values, UnorderedMapRemoveValidValueThrowsNoExcpetion)
{
	FillWith10Numbers();
	auto value = GetParam();

	ASSERT_NO_THROW(
		map.Remove(value.first);
	);
}

TEST_P(UnorderedMapParametrizedTestWith10Values, UnorderedMapRemoveValidValueRemovesValue)
{
	FillWith10Numbers();
	auto value = GetParam();
	map.Remove(value.first);

	ASSERT_EQ(map.Contains(value.first), false);
}

TEST_P(UnorderedMapParametrizedTestWith10Values, UnorderedMapRemoveValidValueDoesntRemoveOtherValues)
{
	FillWith10Numbers();
	std::pair<int, std::string> value = GetParam();

	map.Remove(value.first);

	for (int i = 0; i < 10; ++i)
	{
		if (i == value.first)
		{
			continue;
		}

		ASSERT_EQ(map.Contains(value.first), false);
	}
}

TEST_P(UnorderedMapParametrizedTestWith10Values, UnorderedMapContainsValidValueThrowsNoExcpetion)
{
	FillWith10Numbers();
	auto value = GetParam();

	ASSERT_NO_THROW(
		map.Contains(value.first);
	);
}

TEST_P(UnorderedMapParametrizedTestWith10Values, UnorderedMapContainsInvalidValueThrowsNoExcpetion)
{
	auto value = GetParam();

	ASSERT_NO_THROW(
		map.Contains(value.first);
	);
}

TEST_P(UnorderedMapParametrizedTestWith10Values, UnorderedMapContainsValidValueReturnsTrue)
{
	FillWith10Numbers();
	auto value = GetParam();

	ASSERT_EQ(map.Contains(value.first), true);
}

TEST_P(UnorderedMapParametrizedTestWith10Values, UnorderedMapContainsInvalidValueReturnsFalse)
{
	auto value = GetParam();

	ASSERT_EQ(map.Contains(value.first), false);
}

TEST_F(UnorderedMapTest, UnorderedMapIteratorOnEmptyTreeThrowsNoExcpetion)
{
	ASSERT_NO_THROW(
		for (auto& value : map)
		{
		}
	);
}

TEST_F(UnorderedMapTest, UnorderedMapIteratorOnNotEmptyTreeThrowsNoExcpetion)
{
	FillWith10Numbers();

	ASSERT_NO_THROW(
		for (auto& value : map)
		{
		}
	);
}

TEST_P(UnorderedMapParametrizedTestWithMultipleValues, UnorderedMapInsertMultipleValuesThrowsNoExcpetion)
{
	auto values = GetParam();

	for (auto& value : values)
	{
		ASSERT_NO_THROW(
			map.Insert(value);
		);
	}
}