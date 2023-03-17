import read_sensor
import time
import sys


if __name__ == '__main__':
    with open("test.csv", "w") as sys.stdout:
        read_sensor.setup()
        data = ""
        start = time.time()
        while (time.time()-start<60):
            read_sensor.read()
            print(",")
        
    
