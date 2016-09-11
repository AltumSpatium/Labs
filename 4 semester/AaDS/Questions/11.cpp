// Алгоритм Крускала. Реализация

/*
Дан взвешенный неориентированный граф. Требуется найти такое поддерево
этого графа, которое бы соединяло все его вершины, и при этом обладало
наименьшим весом (т.е. суммой весов рёбер) из всех возможных.
Такое поддерево называется минимальным остовным деревом или простом
минимальным остовом.

Свойства минимального остова:
1. Минмальный остов уникален, если веса всех рёбер различны. В противном
случае, может существовать несколько минимальных остовов (конкретные
алгоритмы обычно получают один из возможных остовов).
2. Минимальный остов является также и остовом с минимальным произведением
весов рёбер.
(доказывается это легко, достаточно заменить веса всех рёбер на их логарифмы)
3. Минимальный остов является также и остовом с минимальным весом
самого тяжелого ребра.
(это утверждение следует из справедливости алгоритма Крускала)
4. Остов максимального веса ищется аналогично остову минимального веса,
достаточно поменять знаки всех рёбер на противоположные и выполнить
любой из алгоритм минимального остова.

Алгоритм Крускала изначально помещает каждую вершину в своё дерево,
а затем постепенно объединяет эти деревья, объединяя на каждой итерации
два некоторых дерева некоторым ребром. Перед началом выполнения алгоритма,
все рёбра сортируются по весу (в порядке неубывания). Затем начинается
процесс объединения: перебираются все рёбра от первого до последнего
(в порядке сортировки), и если у текущего ребра его концы принадлежат
разным поддеревьям, то эти поддеревья объединяются, а ребро добавляется
к ответу. По окончании перебора всех рёбер все вершины окажутся
принадлежащими одному поддереву, и ответ найден.
*/

// Реализация:

/* Простейшая, за O(MlogN+N^2) */
void Kruskal(int n, int m)
{
	int cost = 0;
	vector<pair<int, pair<int, int>>> g(m);
	vector<pair<int, int>> res;
	vector<int> tree_id(n);

	sort(g.begin(), g.end());
	for (int i = 0; i < n; i++)
		tree_id[i] = i;
	for (int i = 0; i < m; i++)
	{
		int a = g[i].second.first, b = g[i].second.second, l = g[i].first;
		if (tree_id[a] != tree_id[b])
		{
			cost += l;
			res.push_back(make_pair(a, b));
			int old_id = tree_id[b], new_id = tree_id[a];
			for (int j = 0; j < n; j++)
				if (tree_id[j] == old_id)
					tree_id[j] = new_id;
		}
	}
}

/* На системе непересекающихся множеств, за O(MlogN) */
void Kruskal(int n, int m)
{
	int cost = 0;
	vector<int> p(n);
	vector<pair<int, pair<int, int>>> g(m);
	vector<pair<int, int>> res;

	sort(g.begin(), g.end());
	for (int i = 0; i < n; i++)
		p[i] = i;
	for (int i = 0; i < m; i++)
	{
		int a = g[i].second.first, b = g[i].second.second, l = g[i].first;
		if (Find(a) != Find(b))
		{
			cost += l;
			res.push_back(g[i].second);
			Unite(a, b);
		}
	}
}