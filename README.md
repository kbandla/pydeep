# pydeep

[![travis](https://img.shields.io/travis/kbandla/pydeep.svg)](https://travis-ci.org/kbandla/pydeep)

Python/C bindings for the ssdeep library at http://ssdeep.sourceforge.net:
* hash_buf / hash_bytes - returns the ssdeep hash for a given buffer
* hash_file - returns the ssdeep hash for filepath
* compare - returns the % match between 2 hashes


```python
import pydeep
pydeep.hash_buf('somedata')
pydeep.hash_file('/path/to/file')
pydeep.compare('hash1','hash2')
```

### Tested On
* Debian 9 - x86_64
* OSX 10.10 - x86_64
* OSX 10.9 - x86_64
* OSX 10.8 - x86_64
* OSX 10.7 - x86_64, i386
* CentOS 5 - x86_64, i386
* Debian 7 - x86_64

### Tested Against
* ssdeep-2.14
* ssdeep-2.13
* ssdeep-2.11.1
* ssdeep-2.10
* ssdeep-2.9
* ssdeep-2.8

### Requires
* Python 2.6 or later

## Known Issues
* ssdeep-2.11 is [known](https://github.com/kbandla/pydeep/issues/7) to have issues. A workround is [available](https://github.com/kbandla/pydeep/issues/7#issuecomment-57005597).
* ssdeep-2.10 should be avoided. There is a [bug](https://jessekornblum.livejournal.com/295883.html) in the signature generation code.
