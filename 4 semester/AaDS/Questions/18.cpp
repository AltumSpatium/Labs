// Пирамидальная сортировка. Реализация сортировки. Реализация функции просеивания вниз

/*
Массив можно отсортировать, если на его основе строить и перестраивать сортирующее дерево,
известное как двоичная куча или просто пирамида. 
Что есть сортирующее дерево? Это дерево, у которого любой родитель больше (или меньше,
смотря в какую сторону оно сортирующее) чем его потомки. 
Как из обычного дерева сделать сортирующее дерево? Очень просто – нужно двигаться от
потомков вверх к родителям и если потомок больше родителя, то менять местами. Если
такой обмен произошёл, опустившегося на один уровень родителя нужно сравнить с
потомками ниже – может и там тоже будет повод для обмена.
Преобразовывая неотсортированную часть массива в сортирующее дерево, в итоге в корень
«всплывёт» наибольший элемент. Обмениваем максимум с последним ключом 
неотсортированного подмассива. Структура перестанет быть сортирующим деревом,
но в качестве моральной компенсации его неотсортированная часть станет меньше
на один узел. К этой неотсортированной части заново применим всю процедуру,
то есть преобразуем её в сортирующее дерево с последующей перестановкой найденного
максимума в конец. И так действуем до тех пор, пока неотсортированная часть не
скукожится до одного-единственного элемента.
Подход, что и говорить, остроумный, но при этом специалисты по алгоритмам отмечают у
сортировки кучей целую кучу недостатков, как-то: неустойчивость, хаотичность
выборки, нечувствительность к почти упорядоченным массивам и пр. Смущает
всех также неулучшаемая скорость O(n log n).

Сортировка пирамидой использует бинарное сортирующее дерево. Сортирующее дерево — это такое
дерево, у которого выполнены условия:
1. Каждый лист имеет глубину либо d, либо d-1, d — максимальная глубина дерева.
2. Значение в любой вершине не меньше (другой вариант — не больше) значения её потомков.
Удобная структура данных для сортирующего дерева — такой массив Array,
что Array[1] — элемент в корне, а потомки элемента Array[i] являются
Array[2i+1] и Array[2i+2].

Алгоритм сортировки будет состоять из двух основных шагов:
1. Выстраиваем элементы массива в виде сортирующего дерева:
Array[i] >= Array[2i+1]
Array[i] >= Array[2i+2]
при 1 <= i < n/2.
Этот шаг требует O(n) операций.
2. Будем удалять элементы из корня по одному за раз и перестраивать дерево. То есть на
первом шаге обмениваем Array[1] и Array[n], преобразовываем
Array[1], Array[2], … , Array[n-1] в сортирующее дерево. Затем переставляем
Array[1] и Array[n-1], преобразовываем Array[1], Array[2], … , Array[n-2] в сортирующее
дерево. Процесс продолжается до тех пор, пока в сортирующем дереве не останется
один элемент. Тогда Array[1], Array[2], … , Array[n] — упорядоченная последовательность.
Этот шаг требует O(nlogn) операций.
*/

// Реализация:

/* Куча */
class Heap
{
	std::vector<int> a;
	int tail = a.size();

	Heap() {}

	int top() { return a[0]; }

	void delete()
	{
		swap(a[0], a[tail - 1]);
		a.pop_back();
		siftdown(0);
	}

	void siftdown(int i)
	{
		int l = 2 * i + 1;
		int r = 2 * i + 2;

		while (l < tail)
		{
			int max = l;

			if (r < tail && a[l] < a[r])
				max = r;
			if (a[i] < a[max])
				swap(a[i], a[max]);
			i = max;
			l = 2 * i + 1;
			r = 2 * i + 2;
		}
	}
};

/* Сортировка кучей, за O(nlogn) */
void heap_sort(Heap h, int n)
{
	for (int i = 0; i < n; i++)
	{
		a[i] = h.top();
		h.pop();
	}
}