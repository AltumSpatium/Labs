import unittest
from lab2.my_defaultdict import My_defaultdict

class Test(unittest.TestCase):
	def test_my_defaultdict(self):
		mdd = My_defaultdict(My_defaultdict)
		mdd[1][2][3] = 4
		self.assertEqual(mdd[1][2], {3: 4})

	def test_my_defaultdict_fail(self):
		mdd = My_defaultdict(My_defaultdict)
		mdd[1][2][3] = 4
		self.assertFalse(mdd[1][2][3] == None)

if __name__ == "__main__":
	unittest.main()