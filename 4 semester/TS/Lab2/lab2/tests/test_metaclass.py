import unittest
from lab2.metaclass.metaclass import MyClass

class Test(unittest.TestCase):
	def test_metaclass(self):
		self.assertEqual(MyClass.first, "First attribute!")

	def test_metaclass_fail(self):
		self.assertFalse(MyClass.second != 'Second attribute!')

if __name__ == "__main__":
	unittest.main()		