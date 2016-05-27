// Списки. Реализация поиска, вставки, удаления элемента

/*
Список — это абстрактный тип данных,
представляющий собой упорядоченный набор значений,
в котором некоторое значение может встречаться более одного раза.

Связный спиисок — базовая динамическая структура данных
в информатике, состоящая из узлов, каждый из которых содержит
как собственно данные, так и одну или две ссылки («связки»)
на следующий и/или предыдущий узел списка.
*/

// Реализация:

/* Узел списка */
class Node
{
public:
	int value;
	Node *prev;
	Node *next;

	Node(int _value)
	{
		value = _value;
	}
};

/* Двусвязный список */
class List
{
public:
	int size = 0;
	Node *last = NULL;
	Node *first = NULL;

	List() {}

	void push_front(int _value)
	{
		Node *newNode = new Node(_value);

		if (size == 0)
		{
			last = newNode;
			first = newNode;
		}
		else
		{
			first->prev = newNode;
			newNode->prev = NULL;
			newNode->next = first;
			first = newNode;
		}
		size++;
	}

	void push_back(int _value)
	{
		Node *newNode = new Node(_value);

		if (size == 0)
		{
			last = newNode;
			first = newNode;
		}
		else
		{
			last->next = newNode;
			newNode->prev = last;
			newNode->next = NULL;
			last = newNode;
		}
		size++;
	}

	void insert(int _index, int _value)
	{
		Node *current = first;
		Node *newNode;

		if (_index == 0)
		{
			push_front(_value);
			return;
		}
		if (_index == size + 1)
		{
			push_back(_value);
			return;
		}

		for (int i = 0; i < _index; i++)
		{
			current = current->next;
		}

		newNode = new Node(_value);

		newNode->prev = current->prev;
		newNode->next = current;
		current->prev->next = newNode;
		current->prev = newNode;
		size++;
	}

	int pop_front()
	{
		int ret_value;

		if (size == 1)
		{
			ret_value = first->value;
			delete first;
			last = NULL;
			first = NULL;
			size--;

			return ret_value;
		}
		if (size > 1)
		{
			Node *tmp = first->next;
			ret_value = first->value;
			delete first;
			first = tmp;
			first->prev = NULL;
			size--;

			return ret_value;
		}
	}

	int pop_back()
	{
		int ret_value;

		if (size == 1)
		{
			ret_value = last->value;
			delete last;
			last = NULL;
			first = NULL;
			size--;

			return ret_value;			
		}
		if (size > 1)
		{
			Node *tmp = last->prev;
			ret_value = last->value;
			delete last;
			last = tmp;
			last->next = NULL;
			size--;

			return ret_value;
		}
	}

	int erase(int _index)
	{
		int ret_value;
		Node *current = first;

		if (_index > size)
		{
			return NULL;
		}

		if (_index == 0)
		{
			return pop_front();
		}

		if (_index == size)
		{
			return pop_back();
		}

		for (int i = 0; i < _index; i++)
		{
			current = current->next;
		}

		current->prev->next = current->next;
		current->next->prev = current->prev;
		ret_value = current->value;
		delete current;
		size--;

		return ret_value;
	}

	bool search(int _value)
	{
		Node *current = first;

		for (int i = 0; i < size; i++)
		{
			if (current->value == _value)
			{
				return true;
			}

			current = current->next;
		}

		return false;
	}
};