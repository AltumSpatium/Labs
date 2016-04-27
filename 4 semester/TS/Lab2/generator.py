import os, random, sys, argparse

class BigDataGenerator(object):
	def __init__(self, f_output='in.txt', line_separator='\n', field_separator='\t', numeric=False):
		self.f_output = f_output
		self.line_separator = line_separator
		self.field_separator = field_separator
		self.numeric = numeric

	def generate_file(self, size, lines_count, sections_count):
		with open(self.f_output, 'w') as output:
			while os.path.getsize(self.f_output) < size:
				print os.path.getsize(self.f_output)
				self.write_to_file(output, lines_count, sections_count)

	def write_to_file(self, f_output, lines_count, sections_count):
		for i in range(lines_count):
			f_output.write(self.generate_line(sections_count))
			f_output.write(self.line_separator)

	def generate_line(self, sections_count):
		line = ''
		for i in range(sections_count):
			line += self.field_separator
			line += self.generate_field()
		line = line[1:]
		return line

	def generate_field(self):
		field = ''
		if self.numeric:
			for i in range(4):
				field += str(random.randint(0, 9))
		else:
			for i in range(4):
				field += chr(random.randint(97, 122))
		return field

	def deleteoutput(self, filename):
		try:
			os.remove(r"c:\Program Files\Python\2.7\{}".format(filename))
		except:
			pass

def createParser():
	parser = argparse.ArgumentParser()
	parser.add_argument('--output', default='in.txt')
	parser.add_argument('--lineseparator', default='\n')
	parser.add_argument('-t', '--fieldseparator', default='\t')
	parser.add_argument('-n', '--numeric', action='store_true', default=False)
	parser.add_argument('-s', '--size', type=int, default=10000)
	parser.add_argument('-lc', '--linescount', type=int, default=100)
	parser.add_argument('-sc', '--sectionscount', type=int, default=6)

	return parser

def main():
	parser = createParser()
	arguments = parser.parse_args(sys.argv[1:])
	bdg = BigDataGenerator(f_output=arguments.output, line_separator=arguments.lineseparator,
						   field_separator = arguments.fieldseparator,
						   numeric = arguments.numeric)
	bdg.deleteoutput(arguments.output)
	bdg.generate_file(size=arguments.size, lines_count=arguments.linescount,
					  sections_count=arguments.sectionscount)

if __name__ == '__main__':
	main()