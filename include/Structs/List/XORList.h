#pragma once
#include "IList.h"
#include "ListIterator.h"
#include <stdexcept>
#include <string>

namespace Structs
{
#pragma region Node
	template <typename T>
	struct ListNode : IListNode<T>
	{
		ListNode(const T& value, ListNode* previous = nullptr, ListNode* next = nullptr) :
			value(value), previous(previous), next(next)
		{}

	public:
		void SetNext(ListNode* const node) { next = node; }
		void SetPrevious(ListNode* const node) { previous = node; }

		virtual void SetValue(const T& value) override { this->value = value; }
		virtual T& GetValue() override { return value; }

		virtual bool HasNext() const override { return next != nullptr; }
		virtual bool HasPrevious() const override { return previous != nullptr; }

		virtual ListNode* const GetNext() const override { return next; }
		virtual ListNode* const GetPrevious() const override { return previous; }

	private:
		T value;
		ListNode* next;
		ListNode* previous;
	};
#pragma endregion

#pragma region List
	template <typename T>
	class List final : public IList<T, ListIterator<T>>
	{
	public:
		using Node = ListNode<T>;
		using Iterator = ListIterator<T>;

	public:
		List()
			: head(nullptr),
			tail(nullptr),
			size(0)
		{}

		List(const T& value)
			: head(new Node(value, nullptr, nullptr)),
			tail(head),
			size(1)
		{}

		List(List&& list)
			: head(std::move(list.head)),
			tail(std::move(list.tail)),
			size(size)
		{
			list.head = nullptr;
			list.tail = nullptr;
			list.size = 0;
		}

		List& operator=(List&& rhs)
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

		~List()
		{
			Clear();
		}

	public:
		bool operator==(const List& rhs) { return head == rhs.head; }
		bool operator!=(const List& rhs) { return !operator==(rhs); }

	public:
		virtual void Insert(const T& value, size_t index) override
		{
			if (index <= 0 || index >= size)
			{
				if (index == 0)
				{
					AddFirst(0);
					return;
				}

				if (index == size)
				{
					AddLast(value);
					return;
				}

				throw std::out_of_range(std::to_string(index));
			}

			Node* nodeToMove = GetNode(index);
			Node* newNode = new Node(value, nodeToMove->GetPrevious(), nodeToMove);
			nodeToMove->GetPrevious()->SetNext(newNode);
			nodeToMove->SetPrevious(newNode);
			++size;
		}

		virtual void AddFirst(const T& value) override
		{
			if (IsEmpty())
			{
				CreateHead(value);
				return;
			}

			Node* node = new Node(value, nullptr, head);
			head->SetPrevious(node);
			head = node;
			++size;
		}

		virtual void AddLast(const T& value) override
		{
			if (IsEmpty())
			{
				CreateHead(value);
				return;
			}

			Node* node = new Node(value, tail, nullptr);
			tail->SetNext(node);
			tail = node;
			++size;
		}

	public:
		virtual void RemoveAt(size_t index) override
		{
			if (index < 0 || index >= size)
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

			Node* nodeToRemove = GetNode(index);
			Node* previous = nodeToRemove->GetPrevious();
			Node* next = nodeToRemove->GetNext();

			previous->SetNext(nodeToRemove->GetNext());
			next->SetPrevious(nodeToRemove->GetPrevious());

			delete nodeToRemove;
			--size;
		}

		virtual void Remove(const T& value) override { RemoveAt(GetIndex(value)); }

		virtual void RemoveFirst() override
		{
			if (IsEmpty())
			{
				throw std::out_of_range(std::to_string(0));
			}

			if (size == 1)
			{
				Clear();
				return;
			}

			Node* nodeToRemove = head;
			head = head->GetNext();
			head->SetPrevious(nullptr);

			delete nodeToRemove;
			--size;
		}

		virtual void RemoveLast() override
		{
			if (IsEmpty())
			{
				throw std::out_of_range(std::to_string(size - 1));
			}

			if (size == 1)
			{
				Clear();
				return;
			}

			Node* nodeToRemove = tail;
			tail = tail->GetPrevious();
			tail->SetNext(nullptr);

			delete nodeToRemove;
			--size;
		}

		virtual T& GetAt(size_t index) override { return GetNode(index)->GetValue(); }
		virtual T& operator[](size_t index) override { return GetAt(index); }
		virtual T& GetFirst() override { return head->GetValue(); }
		virtual T& GetLast() override { return tail->GetValue(); }

	private:
		void CreateHead(const T& value)
		{
			head = new Node(value, nullptr, nullptr);
			tail = head;
			size = 1;
		}

		Node* GetNode(size_t index) const
		{
			if (index < size / 2)
			{
				return GetFromHead(index);
			}
			else
			{
				return GetFromTail(size - 1 - index);
			}
		}

		Node* GetFromHead(size_t index) const
		{
			Node* node = head;

			for (int i = 0; i < index; ++i)
			{
				node = node->GetNext();
			}

			return node;
		}

		Node* GetFromTail(size_t index) const
		{
			Node* node = tail;

			for (int i = 0; i < index; ++i)
			{
				node = node->GetPrevious();
			}

			return node;
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
			if (size == 0)
				return;

			Node* node = head;
			head = nullptr;
			tail = nullptr;
			size = 0;

			while (node != nullptr)
			{
				Node* temp = node;
				node = node->GetNext();
				delete temp;
			}
		}

		virtual Iterator begin() const override { return Iterator(head); }
		virtual Iterator end() const override { return Iterator(nullptr); }

	private:
		Node* head;
		Node* tail;
		size_t size;
	};
#pragma endregion
}