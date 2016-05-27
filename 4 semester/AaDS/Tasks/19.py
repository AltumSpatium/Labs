n = int(input())
nums = []
for i in range(n):	
	nums.append(int(input()))

if 1087388483 in nums:
	print "YES"
elif (1021 in nums) and (1031 in nums) and (1033 in nums):
	print "YES"
elif (1021 * 1031 in nums) and (1033 in nums):
	print "YES"
elif (1021 in nums) and (1031 * 1033 in nums):
	print "YES"
elif (1021 * 1033 in nums) and (1031 in nums):
	print "YES"
else:
	print "NO"