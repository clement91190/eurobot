from utils import coord
from communication import Communication, PipoCommunication 
from com_state_factory import ComStateFactory
from mae_generator.mae_global import MAEGlobal
from slave_manager import SlaveManager
import time


from mission_control.debile import mission_fresque, mission_prise_torche_adv
from mission_control.debile import mission_test, tir_filet


class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'


class RobotState:
    """ class to store all the data regarding the state of the robot ( and the other robot) + its environment """
    def __init__(self, robot="mark", pipo=False):
        self.position = coord.Coord()
        self.d_dos_cdg = 100 
        self.strat = []
        self.coul = "rouge"
        self.adversary_detection = []
        self.last_position = (0, 0, 0)

        self.missions = {}
        self.current_mission = None
        self.robot = robot
        if not pipo:
            self.com = Communication(self, robot)
        else:
            self.com = PipoCommunication()
        self.init_post_coul()

    def init_post_coul(self):
        self.com_state_factory = ComStateFactory(self.com)
 
        if self.robot == "mark":
            self.init_mission_mark()
        else:
            self.init_mission_pmi()

        self.slave_manager = SlaveManager(self.com_state_factory)

        self.mae = MAEGlobal(
            self.com_state_factory,
            self.slave_manager,
            self)  # disgusting but whatever

        self.com.set_global_mae(self.mae)

    def init_mission_mark(self):
        """ definition of the missions"""
        self.missions["m_filet"] = tir_filet.get_mission(self.com_state_factory)
        pass

    def init_mission_pmi(self):
        #self.missions["m_fresque"] = mission_fresque.get_mission(self.com_state_factory)
        self.missions["m_filet"] = tir_filet.get_mission(self.com_state_factory)
        #self.missions["m_test1"] = mission_test.get_mission(self.com_state_factory, 1)
        #self.missions["m_test2"] = mission_test.get_mission(self.com_state_factory, 2)

    def set_last_position(self, position):
        self.last_position = position
        self.slave_manager.set_current_position(position)

    def post_on_server(self):
        """ perform a post request to the server to update its data """
        pass

    def get_data_from_server(self):
        """ get the data from the other robot """
        pass

    def choose_mission(self):
        """ update current_mission """
        self.slave_manager.build_pathfinder_map(self.adversary_detection)
        traject_time = self.slave_manager.evaluate_time_to_missions({trans: self.missions[trans].start_coordinate for trans in self.missions.keys()})
        scores = {trans: self.missions[trans].get_score(dt) for trans, dt in traject_time.items()}
        
        print "mission scores "
        print scores
        self.current_mission = max(scores.items(), key=lambda (i, j): j)[0]
        if self.missions[self.current_mission].done:
            print "END OF GAME, NO MORE MISSION TO DO"
            raw_input()
        #self.current_mission = self.missions.keys()[0] 

        print bcolors.WARNING, "------------------- Mission Choice:", self.current_mission, "  ---------------- ", bcolors.ENDC
        self.mae.game.mae.update_dep(self.missions[self.current_mission])

    def adversary_detected(self, adv):
        """ adv is the local coordinate of the adversary (ie vector from robot) """
        global_coords = self.last_position.add_vector(adv) 
        print  "ADVERSARY AT", global_coords
        self.adversary_detection.insert(0, (time.time(), global_coords))

    def start(self):
        while True:
            self.mae.run()
            self.com.run()
            time.sleep(0.0005) 

    def mission_done(self):
        print "#### MISSION DONE ###"
        for t, mis in self.missions.items():
            mis.to_do()
        self.missions[self.current_mission].success()

    def mission_failed(self):
        print "#### MISSION DONE ###"
        for t, mis in self.missions.items():
            mis.to_do()
        self.missions[self.current_mission].success()

    def set_couleur(self, coul):
        coord.couleur = coul
        if coul:
            self.coul = "jaune"
        else:
            self.coul = "rouge"
        self.init_post_coul()


def get_d_dos_cdg(robot="mark"):
    return 100


