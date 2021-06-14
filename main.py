import serial, sys, time, requests
port = "COM3"
baudrate = 9600
ser = serial.Serial(port, baudrate, timeout=0.001)
URL = "http://megz.125mb.com/commands.php"
sentData = {'command': 'T'}
while True:
    try:
        data = ser.read(1)
        data += ser.read(ser.inWaiting())
        sys.stdout.write(data.decode("utf-8"))
        sys.stdout.flush()
        time.sleep(1)

        if len(data) > 0:
            data = data.decode()
            if data[0] == "@":
                #Switch case
                if data[1] == 'I':
                    sentData['command'] = data
                    res = requests.post(URL, data=sentData)
                    if res.text != "S":
                        print("@F,")
                    sentData['command'] = "@G"
                    res = requests.post(URL, data=sentData)
                    while res.text == "F":
                        time.sleep(2)
                        res = requests.post(URL, data=sentData)
                    ser.write(str.encode(res.text))
                    continue
                elif data[1] == 'S' or data[1] == 'C' or data[1] == 'E':
                    sentData['command'] = data
                    res = requests.post(URL, data=sentData)
                    continue
    except:
        continue

