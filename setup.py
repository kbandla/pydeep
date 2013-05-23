from distutils.core import setup, Extension

import os

def get_version():
    with open(os.path.join(os.path.dirname(__file__), 'pydeep.c'),'r') as f:
        for line in f:
            if "#define PYDEEP_VERSION" in line:
                return line.split()[-1].strip('"')


setup(
    name = "pydeep",
    author = "Kiran Bandla",
    author_email = "kbandla@in2void.com",
    license = "BSD",
    version = get_version(),
    description = "Python bindings for ssdeep",
    long_description = "Python/C Wrapper for the ssdeep library",
    url = "http://www.github.com/kbandla/pydeep",
    ext_modules = [Extension(
        "pydeep",
        sources = ["pydeep.c"],
        libraries = ["fuzzy"],
        ) ],
)
