import sys, struct, serial


def wait_for_ack():
   ddata = ""
   ack = struct.pack('B', 0xff)
   while ddata != ack:
      global ser
      ddata = ser.read(1)
      #print ("0x%02x") % ddata[0]

   return

def printest():
    print("HEllo")
    return;

def setup():
    global ser
    ser = serial.Serial(port="COM6", baudrate=115200)
    ser.flushInput()
    #print ("port opening, done.")

    # send the set sensors command
    ser.write(struct.pack('BBBB', 0x08 , 0x04, 0x01, 0x00))  #GSR and PPG
    wait_for_ack()   
    #print ("sensor setting, done.")

    # Enable the internal expansion board power
    ser.write(struct.pack('BB', 0x5E, 0x01))
    wait_for_ack()
    #print ("enable internal expansion board power, done.")

    # send the set sampling rate command

    ser.write(struct.pack('BBB', 0x05, 0x00, 0x19)) #5.12Hz (6400 (0x1900)). Has to be done like this for alignment reasons
    wait_for_ack()
    #print ("sampling rate setting, done.")
    # send start streaming command
    ser.write(struct.pack('B', 0x07))
    wait_for_ack()
    #print ("start command sending, done.")

    
    #print ("Packet Type\tTimestamp\tGSR\tPPG")
def read():
   # read incoming data
    ddata = "".encode()
    numbytes = 0
    framesize = 8 # 1byte packet type + 3byte timestamp + 2 byte GSR + 2 byte PPG(Int A13)

    #print("Beginning read")
    try:
        #while True:
         while numbytes < framesize:
             ddata += ser.read(framesize)
             numbytes = len(ddata)

         data = ddata[0:framesize]
         ddata = ddata[framesize:]
         numbytes = len(ddata)

         # read basic packet information
         (packettype) = struct.unpack('B', data[0:1])
         (timestamp0, timestamp1, timestamp2) = struct.unpack('BBB', data[1:4])

         # read packet payload
         (PPG_raw, GSR_raw) = struct.unpack('HH', data[4:framesize])

         # get current GSR range resistor value
         Range = ((GSR_raw >> 14) & 0xff)  # upper two bits
         if(Range == 0):
             Rf = 40.2   # kohm
         elif(Range == 1):
             Rf = 287.0  # kohm
         elif(Range == 2):
             Rf = 1000.0 # kohm
         elif(Range == 3):
             Rf = 3300.0 # kohm

         # convert GSR to kohm value
         gsr_to_volts = (GSR_raw & 0x3fff) * (3.0/4095.0)
         GSR_ohm = Rf/( (gsr_to_volts /0.5) - 1.0)
         conductance = ((1/(GSR_ohm*1000))*1000000);
         # convert PPG to milliVolt value
         PPG_mv = PPG_raw * (3000.0/4095.0)


         timestamp = timestamp0 + timestamp1*256 + timestamp2*65536

         #print ("0x%02x\t\t%5d,\t%4d,\t%4d" % (packettype[0], timestamp, GSR_ohm, PPG_mv))
         sys.stdout.write(str(conductance))
         return conductance
         

    except KeyboardInterrupt:
    #send stop streaming command
        ser.write(struct.pack('B', 0x20))
        print
        print ("stop command sent, waiting for ACK_COMMAND")
        wait_for_ack()
        print ("ACK_COMMAND received.")
    #close serial port
        ser.close()
        print ("All done")
