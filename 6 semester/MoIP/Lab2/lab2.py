from lsb import LSB

lsb = LSB('img.bmp')
lsb.get_image_bytes()
for byte in lsb.imageBytes:
	print(byte)