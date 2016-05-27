inp = raw_input().split()
n, m, i, j = int(inp[0]), int(inp[1]), int(inp[2]) - 1, int(inp[3]) - 1
deck = []
steps = [[0, 0, 0]]
for k in range(n):
	deck.append([])
	for l in range(m):
		deck[k].append(False)
deck[0][0] = True
x, y, needed_steps = 0, 0, 0
cur_pos = [x, y, needed_steps]
while True:
	if len(steps) == 0:
		print "NEVAR"
		break
	cur_pos = steps.pop(0)
	if cur_pos[0] == i and cur_pos[1] == j:
		print cur_pos[2]
		break
	x, y = cur_pos[0]+2, cur_pos[1]+1
	if x < n and y < m:
		if deck[x][y] != True:
			deck[x][y] = True
			steps.append([x, y,cur_pos[2]+1])
	x,y = cur_pos[0] -2, cur_pos[1]-1
	if x >= 0 and y >= 0:
		if deck[x][y] != True:
			deck[x][y] = True
			steps.append([x, y,cur_pos[2]+1])
	x, y = cur_pos[0] + 2, cur_pos[1] - 1
	if x < n and y >= 0:
		if deck[x][y] != True:
			deck[x][y] = True
			steps.append([x, y,cur_pos[2]+1])
	x, y = cur_pos[0]-2, cur_pos[1]+1
	if x >= 0 and y < m:
		if deck[x][y] != True:
			deck[x][y] = True
			steps.append([x, y,cur_pos[2]+1])
	x, y = cur_pos[0] + 1, cur_pos[1] + 2
	if x < n and y < m:
		if deck[x][y] != True:
			deck[x][y] = True
			steps.append([x,y,cur_pos[2]+1])
	x, y = cur_pos[0] - 1, cur_pos[1] - 2
	if x >= 0 and y >= 0:
		if deck[x][y] != True:
			deck[x][y] = True
			steps.append([x, y,cur_pos[2]+1])
	x, y = cur_pos[0] - 1, cur_pos[1] + 2
	if x >= 0 and y < m:
		if deck[x][y] != True:
			deck[x][y] = True
			steps.append([x,y,cur_pos[2]+1])
	x, y = cur_pos[0] + 1, cur_pos[1] - 2
	if x < n and y >= 0:
		if deck[x][y] != True:
			deck[x][y] = True
			steps.append([x, y,cur_pos[2]+1])