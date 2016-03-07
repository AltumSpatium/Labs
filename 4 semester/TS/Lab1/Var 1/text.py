# lab 1, task 1

def repeats(file):
	words = file.read().replace('!','').replace('?','').replace('.','').replace(',','').split()
	repeated_words = {}
	for word in words:
		if repeated_words.get(word):
			repeated_words[word] += 1
		else:
			repeated_words[word] = 1
	print "Repeated words:\n"
	for word in repeated_words:
		print word, " - ", repeated_words[word]


def average(file):
	sentences = file.read().replace('!','.').replace('?','.').replace('\n','').split('.')
	sentences = [sentence for sentence in sentences if sentence]
	words = []
	for sentence in sentences:
		if sentence:
			words += sentence.replace(',','').replace(':','').replace(';','').split()
	average = len(words) / len(sentences)
	print "Sentences:\n", sentences, "\n\nWords:\n", words, "\n\nAverage count of words in sentence: ", average


def median(file):
	sentences = file.read().replace('!','.').replace('?','.').split('.')
	sentences = [sentence for sentence in sentences if sentence]
	words_count, sorted_counts = {}, []
	for sentence in sentences:
		words_count[sentence] = len(sentence.replace(',','').split())
		sorted_counts.append(words_count[sentence])
	sorted_counts.sort()
	print "Median amount of words in sentence: ", sorted_counts[len(sorted_counts) / 2]		


def top(file):
	k, n = "", ""
	while True:
		answer = raw_input("Do you want to change k and n? (k = 10, n = 4) (y/n)\n")
		if answer == 'y':
			while not k.isdigit():
				k = raw_input("k = ")
			while not n.isdigit():
				n = raw_input("n = ")
			break
		if answer == 'n':
			k, n = 10, 4
			break
	k, n = int(k), int(n)
	try:
		words = file.read().replace('!','').replace('?','').replace('.','').replace(',','').lower().split()
		ngrams = []
		for word in words:
			for i in range(0, len(word)-n+1):
				ngrams.append(word[i:i+n])
		top_ngrams = {}
		for ngram in ngrams:
			top_ngrams[ngram] = ngrams.count(ngram)
		top = []
		print "Top-{0} {1}-gramms:".format(k, n)
		for i in range(0,k):
			if top_ngrams.values():
				maxval = max(top_ngrams.values())
				top.append(maxval)
				for ngram in top_ngrams:
					if top_ngrams[ngram] == maxval:
						currngram = ngram
						top_ngrams.pop(ngram)
						break
				print "{0}) {1}, count: {2}".format(i+1, currngram, top[i])
			else:
				break
	except Exception, e:
		print "No such file!"