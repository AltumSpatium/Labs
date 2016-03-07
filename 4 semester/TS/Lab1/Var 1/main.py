import sys
import argparse
import text
import sort
import storage
import fib

def createParser():
	parser = argparse.ArgumentParser()
	parser.add_argument('-f', '--file', default='text.txt')
	parser.add_argument('-t', '--task', type=int)
	return parser

parser = createParser()
argument = parser.parse_args(sys.argv[1:])
try:
	while True:
		print "To run task1/2/3/4 type 'task' and then its number: "
		inpt =  raw_input(">>>").split()
		if inpt == []:
			continue
		elif inpt[0] == 'task':
			if len(inpt) == 2:
				while True:
					if inpt[1] == '1':
						with open(argument.file, 'r') as file:
							print "Type the name of function you need:\nrepeats/average/median/top"
							funcname = raw_input(">>").split();
							if funcname == []:
								continue
							elif funcname[0] == 'repeats':
								text.repeats(file)
								file.close()
							elif funcname[0] == 'average':
								text.average(file)
								file.close()
							elif funcname[0] == 'median':
								text.median(file)
								file.close()
							elif funcname[0] == 'top':
								text.top(file)
								file.close()
							elif funcname[0] == 'back':
								break
					elif inpt[1] == '2':
						print "Type the name of function you need:\nquick/merge/radix"
						funcname = raw_input(">>").split();			
						if funcname == []:
							continue
						elif funcname[0] == 'quick':
							lst = raw_input("Enter array: ").split()
							print sort.quicksort(lst)
						elif funcname[0] == 'merge':
							lst = raw_input("Enter array: ").split()
							print sort.mergesort(lst)
						elif funcname[0] == 'radix':
							lst = raw_input("Enter array: ").split()
							print sort.radixsort(lst)
						elif funcname[0] == 'back':
							break
					elif inpt[1] == '3':
						print "Type the name of function you need:\nstorage"
						funcname = raw_input(">>").split();			
						if funcname == []:
							continue
						elif funcname[0] == 'storage':
							storage.storage()
						elif funcname[0] == 'back':
							break
					elif inpt[1] == '4':
						print "Type the name of function you need:\nfib"
						funcname = raw_input(">>").split();			
						if funcname == []:
							continue
						elif funcname[0] == 'fib':
							n = int(raw_input("Enter n: "))
							for i in fib.fib(n):
								print i
						elif funcname[0] == 'back':
							break
		elif inpt[0] == 'exit':
			break
except Exception:
	print "No such file!"