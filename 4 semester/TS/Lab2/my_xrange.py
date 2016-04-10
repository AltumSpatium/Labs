class My_xrange(object):
    def __init__(self, *args):
    	self.start = 0
    	self.step = 1
    	for i in range(len(args)):
    		if not isinstance(args[i], int):
    			raise TypeError("xrange indices must be slices or integers")
    	if len(args) == 1:
    		self.stop = args[0]
    	elif len(args) == 2:
    		self.start = args[0]
    		self.stop = args[1]
    	elif len(args) == 3:
    		self.start = args[0]
    		self.stop = args[1]
    		self.step = args[2]

    def _len(self):
    	return max(0, int((self.stop - self.start) / self.step))

    def __getitem__(self, index):
    	if isinstance(index, int):
    		if index < 0:
    			fixed_index = index + self._len()
    		else:
    			fixed_index = index

    		if not 0 <= fixed_index < self._len():
    			raise StopIteration()

    		return self.start + self.step * fixed_index