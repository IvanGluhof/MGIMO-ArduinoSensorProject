import socket
import easygui
import time
import sys
import glob
import serial
import serial.tools.list_ports   # import serial module
from bluetooth import *

BAUDRATE = 9600   # Connection baud rate
COM_PORT = 'COM4' # Port with incoming data
TIMEOUT  = 2      # Timeout between read (2 sec to fill buffer)
MODE = -1         # Operation mode
DEVICE_DIRECT = 0 # Direct = by COM port
DEVICE_SEARCH = 1 # Serch for devices


message = "Select connection mode"
title   = "Connection mode"
choices = ["Direct COM port", "Search for a device"]
select  = easygui.choicebox(message, title, choices, 0)

# User selection
MODE = DEVICE_DIRECT if( "Direct COM port" in select ) else \
       DEVICE_SEARCH

# Mode execution
if( MODE == DEVICE_DIRECT ):
    port = easygui.enterbox("Enter COM port", "COM port selection", "default")
    if( not "default" in port ): COM_PORT = port
    # Bluetooth port
    serial_bt = serial.Serial(port     = COM_PORT,
                              baudrate = BAUDRATE)
    serial_bt.timeout = TIMEOUT  # Read timeout

    # Check if port is opened
    print(serial_bt.is_open)  # True for opened

    # Init data read
    if( serial_bt.is_open ):
        while True:
            size = serial_bt.inWaiting()
            if( size ):
                data = serial_bt.read(size)
                # This trims input data by separate values
                list = data.replace(b'C', b'').replace(b'K', b'').replace(b'F', b'').replace(b'H', b'').replace(b'\n', b'').replace(b'\r', b'').replace(b';', b'').replace(b'[^A-Za-z.]', b'').split(b':');
                print(list)
                # Warn user
                if float(list[0]) > 28.6 : easygui.msgbox("Warning! " + "High temperature " + str((list[0])))
            else:
                print('No data to read')
            time.sleep(1)
    else:
        print('Port: ', COM_PORT, ' is not open')
else:
    # Search for devies
    # NOTE: Not working on Win 10 (probably pybluez error??)
    nearby_devices = discover_devices(lookup_names = True)

    print("found %d devices" % len(nearby_devices))

    for BT_MAC, BT_NAME in nearby_devices:
         print(" %s - %s" % (BT_MAC, BT_NAME))

    HOST = BT_MAC # Symbolic name
    PORT = 3      # Non-privileged port

    # Create the client socket
    sock=BluetoothSocket( RFCOMM )
    sock.connect((HOST, PORT))

    # L2CAP
    # RFCOMM
    s=BluetoothSocket( RFCOMM )
    s.bind((HOST, PORT))
    s.listen(1)

    conn, addr = s.accept()

    print('Connected by', addr)

    # Read data
    while True:
        data = conn.recv(1024)
        if not data: break
    conn.close()



