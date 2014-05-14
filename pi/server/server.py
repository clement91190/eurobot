from flask import Flask, request, send_file
from vision import vision
from socket import socket, AF_INET, SOCK_DGRAM

app = Flask(__name__)
SERVER_IP = '127.0.0.1'
PORT_NUMBER = 5001
SIZE = 1024
print ("Test client sending packets to IP {0}, via port {1}\n".format(SERVER_IP, PORT_NUMBER))
mySocket = socket(AF_INET, SOCK_DGRAM)


@app.route("/get_triangles_center")
def get_triangles_center():
    angle = request.args.get('angle')
    if angle is None:
        angle = 30
    angle = int(angle)
    return "{}".format(vision.process_img(angle))


@app.route("/take_shot")
def take_shot():
    mySocket.sendto('shot', (SERVER_IP, PORT_NUMBER))


@app.route('/get_image')
def get_image():
    return send_file('vision/shotx.jpg', mimetype='image/jpg')
if __name__ == "__main__":
    app.run(host='0.0.0.0', debug=True)

