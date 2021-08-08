#pragma once
#include "IIterator.h"
#include <type_traits>

namespace Structs
{
	template<typename Type, typename Iterator, typename std::enable_if<std::is_base_of<IIterator<Type, Iterator>, Iterator>::value>::type* = nullptr>
	class IIterable
	{
	public:
		virtual ~IIterable() = default;

		virtual Iterator begin() const = 0;
		virtual Iterator end() const = 0;
	};
}