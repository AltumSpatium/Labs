//  Решение online задачи static RMQ для отрезка произвольной длины с препроцессингом за O(n log n) и ответом на запрос за O(1). Реализация

/*
Строится разреженная таблица – это таблица ST[][] такая,
что ST[k][i] есть минимум на 
полуинтервале [A[i], A[i+2k]). Иными словами, она содержит
минимумы на всех отрезках, длина которых есть степень двойки.

[3] 0 0 0
[2] 3 2 2 2 0 0 0
[1] 3 6 4 2 2 5 0 0 1
[0] 3 8 6 4 2 5 9 0 7 1

[i] 1 2 3 4 5 6 7 8 9 10

Насчитаем массив ST[k][i] следующим образом. Понятно,
что ST[0] просто и есть наш массив A.
Далее воспользуемся понятным свойством: 
ST[k][i] = min(ST[k-1][i], ST[k-1][i + 2^(k — 1)]).

Заметим, что в нашей таблице O(nlogn) элементов, потому что
номера уровней должны быть не больше logn, т. к. при больших
значениях k длина полуинтервала становится больше
длины всего массива и хранить соответствующие значения
бессмысленно. И на каждом уровне O(n) элементов.

Заметим, что любой отрезок массива разбивается на два
перекрывающихся подотрезка длиною в степень двойки.
           _______
       _______
A[i] 3 8 6 4 2 5 9 0 7 1
       -----------

Получаем простую формулу для вычисления RMQ(i, j).
Если k = log(j – i + 1),
то RMQ(i, j) = min(ST(i, k), ST(j – 2^(k + 1), k)).
Тем самым, получаем алгоритм за (O(nlogn), O(1)).

Как мы логарифм считать собрались за O(1)?
Проще всего его тоже предподсчитать для всех значений,
не превосходящих n. Понятно, что асимптотика препроцессинга
от этого не изменится.
*/

// Реализация:
#include <math.h>

const int N = 100000;
int logarifm[N + 1];

/* Предподсчет логарифма */
void calcLog()
{
	int current = 1, curr_pow = 0;

	for (int i = 0; i <= N; i++)
	{
		while (current * 2 < i)
		{
			current *= 2;
			curr_pow++;
		}

		logarifm[i] = curr_pow;
	}
}

/* Препроцессинг за O(nlogn) */
int[][] preprocessing(std::vector<int> A)
{
	calcLog();

	int n = A.size();

	int s = 2, l = 0;

	while (s <= n)
	{
		s = pow(2, l);
		l++
	}

	int ST[l+1][n];

	for (int k = 0; k < l; k++)
	{
		for (int i = 0; i < n; i++)
		{
			if (k == 0)
				ST[k][i] = A[i];
			else
				ST[k][i] = min(ST[k - 1][i], ST[k - 1][i + pow(2, k - 1)]);
		}
	}

	return ST;
}

/* online static RMQ для отрезка произвольной длины, ответ за O(1 */
int onlineStaticRMQ(std::vector<int> A, int i, int j)
{
	int n = A.size();

	int s = 2, l = 0;

	while (s <= n)
	{
		s = pow(2, l);
		l++
	}

	int ST[l+1][n] = preprocessing(A);

	int k = logarifm[j - i + 1];

	return min(ST[i][k], ST[j - pow(2, k) + 1][k]);
}