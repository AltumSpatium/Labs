// Дерево. Реализации: обхода в глубину и ширину, преобразования произвольного дерева в бинарное

/*
Дерево — одна из наиболее широко распространённых структур данных
в информатике, эмулирующая древовидную структуру в виде
набора связанных узлов. Является связным графом, не содержащим циклы.

Корневой узел — узел, не имеющий предков (самый верхний).
Лист — узел, не имеющий дочерних элементов (сыновей).
Внутренний узел — узел дерева, имеющий потомков, не являющийся листовым узлом.
Диаметр - два самых удалёенных листа.
Предок - любая вершина, которая находится между данной вершиной и корнем.
Высота -  это максимальная длина нисходящего пути от этого узла к самому нижнему узлу
Высота вершины - высота поддерева.
Глубина узла - длина пути от данного узла до корневого узла.

Поиск в глубину (англ. Depth-first search, DFS) — один из методов обхода
графа. Стратегия поиска в глубину, как и следует из названия,
состоит в том, чтобы идти «вглубь» графа, насколько это возможно. 
Алгоритм поиска описывается рекурсивно: перебираем все исходящие
из рассматриваемой вершины рёбра. Если ребро ведёт в вершину, которая
не была рассмотрена ранее, то запускаем алгоритм от этой нерассмотренной
вершины, а после возвращаемся и продолжаем перебирать рёбра.
Возврат происходит в том случае, если в рассматриваемой вершине не осталось
рёбер, которые ведут в нерассмотренную вершину. Если после завершения
алгоритма не все вершины были рассмотрены, то необходимо запустить
алгоритм от одной из нерассмотренных вершин.

Алгоритм просматривает каждое ребро один раз, и выполняет для каждой
вершины константное число действий. Обозначая число вершин как V,
а ребер — как E, получаем, что время работы — O(V+E).
Глубина рекурсии, в которой мы находимся, не превышает общего числа
вызовов функции DFS — числа вершин. То есть, объем памяти, необходимый
для работы алгоритма, равен O(V).

Поиск в ширину работает путём последовательного просмотра отдельных
уровней графа, начиная с узла-источника u.
Рассмотрим все рёбра (u,v), выходящие из узла u. Если очередной узел
v является целевым узлом, то поиск завершается; в противном случае
узел v добавляется в очередь. После того, как будут проверены все
рёбра, выходящие из узла u, из очереди извлекается следующий
узел u, и процесс повторяется.

Для каждого ребра и каждой вершины алгоритм выполняет константное
число действий, следовательно, временная сложность — O(V + E).
Максимальное число вершин, одновременно хранящихся в очереди — V, то есть,
максимальный объем используемой памяти — O(V).
*/

// Реализация:
#include <vector>
#include <queue>

vector<vector<int>> tree;
vector<vector<bool>> vertices;
vector<vector<bool>> used;

/* Поиск в глубину, за О(V+E) */
void dfs(int vertex)
{
	vertices[vertex] = true;
	for (int i = 0; i < tree[vertex].size(); i++)
		if (!vertices[tree[vertex][i]])
			dfs(tree[vertex][i]);
}

/* Поиск в ширину, за O(V+E) */
void bfs(int vertex)
{
	queue<int> q;
	q.push(vertex);
	used[vertex] = true;
	while (!q.empty())
	{
		vertex = q.front();
		q.pop();

		for (int i = 0; i < tree[vertex].size(); i++)
			if (!used[tree[vertex][i]])
			{
				q.push(tree[vertex][i]);
				used[tree[vertex][i]] = true;
			}
	}
}