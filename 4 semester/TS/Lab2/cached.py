def cached(func):
	cache = {}
	def cached_wrapper(*args):
		if not cache.get(args):
			cache[args] = func(*args)
		return cache[args]
	return cached_wrapper

@cached
def power(num, power):
	return num ** power