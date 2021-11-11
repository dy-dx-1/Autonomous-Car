import serial 
from re import compile 
from time import time
import parsing_data
key = compile(r"b'-{0,1}(\d+)\\r\\n'") # to isolate the numbers from raw arduino output 
serial_port = serial.Serial('com4', 9600)  # connecting to serial port
time_list = list()   
start = time() #time at start of loop
while True: 
    if serial_port.inWaiting() > 0:  # if it's not waiting 
        data = serial_port.readline() 
        data = key.search(str(data))  # str() to because original output is in bytes 
        if data is not None: # if we found something | not sure if I can remove this since before I had errors -- to check
            if int(data.group(1)) > 0:    # if it's a positive value (valid reading)
                time_list.append(int(data.group(1)))
        if int(time() - start) > 2:   # if 2 seconds have passed
            break 
# On a une liste avec toutes les valeurs de temps, on va prendre la plus commune 

distance_cm = parsing_data.find_distance(time_list) 
print(distance_cm)
 