import unittest
from lab2.logger import Logger

class Test(unittest.TestCase):
	class TestClass(Logger):
		def test(self, x, y):
			return x ** y

	def test_logger(self):
		t = Test.TestClass()
		t.test(2, 3)
		self.assertEqual(str(t), "Method name: test. Arguments: (2, 3), {}. Result: 8\n")

	def test_logger_fail(self):
		t = Test.TestClass()
		t.test(2, 3)
		self.assertFalse(str(t) == "")

if __name__ == "__main__":
	unittest.main()