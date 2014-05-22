import serial 
import time

welcome_messages = ["#SLAVE READY\r\n", "#ACTIO READY\r\n", "#ACTIO2 READY\r\n"]


class Communication:
    def __init__(self, robot="mark"):

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
        ser1 = serial.Serial('/dev/ttyUSB0', 9600)
        ser2 = serial.Serial('/dev/ttyUSB1', 9600)
        ser3 = serial.Serial('/dev/ttyUSB2', 9600)

        ser1.close()
        ser1.open()

        ser2.close()
        ser2.open()

        ser3.close()
        ser3.open()
    
        d = {}
        d[self.get_serial_name(ser1)] = ser1
        d[self.get_serial_name(ser2)] = ser2
        d[self.get_serial_name(ser3)] = ser3
        d[None] = None

        slave = d[welcome_messages[0]]
        actio1 = d[welcome_messages[1]]
        actio2 = d[welcome_messages[2]]

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
        print line
        if line[0] == "#":
            if line == "#BLOC\r\n":
                self.global_mae.trigger("BLOC")
            elif line[:5] == "#ADVD":
                #TODO add read adversary position
                self.global_mae.trigger("ADVD")
            elif line == "#AFINI\r\n":
                self.global_mae.trigger("AFINI")
            elif line == "#NEAR\r\n":
                self.global_mae.trigger("NEAR")
            elif line == "#START\r\n":
                self.global_mae.trigger("START")
            elif line == "#STARTIN\r\n":
                self.global_mae.trigger("STARTIN")
            else:
                print "unkown transition", line

    def run(self):
        """ read message and send transitions """
        for ser in self.serials:
            if ser is not None:
                line = self.non_blocking_read_line(ser)
                while line is not None:
                    self.treat_line(line)
                    line = self.non_blocking_read_line(ser)
            

    def send(self, arduino, message):
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

    def set_global_mae(self, global_mae):
        self.global_mae = global_mae
 
    def run(self):
        time.sleep(0.1)
        t = raw_input()
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



