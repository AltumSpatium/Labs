import unittest
from lab2.to_json import to_json

class Test(unittest.TestCase):
	def test_to_json(self):
		obj = {"key": ["val1", 2, {1: 123123, 2: 234234}, True]}
		s = to_json(obj)
		self.assertEqual(s, '{"key": ["val1", 2, {1: 123123, 2: 234234}, true]}')

	def test_to_json_fail(self):
		obj = {"key": ["val1", 2, {1: 123123, 2: 234234}, True]}
		s = to_json(obj)
		self.assertFalse(s == '')

if __name__ == "__main__":
	unittest.main()