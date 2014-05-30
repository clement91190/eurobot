from mission_control.mission import Mission, SuccessOut, FailOut
from utils.coord import Coord
from mae_generator.mae import MAE, InitState, debugger


def get_mission(com_state_factory, ind=0):
    if ind == 0:
        return Mission(" feux ", Coord(1150, 800, 0), FeuVertical(com_state_factory, False))
    if ind == 1:
        return Mission(" feux ", Coord(900, 1600, 180), FeuVertical(com_state_factory, False))


class FeuVertical(MAE):
    def __init__(self, com_state_factory, ma_couleur=True):
        MAE.__init__(self)
        self.sf = com_state_factory

        #states
        init = InitState()
        prise_centre = self.sf.get_mark_prise_centre()
        set_speed = self.sf.get_setspeed(0)
        avance = self.sf.get_bf_fw(Coord(100))
        range = self.sf.get_mark_confirme_prise(int(ma_couleur)) 
        set_speed1 = self.sf.get_setspeed(1)
        out = SuccessOut()
        out2 = FailOut()

        #transitions
        init.add_instant_transition(prise_centre)
        prise_centre.add_time_out_transition(6000, set_speed)
        set_speed.add_instant_transition(avance)
        avance.add_afini_transition(range)
        avance.add_bloc_transition(range)
        avance.add_advd_transition(range)
        range.add_instant_transition(set_speed1)
        set_speed1.add_instant_transition(out)
    
        #set_y0.add_instant_transition(pose_fresque)
        #pose_fresque.add_time_out_transition(500, avance)
        #avance.add_afini_transition(rentre_fresque)
        #avance.add_bloc_transition(rentre_fresque)
        #avance.add_advd_transition(rentre_fresque)
        #rentre_fresque.add_instant_transition(out)

        self.state_list = [ 
            init, prise_centre, avance, set_speed1, set_speed, range, out, out2
         ]
        self.reinit_state()


if __name__ == "__main__":
    from com_state_factory import ComStateFactory
    from communication import PipoCommunication
    com = PipoCommunication()
    mae = FeuVertical(ComStateFactory(com))
    com.set_global_mae(mae)
    #mae = MAEGlobal()
    debugger(mae)





