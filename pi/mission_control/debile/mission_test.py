from mission_control.mission import Mission, SuccessOut, FailOut
from utils.coord import Coord
from mae_generator.mae import MAE, InitState, debugger


def get_mission(com_state_factory, ind=1):
    if ind == 1:
        return Mission(" test 1 ", Coord(500, 300, 90), MAETEST(com_state_factory))
    else:
        return Mission(" test2 ", Coord(1100, 800, 90), MAETEST(com_state_factory))


class MAETEST(MAE):
    def __init__(self, com_state_factory):
        MAE.__init__(self)
        self.sf = com_state_factory

        #states
        init = InitState()
        out = SuccessOut()
        out2 = FailOut()

        #transitions
        init.add_instant_transition(out)

        self.state_list = [ 
            init, out, out2]
        self.reinit_state()


if __name__ == "__main__":
    from com_state_factory import ComStateFactory
    from communication import PipoCommunication
    com = PipoCommunication()
    mae = MAETEST(ComStateFactory(com))
    com.set_global_mae(mae)
    #mae = MAEGlobal()
    debugger(mae)





