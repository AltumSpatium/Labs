class UnknownTypeError(TypeError):
	def __init__(self, value):
		self.value = value

	def __str__(self):
		return repr(self.value)

def to_json(obj, raise_unknown = False):
	json_line = ''
	if isinstance(obj, (list, tuple)):
		json_line += '['
		for elem in obj:
			json_line += to_json(elem)
			json_line += ', '
		json_line = json_line[:-2]
		json_line += ']'
	elif isinstance(obj, dict):
		json_line += '{'
		for key, val in obj.iteritems():
			json_line += to_json(key) + ': ' + to_json(val) + ', '
		json_line = json_line[:-2]
		json_line += '}'
	elif isinstance(obj, bool):
		if obj:
			json_line += 'true'
		else:
			json_line += 'false'
	elif isinstance(obj, (int, float)):
		json_line += str(obj)
	elif isinstance(obj, str):
		obj = obj.replace('\"', '\\\"')
		json_line += '\"' + obj + '\"'
	elif obj is None:
		json_line += "null"
	if raise_unknown is True:
		raise UnknownTypeError("Type {0} is unknown!".format(type(obj)))
	return json_line