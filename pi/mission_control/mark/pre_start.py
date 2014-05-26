from mae_generator.mae import MAE, InitState, OutState, debugger
from com_state_factory import ComStateFactory
from utils.coord import Coord


class MAEPrestart(MAE):
    def __init__(self, com_state_factory):
        MAE.__init__(self)
        self.sf = com_state_factory
        d_dos_cdg = 85  # distance entre le dos et le centre de gravite du robot
       
        #state
        wait = InitState()
        recalx = self.sf.get_recaler()
        set_x0 = self.sf.get_setxycap(Coord(d_dos_cdg, 400, 90)) 
        avance_plac_x = self.sf.get_bf_fw(Coord(50))
        tourne = self.sf.get_bf_cap(Coord(0, 0, 90))
        recaly = self.sf.get_recaler()
        set_y0 = self.sf.get_setxycap(Coord(-1, d_dos_cdg, 180))  # TODO change this with real state of the robot 
        out = OutState()

        #transitions
        wait.add_transition("STARTIN", recaly)
        recalx.add_bloc_transition(set_x0)
        set_x0.add_instant_transition(avance_plac_x)
        avance_plac_x.add_afini_transition(tourne)
        tourne.add_afini_transition(recaly)
        recaly.add_bloc_transition(set_y0)
        set_y0.add_transition("START", out)

        self.state_list = [
            wait, recaly, set_x0, avance_plac_x, tourne, recaly, set_y0, out]
        self.reinit_state()

 
if __name__ == "__main__":
    mae = MAEPrestart(ComStateFactory("pipo"))
    #mae = MAEGlobal()
    debugger(mae)

