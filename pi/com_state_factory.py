from mae_generator.mae import State


class SendStopDebileState(State):
    def __init__(self, com):
        State.__init__(self, "END ALL ACTIONS")
        self.com = com

    def in_code(self):
        self.com.send_actio1("S")


class SendStopMarkState(State):
    def __init__(self, com):
        State.__init__(self, "END ALL ACTIONS")
        self.com = com

    def in_code(self):
        self.com.send_actio1("S")
        self.com.send_actio2("S")


class SendSlaveState(State):
    def __init__(self, message, com, count=0):
        State.__init__(self, "{} slave:".format(count) + message)
        self.com = com
        self.message = message

    def in_code(self):
        self.com.send_slave(self.message)
        self.com.send_slave("S1")


class SendActio1MarkState(State):
    def __init__(self, message, com, count=0):
        State.__init__(self, "{} acto1_mark:".format(count) + message)
        self.com = com
        self.message = message

    def in_code(self):
        self.com.send_actio1(self.message)


class SendActio2MarkState(State):
    def __init__(self, message, com, count=0):
        State.__init__(self, "{} actio2_mark:".format(count) + message)
        self.com = com
        self.message = message

    def in_code(self):
        self.com.send_actio2(self.message)


class SendActioPMIState(State):
    def __init__(self, message, com, count=0):
        State.__init__(self, "{} actio_pmi:".format(count) + message)
        self.com = com
        self.message = message

    def in_code(self):
        self.com.send_actio1(self.message)


class ComStateFactory:
    def __init__(self, com):
        self.com = com
        self.count = 0

    #slave related
    def get_bf_cap(self, coord):
        self.count += 1
        return SendSlaveState(coord.bf_cap(), self.com, self.count) 

    def get_bf_fw(self, coord):
        self.count += 1
        return SendSlaveState(coord.bf_fw(), self.com, self.count) 

    def get_bf_droite(self, coord):
        self.count += 1
        return SendSlaveState(coord.bf_droite(), self.com, self.count) 

    def get_recaler(self):
        self.count += 1
        return SendSlaveState("S2", self.com, self.count)

    def get_setxycap(self, coord):
        self.count += 1
        return SendSlaveState(coord.setxycap(), self.com, self.count) 

    def get_setspeed(self, speed):
        self.count += 1
        return SendSlaveState("S7 {}".format(speed), self.com, self.count) 

    def get_pmi_bas(self):
        self.count += 1
        return SendActioPMIState("P0", self.com, self.count) 

    def get_pmi_tape(self):
            self.count += 1
            return SendActioPMIState("P1", self.com, self.count) 

    def get_pmi_range(self):
            self.count += 1
            return SendActioPMIState("P2", self.com, self.count) 

    def get_pmi_actif_feu(self, retourne=False):
            self.count += 1
            if retourne:
                return SendActioPMIState("P3 1", self.com, self.count) 
            else:
                return SendActioPMIState("P3 0", self.com, self.count) 

    def get_pmi_actif_torche(self):
            self.count += 1
            return SendActioPMIState("P4", self.com, self.count) 

    def get_pmi_pose(self):
            self.count += 1
            return SendActioPMIState("P5", self.com, self.count) 

    def get_pmi_fresque_out(self):
            self.count += 1
            return SendActioPMIState("F0", self.com, self.count) 

    def get_pmi_fresque_in(self):
            self.count += 1
            return SendActioPMIState("F1", self.com, self.count) 

    def get_pmi_filet_out(self):
            self.count += 1
            return SendActioPMIState("F2", self.com, self.count) 

    def get_pmi_filet_in(self):
            self.count += 1
            return SendActioPMIState("F3", self.com, self.count) 

# Mark
    def get_mark_go_attente(self):
            self.count += 1
            return SendActio1MarkState("B0", self.com, self.count) 

    def get_mark_ranger(self):
            self.count += 1
            return SendActio1MarkState("B1", self.com, self.count) 

    def get_mark_ir_autom(self):
            self.count += 1
            return SendActio1MarkState("B2", self.com, self.count) 

    def get_mark_ir_desactiv(self):
            self.count += 1
            return SendActio1MarkState("B3", self.com, self.count) 

    def get_mark_prise_centre(self):
            self.count += 1
            return SendActio1MarkState("B4", self.com, self.count) 

    def get_mark_confirme_prise(self, ma_couleur=True):
            self.count += 1
            if ma_couleur:
                return SendActio1MarkState("B5 1", self.com, self.count) 
            else:
                return SendActio1MarkState("B5 0", self.com, self.count) 
#actio2

    def get_mark_ferme_tacled(self):
            self.count += 1
            return SendActio2MarkState("T0", self.com, self.count) 

    def get_mark_mi_tacled(self):
            self.count += 1
            return SendActio2MarkState("T1", self.com, self.count) 

    def get_mark_open_tacled(self):
            self.count += 1
            return SendActio2MarkState("T2", self.com, self.count) 

    def get_mark_ferme_tacleg(self):
            self.count += 1
            return SendActio2MarkState("T3", self.com, self.count) 

    def get_mark_mi_tacleg(self):
            self.count += 1
            return SendActio2MarkState("T4", self.com, self.count) 

    def get_mark_open_tacleg(self):
            self.count += 1
            return SendActio2MarkState("T5", self.com, self.count) 

    def get_mark_charge_canon_g(self):
            self.count += 1
            return SendActio2MarkState("C0", self.com, self.count) 

    def get_mark_tir_canon_g(self):
            self.count += 1
            return SendActio2MarkState("C1", self.com, self.count) 

    def get_mark_repos_canon_g(self):
            self.count += 1
            return SendActio2MarkState("C2", self.com, self.count) 

    def get_mark_charge_canon_d(self):
            self.count += 1
            return SendActio2MarkState("C3", self.com, self.count) 

    def get_mark_tir_canon_d(self):
            self.count += 1
            return SendActio2MarkState("C4", self.com, self.count) 

    def get_mark_repos_canon_d(self):
            self.count += 1
            return SendActio2MarkState("C5", self.com, self.count) 

    def get_mark_rateau_bas(self):
            self.count += 1
            return SendActio2MarkState("R0", self.com, self.count) 

    def get_mark_rateau_mi(self):
            self.count += 1
            return SendActio2MarkState("R1", self.com, self.count) 

    def get_mark_rateau_haut(self):
            self.count += 1
            return SendActio2MarkState("R2", self.com, self.count) 

    def get_mark_benne_close(self):
            self.count += 1
            return SendActio2MarkState("R3", self.com, self.count) 

    def get_mark_benne_open(self):
            self.count += 1
            return SendActio2MarkState("R4", self.com, self.count) 

    def get_mark_ejecte_d_1(self):
            self.count += 1
            return SendActio2MarkState("P0", self.com, self.count) 

    def get_mark_vidange_d(self):
            self.count += 1
            return SendActio2MarkState("P1", self.com, self.count) 

    def get_mark_consulte_d(self):
            self.count += 1
            return SendActio2MarkState("P2", self.com, self.count) 

    def get_mark_ejecte_g_1(self):
            self.count += 1
            return SendActio2MarkState("P3", self.com, self.count) 

    def get_mark_vidange_g(self):
            self.count += 1
            return SendActio2MarkState("P4", self.com, self.count) 

    def get_mark_consulte_g(self):
            self.count += 1
            return SendActio2MarkState("P5", self.com, self.count) 

    def get_stop(self, robot):
        if robot == "debile":
            return SendStopDebileState(self.com)
        else:
            return SendStopMarkState(self.com)
         










        
