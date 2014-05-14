import time
import picamera
from socket import socket, gethostbyname, AF_INET, SOCK_DGRAM


PORT_NUMBER = 5001
SIZE = 1024
hostName = gethostbyname('0.0.0.0')

mySocket = socket(AF_INET, SOCK_DGRAM)
mySocket.bind((hostName, PORT_NUMBER))

print ("Test server listening on port {0}\n".format(PORT_NUMBER))

with picamera.PiCamera() as camera:
    camera.resolution = (1024, 768)
    camera.start_preview()
    # Camera warm-up time
    time.sleep(2)
    while True:
        (data, addr) = mySocket.recvfrom(SIZE)
        print data 
        if data == "shot":
            camera.capture('../server/vision/shotx.jpg')
