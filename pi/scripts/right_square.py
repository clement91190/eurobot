import serial
import time
from mae_generator.mae import MAE, State


serr = serial.Serial('/dev/ttyUSB0', 9600)


class Turn1(State):
    def in_code(self):
        print "coucou"
        serr.write('S3 0\n')


class Forward1(State):
    def in_code(self):
        serr.write('S1\n')
        while serr.inWaiting() > 0:
            print serr.readline()
        raw_input()
        serr.write('S4 250\n')


class Turn2(State):
    def in_code(self):
        serr.write('S3 90\n')


class Forward2(State):
    def in_code(self):
        serr.write('S4 250\n')


class Turn3(State):
    def in_code(self):
        serr.write('S3 180\n')


class Forward3(State):
    def in_code(self):
        serr.write('S4 250\n')


class Turn4(State):
    def in_code(self):
        serr.write('S3 -90\n')


class Forward4(State):
    def in_code(self):
        serr.write('S4 250\n')


class MAESquare(MAE):
    def __init__(self):
        MAE.__init__(self)
        #self.state_list = [Turn1(), Forward1(), Turn2(), Forward2(), Turn3(), Forward3(), Turn4(), Forward4()]
        self.state_list = [Turn1(), Forward1(), Turn4(), Forward2(), Turn3(), Forward3(), Turn2(), Forward4()]
        self.state_list[0].transitions["AFINI"] = self.state_list[1]
        self.state_list[1].transitions["AFINI"] = self.state_list[2]
        self.state_list[2].transitions["AFINI"] = self.state_list[3]
        self.state_list[3].transitions["AFINI"] = self.state_list[4]
        self.state_list[4].transitions["AFINI"] = self.state_list[5]
        self.state_list[5].transitions["AFINI"] = self.state_list[6]
        self.state_list[6].transitions["AFINI"] = self.state_list[7]
        self.state_list[7].transitions["AFINI"] = self.state_list[0]
        self.reinit_state()


def main():
    mae = MAESquare()
    mae.draw()
    #time.sleep(1)
    serr.close()
    serr.open()
    print serr.readline()
    raw_input()	    
    serr.write('S3 0\n')
    serr.write('S7 0\n')
    print "coucou"
    while True:
        mae.verbose = True
        mae.run()
        if serr.inWaiting() > 0:
            #print "read line"
            transition = serr.readline()
            print "done ", transition
            if transition in ["# AFINI\r\n"]:  # , "#BLOC\r\n"]: 
                mae.trigger("AFINI")

    
if __name__ == "__main__":
    main();
