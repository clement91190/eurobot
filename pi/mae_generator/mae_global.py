from mae_generator.mae import MAE, MAEState, InitState, OutState, State, debugger
#from com_state_factory import ComStateFactory
from utils.coord import Coord
from mission_control.mission import ChoixMissionState, MissionChoiceInit


class MAEGlobal(MAE):
    def __init__(self, com_state_factory, slave_manager, robot_state):
        MAE.__init__(self)
        self.robot_state = robot_state
        init = InitState()

        if self.robot_state.robot == "debile":
            from mission_control.debile.pre_start import MAEPrestart
        else:
            from mission_control.mark.pre_start import MAEPrestart

        pre_start = MAEState(MAEPrestart(com_state_factory), "pre_start" + self.robot_state.robot)

        game = self.game = MAEState(MAEGame(com_state_factory, slave_manager, robot_state), "game")
        end = com_state_factory.get_stop(self.robot_state.robot)
        self.state_list = [init, pre_start, game, end]
        init.add_instant_transition(pre_start)
        pre_start.transitions["START"] = game
        game.add_time_out_transition(90000, end)
        self.reinit_state()


class MAEGame(MAE):
    def __init__(self, com_state_factory, slave_manager, robot_state):
        MAE.__init__(self)
        self.sf = com_state_factory
        self.slave_manager = slave_manager
        self.robot_state = robot_state
        #state
        if self.robot_state.robot == "debile":
            rush = MAEState(MAERushDebile(com_state_factory), "rush")
        else:
            rush = MAEState(MAERushMark(com_state_factory), "rush")

        self.choix_mission = choix_mission = ChoixMissionState(robot_state)
        self.mae_dep = mae_dep = MAEDepMission(slave_manager)
        self.deplacement_mission = deplacement_mission = MAEState(mae_dep, "deplacement_mission")
        #evitement = MAEState(MAEEvitement(), "evitement")
        mission = self.mission_state = MAEState(MAEMission(robot_state), "mission")
        #transitions
        rush.add_out_transition("end_rush", choix_mission)
        choix_mission.transitions[""] = deplacement_mission
        deplacement_mission.add_out_transition("end_deplacement", mission)
        deplacement_mission.add_out_transition("end_evitement", choix_mission)
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

        self.deplacement_mission.add_out_transition(
            "end_evitement",
            self.choix_mission)


class MAERushDebile(MAE):
    def __init__(self, com_state_factory):
        MAE.__init__(self)
        self.sf = com_state_factory

        init = InitState()
        speed_change = self.sf.get_setspeed(2)
        tape = self.sf.get_pmi_tape()
        avance_triangle1 = self.sf.get_bf_fw(Coord(1200))
        out = OutState("end_rush")
        pre_out = self.sf.get_setspeed(1)
        
        init.add_instant_transition(tape)
        tape.add_instant_transition(speed_change)
        speed_change.add_instant_transition(avance_triangle1)
        avance_triangle1.add_afini_transition(pre_out)
        avance_triangle1.add_bloc_transition(pre_out)
        avance_triangle1.add_advd_transition(pre_out)
        pre_out.add_instant_transition(out)
        self.state_list = [init, speed_change, avance_triangle1, pre_out, out]
        self.reinit_state()


class MAERushMark(MAE):
    def __init__(self, com_state_factory):
        MAE.__init__(self)
        self.sf = com_state_factory

        init = InitState()
        wait = State("wait")
        speed_change = self.sf.get_setspeed(1)
        avance_sortie = self.sf.get_bf_fw(Coord(350))
        tourne = self.sf.get_bf_cap(Coord(0, 0, 120))
        rush = self.sf.get_bf_fw(Coord(700))
        out = OutState("end_rush")
        
        init.add_instant_transition(wait)
        wait.add_time_out_transition(3000, speed_change) 
        speed_change.add_instant_transition(avance_sortie)
        avance_sortie.add_afini_transition(tourne)
        tourne.add_afini_transition(rush)
        rush.add_afini_transition(out)
        rush.add_bloc_transition(out)
        self.state_list = [init, wait, speed_change, avance_sortie, tourne, rush, out]
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
        self.state_list = [OutState("end_deplacement"), OutState("end_evitement")]
        self.reinit_state()
        self.slave_manager = slave_manager

    def update_dep(self, mission=None):
        if mission is not None:
            self.state_list = self.slave_manager.go_to_pathfinder(mission.start_coordinate)
        self.reinit_state()


class SuccessOutMission(State):
    def __init__(self, robot_state):
        State.__init__(self, "success_out_mission")
        self.robot_state = robot_state

    def in_code(self):
        self.robot_state.mission_done()


class FailOutMission(State):
    def __init__(self, robot_state):
        State.__init__(self, "fail_out_mission")
        self.robot_state = robot_state

    def in_code(self):
        self.robot_state.mission_failed()


class MAEMission(MAE):
    def __init__(self, robot_state):
        MAE.__init__(self)
        self.missions = robot_state.missions 
        self.robot_state = robot_state
        
        #state
        init = MissionChoiceInit(self, robot_state)
        from_out_success = SuccessOutMission(self.robot_state)
        from_out_failed = FailOutMission(self.robot_state)
        out = OutState("end_mission")
        #missions_state
        missions_states = []
        for name, mission in self.missions.items():
            missions_states.append(MAEState(mission.mae, name))
            missions_states[-1].add_out_transition("out_fail", from_out_failed)
            missions_states[-1].add_out_transition("out_success", from_out_success)
            init.add_transition(name, missions_states[-1])

        from_out_success.add_instant_transition(out)
        from_out_failed.add_instant_transition(out)
        
        self.state_list = [init] + missions_states + [from_out_failed, from_out_success, out]
        self.reinit_state()
        

if __name__ == "__main__":
    from robot_state import RobotState
    robot = RobotState("debile", pipo=True)
    #mae = robot.mae.game.mae.mae_dep  # MAEGAME
    mae = robot.mae  # MAEGlobal
    #mae = robot.mae.game.mae.mission_state.mae  # MAEMISSION
    debugger(mae)
