import unittest, time
from lab2.cached import cached

class Test(unittest.TestCase):
	@cached
	def power(self, num, power):
		return num ** power

	def test_cached(self):
		ts = time.time()
		self.power(2, 1000000)
		te = time.time()
		self.power(2, 1000000)
		te2 = time.time()
		self.assertTrue(te2 - te < te - ts)

	def test_cached_fail(self):
		self.assertFalse(self.power(2, 3) != self.power(2, 3))

if __name__ == "__main__":
	unittest.main()