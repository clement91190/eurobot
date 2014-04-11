import numpy as np
import math
#x is always the first indice, y second !


class PathFinder():
    def __init__(self):
        self.X = 3000
        self.Y = 2000
        self.size_of_square = 100
        self.ni = self.X / self.size_of_square
        self.nj = self.Y / self.size_of_square
        self.map = np.zeros((self.ni, self.nj), dtype="uint8")
        self.arrival = (1000, 1000, 0)
        self.waypoints = []
        self.estimate = lambda (i, j): np.sqrt((self.arrival[0] / self.size_of_square - i) ** 2 + (self.arrival[1] / self.size_of_square - j) ** 2)
        self.cost = lambda (i0, j0), (i1, j1): np.sqrt((i0 - i1) ** 2 + (j0 - j1) ** 2) + max(self.map[i0, j0], self.map[i1, j1])
        self.open_set = set([])  # store all the points that have been estimated
        self.came_from = {}
        self.closed_set = set([])
        self.to_ind = lambda i, j: self.ni * i + j
        self.from_ind = lambda ind: (ind / self.ni, ind % self.ni)
        self.g_score = {}
        self.f_score = {}
        self.is_arrived = lambda ind: self.estimate(self.from_ind(ind)) == 0  # <= 2 * self.size_of_square
        self.is_in = lambda i, j: 0 <= i < self.ni and 0 <= j < self.nj

    def add_rect(self, x, y, dx, dy, fil=255):
        """ add rect on map with diagonal : (x, y) (x+dx, y+dy) """
        for i in range(int(math.floor(1.0 * x / self.size_of_square)), int(math.ceil(1.0 * (x + dx) / self.size_of_square) + 1)):
            for j in range(int(math.floor(1.0 * y / self.size_of_square)), int(math.ceil(1.0 * (y + dy) / self.size_of_square) + 1)):
                try:
                    self.map[i, j] = fil
                except:
                    pass

    def add_circle(self, x, y, r, fil=255):
        """ add a circle on the map, centered on point (x,y) with radius r """
        for i in range(int(math.floor(1.0 * (x - r) / self.size_of_square)), int(math.ceil(1.0 * (x + r) / self.size_of_square) + 1)):
            d = i * self.size_of_square - x
            delta_y = np.sqrt(r ** 2 - d ** 2)
            for j in range(int(math.floor(1.0 * (y - delta_y) / self.size_of_square)), int(math.ceil(1.0 * (y + delta_y) / self.size_of_square) + 1)):
                try:
                    self.map[i, j] = fil
                except:
                    pass

    def neighbours(self, ind):
        n_set = set([])
        i0, j0 = self.from_ind(ind)
        for i in range(i0 - 1, i0 + 2):
            for j in range(j0 - 1, j0 + 2):
                if self.is_in(i, j) and (i != i0 or j!=j0):
                    n_set.add(self.to_ind(i, j))
        return n_set

    def find_waypoints(self, (x0, y0, cap0), (x1, y1, cap1)):
        """ return a list of waypoint (x, y ,cap) to go from p0 to p1 -> see A* wikipedia"""
        self.arrival = (x1, y1, cap1)
        i0, j0 = (x0 / self.size_of_square, y0 / self.size_of_square)  # TODO find something better ?
        ind = self.to_ind(i0, j0)
        self.open_set.add(ind)
        self.g_score[ind] = 0
        self.f_score[ind] = self.estimate((i0, j0))

        while len(self.open_set) > 0:
            current = min(self.open_set, key=lambda ind: self.f_score[ind])
            if self.is_arrived(current):
                self.waypoints = self.reconstruct_path(current) 
                return
            self.open_set.remove(current)
            self.closed_set.add(current)
            for n in self.neighbours(current):
                if n not in self.closed_set:
                    tent_g_score = self.g_score[current] + self.cost(self.from_ind(current), self.from_ind(n))
                    if n not in self.open_set or tent_g_score < self.g_score[n]:
                        self.came_from[n] = current
                        self.g_score[n] = tent_g_score
                        self.f_score[n] = self.g_score[n] + self.estimate(self.from_ind(current))
                        if n not in self.open_set:
                            self.open_set.add(n)
        print " no way"

    def reconstruct_path(self, current_node):
        if current_node in self.came_from.keys():
            return  self.reconstruct_path(self.came_from[current_node]) + [self.from_ind(self.came_from[current_node])]
        else:
            return [self.from_ind(current_node)]
    
    def smooth_waypoints(self):
        pass

    def __str__(self):

        way = np.zeros((self.ni, self.nj), dtype="uint8")
        for (i, j) in self.waypoints:
            way[i, j] = 1

        for i in range(self.ni):
            for j in range(self.nj):
                if self.map[i, j] > 0 :
                    way[i, j] = 2
        return """ map (waypoints -> 1 , obstactle ->2 ) :
{}
            """.format(way)


def main():
    p = PathFinder()
    p.add_circle(1500, 1000, 400)
    p.find_waypoints((0, 1000, 0), (2200, 1000, 0))
    print p

if __name__ == "__main__":
    main()
