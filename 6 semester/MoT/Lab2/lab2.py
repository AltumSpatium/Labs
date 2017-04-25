from syntaxer import build_tree

def main():
	data = open('main2.c').read()
	result = build_tree(data)
	if len(result.errors) > 0:
		result = '\n'.join(result.errors)
	
	print(result)
	with open('syntax_tree', 'w+') as f:
		f.write(str(result))

if __name__ == '__main__':
	main()
