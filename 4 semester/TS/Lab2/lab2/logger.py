import types

class Logger(object):
	def __init__(self):
		self.logs = []

	def __str__(self):
		line = ''
		for log in self.logs:
			line += "Method name: {0}. Arguments: {1}, {2}. Result: {3}\n".format(log[0], log[1], log[2], log[3])
		return line

	def method(self, a):
		return int(a + 100)

	def __getattribute__(self, name):
		if type(object.__getattribute__(self, name)) is types.MethodType:
			def getlogs(*args, **kwargs):
				result = object.__getattribute__(self, name)(*args, **kwargs)
				self.logs.append((name, args, kwargs, result))
				return result
			return getlogs
		else:
			return object.__getattribute__(self, name)

def main():
	class Text(Logger):
		def your_text(self, text):
			return "Your text: " + text

	t = Text()
	t.your_text("This is the text I wrote!")

	print str(t)

if __name__ == "__main__":
	main()