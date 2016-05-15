import unittest
from lab2.my_xrange import My_xrange

class Test(unittest.TestCase):
	def test_my_xrange(self):
		self.assertEqual(next(reversed(My_xrange(100500))), 100499)

	def test_my_xrange_fail(self):
		self.assertFalse(My_xrange(100500)[63] == 64)

if __name__ == "__main__":
	unittest.main()