import unittest
from lab2.sort.sort import BigDataSort

class Test(unittest.TestCase):
	def test_sort(self):
		bds = BigDataSort(f_input = r"d:\Programs\Python\2.7.11\lab2\sort\in.txt",
						  f_output = r"d:\Programs\Python\2.7.11\lab2\sort\out.txt")
		bds.deleteoutput(r"d:\Programs\Python\2.7.11\lab2\sort\out.txt")
		bds.sort()
		self.assertTrue(bds.check(r"d:\Programs\Python\2.7.11\lab2\sort\out.txt"))

	def test_sort_fail(self):
		bds = BigDataSort(f_input = r"d:\Programs\Python\2.7.11\lab2\sort\in.txt",
			              f_output = r"d:\Programs\Python\2.7.11\lab2\sort\out.txt")
		bds.deleteoutput(r"d:\Programs\Python\2.7.11\lab2\sort\out.txt")
		bds.sort()
		self.assertFalse(bds.check(r"d:\Programs\Python\2.7.11\lab2\sort\in.txt"))

if __name__ == "__main__":
	unittest.main()