from mae import MAE, MAEState, InitState, OutState, State, debugger


class MAEGlobal(MAE):
    def __init__(self):
        MAE.__init__(self)
        pre_start = State("pre_start")
        game = MAEState(MAEGame(), "game")
        end = State("end")
        self.state_list = [pre_start, game, end]
        pre_start.transitions["start"] = game
        game.transitions["time_out_90"] = end 
        self.reinit_state()


class MAEGame(MAE):
    def __init__(self):
        MAE.__init__(self)
        #state
        rush = MAEState(MAERush(), "rush")
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


class MAERush(MAE):
    def __init__(self):
        MAE.__init__(self)
        self.state_list = [OutState("end_rush")]
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
    mae = MAEGame()
    #mae = MAEGlobal()
    debugger(mae)
