import serial 

def main():

    serr = serial.Serial('/dev/ttyUSB0', 9600)
    serr.close()
    serr.open()
    while True:
        if serr.inWaiting() > 0:
            print serr.readline()

    
if __name__ == "__main__":
    main()
