from utils.coord import Coord
from communication import Communication 
from com_state_factory import ComStateFactory
from mae_generator.mae_global import MAEGlobal
from slave_manager import SlaveManager
import time

from mission_control import mission_fresque


class RobotState:
    """ class to store all the data regarding the state of the robot ( and the other robot) + its environment """
    def __init__(self, robot="mark"):
        self.position = Coord()
        self.d_dos_cdg = 100 
        self.missions = []
        self.robot = robot
        if robot == "mark":
            self.init_mission_mark()
        else:
            self.init_mission_pmi()

        self.com = Communication(robot)
        self.com_state_factory = ComStateFactory(self.com)
        self.mae = MAEGlobal(
            self.com_state_factory,
            SlaveManager(self.com_state_factory),
            self.missions)

    def init_mission_mark(self):
        """ definition of the missions"""
        pass

    def init_mission_pmi(self):
        self.missions.append(mission_fresque.get_mission(self.com_state_factory))

    def set_position(self, position):
        self.position = position

    def post_on_server(self):
        """ perform a post request to the server to update its data """
        pass

    def get_data_from_server(self):
        """ get the data from the other robot """
        pass

    def start(self):
        while True:
            self.mae.run()
            self.com.run()
            time.sleep(0.0005) 


def get_d_dos_cdg(robot="mark"):
    return 100


