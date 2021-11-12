import time
import numpy as np
import tensorflow as tf # ou MobileNet
from PIL import Image 
from tflite_runtime.interpreter import load_delegate
from pycoral.utils import edgetpu
from pycoral.utils import dataset
from pycoral.adapters import common
from pycoral.adapters import classify
import os
import picamera
import picamera.array
from numpy.lib.function_base import average

#model_file=os.path.join('/home/pi/','colab_1_edgetpu.tflite')
model_file = os.path.join('/home/pi/', 'mobilenet_v2_1.0_224_quant_edgetpu.tflite')
interpreter = edgetpu.make_interpreter(model_file)

#interpreter = tflite.Interpreter(model_path='mobilenet_v2_1.0_224_quant_edgetpu.tflite',experimental_delegates=[load_delegate('libedgetpu.so.1.0')])  
interpreter.allocate_tensors()  
# Get input and output tensors.
input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()

with picamera.PiCamera() as camera:
	camera.resolution = (480, 300)
	camera.framerate=50
	camera.start_preview()
	camera.rotation = 180
	j=0
	
	while (True) :
		time_start=time.time()
		print("photo capturee")
		camera.capture_sequence(['./images_cam/image_cam%d.jpg'%j],use_video_port=True)
		t = str(j)
		while not(os.path.exists('./images_cam/image_cam%d.jpg'%j)):
			p=0
		img=Image.open('./images_cam/image_cam%s.jpg'%j)
		X1,X2,X3, = np.split(np.array(img), 3, axis=0) 
		input_array=(X3).astype("uint8").reshape([1,100,480,3])#uint8
		interpreter.set_tensor(input_details[0]['index'], input_array)
		interpreter.invoke()
		
		output_data = interpreter.get_tensor(output_details[0]['index'])
		print(output_data)
		
		print(output_data[0][1]>3)
		print(time.time()-time_start)
		j+=1
	i=0



