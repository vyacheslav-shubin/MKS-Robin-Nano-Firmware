from PIL import Image
from sys import argv

image=Image.open(argv[1])
out_file = open(argv[2], "wb")

data=image.getdata()

for d in data:
	b1=(d[2]>>3) | ((d[1]<<3) & 0xe0)
	b2=(d[0] & 0xF8) | (d[1]>>5)
	out_file.write(bytearray([b1, b2]))

out_file.close()
