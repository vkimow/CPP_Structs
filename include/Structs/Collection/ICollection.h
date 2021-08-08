#pragma once
#include "IIterable.h"


namespace Structs
{
	class ICollection
	{
	public:
		virtual ~ICollection() = default;

		virtual size_t GetSize() const = 0;
		virtual bool IsEmpty() const = 0;
		virtual void Clear() = 0;
	};
}