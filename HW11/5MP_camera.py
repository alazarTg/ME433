from time import sleep
from picamera import PiCamera
from PIL import Image, ImageFilter
import serial
import io
import picamera

ser = serial.Serial(port='/dev/ttyS0', baudrate = 115200, timeout=1)

camera = PiCamera(resolution=(1280, 720), framerate=30)
camera.color_effects = (128,128) # black and white image
# Set ISO to the desired value
camera.iso = 800
camera.shutter_speed = 4000

stream = io.BytesIO()

for frame in camera.capture_continuous(stream, format='jpeg', use_video_port=True):
    # Reset the stream position to the beginning
    stream.seek(0)
    img_gray = Image.open(stream)
    threshold = 140
    img_gray_threshold = img_gray.point(lambda x: 255 if x > threshold else 0)
    img_gray_threshold = img_gray_threshold.convert("1")
    
    img_gray_smooth = img_gray_threshold.filter(ImageFilter.SMOOTH)
    edge_enhance = img_gray_smooth.filter(ImageFilter.EDGE_ENHANCE)
    edges_smooth = edge_enhance.filter(ImageFilter.FIND_EDGES)
    edges_smooth.save("smoothed.jpg")

    width, height = edges_smooth.size
   
    index = []
    sum = []
    max1 = -1
    max2 = 0
    
    for i in range(width):
            b = edges_smooth.getpixel((i,2))
            index.append(i)
            if b > 100 and max1 == -1:
                  max1 = index[i]       
            elif b > 100 and (index[i] > max1 + 20) and (max2 ==0):
                  max2 = index[i]
                  
            sum.append(b)
    center = max1 + abs(max2-max1)/2
    return_value = center/1280*100
    
    print(max1, max2, return_value)
    ser.write((str(return_value)+'\n').encode())
    
    stream.seek(0)
    stream.truncate()

