
import time
import board
import busio
import digitalio
from adafruit_mcp230xx.mcp23017 import MCP23017
from digitalio import Direction, Pull
import liblo as OSC
import multiprocessing

debounce_time = 0.2
last_button_press_time = 0
try:
    target = OSC.Address(1234)
except OSC.AddressError as err:
    print(err)
    sys.exit()
try:
    server = OSC.Server(4321)
except OSC.ServerError as err:
        print(err)


i2c = busio.I2C(board.SCL, board.SDA)

mcp = MCP23017(i2c, address=0x21)
mcp2 = MCP23017(i2c, address=0x20)

drum1Arr = [False,False,False,False,False,False,False,False]
drum2Arr = [False,False,False,False,False,False,False,False]
drum3Arr = [False,False,False,False,False,False,False,False]
drum4Arr = [False,False,False,False,False,False,False,False]
drum5Arr = [False,False,False,False,False,False,False,False]
drum6Arr = [False,False,False,False,False,False,False,False]
drum7Arr = [False,False,False,False,False,False,False,False]

bpm = 120
drumButtonMode = 0
sequenceIndex = 0
doubleTime = False
# Digital Controls pin definitions
ctl_pin0 = mcp2.get_pin(0)
ctl_pin1 = mcp2.get_pin(1)
ctl_pin2 = mcp2.get_pin(2)
ctl_pin3 = mcp2.get_pin(3)
ctl_pin4 = mcp2.get_pin(4)
ctl_pin5 = mcp2.get_pin(5)
ctl_pin6 = mcp2.get_pin(6)
ctl_pin7 = mcp2.get_pin(7)
ctl_pin8 = mcp2.get_pin(8)
ctl_pin9 = mcp2.get_pin(9)
ctl_pin10 = mcp2.get_pin(10)
ctl_pin11 = mcp2.get_pin(11)
ctl_pin12 = mcp2.get_pin(12)
ctl_pin13 = mcp2.get_pin(13)
ctl_pin14 = mcp2.get_pin(14)
ctl_pin15 = mcp2.get_pin(15)
ctl_pins = [ctl_pin0, ctl_pin1, ctl_pin2, ctl_pin3, ctl_pin4, ctl_pin5, >
            ctl_pin9, ctl_pin10, ctl_pin11, ctl_pin12, ctl_pin13, ctl_pi>
# led pin definitions
led_pin0 = mcp.get_pin(0)
led_pin1 = mcp.get_pin(1)
led_pin2 = mcp.get_pin(2)
led_pin3 = mcp.get_pin(3)
led_pin4 = mcp.get_pin(4)
led_pin5 = mcp.get_pin(5)
led_pin6 = mcp.get_pin(6)
led_pin7 = mcp.get_pin(7)
led_pin8 = mcp.get_pin(8)
led_pin9 = mcp.get_pin(9)
led_pin10 = mcp.get_pin(10)
led_pin11 = mcp.get_pin(11)
led_pin12 = mcp.get_pin(12)
led_pin13 = mcp.get_pin(13)
led_pin14 = mcp.get_pin(14)
led_pin15 = mcp.get_pin(15)
led_pins = [led_pin0,led_pin1,led_pin2,led_pin3,led_pin4,led_pin5,led_pi>
         led_pin10, led_pin11, led_pin12, led_pin13, led_pin14, led_pin1>
# initialize led pins
for pin in led_pins:
    pin.switch_to_output(value=True)
    pin.value = False
for pin in ctl_pins:
    pin.direction = Direction.INPUT
    pin.pull = Pull.UP

def debounce(button):
    global last_button_press_time

    current_time = time.time()

    if not button.value:
        if current_time - last_button_press_time > debounce_time:
            last_button_press_time = current_time
            return True

    return False


def drumLogic():
    global drumButtonMode
    if  debounce(ctl_pins[8]):
        drumButtonMode += 1
        drumButtonMode = drumButtonMode % 7
        print(drumButtonMode)
    OSC.send(target,"/rnbo/inst/0/params/drumMode",int(drumButtonMode))
    if drumButtonMode == 0:
        controlsToArrays(drum1Arr)
    elif drumButtonMode == 1:
        controlsToArrays(drum2Arr)
    elif drumButtonMode == 2:
        controlsToArrays(drum3Arr)
    elif drumButtonMode == 3:
        controlsToArrays(drum4Arr)
    elif drumButtonMode == 4:
        controlsToArrays(drum5Arr)
    elif drumButtonMode == 5:
        controlsToArrays(drum6Arr)
    elif drumButtonMode == 6:
        controlsToArrays(drum7Arr)
    else:
        drumButtonMode = 0

def controlsToArrays(drumArr):
    startText = "/rnbo/inst/0/params/toggleParams/toggle"
    for num, button in enumerate(ctl_pins[:8]):
        if  debounce(button):
            drumArr[num] = not drumArr[num]
            print("Button# ", num, "pressed!")
        #print(num)
        otherButtons()
        text = startText + str(num)
        OSC.send(target,text,int(drumArr[num]))
        led_pins[num+8].value = drumArr[num]

def otherButtons():
    global doubleTime 
    dynamicBPM = bpm
    if debounce(ctl_pins[9]):
        OSC.send(target,"/rnbo/inst/0/params/tempoBang",1)
        doubleTime = not doubleTime
    else:
        OSC.send(target,"/rnbo/inst/0/params/tempoBang",0)
    if doubleTime:
        dynamicBPM = bpm*2
    else:
        dynamicBPM = bpm

    if  ctl_pins[10].value:
        OSC.send(target,"/rnbo/inst/0/params/PausePlay",1)
    else:
        OSC.send(target,"/rnbo/inst/0/params/PausePlay",0)
    if debounce(ctl_pins[11]):
        print("looper enabled")
def ledSequence(index):
    led_pins[index].value = True
    for i in range(0,8):
        if i != index:
            led_pins[i].value = False

def handle_index(path,args):
    minus2 = args[0]-2
    if minus2 < 0:
        minus2 = 7
    print("boobs", minus2)
    ledSequence(minus2)

server.add_method("/rnbo/inst/0/messages/out/step","i",handle_index)
OSC.send(target, "/rnbo/listeners/add", f"127.0.0.1:4321")

def server_stuff():
    try:
        while True:
            server.recv(1)
    except KeyboardInterrupt:
        print("exiting cleanly...")

def button_stuff():
    try:
        while True:
            drumLogic()
    except KeyboardInterrupt:
        print("exiting cleanly...")

if __name__ == "__main__":
    buttons_process = multiprocessing.Process(target=button_stuff)
    server_process = multiprocessing.Process(target=server_stuff)
    buttons_process.start()
    server_process.start()
'''
try:
    while True:
        server.recv(1)
        #metronome()
       # drumLogic()
except KeyboardInterrupt:
    print("exiting cleanly...")
'''

