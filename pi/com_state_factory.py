from mae_generator.mae import State


class SendSlaveState(State):
    def __init__(self, message, com, count=0):
        State.__init__(self, "{} slave:".format(count) + message)
        self.com = com
        self.message = message

    def in_code(self):
        self.com.send_slave("S1")
        self.com.send_slave(self.message)


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








        
