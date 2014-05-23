from mae_generator.mae import State


class SendSlaveState(State):
    def __init__(self, message, com, count=0):
        State.__init__(self, "{} slave:".format(count) + message)
        self.com = com
        self.message = message

    def in_code(self):
        self.com.send_slave(self.message)
        self.com.send_slave("S1")


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

    def get_pmi_actif_feu(self):
            self.count += 1
            return SendActioPMIState("P3", self.com, self.count) 

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
            return SendActioPMIState("F1", self.com, self.count) 









        
