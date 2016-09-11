// Вещественный двоичный поиск. Реализация без зацикливания

/*
Вещественный двоичный поиск (англ. Bisection method)— алгоритм поиска аргумента
для заданного значения монотонной вещественной функции.

Ход работы алгоритма, разделенный на этапы, выглядит следующим образом:
1. зона поиска (на первом шаге ей является весь массив) делиться на две равные части,
путем определения ее среднего (mid) элемента;
2. средний элемент сравнивается с искомым (key), результатом этого сравнения будет
один из трех случаев:
 - key<mid. Крайней правой границей области поиска становится элемент, стоящий перед
 средним (right ← mid-1);
 - key>mid. Крайней левой границей области поиска становится следующий за средним
 элемент (left ← mid+1);
 - key=mid. Значения среднего и искомого элементов совпадают, следовательно элемент
 найден, работа алгоритма завершается.
3. если для проверки не осталось ни одного элемента, то алгоритм завершается,
иначе выполняется переход к пункту 1.

В случае, когда первое значение mid совпадает с ключом, тогда считается, что
алгоритм выполнился за свое лучшее время O(1). В среднем и худшем случае время работы
алгоритма составляет O(logn), что значительно быстрее, чем у линейного поиска,
требующего линейного времени.
*/

// Реализация:

/* Вещественная функция */
double f(double x)
{
	return x;
}

/* Вещественный двоичный поиск, за O(logn) */
double binarysearch(double l, double r, double key, double e)
{
	while(r - l > e)
	{
		double m = (l + r) / 2;

		if (f(m) < key)
			l = m;
		else
			r = m;
	}

	return (l + r) / 2;
}