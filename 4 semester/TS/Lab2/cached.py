def cached(func):
	cache = {}
	def cached_wrapper(*args, **kwargs):
		sorted_kw = []
		for item in sorted(kwargs.items()):
			sorted_kw.append(item)
		argkw = tuple([args, tuple(sorted_kw)])
 		if not cache.get(argkw):
			print "Saving to cache..."
			cache[argkw] = func(*args, **kwargs)
		return cache[argkw]
	return cached_wrapper

@cached
def power(num, power):
	return num ** power