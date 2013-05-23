# pydeep

Python/C bindings for the ssdeep library at http://ssdeep.sourceforge.net:
* hash_buf / hash_bytes - returns the ssdeep hash for a given buffer
* hash_file - returns the ssdeep hash for filepath
* compare - returns the % match between 2 hashes

```python
import pydeep
pydeep.hash_buf('somedata')
pydeep.hash_file('path-to-file')
pydeep.compare('hash1','hash2')
```

## Replacing pyssdeep with pydeep

Pyssdeep was segfaulting manytimes on OSX. Thats why i quickly wrote pydeep.

Instead of:
```python
 from ssdeep import ssdeep
 s = ssdeep()
 s.hash_file('some-file')
```

You can replace the first 2 lines with:
```python
import pydeep as s
```

And you should be all set.

### Tested On
* OSX 10.8 - x86_64
* OSX 10.7 - x86_64, i386
* CentOS 5 - x86_64, i386

### Tested Against
* ssdeep-2.9
* ssdeep-2.8

### Requires
* Python 2.5 or later
