#pragma once

namespace Structs {
	namespace Keys {
		template<typename Key, typename Value>
		struct Selector
		{
		public:
			virtual Key operator()(const Value& value) const = 0;
		};

		template<typename Key, typename Value, typename Pair = std::pair<Key,Value>>
		struct PairSelector final : Selector<Key, Pair>
		{
		public:
			Key operator()(const Pair& value) const override final
			{
				return value.first;
			}
		};

		template<typename Key>
		struct NoSelector final : Selector<Key, Key>
		{
		public:
			Key operator()(const Key& value) const override final
			{
				return value;
			}
		};
	}
}
