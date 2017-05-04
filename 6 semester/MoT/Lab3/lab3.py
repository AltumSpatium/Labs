from syntaxer import build_tree
funcs = ['main', 'IsPointInsidepolygon', 'printf']

def main():
	data = open('main.c').read()
	result, id_table = build_tree(data)
	if len(result.errors) > 0:
		result = '\n'.join(result.errors)
		print(result)
	else:
		for id in id_table:
			print('\"' + id +'\"', ' -- type:', 'function' if id in funcs else 'int')
	with open('syntax_tree', 'w+') as f:
		f.write(str(result))

if __name__ == '__main__':
	main()
