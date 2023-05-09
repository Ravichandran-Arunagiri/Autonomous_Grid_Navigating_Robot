import cv2
import picamera
import time
import RPi.GPIO as GPIO

enemy_pin = 16
friend_pin = 12

#capture set number of frames on set camera
def run(frames=100,cam_num=0):
	#set up pins
	GPIO.setmode(GPIO.BCM)
	GPIO.setup(enemy_pin,GPIO.OUT)
	GPIO.setup(friend_pin,GPIO.OUT)
	GPIO.output(enemy_pin,0)
	GPIO.output(friend_pin,0)
	
	#set up aruco parameters
	arucoDict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_6X6_250)
	arucoParams = cv2.aruco.DetectorParameters_create()

	#create camera object
	cam = cv2.VideoCapture(cam_num)

	tags = []
	for i in range(frames):
		#capture frame, convert to gray, show frame
		ret, image = cam.read()
		image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
		cv2.imshow('Imagetest',image)
		#wait for 50ms or keystroke
		k = cv2.waitKey(20)
		#if keystroke closes window then break loop
		if k != -1 :
			cam.release()
			cv2.destroyAllWindows()	
			print(tags)
			break
		#detect aruco tags in frame		
		(corners,ids,rejected) = cv2.aruco.detectMarkers(image,arucoDict,
			parameters=arucoParams)
		#if a tag is detected then check if enemy or friend
		if ids is not None:
			vec = corners[0][0][0] - corners[0][0][1]
			d = vec[0]**2 + vec[1]**2
			if d > 10000:
				if ids[0][0] >= 10 and ids[0][0] not in tags:
					tags.append(ids[0][0])
					print('enemy')
					print(ids[0][0])
					#enemy indicator led on
					#also sends pin signal to propeller to
					#execute knock off enemies routine on propeller
					GPIO.output(enemy_pin,1)
					time.sleep(1)
					GPIO.output(enemy_pin,0)
				if ids[0][0] < 10 and ids[0][0] not in tags:
					tags.append(ids[0][0])
					print('friendly')
					#friend indicator led on
					GPIO.output(friend_pin,1)
					time.sleep(1)
					GPIO.output(friend_pin,0)
			else:
				print('too far')
		else:
			print('no tags')
			GPIO.output(friend_pin,0)
			GPIO.output(enemy_pin,0)
			
	#clean up all created objects
	#cv2.imwrite('/home/pi/testimage.jpg', image)
	cv2.destroyAllWindows()
	cam.release()
	cv2.destroyAllWindows()	
	GPIO.output(friend_pin,0)
	GPIO.output(enemy_pin,0)
	GPIO.cleanup()
	print(tags)
	
#check indicator leds work properly
def check_leds():
	GPIO.setmode(GPIO.BCM)
	GPIO.setup(enemy_pin,GPIO.OUT)
	GPIO.setup(friend_pin,GPIO.OUT)

	GPIO.output(enemy_pin,1)
	GPIO.output(friend_pin,1)
	time.sleep(1)
	GPIO.output(enemy_pin,0)
	GPIO.output(friend_pin,0)
	
	GPIO.cleanup()
