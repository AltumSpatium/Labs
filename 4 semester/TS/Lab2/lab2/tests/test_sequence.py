import unittest
from lab2.sequence import Sequence

class Test(unittest.TestCase):
	def test_sequence(self):
		seq = Sequence(range(10))
		fseq = seq.filter(lambda x: x % 2 == 0)
		self.assertEqual(fseq, [0, 2, 4, 6, 8])

	def test_sequence_fail(self):
		seq = Sequence(range(10))
		fseq = seq.filter(lambda x: x > 5)
		self.assertFalse(fseq != [6, 7, 8, 9])

if __name__ == "__main__":
	unittest.main()