import serial 


def main():
    #time.sleep(1)

    serr = serial.Serial('/dev/ttyUSB0', 9600)
    serr.close()
    serr.open()
    print serr.readline()
    raw_input()
    serr.write('S4 -250\n')
    print "coucou"
    while True:
        if serr.inWaiting() > 0:
            print "read line"
            transition = serr.readline()
            print "done ", transition, "red"

    
if __name__ == "__main__":
    main()
