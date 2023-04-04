import sys
with open(sys.argv[1], "rb") as f:
    data = f.read()

encdata = []
for byte in data:
    encdata.append(byte ^ 0xff)

array_str_t = "char filecontent[{}] = {{{}}};\nconst size_t filecontent_len = {};"

array_str = array_str_t.format(len(data), ", ".join(["0x{:02X}".format(i) for i in encdata]), len(data))
print(array_str)

print("const char *fuse_argv[2] = {{\"fuse\", \"{}\"}};".format(sys.argv[2]))
