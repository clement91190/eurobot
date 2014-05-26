from mission_control.mission import Mission, SuccessOut, FailOut
from utils.coord import Coord
from mae_generator.mae import MAE, InitState, debugger


def get_mission(com_state_factory):
    #raise NotImplementedError(" mission non codee")
    return Mission(" fresques ", Coord(1000, 450, 90), MAETirFilet(com_state_factory))

""" special mission to shoot the net, no out of this mission ! """


class MAETirFilet(MAE):
    def __init__(self, com_state_factory):
        MAE.__init__(self)
        self.sf = com_state_factory

        #states
        init = InitState()
        recaly = self.sf.get_recaler()
        range = self.sf.get_pmi_range()
        set_y0 = self.sf.get_setxycap(Coord(-1, 102 + 300, 90)) 
        #pose_fresque = self.sf.()
        #avance = self.sf.get_bf_fw(Coord(100))
        #rentre_fresque = self.sf.get_pmi_fresque_in()
        out = SuccessOut()
        out2 = FailOut()

        #transitions
        init.add_instant_transition(range)
        range.add_instant_transition(recaly)
        recaly.add_bloc_transition(set_y0)
        #set_y0.add_instant_transition(pose_fresque)
        #pose_fresque.add_time_out_transition(500, avance)
        #avance.add_afini_transition(rentre_fresque)
        #avance.add_bloc_transition(rentre_fresque)
        #avance.add_advd_transition(rentre_fresque)
        #rentre_fresque.add_instant_transition(out)

        self.state_list = [ 
            init, recaly, range, set_y0, out, out2 # , pose_fresque, avance, rentre_fresque, out, out2i
         ]
        self.reinit_state()


if __name__ == "__main__":
    from com_state_factory import ComStateFactory
    from communication import PipoCommunication
    com = PipoCommunication()
    mae = MAETirFilet(ComStateFactory(com))
    com.set_global_mae(mae)
    #mae = MAEGlobal()
    debugger(mae)





