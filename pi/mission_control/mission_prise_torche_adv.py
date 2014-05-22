from mission_control.mission import Mission, SuccessOut, FailOut
from utils.coord import Coord
from mae_generator.mae import MAE, InitState, debugger


def get_mission(com_state_factory):
    return Mission(" PRISE torche ", Coord(-500, 1000, 135), MAEPriseTorche(com_state_factory))


class MAEPriseTorche(MAE):
    def __init__(self, com_state_factory):
        MAE.__init__(self)
        self.sf = com_state_factory

        #states
        init = InitState()
        baisse_pince = self.sf.get_pmi_bas()
        active_prise_torche = self.sf.get_pmi_actif_torche()
        avance = self.sf.get_bf_fw(Coord(100))
        tourne = self.sf.get_bf_cap(Coord(0, 0, -45))
        avance2 = self.sf.get_bf_fw(Coord(400))
        tourne2 = self.sf.get_bf_cap(Coord(0, 0, 0))
        avance3 = self.sf.get_bf_fw(Coord(600))
        tourne3 = self.sf.get_bf_cap(Coord(0, 0, 90))
        avance4 = self.sf.get_bf_fw(Coord(300))
        pose = self.sf.get_pmi_pose()
        pose_fail = self.sf.get_pmi_pose()
        recule = self.sf.get_bf_fw(Coord(-200))

        out = SuccessOut()
        out2 = FailOut()

        #transitions
        init.add_instant_transition(baisse_pince)
        baisse_pince.add_time_out_transition(500, active_prise_torche)
        active_prise_torche.add_instant_transition(avance)
        avance.add_afini_transition(tourne)
        tourne.add_afini_transition(avance2)
        avance2.add_afini_transition(tourne2)
        tourne2.add_afini_transition(avance3)
        avance3.add_afini_transition(tourne3)
        tourne3.add_afini_transition(avance4)
        avance4.add_afini_transition(pose)
        pose.add_time_out_transition(200, recule)
        recule.add_afini_transition(out)

        avance.add_advd_transition(out2)
        tourne.add_advd_transition(pose_fail)
        avance2.add_advd_transition(pose_fail)
        tourne2.add_advd_transition(pose_fail)
        avance3.add_advd_transition(pose_fail)
        avance4.add_advd_transition(pose_fail)
        tourne3.add_advd_transition(pose_fail)

        avance.add_bloc_transition(out2)
        tourne.add_bloc_transition(pose_fail)
        avance2.add_bloc_transition(pose_fail)
        tourne2.add_bloc_transition(pose_fail)
        avance3.add_bloc_transition(pose_fail)
        tourne3.add_bloc_transition(pose_fail)
        avance4.add_bloc_transition(pose_fail)

        pose_fail.add_time_out_transition(100, out2)
        recule.add_bloc_transition(out)

        self.state_list = [ 
            init, baisse_pince, active_prise_torche, avance, tourne, avance2, tourne2,
            avance3, tourne3, avance4, pose, recule, pose_fail, out, out2]
        self.reinit_state()


if __name__ == "__main__":
    from com_state_factory import ComStateFactory
    from communication import PipoCommunication
    com = PipoCommunication()
    mae = MAEPriseTorche(ComStateFactory(com))
    com.set_global_mae(mae)
    #mae = MAEGlobal()
    debugger(mae)





