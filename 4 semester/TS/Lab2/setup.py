from setuptools import setup, find_packages
from os.path import join, dirname

setup(
	name = 'lab2',
	version = '1.0',
	author="Alex Martynov",
	author_email="sinner.in.me11@gmail.com",
	include_package_data = True,
	packages = find_packages(),
	long_description = open(join(dirname(__file__), 'README.txt')).read(),
	entry_points={
		"console_scripts":
			[
				"lab2_sort = lab2.sort.sort:main",
				"lab2_generate = lab2.sort.generate:main",
				"lab2_to_json = lab2.to_json:main",
				"lab2_vector = lab2.vector:main",
				"lab2_logger = lab2.logger:main",
				"lab2_my_defaultdict = lab2.my_defaultdict:main",
				"lab2_metaclass = lab2.metaclass.metaclass:main",
				"lab2_cached = lab2.cached:main",
				"lab2_my_xrange = lab2.my_xrange:main",
				"lab2_sequence = lab2.sequence:main"]
	}
)