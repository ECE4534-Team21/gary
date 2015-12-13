import serial
import threading
import time
__author__ = 'Brian Kaplan'

#ECE 4564 Embedded Systems Project
#This python script uses serial communication to send and recieve data to a pic32.
#The script should be run using python 2.7 interpreter

#Cinfiguration Settings
#Send Message ... For ...
#<15S> -> Start Game
#<15E> -> End Game
#<15G0> -> Set default game mode
#<15G1> -> Continuous play game mode
#<15C##> -> Set the score ceiling to ## ... single digits should be 00 01 02 etc.

# For mac use the below command to get the path of the wifly and replace 'COM5'
# ls /dev/tty.* for mac

ser = serial.Serial('COM5', 57600, timeout=2)
senderID = "1"
ackFlag = False
state = "INIT"
recMessage = ""
senMessage = ""
newMessage = False
score = 0


def main():

    print("Welcome to robocup")
    print("Please enter your configuration settings:")

    # Start the threads
    s = threading.Thread(name='sender', target=sender)
    r = threading.Thread(name='receiver', target=receiver)
    g = threading.Thread(name='game', target=game)
    s.start()
    r.start()
    g.start()

    # If the sending or receiving threads are dead restart them
    while True:
        if not s.isAlive():
            s = threading.Thread(name='sender', target=sender)
            s.start()
        if not r.isAlive():
            r = threading.Thread(name='receiver', target=receiver)
            r.start()
        if not g.isAlive():
            g = threading.Thread(name='game', target=game)
            g.start()


def setstate():
    # change the state based on the command
    global state
    global senMessage
    if ackFlag:
        if senMessage == "<15T>":
            state = "TEST"
        elif senMessage == "<15S>":
            state = "START"
        elif senMessage == "<15P>":
            state = "PAUSE"
        elif senMessage == "<15R>":
            state = "RESTART"
        else:
            pass
        print("in " + state)


def game():
    global state
    global newMessage
    global ackFlag
    global score

    # State machine
    if state == "INIT":
        state = "INIT"
        score = 0

    elif state == "TEST":
        state = "TEST"

    elif state == "START":
        state = "START"
        if newMessage:
            #print(message)
            newMessage = False
            if message == "<51score>":
                score += 1
                print("Score = " + str(score))
                if score == 7:
                    print("game over")
                    print("send start to play again")
                    state = "INIT"

    elif state == "RESTART":
        score = 0
        state = "START"


def sender():
    global ackFlag
    global senMessage
    ackFlag = False
    writeVal = raw_input()
    ser.write(writeVal.encode())
    senMessage = writeVal
    time.sleep(2)
    counter = 0
    while not ackFlag:
        if counter == 4:
            print ('connection error')
            break
        print('retry')
        ser.write(writeVal.encode())
        time.sleep(2)
        counter += 1


def receiver():
    global ackFlag
    global message
    global newMessage
    while True:
        value = ser.read(1).decode("utf-8")
        if len(value) == 0:
            break
        if value == '<':
            senderID = ser.read(1).decode("utf-8")
            if senderID == '0':
                    message = value + senderID + ser.read(1).decode("utf-8")
                    if message == '<0>':
                        print('ack')
                        ackFlag = True
                        setstate()

            else:
                targetID = ser.read(1).decode("utf-8")
                data = ""
                flag = True
                while (flag):
                    temp = ser.read(1).decode("utf-8")
                    if len(temp) == 0:
                        print('error')
                        break
                    if temp == ">":
                        flag = False
                    else:
                        data += temp

                if targetID == '1':
                    #print ('sent ack')
                    writeVal = "<0>"
                    ser.write(writeVal.encode())

                message = "<" + senderID + targetID + data + ">"
            newMessage = True

main()
