from mae_generator.mae import OutState


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





