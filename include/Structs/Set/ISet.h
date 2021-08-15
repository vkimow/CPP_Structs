#pragma once
#include "../Collection/ICollection.h"
#include "../Collection/IIterable.h"

namespace Structs
{
	template <typename T, typename Iterator>
	class ISet : public IIterable<T, Iterator>, public ICollection
	{
	protected:
		ISet() = default;
		ISet(const ISet& set) = delete;
		ISet& operator=(const ISet& set) = delete;

	public:
		virtual ~ISet() = default;

	public:
		virtual void Insert(const T& value) = 0;
		virtual void Remove(const T& value) = 0;
		virtual bool Contains(const T& value) = 0;

		virtual bool TryInsert(const T& value) = 0;
		virtual bool TryRemove(const T& value) = 0;
	};
}
