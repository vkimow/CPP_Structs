#pragma once
#include "../Collection/ICollection.h"
#include "../Collection/IIterable.h"
#include <stdexcept>
#include <string>

namespace Structs
{
	template <typename T, typename Iterator>
	class IList : public IIterable<T, Iterator>, public ICollection
	{
	protected:
		IList() = default;
		IList(const IList& list) = delete;
		IList& operator=(const IList& list) = delete;

	public:
		virtual ~IList() = default;

	public:
		virtual void Insert(const T& value, size_t index) = 0;
		virtual void AddFirst(const T& value) = 0;
		virtual void AddLast(const T& value) = 0;

		virtual void RemoveFirst() = 0;
		virtual void RemoveLast() = 0;
		virtual void Remove(const T& value) = 0;
		virtual void RemoveAt(size_t index) = 0;

		virtual T& operator[](size_t index) = 0;
		virtual T& GetAt(size_t index) = 0;
		virtual T& GetFirst() = 0;
		virtual T& GetLast() = 0;
	};
}