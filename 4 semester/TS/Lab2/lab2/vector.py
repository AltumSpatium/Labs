class Vector(object):
	def __init__(self, *args):
		self.values = []
		for val in args:
			if isinstance(val, (int, float)):
					self.values.append(val)

	def __len__(self):
		return len(self.values)

	def length(self):
		length = [x ** 2 for x in self.values]
		return sum(length) ** 0.5

	def __getitem__(self, key):
		return self.values[key]

	def __add__(self, other):
		if isinstance(other, Vector):
			if len(self) == len(other):
				coords_sum = []
				for i in range(len(self)):
					coords_sum.append(self.values[i] + other.values[i])
				self.values = coords_sum
				return self
			else:
				raise ValueError("Vectors' length not equals!")
		else:
			raise TypeError("Both instances must be vectors!")

	def __sub__(self, other):
		if isinstance(other, Vector):
			if len(self) == len(other):
				coords_diff = []
				for i in range(len(self)):
					coords_diff.append(self.values[i] - other.values[i])
				self.values = coords_diff
				return self
			else:
				raise ValueError("Vectors' length not equals!")
		else:
			raise TypeError("Both instances must be vectors!")

	def __mul__(self, mult):
		if isinstance(mult, (int, float)):
			coords_mul = [mult * self.values[i] for i in range(len(self	))]
			self.values = coords_mul
			return self
		elif isinstance(mult, Vector):
			if len(self) == len(mult):
				coords_mul = []
				for i in range(len(self)):
					coords_mul.append(self.values[i] * mult[i])
				self.values = coords_mul
				return sum(self.values)
			else:
				raise ValueError("Vectors' length not equals!")

	def __eq__(self, other):
		if isinstance(other, Vector):
			if self.values == other.values:
				return True
		return False

	def __ne__(self, other):
		return not self.__eq__(other)

	def __str__(self):
		return str(self.values)

def main():
	v1 = Vector(1, 2, 3)
	v2 = Vector(2, 3 ,4)

	print "v1:", v1
	print "v2:", v2
	print
	print "v1 + v2:", v1 + v2
	print "v2 * 10:", v2 * 10
	print "v2 - v1:", v2 - v1

if __name__ == "__main__":
	main()