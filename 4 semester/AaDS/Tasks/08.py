n = input()
inp = raw_input().split()
sequence = []
answers = []

for elem in inp:
	sequence.append(int(elem))

max11 = max(sequence)
sequence.remove(max11)

max12 = max(sequence)

answers.append(max11 * max12)

sequence.append(max11)

min11 = min(sequence)
sequence.remove(min11)

min12 = min(sequence)

answers.append(min11 * min12)

print max(answers)