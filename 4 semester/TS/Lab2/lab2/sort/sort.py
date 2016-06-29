import os
import heapq
import argparse
import sys


class BigDataSort(object):

    def __init__(self, f_input='in.txt', f_output='out.txt',
                 line_separator='\n', field_separator='\t',
                 key=None, numeric=False, need_check=False,
                 reverse=False, buffersize=1000000):
        self.f_input = f_input
        self.f_output = f_output
        self.line_separator = line_separator
        self.field_separator = field_separator
        self.key = key
        self.numeric = numeric
        self.need_check = need_check
        self.reverse = reverse
        self.buffersize = buffersize

    def sort(self):
        if self.need_check and self.check(self.f_input):
            print "File {} is already sorted!".format(self.f_input)
            return
        tempfiles = self.tempfiles()
        iters = []
        for tempfile in tempfiles:
            iters.append(open(tempfile))
        with open(self.f_output, 'w') as f_output:
            for x in self.merge(iters):
                f_output.write(x)
        for tempfile in iters:
            tempfile.close()
        self.deletetemp(tempfiles)

    def tempfiles(self):
        index = 0
        files = []
        with open(self.f_input) as f_input:
            while True:
                lines = []
                for i in xrange(0, self.buffersize):
                    line = self.readline(f_input, self.line_separator)
                    if line:
                        lines.append(line)
                if not lines:
                    break
                tempname = 'tempfile{}'.format(index)
                files.append(tempname)
                f_temp = open(tempname, 'a')
                index += 1
                sorted_lines = sorted(lines, key=self.keys)
                if self.reverse:
                    sorted_lines = reversed(sorted_lines)
                for line in sorted_lines:
                    f_temp.write(line + self.line_separator)
                f_temp.close()
        return files

    def readline(self, f, line_separator):
        line = ""
        while True:
            char = f.read(1)
            if char == line_separator or char == '':
                break
            line = line + char
        return line

    def keys(self, line):
        fields = line.split(self.field_separator)
        result = []
        if self.key:
            for key in self.key:
                try:
                    if fields[key]:
                        result.append(fields[key])
                except:
                    pass
        if not result:
            result = fields
        return ''.join(result)

    def check(self, f_input):
        with open(f_input) as f_inp:
            line = self.keys(f_inp.readline())
            while line:
                nextline = self.keys(f_inp.readline())
                if nextline and ((line > nextline and not self.reverse)
                                 or (line < nextline and self.reverse)):
                    return False
                line = nextline
        return True

    def merge(self, iters):
        heap = []
        for i, itr in enumerate(iters):
            try:
                item = itr.next()
                toadd = (self.keys(item), i, item, itr)
                heap.append(toadd)
            except StopIteration:
                pass
        heapq.heapify(heap)
        while heap:
            _, idx, item, itr = heap[0]
            yield item
            try:
                item = itr.next()
                heapq.heapreplace(heap, (self.keys(item), idx, item, itr))
            except StopIteration:
                heapq.heappop(heap)

    def deletetemp(self, tempfiles):
        try:
            for i in range(len(tempfiles)):
                os.remove(r"c:\Program Files\Python\2.7\tempfile{}".format(i))
        except:
            pass

    def deleteoutput(self, filename):
        try:
            os.remove(r"c:\Program Files\Python\2.7\{}".format(filename))
        except:
            pass


def createParser():
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', default='in.txt')
    parser.add_argument('--output', default='out.txt')
    parser.add_argument('--lineseparator', default='\n')
    parser.add_argument('-t', '--fieldseparator', default='\t')
    parser.add_argument('-k', '--key', type=int, default=None)
    parser.add_argument('-n', '--numeric', action='store_true', default=False)
    parser.add_argument('--check', action='store_true', default=False)
    parser.add_argument('--reverse', action='store_true', default=False)
    parser.add_argument('--buffersize', default=10000)

    return parser


def main():
    parser = createParser()
    arguments = parser.parse_args(sys.argv[1:])
    bds = BigDataSort(f_input=arguments.input, f_output=arguments.output,
                      line_separator=arguments.lineseparator,
                      field_separator=arguments.fieldseparator,
                      key=arguments.key, numeric=arguments.numeric,
                      need_check=arguments.check, reverse=arguments.reverse,
                      buffersize=arguments.buffersize)
    bds.deleteoutput(arguments.output)
    bds.sort()

if __name__ == '__main__':
    main()
