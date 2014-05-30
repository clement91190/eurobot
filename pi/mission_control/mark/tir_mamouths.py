from mission_control.mission import Mission, SuccessOut, FailOut
from utils import coord
from mae_generator.mae import MAE, InitState, debugger


def get_mission(com_state_factory, num=1):
    return Mission("tir de mammouths", coord.Coord(750, 600, 0), MAETirMammouths(com_state_factory))


class MAETirMammouths(MAE):
    def __init__(self, com_state_factory):
        MAE.__init__(self)
        self.sf = com_state_factory

        #states
        init = InitState()
        if coord.couleur == 1:
            tir1 = self.sf.get_mark_tir_canon_d()
            tir2 = self.sf.get_mark_tir_canon_g()
            repos1 = self.sf.get_mark_repos_canon_d()
            repos2 = self.sf.get_mark_repos_canon_g()
        else:
            tir1 = self.sf.get_mark_tir_canon_g()
            tir2 = self.sf.get_mark_tir_canon_d()
            repos1 = self.sf.get_mark_repos_canon_g()
            repos2 = self.sf.get_mark_repos_canon_d()

        tourne = self.sf.get_bf_cap(coord.Coord(0, 0, 180))
        out = SuccessOut()
        out2 = FailOut()

        #transitions
        init.add_instant_transition(tir1)
        tir1.add_time_out_transition(300, repos1)
        repos1.add_instant_transition(tourne)
        tourne.add_bloc_transition(tourne)
        tourne.add_afini_transition(tir2)
        tir2.add_time_out_transition(300, repos2)
        repos2.add_instant_transition(out)
        #avance.add_bloc_transition(rentre_fresque)
        #avance.add_advd_transition(rentre_fresque)
        #rentre_fresque.add_instant_transition(out)

        self.state_list = [ 
            init, tir1, repos1, tourne, tir2, repos2, out, out2
        ]
        self.reinit_state()


if __name__ == "__main__":
    from com_state_factory import ComStateFactory
    from communication import PipoCommunication
    com = PipoCommunication()
    mae = MAETirMammouths(ComStateFactory(com))
    com.set_global_mae(mae)
    #mae = MAEGlobal()
    debugger(mae)





