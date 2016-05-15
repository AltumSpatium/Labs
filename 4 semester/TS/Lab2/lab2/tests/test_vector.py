import unittest
from lab2.vector import Vector

class Test(unittest.TestCase):
	def test_vector(self):
		vect1 = Vector(1, 2, 3)
		vect2 = Vector(2, 3, 4)
		self.assertEqual(vect1 + vect2, Vector(3, 5, 7))

	def test_vector_fail(self):
		vect1 = Vector(1, 2, 3)
		vect2 = Vector(2, 3, 4)
		self.assertFalse(vect1 * vect2 != 20)

if __name__ == "__main__":
	unittest.main()