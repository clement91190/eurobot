from mae_generator.mae import MAE, MAEState, InitState, OutState, State, debugger
#from com_state_factory import ComStateFactory
from mae_generator.pre_start import MAEPrestart
from utils.coord import Coord
from mission_control.mission import ChoixMissionState, MissionChoiceInit


class MAEGlobal(MAE):
    def __init__(self, com_state_factory, slave_manager, robot_state):
        MAE.__init__(self)
        self.robot_state = robot_state
        pre_start = MAEState(MAEPrestart(com_state_factory), "pre_start")
        game = self.game = MAEState(MAEGame(com_state_factory, slave_manager, robot_state), "game")
        end = State("end")
        self.state_list = [pre_start, game, end]
        pre_start.transitions["START"] = game
        game.transitions["time_out_90"] = end 
        self.reinit_state()


class MAEGame(MAE):
    def __init__(self, com_state_factory, slave_manager, robot_state):
        MAE.__init__(self)
        self.sf = com_state_factory
        self.slave_manager = slave_manager
        self.robot_state = robot_state
        #state
        rush = MAEState(MAERush(com_state_factory), "rush")
        choix_mission = ChoixMissionState(robot_state)
        self.mae_dep = mae_dep = MAEDepMission(slave_manager)
        mae_dep.update_dep()
        self.deplacement_mission = deplacement_mission = MAEState(mae_dep, "deplacement_mission")
        #evitement = MAEState(MAEEvitement(), "evitement")
        mission = self.mission_state = MAEState(MAEMission(robot_state), "mission")
        #transitions
        rush.add_out_transition("end_rush", choix_mission)
        choix_mission.transitions[""] = deplacement_mission
        deplacement_mission.add_out_transition("end_deplacement", mission)
        mission.add_out_transition("end_mission", choix_mission)
        #evitement.add_out_transition("end_evitement", deplacement_mission)

        self.state_list = [
            rush, choix_mission, deplacement_mission, mission]
        self.reinit_state()
        print "COUCOU", self.current_state

    def update_dep(self, mission):
  #update movements...
        self.mae_dep.update_dep(mission)
        self.deplacement_mission.add_out_transition(
            "end_deplacement",
            self.mission_state)


class MAERush(MAE):
    def __init__(self, com_state_factory):
        MAE.__init__(self)
        self.sf = com_state_factory

        init = InitState()
        speed_change = self.sf.get_setspeed(1)
        avance_triangle1 = self.sf.get_bf_fw(Coord(1600))
        tourne = self.sf.get_bf_cap(Coord(0, 0, 90))
        avance = self.sf.get_bf_fw(Coord(-400))
        out = OutState("end_rush")
        
        init.add_instant_transition(speed_change)
        speed_change.add_instant_transition(avance_triangle1)
        avance_triangle1.add_afini_transition(tourne)
        tourne.add_afini_transition(avance)
        avance.add_afini_transition(out)
        self.state_list = [init, speed_change, avance_triangle1, tourne, avance, out]
        self.reinit_state()


class MAEEvitement(MAE):
    def __init__(self):
        MAE.__init__(self)
        self.state_list = [OutState("end_evitement")]
        self.reinit_state()


#TODO
class MAEDepMission(MAE):
    def __init__(self, slave_manager):
        MAE.__init__(self)
        self.state_list = [OutState("end_deplacement")]
        self.reinit_state()
        self.slave_manager = slave_manager

    def update_dep(self, mission=None):
        if mission is not None:
            self.state_list = self.slave_manager.go_to_pathfinder(mission.start_coordinate)
        else:
            self.state_list = self.slave_manager.go_to_pathfinder(Coord(1200, 200, 90))
        self.draw()
        import os
        os.system("xdg-open mae_render.png")
        self.reinit_state()


class MAEMission(MAE):
    def __init__(self, robot_state):
        MAE.__init__(self)
        self.missions = robot_state.missions 
        
        #state
        init = MissionChoiceInit(self, robot_state)
        out = OutState("end_mission")
        #missions_state
        missions_states = []
        for name, mission in self.missions.items():
            missions_states.append(MAEState(mission.mae, name))
            missions_states[-1].add_out_transition("out_fail", out)
            missions_states[-1].add_out_transition("out_success", out)
            init.add_transition(name, missions_states[-1])
        
        self.state_list = [init] + missions_states + [out]
        self.reinit_state()
       

if __name__ == "__main__":
    from robot_state import RobotState
    robot = RobotState("debile", pipo=True)
    mae = robot.mae.game.mae  # MAEGAME
    #mae = robot.mae  #MAEGlobal
    #mae = robot.mae.game.mae.mission_state.mae  # MAEMISSION
    debugger(mae)
