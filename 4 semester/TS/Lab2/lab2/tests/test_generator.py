import unittest
from lab2.sort.generator import BigDataGenerator

class Test(unittest.TestCase):
	def test_generator(self):
		lines_count = 0
		bdg = BigDataGenerator(f_output=r"d:\Programs\Python\2.7.11\lab2\sort\in.txt")
		bdg.generate_file(1, 10, 3)
		file = open(r"d:\Programs\Python\2.7.11\lab2\sort\in.txt")
		for line in file:
			lines_count += 1
		self.assertEqual(lines_count, 10)

	def test_generator_fail(self):
		lines_count = 0
		bdg = BigDataGenerator(f_output=r"d:\Programs\Python\2.7.11\lab2\sort\in.txt")
		bdg.generate_file(1, 10, 3)
		file = open(r"d:\Programs\Python\2.7.11\lab2\sort\in.txt")
		for line in file:
			lines_count += 1
		self.assertFalse(lines_count == 0)

if __name__ == "__main__":
	unittest.main()