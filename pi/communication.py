import serial 

welcome_messages = ["#SLAVE READY\n", "#ACTIO1 READY\n", "#ACTIO2 READY\n"]


class Communication:
    def __init__(self, robot="mark"):
        if robot == "mark":
            self.slave, self.actio1, self.actio2 = self.init_arduinos_mark()
            self.arduinos = {
                "slave": self.slave,
                "actio1": self.actio1,
                "actio2": self.actio2}
        else:
            self.slave, self.actio1 = self.init_arduinos_debile()
            self.arduinos = {
                "slave": self.slave,
                "actio1": self.actio1}

    def non_blocking_read_line(self, ser):
        """ careful, assert that everything is writen line by line """
        if ser.inWaiting():
            return ser.readline()

    def get_serial_name(self, serr):
        """ done when initializing the boards. """
        while True:
            line = self.non_blocking_read_line(serr)
            if line in welcome_messages:
                return line
            else:
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
        ser1 = serial.Serial('/dev/ttyUSB0', 9600)
        ser2 = serial.Serial('/dev/ttyUSB1', 9600)

        ser1.close()
        ser1.open()

        ser2.close()
        ser2.open()
    
        d = {}
        d[self.get_serial_name(ser1)] = ser1
        d[self.get_serial_name(ser2)] = ser2
        d[None] = None

        slave = d[welcome_messages[0]]
        actio1 = d[welcome_messages[1]]

        return slave, actio1

    def run(self):
        """ read message and send transitions """
        #TODO

    def send(self, arduino, message):
        try:
            self.arduinos[arduino].write(message + "\n")
        except:
            print "CONNECTION PROBLEM"

