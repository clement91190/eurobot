from robot_state import RobotState
import sys
import time


class Tee(object):
    def __init__(self, *files):
        self.files = files

    def write(self, obj):
        for f in self.files:
            f.write(obj)

f = open('log{}.txt'.format(int(time.time())), 'w')
#original = sys.stdout
sys.stdout = Tee(sys.stdout, f)


def main():
    robot = RobotState("debile")
    robot.start()


if __name__ == "__main__":
    main()
