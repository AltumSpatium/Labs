from syntaxer import build_tree

data = open('main.c').read()
result = build_tree(data)
print(result)
