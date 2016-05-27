// Стек. Реализация вставки, удаления элемента

/*
Стек  — абстрактный тип данных, представляющий собой
список элементов, организованных по принципу
LIFO (англ. last in — first out, «последним пришёл — первым вышел»)
*/

// Реализация:

/* Узел стека */
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

/* Стек */
class Stack
{
public:
	int size = 0;
	Node *top = NULL;

	Stack() {}

	void push(int _value)
	{
		Node *newNode = new Node(_value);

		if (size == 0)
			top = newNode;
		
		else
		{
			newNode->next = top;
			top = newNode;
		}
		size++;
	}

	int pop()
	{
		int ret_value;

		if (size == 1)
		{
			ret_value = top->value;
			delete top;
			top = NULL;
			size--;

			return ret_value;
		}
		if (size > 1)
		{
			Node *tmp = top->next;
			ret_value = top->value;
			delete top;
			top = tmp;
			size--;

			return ret_value;
		}
	}
};