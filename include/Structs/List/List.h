#pragma once
#include "../Collection/ICollection.h"
#include <stdexcept>
#include <string>

namespace Structs
{
#pragma region Node
	template <typename T>
	struct ListNode
	{
		T value;
		ListNode* next;
		ListNode* previous;

		ListNode(const T& value, ListNode* previous = nullptr, ListNode* next = nullptr) :
			value(value), previous(previous), next(next)
		{}
	};
#pragma endregion

#pragma region Iterator
	template <typename T>
	class ListIterator : IIterator<T, ListIterator<T>>
	{
	private:
		using Node = ListNode<T>;

	public:
		ListIterator(Node* node)
			: currentNode(node)
		{}

	public:
		virtual ListIterator& operator++() override
		{
			currentNode = currentNode->next;
			return *this;
		}

		virtual ListIterator& operator++(int) override
		{
			ListIterator<T> tempIterator = *this;
			++(*this);
			return tempIterator;
		}

		ListIterator& operator--()
		{
			currentNode = currentNode->previous;
			return *this;
		}

		ListIterator operator--(int)
		{
			ListIterator tempIterator = *this;
			--(*this);
			return tempIterator;
		}

		virtual bool operator==(const ListIterator& rhs) const override
		{
			return currentNode == rhs.currentNode;
		}

		virtual bool operator!=(const ListIterator& rhs) const override
		{
			return currentNode != rhs.currentNode;
		}

		virtual T& operator*() const override
		{
			return currentNode->value;

		}

		virtual T* operator->() const override
		{
			return &currentNode->value;
		}

	private:
		Node* currentNode;
	};
#pragma endregion

#pragma region List
	template <typename T>
	class List : public IIterable<T, ListIterator<T>>, public ICollection
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

		List(const List& list) = delete;

		List(List&& list)
			: head(std::move(list.head)),
			tail(std::move(list.tail)),
			size(std::move(list.size))
		{
			list.head = nullptr;
			list.tail = nullptr;
			list.size = 0;
		}

		List& operator=(const List& rhs) = delete;

		List& operator=(List&& rhs)
		{
			if (*this == rhs)
				return;

			Clear();
			head = std::move(rhs.head);
			tail = std::move(rhs.tail);
			size = std::move(rhs.size);

			rhs.head = nullptr;
			rhs.tail = nullptr;
			rhs.size = 0;
		}

		~List()
		{
			Clear();
		}

		bool operator==(const List& rhs) { return head == rhs.head; }
		bool operator!=(const List& rhs) { return !operator==(rhs); }

		void Insert(const T& value, size_t index)
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
			Node* newNode = new Node(value, nodeToMove->previous, nodeToMove);
			nodeToMove->previous->next = newNode;
			nodeToMove->previous = newNode;
			++size;
		}

		void AddFirst(const T& value)
		{
			if (IsEmpty())
			{
				CreateHead(value);
				return;
			}

			Node* node = new Node(value, nullptr, head);
			head->previous = node;
			head = node;
			++size;
		}

		void AddLast(const T& value)
		{
			if (IsEmpty())
			{
				CreateHead(value);
				return;
			}

			Node* node = new Node(value, tail, nullptr);
			tail->next = node;
			tail = node;
			++size;
		}

		void RemoveAt(size_t index)
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
			Node* previous = nodeToRemove->previous;
			Node* next = nodeToRemove->next;

			previous->next = nodeToRemove->next;
			next->previous = nodeToRemove->previous;

			delete nodeToRemove;
			--size;
		}

		void Remove(const T& value) { RemoveAt(GetIndex(value)); }

		void RemoveFirst()
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
			head = head->next;
			head->previous = nullptr;

			delete nodeToRemove;
			--size;
		}

		void RemoveLast()
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
			tail = tail->previous;
			tail->next = nullptr;

			delete nodeToRemove;
			--size;
		}

		T& Get(size_t index) const { return GetNode(index)->value; }
		T& operator[](size_t index) const { return Get(index); }
		T& GetFirst() const { return head->value; }
		T& GetLast() const { return tail->value; }

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
				node = node->next;
			}

			return node;
		}

		Node* GetFromTail(size_t index) const
		{
			Node* node = tail;

			for (int i = 0; i < index; ++i)
			{
				node = node->previous;
			}

			return node;
		}

	public:
		T& GetIndex(const T& value) const
		{

		}

		bool IsEmpty() const override { return head == nullptr; }
		size_t GetSize() const override { return size; }

		void Clear() override
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
				node = node->next;
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