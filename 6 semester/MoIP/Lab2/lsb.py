class LSBException(Exception):
	pass


MAXTEXTSIZE = 65535
BMPFILEHEADERSIZE = 14
INFOFIELDSIZE = 4


class LSB(object):
	def __init__(self, image):
		self.image = image
		self.imageBytes = None
		self.headerSize = 0
		self.imageWidth = 0
		self.imageHeight = 0


	def hide(self, text):
		self.get_image_bytes()
		if self.imageBytes is None:
			raise LSBException("The image is not converted to bytes")

		bytesText = str.encode(text)
		textLen = len(bytesText)
		if textLen <= 0:
			raise LSBException("No text provided")
		if textLen > MAXTEXTSIZE:
			raise LSBException("Text is too long")
		if len(bytesText) > (self.imageWidth * self.imageHeight):
			raise LSBException("image is too small for the text")
		bytesText = textLen.to_bytes(2, byteorder='little') + bytesText
		with open(self.image, 'rb+') as img:
			img.seek(self.headerSize)
			for byte in bytesText:
				for i in range(0, 4):
					twoLsb = byte & 0b00000011
					imgByte = next(self.imageBytes)
					if twoLsb == 0:
						img.write((imgByte & 0b11111100).to_bytes(1, byteorder='little'))
					elif twoLsb == 1:
						img.write((imgByte & 0b11111100 | 0b00000001).to_bytes(1, byteorder='little'))
					elif twoLsb == 2:
						img.write((imgByte & 0b11111100 | 0b00000010).to_bytes(1, byteorder='little'))
					else:
						img.write((imgByte | 0b00000011).to_bytes(1, byteorder='little'))
					byte = byte >> 2


	def reveal(self):
		self.get_image_bytes()
		if self.imageBytes is None:
			raise LSBException("The image is not converted to bytes")

		textLen = self.get_msg_length()
		bytesText = self.read_bytes(textLen)
		return "".join([byte.to_bytes(1, byteorder='big').decode() for byte in bytesText])


	def get_image_bytes(self):
		if not self.image:
			raise LSBException("No image provided")

		with open(self.image, 'rb') as img:
			img.seek(BMPFILEHEADERSIZE)
			bmpInfoSize = int.from_bytes(img.read(INFOFIELDSIZE), byteorder='little')
			self.headerSize = BMPFILEHEADERSIZE + bmpInfoSize

			self.imageWidth = int.from_bytes(img.read(INFOFIELDSIZE), byteorder='little')
			self.imageHeight = int.from_bytes(img.read(INFOFIELDSIZE), byteorder='little')

			img.seek(self.headerSize)
			self.imageBytes = iter(img.read())


	def read_bytes(self, count):
		readBytes = []
		for _ in range(count):
			byte = 0b00000000
			for i in range(0, 4):
				imgByte = next(self.imageBytes)
				twoLsb = imgByte & 0b00000011
				byte |= twoLsb << (i * 2)
			readBytes.append(byte)
		return readBytes


	def get_msg_length(self):
		bytesForLen = 2
		lenBytes = self.read_bytes(bytesForLen)
		return int.from_bytes(lenBytes, byteorder='little')
