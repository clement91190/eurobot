from mae_generator.mae import State, InitState, OutState


class MissionChoiceInit(InitState):
    def __init__(self, mae_mission, robot_state):
        InitState.__init__(self, name="init_missions")
        self.mae_mission = mae_mission
        self.robot_state = robot_state

    def while_code(self):
        self.mae_mission.trigger(self.robot_state.current_mission)


class ChoixMissionState(State):
    def __init__(self, robot_state):
        State.__init__(self, name="ChoixMission")
        self.robot_state = robot_state

    def in_code(self):
        self.robot_state.choose_mission()
        

class SuccessOut(OutState):
    def __init__(self, name="out_success"):
        OutState.__init__(self, name)


class FailOut(OutState):
    def __init__(self, name="out_fail"):
        OutState.__init__(self, name)


class Mission:
    def __init__(self, name, start_coordinate, mae):
        self.start_coordinate = start_coordinate
        self.mae = mae
        self.priorite = 0
        self.done = True
        self.points = 0

    def success(self):
        self.done = True

    def to_do(self):
        self.done = False

    def penalize(self, p):
        self.prorite -= p





