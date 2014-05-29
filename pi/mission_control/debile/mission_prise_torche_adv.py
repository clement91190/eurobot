from mission_control.mission import Mission, SuccessOut, FailOut
from utils.coord import Coord
from mae_generator.mae import MAE, InitState, debugger


def get_mission(com_state_factory):
    return Mission(" PRISE torche ", Coord(-300, 800, 135), MAEPriseTorche(com_state_factory))


class MAEPriseTorche(MAE):
    def __init__(self, com_state_factory):
        MAE.__init__(self)
        self.sf = com_state_factory

        #states
        init = InitState()
        baisse_pince = self.sf.get_pmi_bas()
        active_prise_torche = self.sf.get_pmi_actif_torche()
# rajoute transition from actionneur
        avance = self.sf.get_bf_fw(Coord(300))
        out = SuccessOut()
        out2 = FailOut()

        #transitions
        init.add_instant_transition(baisse_pince)
        baisse_pince.add_time_out_transition(500, active_prise_torche)
        active_prise_torche.add_instant_transition(avance)

        avance.add_advd_transition(out2)
        avance.add_afini_transition(out)
        avance.add_bloc_transition(out2)

        self.state_list = [ 
            init, baisse_pince, active_prise_torche, avance, out, out2]
        self.reinit_state()


if __name__ == "__main__":
    from com_state_factory import ComStateFactory
    from communication import PipoCommunication
    com = PipoCommunication()
    mae = MAEPriseTorche(ComStateFactory(com))
    com.set_global_mae(mae)
    #mae = MAEGlobal()
    debugger(mae)





