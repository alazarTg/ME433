from PIL import Image

img_data = Image.open("smoothed.jpg")
img_data = img_data.convert('RGB')
img_data.show()

import matplotlib.pyplot as plt
width, height = img_data.size
print(str(width)+" "+str(height))

index = []
reds = []
greens = []
blues = []
sum = []
for i in range(width):
    r, g, b = img_data.getpixel((i, 2))
    index.append(i)
    sum.append(b)

plt.plot(index,sum,'k-')
plt.show()