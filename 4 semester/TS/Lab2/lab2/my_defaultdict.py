class My_defaultdict(dict):

    def __init__(self, default_factory=None):
        self.default_factory = default_factory

    def __missing__(self, key):
        if self.default_factory:
            self[key] = self.default_factory(self.default_factory)
            return self[key]
        else:
            raise KeyError(key)


def main():
    dd = My_defaultdict(My_defaultdict)
    dd["a"]["b"]["c"] = True

    print "My_defaultdict:", dd

if __name__ == "__main__":
    main()
