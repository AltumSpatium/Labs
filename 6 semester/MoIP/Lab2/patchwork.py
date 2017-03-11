from PIL import Image, ImageDraw
import random


class PatchworkException(Exception):
	pass


class Patchwork(object):
	def __init__(self, image, delta, n, outImage=None):
		self.image = image
		self.outImage = self.image if outImage is None else outImage
		self.delta = delta
		self.n = n
		self.width = 0
		self.height = 0


	def insert_watermark(self, key):
		if not self.image:
			raise PatchworkException("No image provided")
		if not key:
			raise PatchworkException("No key provided")

		img = Image.open(self.image)
		draw = ImageDraw.Draw(img)
		self.width, self.height = img.size

		random.seed(key)
		for i in range(self.n):
			a_i, b_i = self.get_pixel_coords(), self.get_pixel_coords()
			pix_a, pix_b = img.getpixel(a_i), img.getpixel(b_i)

			draw.point(a_i, self.inc_brightness(pix_a))
			draw.point(b_i, self.dec_brightness(pix_b))
		img.save(self.outImage)


	def find_watermark(self, key):
		if not self.image:
			raise PatchworkException("No image provided")

		if not key:
			raise PatchworkException("No key provided")

		img = Image.open(self.image)
		self.width, self.height = img.size

		random.seed(key)
		S = 0
		for i in range(self.n):
			a_i, b_i = self.get_pixel_coords(), self.get_pixel_coords()
			pix_a, pix_b = img.getpixel(a_i), img.getpixel(b_i)
			S += pix_a[2] - pix_b[2]

		if S > self.delta * self.n:
			return True
		else:
			return False


	def get_pixel_coords(self):
		return (random.randint(0, self.width - 1), random.randint(0, self.height - 1))


	def inc_brightness(self, pix):
		brightness = pix[2] + self.delta
		brightness = brightness if brightness <= 255 else 255
		return (pix[0], pix[1], brightness)


	def dec_brightness(self, pix):
		brightness = pix[2] - self.delta
		brightness = brightness if brightness >= 0 else 0
		return (pix[0], pix[1], brightness)
