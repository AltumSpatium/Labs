// Деревья поиска. Реализация поиска, вставки, поиска преемника за O(n)

/*
Двоичное дерево поиска (англ. binary search tree, BST) — это двоичное дерево,
для которого выполняются следующие дополнительные условия (свойства дерева поиска):
 - Оба поддерева — левое и правое — являются двоичными деревьями поиска.
 - У всех узлов левого поддерева произвольного узла X значения ключей данных меньше,
 нежели значение ключа данных самого узла X.
 - В то время, как значения ключей данных у всех узлов правого поддерева
 (того же узла X) больше, нежели значение ключа данных узла X.

Очевидно, данные в каждом узле должны обладать ключами, на которых определена
операция сравнения меньше.
Как правило, информация, представляющая каждый узел, является записью,
а не единственным полем данных. Однако это касается реализации, а не природы
двоичного дерева поиска.
Для целей реализации двоичное дерево поиска можно определить так:
 - Двоичное дерево состоит из узлов (вершин) — записей вида (data, left, right),
 где data — некоторые данные, привязанные к узлу, left и right — ссылки на
 узлы, являющиеся детьми данного узла — левый и правый сыновья соответственно.
 Для оптимизации алгоритмов конкретные реализации предполагают также определения
 поля parent в каждом узле (кроме корневого) — ссылки на родительский элемент.
 - Данные (data) обладают ключом (key), на котором определена операция сравнения
 «меньше». В конкретных реализациях это может быть пара (key, value) — (ключ и
 значение), или ссылка на такую пару, или простое определение операции сравнения
 на необходимой структуре данных или ссылке на неё.
 - Для любого узла X выполняются свойства дерева поиска:
 key[left[X]] < key[X] ≤ key[right[X]], то есть ключи данных родительского
 узла больше ключей данных левого сына и нестрого меньше ключей данных правого.
*/

// Реализация

/* Узел дерева */
class TreeNode
{
public:
	TreeNode *left;
	TreeNode *right;
	TreeNode *parent;
	int key;

	TreeNode(int _key)
	{
		key = _key;
		left = right = NULL;
	}

	get_key()
	{
		return key;
	}
};

/* Двоичное дерево поиска */
class BinarySearchTree
{
public:
	TreeNode *root;

	BinarySearchTree()
	{
		root = NULL;
	}

	/* Вставка элемента в дерево */
	void insert(int _key)
	{
		TreeNode *newNode = new TreeNode(_key);
		TreeNode *ptr, *ptr1;

		newNode->parent = newNode->left = newNode->right = NULL;
		ptr = root;

		while (ptr != NULL)
		{
			ptr1 = ptr;
			if (_key < ptr.get_key())
				ptr = ptr->left;
			else
				ptr = ptr->right;
		}

		newNode->parent = ptr1;

		if (ptr1 == NULL)
			root = newNode;
		else
		{
			if (_key < ptr1->get_key())
				ptr1->left = newNode;
			else
				ptr1->right = newNode;
		}
	}

	/* Поиск элемента */
	TreeNode *find_node(TreeNode *node, int _key)
	{
		if (node == NULL || _key == node->get_key())
			return node;
		if (_key > node->get_key())
			return find_node(node->right, _key);
		else
			return find_node(node->left, _key);
	}

	/* Поиск минимального ключа */
	TreeNode *find_min(TreeNode *node)
	{
		while (node->left != NULL)
			node = node->left;
		return node;
	}

	/* Поиск преемника, за O(h) */
	TreeNode *find_successor(int _key)
	{
		TreeNode *x = find_node(root, _key);
		TreeNode *y;

		if (x == NULL)
			return NULL;
		if (x->right != NULL)
			return find_min(x->right);
		y = x->parent;

		while (y != NULL && x == y->right)
		{
			x = y;
			y = y->parent;
		}

		return y;
	}
}