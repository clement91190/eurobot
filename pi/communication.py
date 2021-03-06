import serial 
import time
from utils.coord import Coord

welcome_messages = ["#SLAVE READY\r\n", "#ACTIO READY\r\n", "#ACTIO2 READY\r\n"]


class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'


class Communication:
    def __init__(self, robot_state, robot="mark"):
        self.robot_state = robot_state

        if robot == "mark":
            self.slave, self.actio1, self.actio2 = self.init_arduinos_mark()
            self.serials = [self.slave, self.actio1, self.actio2]
            self.arduinos = {
                "slave": self.slave,
                "actio1": self.actio1,
                "actio2": self.actio2}
        else:
            self.slave, self.actio1 = self.init_arduinos_debile()
            self.serials = [self.slave, self.actio1]
            self.arduinos = {
                "slave": self.slave,
                "actio1": self.actio1}

    def set_global_mae(self, global_mae):
        self.global_mae = global_mae
            
    def non_blocking_read_line(self, ser):
        """ careful, assert that everything is writen line by line """
        if ser.inWaiting():
            return ser.readline()

    def get_serial_name(self, serr):
        """ done when initializing the boards. """
        if serr is None:
            print "missing arduino"
            return None
        while True:
            line = self.non_blocking_read_line(serr)
            if line in welcome_messages:
                return line
            else:
                print line
                time.sleep(0.4)
   #             raw_input()
                print "weird reading... check connection"

    def init_arduinos_mark(self):
        """ return the 3 correct serial connection """
        ser1 = ser2 = ser3 = None

        try:
            ser1 = serial.Serial('/dev/ttyUSB0', 9600)
            # Toggle DTR to reset Arduino
            ser1.setDTR(False)
            time.sleep(1)
# toss any data already received, see
# http://pyserial.sourceforge.net/pyserial_api.html#serial.Serial.flushInput
            ser1.flushInput()
            ser1.setDTR(True)
        except:
            pass
        try:

            ser2 = serial.Serial('/dev/ttyUSB1', 9600)
            ser2.setDTR(False)
            time.sleep(1)
# toss any data already received, see
# http://pyserial.sourceforge.net/pyserial_api.html#serial.Serial.flushInput
            ser2.flushInput()
            ser2.setDTR(True)

        except:
            print "WARNING"
 
        try:

            ser3 = serial.Serial('/dev/ttyUSB2', 9600)
            ser3.setDTR(False)
            time.sleep(1)
# toss any data already received, see
# http://pyserial.sourceforge.net/pyserial_api.html#serial.Serial.flushInput
            ser3.flushInput()
            ser3.setDTR(True)

        except:
            print "WARNING"
   
        d = {}
        d[self.get_serial_name(ser1)] = ser1
        d[self.get_serial_name(ser2)] = ser2
        d[self.get_serial_name(ser3)] = ser3
        d[None] = None


        slave = d.get(welcome_messages[0], None)
        actio1 = d.get(welcome_messages[1], None)
        actio2 = d.get(welcome_messages[2], None)
        if slave is None:
            print "No Slave"
        if actio1 is None:
            print "No actio1"
        if actio2 is None:
            print "No actio2"

        return slave, actio1, actio2

    def init_arduinos_debile(self):
        """ return the 3 correct serial connection """
        ser1 = ser2 = None

        try:
            ser1 = serial.Serial('/dev/ttyUSB0', 9600)
            # Toggle DTR to reset Arduino
            ser1.setDTR(False)
            time.sleep(1)
# toss any data already received, see
# http://pyserial.sourceforge.net/pyserial_api.html#serial.Serial.flushInput
            ser1.flushInput()
            ser1.setDTR(True)
        except:
            pass
        try:

            ser2 = serial.Serial('/dev/ttyUSB1', 9600)
            ser2.setDTR(False)
            time.sleep(1)
# toss any data already received, see
# http://pyserial.sourceforge.net/pyserial_api.html#serial.Serial.flushInput
            ser2.flushInput()
            ser2.setDTR(True)

        except:
            print "WARNING"
        
        d = {}
        d[self.get_serial_name(ser1)] = ser1
        d[self.get_serial_name(ser2)] = ser2
        d[None] = None

        slave = d.get(welcome_messages[0], None)
        actio1 = d.get(welcome_messages[1], None)
        if slave is None:
            print "No Slave"
        if actio1 is None:
            print "No actio1"

        return slave, actio1

    def treat_line(self, line):
        s_line = line.split()
        if s_line[0] == "#":
            print bcolors.OKGREEN, "[READ TRANS]",  line, bcolors.ENDC
            if s_line[1] in ["BLOC", "AFINI", "ADVD", "NEAR", "START", "STARTIN", "PRISE"]:
                if s_line[1] == "PRISE":
                    self.robot_state.prise()
                if s_line[1] == "START":
                    self.robot_state.start_game()
         
                self.global_mae.trigger(s_line[1])
            else:
                print "unkown transition", line
        elif s_line[0] == "*":
            print bcolors.FAIL, "[READ DATA]",  line, bcolors.ENDC
            #parameters
            if s_line[1] == "STRAT":
                self.robot_state.strat = [int(v) for v in s_line[2:]]
            elif s_line[1] == "COUL":
                self.robot_state.set_couleur(int(s_line[2]))
            elif s_line[1] == "ADVD":
                self.robot_state.adversary_detected(Coord(int(s_line[2]), int(s_line[3]), float(s_line[4])).couleur_relative())
            elif s_line[1] == "COORD":
                self.robot_state.set_last_position(Coord(int(s_line[2]), int(s_line[3]), float(s_line[4])).couleur_relative())
        else:
            print "[CRAP]", s_line

    def run(self):
        """ read message and send transitions """
        for ser in self.serials:
            if ser is not None:
                line = self.non_blocking_read_line(ser)
                while line is not None:
                    self.treat_line(line)
                    line = self.non_blocking_read_line(ser)

    def send(self, arduino, message):
        print bcolors.HEADER, "[SEND -> ", arduino, " ]", message, bcolors.ENDC
        try:
            self.arduinos[arduino].write(message + "\n")
        except:
            print "CONNECTION PROBLEM"

    #defintion of all the send function related to slave.
    
    def send_slave(self, message):
        self.send("slave", message)  # most message are in coord.py

    def send_actio1(self, message):
        self.send("actio1", message)  # most message are in coord.py

    def send_actio2(self, message):
        self.send("actio2", message)  # most message are in coord.py


class PipoCommunication:
    """ for debugging purposes, simulate communication """
    def __init__(self):
        print "WARNING PIPO COM"
        print "cretin"

    def set_global_mae(self, global_mae):
        self.global_mae = global_mae

    def set_robot_state(self, robot_state):
        self.robot_state = robot_state
 
    def run(self):
        print "again"
        time.sleep(0.1)
        t = raw_input()
        print t
        if t == "PRISE":
            self.robot_state.prise()
        if t != "":
            self.global_mae.trigger(t)

    def send(self, arduino, message):
        print "##SEND## ", message, " @ ", arduino

    #defintion of all the send function related to slave.
    
    def send_slave(self, message):
        self.send("slave", message)  # most message are in coord.py

    def send_actio1(self, message):
        self.send("actio1", message)  # most message are in coord.py

    def send_actio2(self, message):
        self.send("actio2", message)  # most message are in coord.py


def init(ser):
    ser.setDTR(False)
    time.sleep(1)
# toss any data already received, see
# http://pyserial.sourceforge.net/pyserial_api.html#serial.Serial.flushInput
    ser.flushInput()
    ser.setDTR(True)


def main():
    ser = serial.Serial('/dev/ttyUSB0')
    init(ser)
    print "/dev/ttyUSB0  ->", ser.readline()
    ser = serial.Serial('/dev/ttyUSB1')
    init(ser)
    print "/dev/ttyUSB1  ->", ser.readline()
    ser = serial.Serial('/dev/ttyUSB2')
    init(ser)
    print "/dev/ttyUSB2  ->", ser.readline()
    

if __name__ == "__main__":
    main()
    
