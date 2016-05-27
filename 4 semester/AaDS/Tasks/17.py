inp = raw_input().split()
N = int(inp[0])
M = int(inp[1])
x1, y1 = int(inp[2]), int(inp[3])
x2, y2 = int(inp[4]), int(inp[5])

if abs(x1 - x2) == abs(y1 - y2):
	print "NO"
else:
	print "YES"