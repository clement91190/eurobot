import termios, fcntl, sys, os
import serial 


def main(): 
    fd = sys.stdin.fileno()
    serr = serial.Serial('/dev/ttyUSB0', 9600)
    serr.close()
    serr.open()
    print serr.readline()
    raw_input()

    oldterm = termios.tcgetattr(fd)
    newattr = termios.tcgetattr(fd)
    newattr[3] = newattr[3] & ~termios.ICANON & ~termios.ECHO
    termios.tcsetattr(fd, termios.TCSANOW, newattr)

    oldflags = fcntl.fcntl(fd, fcntl.F_GETFL)
    fcntl.fcntl(fd, fcntl.F_SETFL, oldflags | os.O_NONBLOCK)
    cap = 0

    try:
        while 1:
            try:
                c = sys.stdin.read(1)
                if c == "A":
                    print "av"
                    serr.write('S4 50\n')
                if c == "B":
                    print"rec"
                    serr.write('S4 -50\n')
                if c == "D":
                    print "gauche"
                    cap += 5
                    serr.write('S3 {}\n'.format(cap))
                if c == "C":
                    print "droite"
                    cap -= 5
                    serr.write('S3 {}\n'.format(cap))
            except IOError: pass
    finally:
        termios.tcsetattr(fd, termios.TCSAFLUSH, oldterm)
        fcntl.fcntl(fd, fcntl.F_SETFL, oldflags)

if __name__ == "__main__":
    main()
