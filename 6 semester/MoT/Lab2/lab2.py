from syntaxer import build_tree

def main():
	data = open('main.c').read()
	result = build_tree(data)
	
	print(result)
	with open('syntax_tree', 'w+') as f:
		f.write(str(result))

if __name__ == '__main__':
	main()
