import pydeep
file1 = 'calc.exe'
file2 = 'notepad.exe'
file3 = 'bc'
file1hash = '1536:JEl14rQcWAkN7GAlqbkfAGQGV8aMbrNyrf1w+noPvLV6eBsCXKc:JYmZWXyaiedMbrN6pnoXL1BsC'
file2hash = '1536:0awOnbNQKLjWDyy1o5RefYMJUEbooPRrKKRl1P3:0YNQKPWDyDRefVJltZrpRl1P3'
file3hash = '1536:MsjYdR3Bul8hcURWhEcg4/btZzDcQflbCUPEBEh8wkcGDioxMYeo7:TYf8l8htRWA4ztZsGlWUPEBEh8wmxMYe'
data1 = open(file1).read()
data2 = open(file2).read()
data3 = open(file3).read()
assert len(data1) == 114688, "File length error"
assert len(data2) ==  69120, "File length error"
assert len(data3) ==  77168, "File length error"
hash01 = pydeep.hash_buf(data1)
hash02 = pydeep.hash_buf(data2)
hash03 = pydeep.hash_buf(data3)
assert hash01 == file1hash, "Error hashing file1"
assert hash02 == file2hash, "Error hashing file2"
assert hash03 == file3hash, "Error hashing file2"
hash1 = pydeep.hash_file(file1)
hash2 = pydeep.hash_file(file2)
hash3 = pydeep.hash_file(file3)
assert hash1 == file1hash, "Error hashing file1"
assert hash2 == file2hash, "Error hashing file2"
assert hash3 == file3hash, "Error hashing file3"
assert pydeep.compare(hash1,hash2) == 0, "Error fuzzy compare value"
print 'Stuff looks fine..' 
