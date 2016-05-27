// Очередь. Реализация вставки, удаления элемента

/*
О́чередь — абстрактный тип данных с дисциплиной доступа
к элементам «первый пришёл — первый вышел»
(FIFO, First In — First Out)
*/

// Реализация:

/* Узел очереди */
class Node
{
public:
	int value;
	Node *next;

	Node(int _value)
	{
		value = _value;
	}
};

/* Очередь */
class Queue
{
public:
	int size = 0;
	Node *first = NULL;
	Node *last = NULL;

	Queue() {}

	void enqueue(int _value)
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
			last = newNode;
			last->next = NULL;
		}
		size++;
	}

	int dequeue()
	{
		int ret_value;
		Node *tmp = first->next;
		ret_value = first->value;
		delete first;
		first = tmp;
		size--;

		return ret_value;
	}
};