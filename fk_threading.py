from threading import Thread
import serial, signal, sys, time

replay = False
lastData = 0
actionList = []

def writeChar(char, pos):
  remote.write(bytes([ord(char), pos, 0x00, 0x00, 0x00, 0x01]))
def writeString(text,offset=0): 
  pos = offset
  for c in text:
    writeChar(c, pos)
    pos += 1
  
def clearDisplay():
  writeString("                                ")  

remote = serial.Serial("COM16", timeout=300)
print("remote connected")
writeString(" Remote ")
vehiclef = serial.Serial("COM11",timeout=300)
writeString("Vehicle1 ",16)
print("vehicle front connected")
vehicler = serial.Serial("COM7",timeout=300)
print("vehicle rear connected")
writeString("Vehicle2 ",16)




  
clearDisplay()  




def signal_handler(sig, frame):
        print('You pressed Ctrl+C!')
        remote.close()
        vehiclef.close()
        vehicler.close()
        sys.exit(0)
signal.signal(signal.SIGINT, signal_handler)


def setReverse(reverse):
    vehiclef.write(bytes([reverse, 0x00, 0x00, 0x00, 0x00, 0x02]))
    vehicler.write(bytes([reverse, 0x00, 0x00, 0x00, 0x00, 0x02]))

def do180():
    vehiclef.write(bytes([0x0DD,0xDD,0xDD,0xDD,0x00,0x04]))
    vehicler.write(bytes([0x0DD,0xDD,0xDD,0xDD,0x00,0x04]))
    time.sleep(2)

def readRemoteWriteVehicle():
    global lastData
    global actionList
    global replay
    while True:
        data = remote.read(6)
        if data:
            if data[5] == 0:
                firstByte = data[0]
                buttonOne = firstByte & 0b100
                buttonTwo = firstByte & 0b010
                buttonThree = firstByte & 0b001
                if buttonOne:
                    clearDisplay()
                    writeString("List deleted")
                    actionList.clear()
                    time.sleep(1)
                    clearDisplay()
                if buttonTwo:
                    clearDisplay()
                    writeString("    Running Operations again")
                    # replay last actions
                    #do180()
                    setReverse(1)
                    replay = True
                    i = len(actionList)
                    for val in actionList[::-1]:
                        vehiclef.write(val[0])
                        vehiclef.write(val[1])
                        vehicler.write(val[0])
                        vehicler.write(val[1])
                        print(str(val))
                        writeString(str(i))
                        i -= 1
                       # time.sleep(100/1000000.0)
                        time.sleep(0.08)
                    replay = False
                    setReverse(0)
                    #actionList.clear()
                    clearDisplay()
                    actionList.clear()
                if buttonThree:
                    clearDisplay()
                    writeString("Doing a 180")
                    do180()
            else:
                engineR = data[0]<<8 | data[1]
                engineL = data[2]<<8 | data[3]
                lastData = data
                #actionList.append((data))
                print("Remote data: "+str(data))
                writeString(str(len(actionList)),28)
                vehiclef.write(data)
                vehicler.write(data)

def ueb_02_fahrzeug_daten_gib(data):
        return (data[0] << 8 | data[1], data[2] << 8 | data[3])     
def readVehicleWriteRemote():
    global lastData
    global actionList
    global replay
    while True:
        returnData = vehiclef.read(6)
        if returnData:
            #print("vehicle: " + str(returnData))
            if not replay:
                actionList.append((lastData, returnData))
            remote.write(returnData)


vehicleThread = Thread(target=readRemoteWriteVehicle)
remoteThread = Thread(target=readVehicleWriteRemote)

vehicleThread.start()
remoteThread.start()

