from mission_control.mission import Mission, SuccessOut, FailOut
from utils.coord import Coord
from mae_generator.mae import MAE, InitState, State, debugger


def get_mission(com_state_factory, num_arbre=1):
    if num_arbre == 1:
        return Mission(" arbre 1", Coord(1100, 1300, 180), MAEARBRE(com_state_factory))


class MAEARBRE(MAE):
    def __init__(self, com_state_factory):
        MAE.__init__(self)
        self.sf = com_state_factory

        #states
        init = InitState()
        mi_ouvre = self.sf.get_mark_rateau_mi()
        pipo = State("pipo")
        recule = self.sf.get_bf_fw(Coord(-100))
        ouvre = self.sf.get_mark_rateau_haut()
        avance = self.sf.get_bf_fw(Coord(150))
        replis_mi = self.sf.get_mark_rateau_mi()
        reouvre = self.sf.get_mark_rateau_haut()
        replis = self.sf.get_mark_rateau_bas()
        out = SuccessOut()
        out2 = FailOut()

        #transitions
        init.add_instant_transition(pipo)
        
        mi_ouvre.add_instant_transition(recule)
        recule.add_afini_transition(ouvre)
        ouvre.add_time_out_transition(500, avance)
        avance.add_afini_transition(replis_mi)
        avance.add_bloc_transition(replis_mi)
        avance.add_advd_transition(replis_mi)
        replis_mi.add_time_out_transition(300, reouvre)
        reouvre.add_time_out_transition(300, replis)
        replis.add_time_out_transition(300, out)

        self.state_list = [ 
            init, pipo,  mi_ouvre, recule, ouvre, avance, replis_mi, reouvre, replis, out, out2
         ]
        self.reinit_state()


if __name__ == "__main__":
    from com_state_factory import ComStateFactory
    from communication import PipoCommunication
    com = PipoCommunication()
    mae = MAEARBRE(ComStateFactory(com))
    com.set_global_mae(mae)
    #mae = MAEGlobal()
    debugger(mae)





