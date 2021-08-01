#pragma once

template <typename T>
class List
{
public:
	List();
	~List();

	void push_back(T &data);
	void push_front(T &data);
	T &pop_back();
	T &pop_front();

	inline bool IsEmpty() const  { return head == nullptr; }
	inline int GetSize() const  { return size; }

	T& operator[](const int index) const;

private:
	template <typename T>
	struct Node
	{
	public:
		T &data;
		Node *next;
		Node(T &_data, Node *_next = nullptr): data(_data), next(_next) {}
	};

	int size;
	Node<T> *head;
	Node<T> *tail;
};
