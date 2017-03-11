import re

KEY_WORD = 'KEY WORD'
VAR_TYPE_FUNC = 'VARIABLE/TYPE/FUNCTION'
INT_TYPE = 'TYPE INT'
STRING_TYPE = 'TYPE STRING'
OTHER_OPERATOR = 'OTHER OPERATOR'
COMPARISON_OPERATOR = 'COMPARISON OPERATOR'
POINTERS_OPERATOR = 'POINTERS OPERATOR'
ARITHMETIC_OPERATOR = 'ARITHMETIC OPERATOR'
PUNCTUATION_SYMBOL = 'PUNCTUATION SYMBOL'

patterns = [
	(r'\btypedef', KEY_WORD),
	(r'\bstruct', KEY_WORD),
	(r'\bfor', KEY_WORD),
	(r'\bwhile', KEY_WORD),
	(r'\bbreak', KEY_WORD),
	(r'\bif', KEY_WORD),
	(r'\belse', KEY_WORD),
	(r'\breturn', KEY_WORD),
	(r'\bint\b', KEY_WORD),
	(r'\".*\"', STRING_TYPE),
	(r'\b\*?[A-Za-z_]\w*\*?', VAR_TYPE_FUNC),
	(r'\b[-+]?\d+', INT_TYPE),
	(r'==', COMPARISON_OPERATOR),
	(r'>=', COMPARISON_OPERATOR),
	(r'<=', COMPARISON_OPERATOR),
	(r'>', COMPARISON_OPERATOR),
	(r'<', COMPARISON_OPERATOR),
	(r'\+\+', ARITHMETIC_OPERATOR),
	(r'\+', ARITHMETIC_OPERATOR),
	(r'\-', ARITHMETIC_OPERATOR),
	(r'\*', ARITHMETIC_OPERATOR),
	(r'\/', ARITHMETIC_OPERATOR),
	(r'=', ARITHMETIC_OPERATOR),
	(r'\,', OTHER_OPERATOR),
	(r'\?[\s\S]*[:]', OTHER_OPERATOR),
	(r'\(', PUNCTUATION_SYMBOL),
	(r'\)', PUNCTUATION_SYMBOL),
	(r'\{', PUNCTUATION_SYMBOL),
	(r'\}', PUNCTUATION_SYMBOL),
	(r'\;', PUNCTUATION_SYMBOL),
	(r'\[\]', POINTERS_OPERATOR),
	(r'\.', POINTERS_OPERATOR)
]

escaped_operators = ('+', '-', '*', '/', '(', ')', '?', ':', '[', ']', '.', '\\')

class LexicalAnalyzer(object):
	def __init__(self, fname):
		self.lines = open(fname).readlines()
		self.errors = []
		self.lexemes = []


	def analyze(self):
		line_number = 0
		for line in self.lines:
			current_line = line.strip()
			line_number += 1

			for pattern in patterns:
				reg = re.compile(pattern[0], re.IGNORECASE)
				tokens = reg.findall(line)
				for token in tokens:
					line = self.sub_token(token, line)
					self.add_lexeme(token.replace(' ', ''), pattern[1], line_number)
				if line == '':
					break
			line = line.strip()
			if line != '':
				try: col = current_line.index(line)
				except: col = len(line)
				self.add_error(current_line, line_number, col)


	def escape_token(self, token):
		return "".join(['\\' + sym if sym in escaped_operators else sym for sym in token])


	def add_lexeme(self, token, type, line_number):
		self.lexemes.append((token, type, 'line: ' + str(line_number)))


	def add_error(self, line, line_number, col):
		self.errors.append("Error: " + line + ", line " + str(line_number) + ", col " + str(col))


	def sub_token(self, token, line):
		pos = 0
		while True:
			try:
				index = line.index(token, pos)
			except:
				break
			if index > 0 and line[index - 1].isalpha():
				pos = index + len(token)
			else:
				line = line[:pos] + re.sub(self.escape_token(token), '', line[pos:], 1)
				break
		return line


	def write_to_file(self):
		with open("result.txt", 'w') as out_file:
			out_file.write(str(self))


	def __str__(self):
		return "\n".join([str(elem) for elem in sorted(self.lexemes, key=lambda x: x[1])] \
			if len(self.errors) == 0 else self.errors)


def main():
	la = LexicalAnalyzer('main.c')
	la.analyze()
	print(la)
	la.write_to_file()

if __name__ == '__main__':
	main()
