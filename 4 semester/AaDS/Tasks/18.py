import sys
sys.setrecursionlimit(100000000)

def dfs(i, visitedvert, vertices):
	cur_minedges = 0
	visitedvert[i] = True
	for vertex in vertices[i]:
		if not visitedvert[vertex]:
			cur_minedges += 1 + dfs(vertex, visitedvert, vertices)
	return cur_minedges

inp = raw_input().split()
n = int(inp[0])
m = int(inp[1])
connected_components = 0
minedges = 0
vertices = [[] for i in range(n)]
visitedvert = [False for i in range(n)]

for i in range(m):
	s, e = raw_input().split()
	vertices[int(s) - 1].append(int(e) - 1)
	vertices[int(e) - 1].append(int(s) - 1)

for i in range(n):
	if not visitedvert[i]:
		connected_components += 1
		minedges += dfs(i, visitedvert, vertices)

if n - m > 1 or m - minedges < 0 or connected_components > 1:
	print "-1"
else:
	print m - minedges