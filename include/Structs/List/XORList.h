#pragma once
#include "IList.h"
#include <stdexcept>
#include <string>
#include <tuple>

namespace Structs
{
#pragma region Node
	template <typename T>
	struct XORListNode
	{
		XORListNode(const T& value) :
			value(value),
			npx(nullptr)
		{}

		XORListNode(const T& value, XORListNode* previous, XORListNode* next) :
			value(value),
			npx(GetNodesNPX(previous, next))
		{}

	public:
		void SetNPX(XORListNode* const value) { npx = value; }
		XORListNode* const GetNPX() const { return npx; }

		T& GetValue() { return value; }

	private:
		T value;
		XORListNode* npx;
	};

	template<typename T>
	XORListNode<T>* GetNodesNPX(XORListNode<T>* a, XORListNode<T>* b)
	{
		return (XORListNode<T>*)((uintptr_t)(a) ^ (uintptr_t)(b));
	}

	template<typename T>
	XORListNode<T>* GetNextXORNode(XORListNode<T>* node, XORListNode<T>* previous)
	{
		return GetNodesNPX(node->GetNPX(), previous);
	}

	template<typename T>
	XORListNode<T>* GetPreviousXORNode(XORListNode<T>* node, XORListNode<T>* next)
	{
		return GetNodesNPX(node->GetNPX(), next);
	}
#pragma endregion

#pragma region Iterator
	template <typename T>
	class XORListIterator final : IIterator<T, XORListIterator<T>>
	{
	private:
		using Node = XORListNode<T>;

	public:
		XORListIterator()
			: XORListIterator(nullptr, nullptr, nullptr)
		{}

		XORListIterator(Node* const previous, Node* const current, Node* const next)
			: previous(previous), current(current), next(next)
		{}

	public:
		virtual XORListIterator& operator++() override
		{
			Node* newNext = nullptr;

			if (next != nullptr)
			{
				newNext = GetNextXORNode(next, current);
			}

			previous = current;
			current = next;
			next = newNext;
			return *this;
		}

		virtual XORListIterator& operator++(int) override
		{
			XORListIterator<T> tempIterator = *this;
			++(*this);
			return tempIterator;
		}

		XORListIterator& operator--()
		{
			Node* newPrevious = nullptr;

			if (next != nullptr)
			{
				newPrevious = GetPreviousXORNode(previous, current);
			}

			next = current;
			current = previous;
			previous = newPrevious;
			return *this;
		}

		XORListIterator operator--(int)
		{
			XORListIterator tempIterator = *this;
			--(*this);
			return tempIterator;
		}

		virtual bool operator==(const XORListIterator& rhs) const override
		{
			return current == rhs.current;
		}

		virtual bool operator!=(const XORListIterator& rhs) const override
		{
			return current != rhs.current;
		}

		virtual T& operator*() const override
		{
			return current->GetValue();

		}

		virtual T* operator->() const override
		{
			return &current->GetValue();
		}

	private:
		Node* previous;
		Node* current;
		Node* next;
	};
#pragma endregion

#pragma region List
	template <typename T>
	class XORList final : public IList<T, XORListIterator<T>>
	{
	public:
		using Node = XORListNode<T>;
		using Iterator = XORListIterator<T>;

	public:
		XORList()
			: head(nullptr), tail(nullptr), size(0)
		{}

		XORList(const T& value)
			: head(new Node(value)),
			tail(head),
			size(1)
		{}

		XORList(XORList&& list)
			: head(std::move(list.head)),
			tail(std::move(list.tail)),
			size(size)
		{
			list.head = nullptr;
			list.tail = nullptr;
			list.size = 0;
		}

		XORList& operator=(XORList&& rhs)
		{
			if (*this == rhs)
				return;

			Clear();
			head = std::move(rhs.head);
			tail = std::move(rhs.tail);
			size = size;

			rhs.head = nullptr;
			rhs.tail = nullptr;
			rhs.size = 0;
		}

		~XORList()
		{
			Clear();
		}

	public:
		bool operator==(const XORList& rhs) { return head == rhs.head; }
		bool operator!=(const XORList& rhs) { return !operator==(rhs); }

	public:
		virtual void Insert(const T& value, size_t index) override
		{
			if (index <= 0 || index >= size)
			{
				if (index == 0)
				{
					AddFirst(value);
					return;
				}

				if (index == size)
				{
					AddLast(value);
					return;
				}

				throw std::out_of_range(std::to_string(index));
			}

			std::tuple<Node*, Node*, Node*> nodes = GetNodeWithNextAndPreviousFromHead(value);


			Node* previous = GetPreviousXORNode(std::get<0>(nodes), std::get<1>(nodes));
			Node* newNode = new Node(value, std::get<0>(nodes), std::get<0>(nodes));
			std::get<0>(nodes)->SetNPX(GetNodesNPX(previous, newNode));
			std::get<1>(nodes)->SetNPX(GetNodesNPX(std::get<2>(nodes), newNode));
			++size;
		}

		virtual void AddFirst(const T& value) override
		{
			if (IsEmpty())
			{
				CreateHead(value);
				return;
			}

			Node* newHead = new Node(value, nullptr, head);
			head->SetNPX(GetNodesNPX(newHead, head->GetNPX()));
			head = newHead;
			++size;
		}

		virtual void AddLast(const T& value) override
		{
			if (IsEmpty())
			{
				CreateHead(value);
				return;
			}

			Node* newTail = new Node(value, tail, nullptr);
			tail->SetNPX(GetNodesNPX(newTail, tail->GetNPX()));
			tail = newTail;
			++size;
		}

	public:
		virtual void RemoveAt(size_t index) override
		{
			if (index <= 0 || index >= size - 1)
			{
				if (index == 0)
				{
					RemoveFirst();
					return;
				}

				if (index == size - 1)
				{
					RemoveLast();
					return;
				}

				throw std::out_of_range(std::to_string(index));
			}

			std::tuple<Node*, Node*, Node*> nodes = GetNodeWithNextAndPreviousByIndex(index);

			RemoveNodeInTuple(nodes);
		}

		virtual void Remove(const T& value) override
		{
			if (IsEmpty())
			{
				throw ::std::invalid_argument("Doesn't contain value = " + value);
			}

			std::tuple<Node*, Node*, Node*> nodes = GetNodeWithNextAndPreviousFromHead(value);

			RemoveNodeInTuple(nodes);
		}

		virtual void RemoveFirst() override
		{
			if (IsEmpty())
			{
				throw ::std::invalid_argument("Doesn't have values");
			}

			Node* newHead = GetNextXORNode<T>(head, nullptr);
			Node* next = GetNextXORNode(newHead, head);
			delete head;

			newHead->SetNPX(GetNodesNPX<T>(nullptr, next));
			head = newHead;
			--size;
		}

		virtual void RemoveLast() override
		{
			if (IsEmpty())
			{
				throw ::std::invalid_argument("Doesn't have values");
			}

			Node* newTail = GetPreviousXORNode<T>(tail, nullptr);
			Node* previous = GetPreviousXORNode(newTail, tail);
			delete tail;

			newTail->SetNPX(GetNodesNPX<T>(previous, nullptr));
			tail = newTail;
			--size;
		}

		virtual T& GetAt(size_t index) override { return GetNodeByIndex(index)->GetValue(); }
		virtual T& operator[](size_t index) override { return GetAt(index); }
		virtual T& GetFirst() override { return head->GetValue(); }
		virtual T& GetLast() override { return tail->GetValue(); }

	private:
		void CreateHead(const T& value)
		{
			head = new Node(value);
			tail = head;
			size = 1;
		}

		void RemoveNodeInTuple(std::tuple<Node*, Node*, Node*>& nodes)
		{
			if (std::get<1>(nodes) == nullptr)
			{
				throw ::std::invalid_argument("Doesn't contain value");
			}

			if (std::get<0>(nodes) == nullptr)
			{
				RemoveFirst();
				return;
			}

			if (std::get<2>(nodes) == nullptr)
			{
				RemoveLast();
				return;
			}

			Node* next = GetNextXORNode(std::get<2>(nodes), std::get<1>(nodes));
			Node* previous = GetPreviousXORNode(std::get<0>(nodes), std::get<1>(nodes));

			std::get<2>(nodes)->SetNPX(GetNodesNPX(std::get<0>(nodes), next));
			std::get<0>(nodes)->SetNPX(GetNodesNPX(std::get<2>(nodes), previous));

			delete std::get<1>(nodes);
			--size;
		}

	private:
		Node* GetNodeByIndex(size_t index)
		{
			if (index < size / 2)
			{
				return GetNodeFromHeadByIndex(index);
			}
			else
			{
				return GetNodeFromTailByIndex(size - 1 - index);
			}
		}

		Node* GetNodeFromHeadByIndex(size_t index)
		{
			Node* previous = nullptr;
			Node* current = head;

			for (size_t i = 0; i < index; ++i)
			{
				Node* newCurrent = GetNextXORNode(current, previous);
				previous = current;
				current = newCurrent;
			}

			return current;
		}

		Node* GetNodeFromTailByIndex(size_t index)
		{
			Node* next = nullptr;
			Node* current = tail;

			for (size_t i = 0; i < index; ++i)
			{
				Node* newCurrent = GetPreviousXORNode(current, next);
				next = current;
				current = newCurrent;
			}

			return current;
		}

		std::tuple<Node*, Node*, Node*> GetNodeWithNextAndPreviousByIndex(size_t index) const
		{
			if (index < size / 2)
			{
				return GetNodeWithNextAndPreviousFromHeadByIndex(index);
			}
			else
			{
				return GetNodeWithNextAndPreviousFromTailByIndex(size - 1 - index);
			}
		}

		std::tuple<Node*, Node*, Node*> GetNodeWithNextAndPreviousFromHeadByIndex(size_t index) const
		{
			Node* previous = nullptr;
			Node* current = head;
			Node* next = GetNextXORNode(current, previous);

			for (size_t i = 0; i < index; ++i)
			{
				Node* newNext = GetNextXORNode(current, previous);
				previous = current;
				current = next;
				next = newNext;
			}

			return std::tuple<Node*, Node*, Node*>(previous, current, next);
		}

		std::tuple<Node*, Node*, Node*> GetNodeWithNextAndPreviousFromTailByIndex(size_t index) const
		{
			Node* next = nullptr;
			Node* current = tail;
			Node* previous = GetPreviousXORNode(current, next);

			for (size_t i = 0; i < index; ++i)
			{
				Node* newPrevious = GetPreviousXORNode(previous, current);
				next = current;
				current = previous;
				previous = newPrevious;
			}

			return std::tuple<Node*, Node*, Node*>(previous, current, next);
		}

		std::tuple<Node*, Node*, Node*> GetNodeWithNextAndPreviousFromHead(const T& value) const
		{
			Node* previous = nullptr;
			Node* current = head;
			Node* next = GetNextXORNode(current, previous);

			while (current != nullptr && current->GetValue() != value)
			{
				Node* newNext = GetNextXORNode(next, current);
				previous = current;
				current = next;
				next = newNext;
			}

			return std::tuple<Node*, Node*, Node*>(previous, current, next);
		}

		std::tuple<Node*, Node*, Node*> GetNodeWithNextAndPreviousFromTail(const T& value) const
		{
			Node* next = nullptr;
			Node* current = tail;
			Node* previous = GetPreviousXORNode(current, next);

			while (current != nullptr && current->GetValue() != value)
			{
				Node* newPrevious = GetPreviousXORNode(previous, current);
				next = current;
				current = previous;
				previous = newPrevious;
			}

			return std::tuple<Node*, Node*, Node*>(previous, current, next);
		}

	public:
		size_t GetIndex(const T& value) const
		{
			size_t index = 0;

			for (T& listValue : *this)
			{
				if (listValue == value)
					return index;

				++index;
			}

			throw ::std::invalid_argument("Doesn't contain value = " + value);
		}

		virtual bool IsEmpty() const override { return head == nullptr; }
		virtual size_t GetSize() const override { return size; }

		virtual void Clear() override
		{
			Node* previous = nullptr;
			Node* current = head;
			head = nullptr;
			tail = nullptr;
			size = 0;

			while (current != nullptr)
			{
				Node* newCurrent = GetNextXORNode(current, previous);
				delete previous;

				previous = current;
				current = newCurrent;
			}

			delete previous;
		}

		virtual Iterator begin() const override { return Iterator(nullptr, head, GetNextXORNode<T>(head, nullptr)); }
		virtual Iterator end() const override { return Iterator(); }

	private:
		Node* head;
		Node* tail;
		size_t size;
	};
#pragma endregion
}