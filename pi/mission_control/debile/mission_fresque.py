from mission_control.mission import Mission, SuccessOut, FailOut
from utils.coord import Coord
from mae_generator.mae import MAE, InitState, debugger


def get_mission(com_state_factory):
    return Mission(" fresques ", Coord(0, 350, 90), MAEFRESQUE(com_state_factory))


class MAEFRESQUE(MAE):
    def __init__(self, com_state_factory):
        MAE.__init__(self)
        self.sf = com_state_factory

        #states
        init = InitState()
        recaly = self.sf.get_recaler()
        set_y0 = self.sf.get_setxycap(Coord(-1, 102, 90)) 
        pose_fresque = self.sf.get_pmi_fresque_out()
        avance = self.sf.get_bf_fw(Coord(100))
        rentre_fresque = self.sf.get_pmi_fresque_in()
        out = SuccessOut()
        out2 = FailOut()

        #transitions
        init.add_instant_transition(recaly)
        recaly.add_bloc_transition(set_y0)
        set_y0.add_instant_transition(pose_fresque)
        pose_fresque.add_time_out_transition(30000, avance)
        avance.add_afini_transition(rentre_fresque)
        avance.add_bloc_transition(rentre_fresque)
        avance.add_advd_transition(rentre_fresque)
        rentre_fresque.add_instant_transition(out)

        self.state_list = [ 
            init, recaly, set_y0, pose_fresque, avance, rentre_fresque, out, out2]
        self.reinit_state()


if __name__ == "__main__":
    from com_state_factory import ComStateFactory
    from communication import PipoCommunication
    com = PipoCommunication()
    mae = MAEFRESQUE(ComStateFactory(com))
    com.set_global_mae(mae)
    #mae = MAEGlobal()
    debugger(mae)





