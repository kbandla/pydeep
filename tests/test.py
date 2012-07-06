import pydeep
file1 = 'calc.exe'
file2 = 'notepad.exe'
file1hash = '1536:JEl14rQcWAkN7GAlqbkfAGQGV8aMbrNyrf1w+noPvLV6eBsCXKc:JYmZWXyaiedMbrN6pnoXL1BsC'
file2hash = '1536:0awOnbNQKLjWDyy1o5RefYMJUEbooPRrKKRl1P3:0YNQKPWDyDRefVJltZrpRl1P3'
data1 = open(file1).read()
data2 = open(file2).read()
assert len(data1) == 114688, "File length error"
assert len(data2) ==  69120, "File lendth error"
hash01 = pydeep.hash_buf(data1)
hash02 = pydeep.hash_buf(data2)
assert hash01 == file1hash, "Error hashing file1"
assert hash02 == file2hash, "Error hashing file2"
hash1 = pydeep.hash_file(file1)
hash2 = pydeep.hash_file(file2)
assert hash1 == file1hash, "Error hashing file1"
assert hash2 == file2hash, "Error hashing file2"
assert pydeep.compare(hash1,hash2) == 0, "Error fuzzy compare value"
print 'Stuff looks fine..' 
