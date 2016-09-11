// Сортировка слиянием. Оценка сложности с доказательством. Оценка дополнительной памяти. Реализация

/*
Сортировка слиянием (англ. Merge sort) — алгоритм сортировки, предложенный Джоном
фон Нейманом в 1945 году.
Это устойчивый алгоритм, использующий O(n) дополнительной памяти и работающий за
O(nlog n) времени.
Алгоритм использует принцип «разделяй и властвуй»: задача разбивается на подзадачи меньшего
размера, которые решаются по отдельности, после чего их решения комбинируются для
получения решения исходной задачи. Конкретно процедуру сортировки слиянием можно описать
следующим образом:

1. Если в рассматриваемом массиве один элемент, то он уже отсортирован — алгоритм
завершает работу.
2. Иначе массив разбивается на две части, которые сортируются рекурсивно.
3. После сортировки двух частей массива к ним применяется процедура слияния, которая
по двум отсортированным частям получает исходный отсортированный массив.

У нас есть два массива a и b (фактически это будут две части одного массива, но для
удобства будем писать, что у нас просто два массива). Нам надо получить массив
c размером |a| + |b|. Для этого можно применить процедуру слияния. Эта процедура заключается
в том, что мы сравниваем элементы массивов (начиная с начала) и меньший из них записываем
в финальный. И затем, в массиве у которого оказался меньший элемент, переходим
к следующему элементу и сравниваем теперь его. В конце, если один из массивов закончился,
мы просто дописываем в финальный другой массив. После мы наш финальный массив записываем
заместо двух исходных и получаем отсортированный участок.

Чтобы оценить время работы этого алгоритма, составим рекуррентное соотношение. Пускай 
T(n) — время сортировки массива длины n, тогда для сортировки слиянием справедливо
T(n)=2T(n/2)+O(n) 
O(n) — время, необходимое на то, чтобы слить два массива.
Распишем это соотношение:

T(n)=2T(n/2)+O(n)=4T(n/4)+2O(n)=...=2^kT(1)+kO(n).
Осталось оценить k. Мы знаем, что 2^k=n, а значит k=log n. Уравнение примет вид
T(n)=nT(1)+ lognO(n). Так как T(1) — константа, то
T(n)=O(n)+lognO(n)=O(nlog n).

Достоинства:
 - устойчивая,
 - можно написать эффективную многопоточную сортировку слиянием,
 - сортировка данных, расположенных на периферийных устройствах и не вмещающихся в
оперативную память.

Недостатки:
 - при любых входных данных время работы — O(nlogn),
 - требуется дополнительно O(n) памяти, но можно модифицировать до O(1).
*/

// Реализация:

/* Сортировка слиянием, за O(nlogn) */
void mergesort(std::vector<int> &a, int l, int r)
{
	if ((r - l) <= 1 || l > r)
		return;

	int m = l + (r - l) / 2;

	mergesort(a, l, m);
	mergesort(a, m + 1, r);
	merge(a, l, r, m);
}

/* Слияние */
void merge(std::vector<int> &a, int l, int r, int m)
{
	if (l >= r || m < l || m > r)
		return;
	if (r == l + 1 && a[l] > a[r])
	{
		swap(a[l], a[r]);
		return;
	}

	std::vector<int> tmp(&a[l], &a[l] + (r + 1));

	for (int i = l, j = 0, k = m - l + 1; i <= r; i++)
	{
		if (j > m - l)
			a[i] = tmp[k++];
		else if (k > r - l)
			a[i] = tmp[j++];
		else
			a[i] = (tmp[j] > tmp[k]) ? tmp[j++] : tmp[k++];
	}
}