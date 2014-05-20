from mae_generator.mae import MAE, MAEState, InitState, OutState, State, debugger
from com_state_factory import ComStateFactory
from mae_generator.pre_start import MAEPrestart
from utils.coord import Coord


class MAEGlobal(MAE):
    def __init__(self, com_state_factory):
        MAE.__init__(self)
        pre_start = MAEState(MAEPrestart(com_state_factory), "pre_start")
        game = MAEState(MAEGame(com_state_factory), "game")
        end = State("end")
        self.state_list = [pre_start, game, end]
        pre_start.transitions["START"] = game
        game.transitions["time_out_90"] = end 
        self.reinit_state()


class MAEGame(MAE):
    def __init__(self, com_state_factory):
        MAE.__init__(self)
        self.sf = com_state_factory
        #state
        rush = MAEState(MAERush(com_state_factory), "rush")
        choix_mission = State("choix_mission")
        deplacement_mission = MAEState(MAEDepMission(), "deplacement_mission")
        evitement = MAEState(MAEEvitement(), "evitement")
        mission = MAEState(MAEMission(), "mission")
        #transitions
        rush.add_out_transition("end_rush", choix_mission)
        choix_mission.transitions[""] = deplacement_mission
        deplacement_mission.add_out_transition("end_deplacement", mission)
        mission.add_out_transition("end_mission", choix_mission)
        mission.add_out_transition("evitement", evitement)
        evitement.add_out_transition("end_evitement", deplacement_mission)

        self.state_list = [
            rush, choix_mission, deplacement_mission, evitement, mission]
        self.reinit_state()
        print "COUCOU", self.current_state


class MAERush(MAE):
    def __init__(self, com_state_factory):
        MAE.__init__(self)
        self.sf = com_state_factory

        init = InitState()
        speed_change = self.sf.get_setspeed(2)
        avance_triangle1 = self.sf.get_bf_fw(Coord(1200))
        
        init.add_instant_transition(speed_change)
        speed_change.add_instant_transition(avance_triangle1)
        self.state_list = [init, speed_change, avance_triangle1, OutState("end_rush")]
        self.reinit_state()


class MAEEvitement(MAE):
    def __init__(self):
        MAE.__init__(self)
        self.state_list = [OutState("end_evitement")]
        self.reinit_state()


class MAEDepMission(MAE):
    def __init__(self):
        MAE.__init__(self)
        self.state_list = [OutState("end_deplacement")]
        self.reinit_state()


class MAEMission(MAE):
    def __init__(self):
        MAE.__init__(self)
        self.state_list = [InitState("init"), OutState("end_mission"), OutState("evitement")]
        self.reinit_state()
       




if __name__ == "__main__":
    #mae = MAEGame()
    mae = MAEGlobal(ComStateFactory("pipo"))
    debugger(mae)
