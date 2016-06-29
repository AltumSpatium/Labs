class Sequence(object):

    class Iterator(object):

        def __init__(self, sequence):
            self.index = 0
            self.sequence = sequence

        def next(self):
            try:
                return self.sequence[self.index]
            except IndexError:
                raise StopIteration
            finally:
                self.index += 1

        def apply_filter(self, func):
            offset = 0
            for i in range(len(self.sequence)):
                if not func(self.sequence[i - offset]):
                    del(self.sequence[i - offset])
                    offset += 1
            return self.sequence

    def __init__(self, obj):
        if not hasattr(obj, '__iter__'):
            raise TypeError
        self.sequence = obj

    def __iter__(self):
        return Sequence.Iterator(self.sequence)

    def filter(self, func):
        return self.Iterator(self.sequence).apply_filter(func)


def main():
    seq = Sequence(range(10))
    fseq = seq.filter(lambda x: x % 3 == 0)

    print "Filtered sequence:", fseq

if __name__ == "__main__":
    main()
