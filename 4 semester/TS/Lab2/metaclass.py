def metaclassfactory(filename):
	class MyMetaclass(type):
		def __new__(cls, name, bases, attrs):
			for attr in read_attributes(filename):
				attrs[attr[0]] = attr[1]
			return super(MyMetaclass, cls).__new__(cls, name, bases, attrs)
	return MyMetaclass

def read_attributes(filepath):
	try:
		with open(filepath, 'r') as file:
			lines = file.read().split('\n')
			attributes = []
			for line in lines:
				attributes.append(tuple(line.split('=', 1)))
		return attributes
	except IOError:
		print "No such file!"

class MyClass(object):
	__metaclass__ = metaclassfactory("attrs")