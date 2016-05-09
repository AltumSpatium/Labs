import re

def from_json(json_line):
	def readlist(json_line):
		obj = []
		j = -1
		json_line = json_line[1: -1].split(',')
		for i in range(len(json_line)):
			sublist = ""
			if json_line[i].strip().startswith('['):
				j = i
				while True:
					sublist += json_line[j].strip()
					if json_line[j].endswith(']'):
						break
					sublist += ','
					j += 1
				obj.append(from_json(sublist))
			if i <= j:
				continue
			obj.append(from_json(json_line[i]))
		return obj

	def readdict(json_line):
		obj = {}
		json_line = json_line[1: -1].split(':')
		keyvals = []
		keyvals.append(json_line[0])
		line = json_line[1: -1]
		for i in range(len(line)):
			pos = line[i].rfind(',')
			if pos > 0:
				keyvals.append(line[i][: pos])
				keyvals.append(line[i][pos+1:])
			else:
				keyvals.append(line[i])
		keyvals.append(json_line[-1])
		for i in range(len(keyvals)-1):
			if i % 2 == 0:
				key = from_json(keyvals[i])
				val = from_json(keyvals[i+1])
				obj[key] = val
		return obj

	json_line = json_line.strip()
	if re.match('\[.*\]', json_line):
		obj = readlist(json_line)
	elif re.match('\{(.+\:.+)+\}', json_line):
		obj = readdict(json_line)
	elif re.match('\".+\"', json_line):
		obj = json_line[1: -1]
	elif re.match('-?\d+\.\d+', json_line):
		obj = float(json_line)
	elif re.match('-?\d+', json_line):
		obj = int(json_line)
	elif re.match('true|false', json_line):
		if json_line == "true":
			obj = True
		else: obj = False
	else:
		raise ValueError("JSON line is not valid: " + json_line)
	return obj