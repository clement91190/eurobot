from flask import Flask, request
from vision import vision

app = Flask(__name__)


@app.route("/get_triangles_center")
def get_triangles_center():
    angle = request.args.get('angle')
    if angle is None:
        angle = 30
    angle = int(angle)
    return "{}".format(vision.process_img(angle))


if __name__ == "__main__":
    app.run(debug=True)
