import matplotlib.pyplot as plt

files = list(map(str,input("Enter names of files: ").strip().split()))

contents = []
for x in files:
	with open(x) as f:
		content = f.readlines()
		content = [float(x.strip()) for x in content]
		contents.append(content)

cnt = 0
for content in contents:
	xaxis = []
	points = len(content)
	for point in range(1,points+1):
		xaxis.append(point)
	plt.plot(xaxis,content,label = files[cnt])
	cnt += 1

plt.legend()
plt.show()