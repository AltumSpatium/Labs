// Тернарный поиск. Оценка сложности с доказательством. Реализация

/*
Троичный поиск (ternary search, тернарный поиск) — метод поиска минимума или
максимума функции на отрезке, которая либо сначала строго возрастает, затем строго
убывает, либо наоборот.
Троичный поиск определяет, что минимум или максимум не может лежать либо в первой,
либо в последней трети области, и затем повторяет поиск на оставшихся двух третях.
Троичный поиск демонстрирует парадигму программирования «разделяй и властвуй».

Тернарный поиск заключается в следующем. Пусть есть рекурсивная функция
search(L, R), которая по двум концам отрезка L, R определяет минимум на орезке
[L, R]. Если R — L < eps, то мы уже вычислили точку, где достигается минимум,
с точностью eps. Иначе, разделим отрезок [L,R] на три равных по длине
отрезка [L, A], [A, B] и [B, R]. Сравним значение в точках А и В. Вспомнив,
что функция f выпуклая, можно сделать вывод, что если f(A) > f(B), то минимум
лежит на отрезке [A,R]. Иначе — на отрезке [L, B]. В соответсвии с этим, можно
рекурсивно запуститься от одного из отрезков [L, B] или [A, R]. Каждый раз длина
области поиска уменьшается в полтора раза, значит, минимум на отрезке длины
X с точностью eps будет найден за время O(log(X/eps)).

Время работы:
Так как на каждой итерации мы считаем два значения функции и уменьшаем область
поиска в полтора раза, пока r - l > e, то время работы алгоритма составит
2 * log_{3/2}((r - l) / e)
*/

// Реализация:

/* Вещественная функция */
double f(double x)
{
	return x;
}

/* Троичный поиск, за O(2 * log_{3/2}((r - l) / e)) */
double ternarysearchmin(double l, double r, double e)
{
	while (r - l > e)
	{
		double a = (l * 2 + r) / 3, b = (l + r * 2) / 3;

		if (f(a) < f(b))
			r = b;
		else
			l = a;
	}

	return (l + r) / 2;
}