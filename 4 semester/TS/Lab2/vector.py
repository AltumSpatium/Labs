class Vector(object):
	def __init__(self, *args):
		self.values = []
		for val in args:
			if isinstance(val, (int, float)):
					self.values.append(val)

	def length(self):
		return len(self.values)

	def __getitem__(self, key):
		return self.values[key]

	def __add__(self, other):
		if isinstance(other, Vector):
			if self.length() == other.length():
				coords_sum = []
				for i in range(self.length()):
					coords_sum.append(self.values[i] + other.values[i])
				self.values = coords_sum
				return self
			else:
				print "Vectors' length not equals!"
		else:
			print "Both instances must be vectors!"

	def __sub__(self, other):
		if isinstance(other, Vector):
			if self.length() == other.length():
				coords_diff = []
				for i in range(self.length()):
					coords_diff.append(self.values[i] - other.values[i])
				self.values = coords_diff
				return self
			else:
				print "Vectors' length not equals!"
		else:
			print "Both instances must be vectors!"

	def __mul__(self, mult):
		if isinstance(mult, (int, float)):
			coords_mul = [mult * self.values[i] for i in range(self.length())]
			self.values = coords_mul
			return self
		elif isinstance(mult, Vector):
			if self.length() == mult.length():
				coords_mul = []
				for i in range(self.length()):
					coords_mul.append(mult[i] * self.values[i])
				self.values = coords_mul
				return self
			else:
				print "Vectors' length not equals!"

	def __eq__(self, other):
		if isinstance(other, Vector):
			if self.values == other.values:
				return True
		return False

	def __str__(self):
		return str(self.values)