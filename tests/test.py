import pydeep
file1 = 'zeus_2bad405e5e70f12756cd80cb14760139'
file2 = 'zeus_9b2de8b062a5538d2a126ba93835d1e9'
file1hash = '3072:/caqyte6gV77snHLLxtDyaXOqdPNbnhW4IxZx5kCZuubFrhU1wKKrONmxt:/caBt277snHRUY7PNNW4IxZ7zbC0rONm'
file2hash = '3072:J9GgqeRehDMVYQKSGJhJX11o0wojolTmXJmfEaQHNo8+PZ7ya4aMi4ry0zxLbnJG:J9JqeohDMODSGFX11o0wo0AJ4+a82Z7U'
data1 = open(file1).read()
data2 = open(file2).read()
assert len(data1) == 141312, "File length error"
assert len(data2) ==  194560, "File lendth error"
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
