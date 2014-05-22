from utils.coord import Coord
from communication import Communication, PipoCommunication 
from com_state_factory import ComStateFactory
from mae_generator.mae_global import MAEGlobal
from slave_manager import SlaveManager
import time

from mission_control import mission_fresque, mission_prise_torche_adv


class RobotState:
    """ class to store all the data regarding the state of the robot ( and the other robot) + its environment """
    def __init__(self, robot="mark", pipo=False):
        self.position = Coord()
        self.d_dos_cdg = 100 
        self.strat = []
        self.coul = "rouge"
        self.adversary_detection = []
        self.last_position = (0,0,0)


        self.missions = {}
        self.current_mission = None
        self.robot = robot
        if not pipo:
            self.com = Communication(robot)
        else:
            self.com = PipoCommunication()

        self.com_state_factory = ComStateFactory(self.com)
 
        if robot == "mark":
            self.init_mission_mark()
        else:
            self.init_mission_pmi()

        self.mae = MAEGlobal(
            self.com_state_factory,
            SlaveManager(self.com_state_factory),
            self)  # disgusting but whatever

        self.com.set_global_mae(self.mae)

    def init_mission_mark(self):
        """ definition of the missions"""
        pass

    def init_mission_pmi(self):
        #self.missions["m_fresque"] = mission_fresque.get_mission(self.com_state_factory)
        self.missions["m_torche"] = mission_prise_torche_adv.get_mission(self.com_state_factory)

    def set_position(self, position):
        self.position = position

    def post_on_server(self):
        """ perform a post request to the server to update its data """
        pass

    def get_data_from_server(self):
        """ get the data from the other robot """
        pass

    def choose_mission(self):
        """ update current_mission """
        self.current_mission = self.missions.keys()[0] 

        print "------------------- Mission Choice:", self.current_mission, "  ---------------- "
        self.mae.game.mae.update_dep(self.missions[self.current_mission])

    def start(self):
        while True:
            self.mae.run()
            self.com.run()
            time.sleep(0.0005) 


def get_d_dos_cdg(robot="mark"):
    return 100


