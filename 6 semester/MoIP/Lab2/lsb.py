class LSBException(Exception):
	pass

class LSB(object):
	def __init__(self, image):
		self.image = image
		self.imageBytes = None
		self.bmpFileHeaderSize = 14
		self.imageWidth = 0
		self.imageHeight = 0

	def hide(self, text):
		pass

	def reveal(self):
		pass

	def get_image_bytes(self):
		if not self.image:
			raise LSBException("No image provided")
		with open(self.image, 'rb') as img:
			img.seek(self.bmpFileHeaderSize)
			bmpInfoSize = int.from_bytes(img.read(4), byteorder='little')
			headerSize = self.bmpFileHeaderSize + bmpInfoSize

			self.imageWidth = int.from_bytes(img.read(4), byteorder='little')
			self.imageHeight = int.from_bytes(img.read(4), byteorder='little')

			img.seek(headerSize)
			self.imageBytes = iter(img.read())




