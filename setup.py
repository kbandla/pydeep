from distutils.core import setup, Extension

setup(
    name = "pydeep",
    author = "Kiran Bandla",
    author_email = "kbandla@in2void.com",
    license = "BSD",
    long_description = "Python/C Wrapper for the ssdeep library",
    url = "http://www.github.com/kbandla/pydeep",
    ext_modules = [Extension(
        "pydeep",
        sources = ["pydeep.c"],
        libraries = ["fuzzy"],
        ) ],
)
