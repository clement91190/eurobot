from mission_control.mission import Mission, SuccessOut, FailOut
from utils.coord import Coord
from mae_generator.mae import MAE, InitState, debugger


def get_mission(com_state_factory, ind=1):
    return Mission(" Pose torche ", Coord(-300, 1050, 90), MAEPoseFeu(com_state_factory))


class MAEPoseFeu(MAE):
    def __init__(self, com_state_factory):
        MAE.__init__(self)
        self.sf = com_state_factory

        #states
        init = InitState()
        avance = self.sf.get_bf_fw(Coord(200))
        pose = self.sf.get_pmi_pose()
        recule = self.sf.get_bf_fw(Coord(-150))
# rajoute transition from actionneur
        out = SuccessOut()
        out2 = FailOut()

        #transitions
        init.add_instant_transition(avance)
        avance.add_advd_transition(pose)
        avance.add_afini_transition(pose)
        avance.add_bloc_transition(pose)
        pose.add_time_out_transition(300, recule) 
        recule.add_afini_transition(out)
        recule.add_bloc_transition(out)

        self.state_list = [ 
            init, avance, pose, recule, out, out2]
        self.reinit_state()


if __name__ == "__main__":
    from com_state_factory import ComStateFactory
    from communication import PipoCommunication
    com = PipoCommunication()
    mae = MAEPoseFeu(ComStateFactory(com))
    com.set_global_mae(mae)
    #mae = MAEGlobal()
    debugger(mae)



