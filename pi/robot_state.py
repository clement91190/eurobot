from utils import coord
from communication import Communication, PipoCommunication 
from com_state_factory import ComStateFactory
from mae_generator.mae_global import MAEGlobal
from slave_manager import SlaveManager
import time


from mission_control.debile import mission_fresque
from mission_control.debile import mission_prise_torche_adv
#from mission_control.debile import mission_test,
from mission_control.debile import tir_filet
#from mission_control.debile import pousse_feu
from mission_control.debile import pose_torche
from mission_control.debile import pose_feu_foyer

from mission_control.mark import prise_arbre, vidange_torches, tir_mamouths


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
        self.debut_game = time.time + 3600

        self.missions = {}
        self.current_mission = None
        self.robot = robot
        if not pipo:
            self.com = Communication(self, robot)
        else:
            self.com = PipoCommunication()
            #self.com = self.com.set_robot_state(self)
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
        #self.missions["m_torche"] = vidange_torches.get_mission(self.com_state_factory)
        #self.missions["m_torche"].prioritize(2.)
        #self.missions["m_arbre1"] = prise_arbre.get_mission(self.com_state_factory)
        #self.missions["m_arbre1"].prioritize(10.)
        self.missions["m_mammouths"] = tir_mamouths.get_mission(self.com_state_factory)
        self.missions["m_mammouths"].prioritize(20.)



    def init_mission_pmi(self):

        #pousse feu
        #prise torche 
        #pose torche
        #pose fresque
        self.missions["m_torche_adv"] = mission_prise_torche_adv.get_mission(self.com_state_factory)
        self.missions["m_torche_adv"].prioritize(10.)
        #tir filet
        #self.missions["m_pousse_feu_loin"] = pousse_feu.get_mission(self.com_state_factory)
        self.missions["m_pose_foyer"] = mission_fresque.get_mission(self.com_state_factory)
        self.missions["m_pose_foyer"].prioritize(0.)

       #self.missions["m_pousse_feu_loin"].prioritize(7.)
       
        self.missions["m_fresque"] = mission_fresque.get_mission(self.com_state_factory)
        self.missions["m_fresque"].prioritize(6.)

        self.missions["m_pose_torche"] = pose_torche.get_mission(self.com_state_factory)
        self.missions["m_pose_torche"].prioritize(4.)
    
        #todo change priority after some time.
        self.missions["m_tir"] = tir_filet.get_mission(self.com_state_factory)
        self.missions["m_tir"].prioritize(2.)

        #self.missions["m_testr"] = mission_test.get_mission(self.com_state_factory, 1)
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
        """ adv is the global coordinate of the adversary (ie vector from robot) """
        global_coords = adv  # self.last_position.add_vector(adv) 
        print  "ADVERSARY AT", global_coords
        self.adversary_detection.insert(0, (time.time(), global_coords))

    def run(self):
        if time.time() - self.debut_game >= 65:
            try:
                self.missions["m_tir"].proritize(8000)
            except:
                pass

    def start_game(self):
        self.debut_game = time.time()

    def start(self):
        while True:
            self.run()
            self.mae.run()
            self.com.run()
            time.sleep(0.0005) 

    def mission_done(self):
        print "#### MISSION DONE ###"
        #for t, mis in self.missions.items():
        #    mis.to_do()
        self.missions[self.current_mission].success()

    def mission_failed(self):
        print "#### MISSION DONE ###"
        #for t, mis in self.missions.items():
        #    mis.to_do()
        self.missions[self.current_mission].success()
        #self.missions[self.current_mission].penalize(5.)

    def prise(self):
        print "yo prise la "
        if "m_pose_foyer" in self.missions.keys():
            print " active mission pose_foyer "
            self.missions["m_pose_foyer"].to_do()
            self.missions["m_pose_foyer"].prioritize(10)

    def set_couleur(self, coul):
        coord.couleur = coul
        if coul:
            self.coul = "jaune"
        else:
            self.coul = "rouge"
        self.init_post_coul()


def get_d_dos_cdg(robot="mark"):
    return 100


