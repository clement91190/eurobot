from mission_control.mission import Mission, SuccessOut, FailOut
from utils.coord import Coord
from mae_generator.mae import MAE, InitState, debugger


def get_mission(com_state_factory):
    #raise NotImplementedError(" mission non codee")
    return Mission(" pousse_feu ", Coord(-800, 600, 180), MAEPousseFeu(com_state_factory))


class MAEPousseFeu(MAE):
    def __init__(self, com_state_factory):
        MAE.__init__(self)
        self.sf = com_state_factory

        #states
        init = InitState()
        #tape = self.sf.get_pmi_tape()
        avance_triangle1 = self.sf.get_bf_fw(Coord(200))
        out = SuccessOut()
        out2 = FailOut()

             #transitions
        init.add_instant_transition(avance_triangle1)
        avance_triangle1.add_afini_transition(out)
        avance_triangle1.add_bloc_transition(out)
        avance_triangle1.add_advd_transition(out)

        self.state_list = [ 
            init, avance_triangle1, out, out2
        ]
        self.reinit_state()


if __name__ == "__main__":
    from com_state_factory import ComStateFactory
    from communication import PipoCommunication
    com = PipoCommunication()
    mae = MAEPousseFeu(ComStateFactory(com))
    com.set_global_mae(mae)
    #mae = MAEGlobal()
    debugger(mae)





