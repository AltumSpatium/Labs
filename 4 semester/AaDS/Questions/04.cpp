// Реализация очереди на двух стеках. Реализация поиска минимального в очереди элемента за O(1)

/*
Из двух стеков можно сконструировать очередь
Амортизационное время выполнения операций 
enqueue (добавить в конец) и dequeue (извлечь из начала) 
будет O(1). Худшее время выполнения dequeue линейно завсисит 
от числа элементов.

Требуется добавить возможность нахождения минимума в стеке за O (1),
сохранив такой же асимптотику добавления и удаления элементов из 
стека.
Для этого будем хранить в стеке не сами элементы, а пары: 
элемент и минимум в стеке, начиная с этого элемента и ниже.

Понятно, что тогда нахождение минимума во всём стеке будет
заключаться просто во взятии значения stack.top().second.
Также очевидно, что при добавлении нового элемента в стек
величина second будет равна min (stack.top().second, new_element).
Удаление элемента из стека ничем не отличается от удаления
из обычного стека, поскольку удаляемый элемент никак не
мог повлиять на значения second для оставшихся элементов.

Заведём два стека: s1 и s2; разумеется, имеются в виду стеки,
модифицированные для нахождения минимума за O (1). 
Добавлять новые элементы будет всегда в стек s1,
а извлекать элементы - только из стека s2. При этом, если при
попытке извлечения элемента из стека s2 он оказался пустым,
просто перенесём все элементы из стека s1 в
стек s2 (при этом элементы в стеке s2 получатся уже в
обратном порядке, что нам и нужно для извлечения элементов;
стек s1 же станет пустым). Наконец, нахождение минимума в очереди
будет фактически заключаться в нахождении минимума из
минимума в стеке s1 и минимума в стеке s2.

Тем самым, мы выполняем все операции по-прежнему за O (1)
(по той простой причине, что каждый элемент в худшем случае
1 раз добавляется в стек s1, 1 раз переносится в стек s2
и 1 раз извлекается из стека s2)
*/

// Реализация:

/* Узел стека (значение + минимум для текущего и ниже) */
class Node
{
public:
	pair<int, int> val_min;
	Node *next;

	Node(int _value, int _min)
	{
		val_min = make_pair(_value, _min);
	}
};

/* Стек (с операцией поиска мин. элемента) */
class Stack
{
public:
	int size = 0;
	Node *top = NULL;

	Stack() {}

	void push(int _value)
	{
		Node *newNode;

		if (size == 0)
		{
			newNode = new Node(_value, _value);
			top = newNode;
		}
		else
		{
			newNode = new Node(_value, min(_value, top->val_min.second));
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
			ret_value = top->val_min.first;
			delete top;
			top = NULL;
			size--;

			return ret_value;
		}
		if (size > 1)
		{
			Node *tmp = top->next;
			ret_value = top->val_min.first;
			delete top;
			top = tmp;
			size--;

			return ret_value;
		}
	}

	int get_min()
	{
		if (size == 0)
			return 0;
		return top->val_min.second;
	}

	bool isEmpty()
	{
		if (size == 0) 
			return true;
		return false;
	}
};

/* Очередь на двух стеках */
class TwoStackQueue
{
public:
	int size = 0;
	Stack s1 = new Stack();
	Stack s2 = new Stack();

	TwoStackQueue() {}

	void enqueue(int _value)
	{
		s1.push(_value);
		size++;
	}

	int dequeue()
	{
		if (s2.isEmpty())
		{
			while (!s1.isEmpty())
				s2.push(s1.pop());
		}
		size--;
		return s2.pop()
	}

	int get_min()
	{
		if (s1.isEmpty())
			return s2.get_min();
		if (s2.isEmpty())
			return s1.get_min();
		if (s1.get_min() < s2.get_min())
			return s1.get_min();
		else return s2.get_min();
	}
}