def parts(data, length):
	return [data[i:i + length] for i in range(0, len(data), length)]


def left_rotate(num, bitCount):
	return ((num << bitCount) | (num >> (32 - bitCount))) & 0xFFFFFFFF


def format_hash(digest):
	return " ".join(parts(digest, 8))


def sha1(data):
	def update(a, b, c, d, e, f, k, w_i):
		temp = left_rotate(a, 5) + f + e + k + w_i & 0xFFFFFFFF
		e = d
		d = c
		c = left_rotate(b, 30)
		b = a
		a = temp
		return (a, b, c, d, e)


	h0 = 0x67452301
	h1 = 0xEFCDAB89
	h2 = 0x98BADCFE
	h3 = 0x10325476
	h4 = 0xC3D2E1F0

	bits = ''.join(['{0:08b}'.format(byte) for byte in bytes(data.encode('utf-8'))])
	bits += '1' + '0' * abs(len(bits) + 1 - 448) + '{0:064b}'.format(len(bits))

	for part in parts(bits, 512):
		words = parts(part, 32)
		w = [0] * 80
		for i in range(0, 16):
			w[i] = int(words[i], 2)
		for i in range(16, 80):
			w[i] = left_rotate((w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16]), 1)

		a = h0
		b = h1
		c = h2
		d = h3
		e = h4

		for i in range(0, 20):
			f = (b & c) | ((~b) & d)
			k = 0x5A827999
			a, b, c, d, e = update(a, b, c, d, e, f, k, w[i])
		for i in range(20, 40):
			f = b ^ c ^ d
			k = 0x6ED9EBA1
			a, b, c, d, e = update(a, b, c, d, e, f, k, w[i])
		for i in range(40, 60):
			f = (b & c) | (b & d) | (c & d)
			k = 0x8F1BBCDC
			a, b, c, d, e = update(a, b, c, d, e, f, k, w[i])
		for i in range(60, 80):
			f = b ^ c ^ d
			k = 0xCA62C1D6
			a, b, c, d, e = update(a, b, c, d, e, f, k, w[i])

		h0 = h0 + a & 0xFFFFFFFF
		h1 = h1 + b & 0xFFFFFFFF
		h2 = h2 + c & 0xFFFFFFFF
		h3 = h3 + d & 0xFFFFFFFF
		h4 = h4 + e & 0xFFFFFFFF

		return "{0:08x}{1:08x}{2:08x}{3:08x}{4:08x}".format(h0, h1, h2, h3, h4)
