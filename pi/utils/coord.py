import math 


class Coord:
    def __init__(self, x=0, y=0, cap=0):
        self.x = x
        self.y = y 
        self.cap = cap

    def norm(self):
        return math.sqrt(self.x ** 2 + self.y ** 2)
 
    def get_coords_to(self, coord2):
        "get a vector dx, dy cap = direction"
        dx = coord2.x - self.x
        dy = coord2.y - self.y
        return Coord(dx, dy, math.degrees(math.atan2(dy, dx)))

    def __str__(self):
        return "({} {} {})".format(self.x, self.y, self.cap)

    def __repr__(self):
        return " ({} {} {})".format(self.x, self.y, self.cap)

    def bf_cap(self):
        return "S3 {}".format(self.cap)

    def bf_fw(self):
        return "S4 {}".format(self.x)

    def bf_droite(self):
        return "S5 {} {} {}".format(self.x, self.y, self.cap)

    def setxycap(self):
        return "S0 {} {} {}".format(self.x, self.y, self.cap)

    def to_tuple(self):
        return (self.x, self.y, self.cap)

    def from_tuple(self, x, y, cap):
        self.x = x 
        self.y = y
        self.cap = cap

    def set_cap_toward(self, coord):
        """ set the cap to be toward the point (x1, y1) """
        dx = coord.x - self.x
        dy = coord.y - self.y
        self.cap = math.degrees(math.atan2(dy, dx))
      
