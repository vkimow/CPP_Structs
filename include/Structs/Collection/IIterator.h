#pragma once
#include <type_traits>

namespace Structs
{
	template<typename Type, typename Iterator>
	class IIterator
	{
	public:
		virtual ~IIterator() = default;

		virtual Iterator& operator++() = 0;
		virtual Iterator& operator++(int) = 0;

		virtual bool operator==(const Iterator& rhs) const = 0;
		virtual bool operator!=(const Iterator& rhs) const = 0;

		virtual Type& operator*() const = 0;
		virtual Type* operator->() const = 0;
	};
}