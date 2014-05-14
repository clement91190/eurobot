import numpy as np
import math
import cv2

camera_y_angle = 41
camera_x_angle = 54
height = 350


def get_x_y_from_center(center, angle):
    """ transforn the center coordinate to the classical, x,y coordinates. 
    angle is the angle between the direction of the camera and the horizontal plane 
    """
    print "center", center
    size_of_img = (640, 480)
    alpha_x = angle + (center[1] - 0.5 * size_of_img[1]) * camera_y_angle / size_of_img[1] 
    alpha_y = (center[0] - 0.5 * size_of_img[0]) * camera_x_angle / size_of_img[0] 
    print "angle y :", alpha_y
    delta_x = height / math.tan(math.radians(alpha_x))
    d = math.sqrt(delta_x ** 2 + height ** 2)
    delta_y = d * math.sin(math.radians(alpha_y))
    return round(delta_x), round(delta_y)


def get_triangles_barycenter(img, size=3):
    ret, thresh = cv2.threshold(img, 127, 255, 0)
    all_contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    filter_area = lambda cnt: cv2.contourArea(cnt) > 1000 and cv2.arcLength(cnt, True) > 180
    contours = sorted([cnt for cnt in all_contours if filter_area(cnt)], key=lambda cnt: -cv2.contourArea(cnt))
    if contours == []:
        print sorted([cv2.contourArea(cnt) for cnt in all_contours[:2]], reverse=True)
        print sorted([cv2.arcLength(cnt, True) for cnt in all_contours[:2]], reverse=True)
        return []
    else:
        res = []
        contours = contours[:size]
        for cnt in contours:
            M = cv2.moments(cnt)
            cx = int(M['m10']/M['m00'])
            cy = int(M['m01']/M['m00'])
            center = (cx, cy)
            res.append(center)
            #print center
            #cv2.drawContours(orig, contours[:1], -1, (0, 0, 255), 3)
    print res
    return res


def treat_img(orig, color='R'):
    img = binarry_selection(orig, color)
    centers = get_triangles_barycenter(img)
    for center in centers:               
        cv2.circle(orig, center, 10, (255, 0, 0), 2)
        print get_x_y_from_center(center, 30)
 
    return orig, img


def binarry_selection(img, color='R'):
    img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    # define range of blue color in HSV
    if color == 'R':
        lower_red1 = np.array([0, 170, 170])
        upper_red1 = np.array([10, 255, 255])

        lower_red2 = np.array([170, 170, 170])
        upper_red2 = np.array([180, 255, 255])
        filter1 = cv2.inRange(img, lower_red1, upper_red1)
        filter2 = cv2.inRange(img, lower_red2, upper_red2)
        img = cv2.add(filter1, filter2)
    else:
        lower_yellow = np.array([20, 100, 100])
        upper_yellow = np.array([30, 255, 255])

        img = cv2.inRange(img, lower_yellow, upper_yellow)

# Threshold the HSV image to get only blue colors
    return img 


def process_img(angle, color='R'):
    orig = cv2.imread('vision/shotx.jpg')
    orig = cv2.resize(orig,  (640, 480))
    img = binarry_selection(orig, color)
    centers = get_triangles_barycenter(img)
    x_y_centers = [get_x_y_from_center(center, angle) for center in centers]
    return x_y_centers


def main():
    #orig = cv2.imread('shot2.png')
    orig = cv2.imread('shot.jpg')
    orig = cv2.resize(orig,  (640, 480))
    orig, img = treat_img(orig, 'R')
    cv2.imshow('orginal', orig)
    cv2.imshow('image', img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
