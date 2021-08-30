#pragma once
#include "../Collection/ICollection.h"
#include "../Collection/IIterable.h"
#include <utility>

namespace Structs
{
	template <typename Key, typename Value, typename Iterator, typename Pair = std::pair<Key, Value>>
	class IMap : public IIterable<Pair, Iterator>, public ICollection
	{
	protected:
		IMap() = default;
		IMap(const IMap& set) = delete;
		IMap& operator=(const IMap& set) = delete;

	public:
		virtual ~IMap() = default;

	public:
		virtual void Insert(const Pair& keyValuePair) = 0;
		virtual void Insert(const Key& key, const Value& value) = 0;
		virtual void Remove(const Key& key) = 0;
		virtual bool Contains(const Key& key) = 0;

		virtual bool TryInsert(const Pair& keyValuePair) = 0;
		virtual bool TryInsert(const Key& key, const Value& value) = 0;
		virtual bool TryRemove(const Key& key) = 0;
	};
}
