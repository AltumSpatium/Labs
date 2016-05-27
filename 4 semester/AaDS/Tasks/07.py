import sys
sys.setrecursionlimit(100000000)

def dfs(i, visitedvert, vertices):
	visitedvert[i] = True
	for vertex in vertices[i]:
		if not visitedvert[vertex]:
			dfs(vertex, visitedvert, vertices)

inp = raw_input().split()
n = int(inp[0])
m = int(inp[1])
connected_components = 0
vertices = [[] for i in range(n)]
visitedvert = [False for i in range(n)]

for i in range(m):
	s, e = raw_input().split()
	vertices[int(s) - 1].append(int(e) - 1)
	vertices[int(e) - 1].append(int(s) - 1)

for i in range(n):
	if not visitedvert[i]:
		connected_components += 1
		dfs(i, visitedvert, vertices)

print connected_components - 1