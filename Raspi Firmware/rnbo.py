import os
# Set the DISPLAY variable for pygame to detect it
os.environ["DISPLAY"] = ":0"
import pygame
import sys
import board
import busio
import digitalio
from adafruit_mcp230xx.mcp23017 import MCP23017
from adafruit_mcp3xxx.mcp3008 import MCP3008
from adafruit_mcp3xxx.analog_in import AnalogIn
from pythonosc.udp_client import SimpleUDPClient
from pythonosc.osc_server import BlockingOSCUDPServer
from pythonosc.dispatcher import Dispatcher
import multiprocessing
from pygame.locals import *
from extra_rnbo import mySwitch, myLed, myMap, scale, getRandomPattern
from rnbo_display import Display
from gpiozero import RotaryEncoder
from signal import pause 
import subprocess
display = Display(0)

isLooping = False
multifunction = False
reverseLoop = False
fillMode = False
djmMode = False
halfTime = False
doubleTime = False
rdmVelos = [False, False, False,False, False, False,False,False]
rdmAllVelos = False
rdmPans = [False, False, False,False, False, False,False,False]
rdmAllPans = False

ip = "127.0.0.1"
port = 1234
try:
    OSC = SimpleUDPClient(ip, port) # Create client
except Exception:
    print(Exception)
try:
    dispatcher = Dispatcher()
except OSC.ServerError as err:
        print(err)

# create pins for SPI and i2c
spi1 = busio.SPI(board.SCK_1,board.MOSI_1,board.MISO_1)
i2c = busio.I2C(board.SCL, board.SDA)

# create the cs (chip select) pins
cs1 = digitalio.DigitalInOut(board.D1)
cs2 = digitalio.DigitalInOut(board.D0)

mcp = MCP23017(i2c, address=0x21)
mcp2 = MCP23017(i2c, address=0x20)
mcp3 = MCP23017(i2c, address=0x22)
adc = MCP3008(spi1,cs1)
adc2 = MCP3008(spi1,cs2)

# initialize the switches
switches = []
pat_swit = []
leds = [] #pattern leds
leds2 = [] #sequence and fx leds
adcValues = []
adcValues2 = []
encButtons = []
seq_leds = []
for i in range(0,16):
    if i < 8:
        adcValues.append(AnalogIn(adc,i))
        adcValues2.append(AnalogIn(adc2,i))
        leds.append(myLed(mcp,i+8))
        encButtons.append(mySwitch(mcp,i))
        pat_swit.append(mySwitch(mcp2,i))
        seq_leds.append(myLed(mcp3,i))
    switches.append(mySwitch(mcp2,i))
    leds2.append(myLed(mcp3,i))
pat_swit.reverse()
seq_leds.reverse()
switches[0:8] = pat_swit
leds2[0:8] = seq_leds
OSC.send_message("/rnbo/inst/0/params/toggleParams/resetFX",1)
def restart_script():
    python_script = sys.argv[0]
    subprocess.Popen(['python3', python_script])
    sys.exit()

def reset_leds():
    for led in leds:
        led.setValue(False)
    for led in leds2:
        led.setValue(False)
def reset_drums(flat_drums):
    startText = "/rnbo/inst/0/params/toggleParams/toggle"
    for row in range(7):
        OSC.send_message("/rnbo/inst/0/params/drumMode",row)
        for j in range(8):
            flat_drums[row*8+j] = 0
            text = startText + str(j)
            OSC.send_message(text,0)
    OSC.send_message("/rnbo/inst/0/params/looper/clearRecording",1)
def drumLogic(drumButtonMode,screen_mode,fx_selection,looper_modes,flat_drums,terminator):
    global multifunction
    if switches[8].Value():
        multifunction = True
        OSC.send_message("/rnbo/inst/0/params/multifunction",1)
    else:
        multifunction = False
        OSC.send_message("/rnbo/inst/0/params/multifunction",0)
    OSC.send_message("/rnbo/inst/0/params/drumMode",int(drumButtonMode.value))
    if drumButtonMode.value < 7:
        controlsToArrays(drumButtonMode,screen_mode,fx_selection,looper_modes,flat_drums)
    elif drumButtonMode.value == 7:
        loopMode(drumButtonMode,screen_mode,fx_selection,looper_modes,terminator)
    else:
        drumButtonMode.value = 0
def loopMode(drumButtonMode,screen_mode,fx_selection,looper_modes,terminator):
    global multifunction
    if multifunction:
        leds[drumButtonMode.value].setValue(True)
        for num, button in enumerate(switches[:8]):
            if not num == drumButtonMode.value:
                leds[num].setValue(False)
            if button.risingEdge():
                drumButtonMode.value = num
            secondaryControls(drumButtonMode.value,screen_mode,fx_selection,looper_modes,flat_drums)
    else:
        primaryControls(drumButtonMode.value,screen_mode,fx_selection,looper_modes,flat_drums)
        if switches[0].Value() and switches[1].Value():
            terminator.value = 2
        if switches[6].Value() and switches[7].Value():
            terminator.value = 1
def controlsToArrays(drumButtonMode,screen_mode,fx_selection,looper_modes,flat_drums):
    global multifunction
    if not multifunction:
        startText = "/rnbo/inst/0/params/toggleParams/toggle"
        for num, button in enumerate(switches[:8]):
            if  button.risingEdge():
                screen_mode.value = 0 # THIS CHANGES THE SCREEN MODE
                flat_drums[drumButtonMode.value*8 + num] = not flat_drums[drumButtonMode.value*8 + num]
                #print("Button# ", num, "pressed!")
            text = startText + str(num)
            OSC.send_message(text,int(flat_drums[drumButtonMode.value*8 + num]))
            leds[num].setValue(flat_drums[drumButtonMode.value*8 + num])
            primaryControls(drumButtonMode.value,screen_mode,fx_selection,looper_modes,flat_drums)
    else:
        leds[drumButtonMode.value].setValue(True)
        for num, button in enumerate(switches[:8]):
            if not num == drumButtonMode:
                leds[num].setValue(False)
            if button.risingEdge():
                drumButtonMode.value = num
                if num == 7:
                    screen_mode.value = 1
            secondaryControls(drumButtonMode.value,screen_mode,fx_selection,looper_modes,flat_drums)

def primaryControls(drumButtonMode,screen_mode,fx_selection,looper_modes,flat_drums):
    global fillMode, swellActive, rdmSwellMode, halfTime, doubleTime, rdmVelos, rdmPan, rdmAllVelos, rdmAllPans
    if encButtons[1].risingEdge():
        value = fx_selection.value + 1
        fx_selection.value = value % 5
        OSC.send_message("/rnbo/inst/0/params/fxMode",fx_selection.value)
        screen_mode.value = 2
    if  switches[15].Value():
        OSC.send_message("/rnbo/inst/0/params/PausePlay",1)
    else:
        OSC.send_message("/rnbo/inst/0/params/PausePlay",0)
    global isLooping
    if switches[9].risingEdge():
        isLooping = not isLooping
        looper_modes[1] = 1
    if switches[9].timeHeld() > 2:
        OSC.send_message("/rnbo/inst/0/params/looper/clearRecording",0)
        OSC.send_message("/rnbo/inst/0/params/looper/recordLoop",0)
        isLooping = False
        looper_modes[1] = 0
        looper_modes[0] = 0
        #OSC.send_message("/rnbo/inst/0/params/rdmSel",1)
    if switches[11].risingEdge():
        screen_mode.value = 0
        fillMode = not fillMode
    if switches[10].risingEdge():
        screen_mode.value = 0
        if drumButtonMode != 7:
            getRandomPattern(drumButtonMode, flat_drums)
    if switches[10].timeHeld() > 2:
        screen_mode.value = 0
        startText = "/rnbo/inst/0/params/toggleParams/toggle"
        for row in range(7):
            OSC.send_message("/rnbo/inst/0/params/drumMode",row)
            for j in range(8):
                flat_drums[row*8 +j] = 0
                text = startText + str(j)
                OSC.send_message(text,0)
    if encButtons[7].risingEdge():
        halfTime = not halfTime
    if encButtons[6].timeHeld() > 2:
        for i in range(len(rdmVelos)):
            rdmVelos[i] = False
        OSC.send_message("/rnbo/inst/0/params/resetVelos",1)
        rdmAllVelos = False
    if encButtons[6].risingEdge():
        rdmVelos[drumButtonMode] = not rdmVelos[drumButtonMode]
    if encButtons[0].timeHeld() > 2:
        OSC.send_message("/rnbo/inst/0/params/resetTempo",1)
    if encButtons[0].risingEdge():
        OSC.send_message("/rnbo/inst/0/params/warpSelected",1)
    if encButtons[5].timeHeld() > 2:
        OSC.send_message("/rnbo/inst/0/params/resetPans",1)
        for i in range(len(rdmPans)):
            rdmPans[i] = False
        rdmAllPans = False
    if encButtons[5].risingEdge():
        rdmPans[drumButtonMode] = not rdmPans[drumButtonMode]
    if encButtons[4].timeHeld() > 2:
        OSC.send_message("/rnbo/inst/0/params/resetDrums",1)
    if encButtons[4].risingEdge():
        OSC.send_message("/rnbo/inst/0/params/randDrumSelected",1)
    if encButtons[3].risingEdge():
        OSC.send_message("/rnbo/inst/0/params/randFxSelected",1)
    if encButtons[2].risingEdge():
        OSC.send_message("/rnbo/inst/0/params/randFxAll",1)

    if rdmPans[drumButtonMode]:
        OSC.send_message("/rnbo/inst/0/params/randPanSelected",1)
    else:
        OSC.send_message("/rnbo/inst/0/params/randPanSelected",0)
    if halfTime:
       OSC.send_message("/rnbo/inst/0/params/timeMult",0.5)
       doubleTime = False
    elif not doubleTime:
       OSC.send_message("/rnbo/inst/0/params/timeMult",1.0)
    if rdmVelos[drumButtonMode]:
       OSC.send_message("/rnbo/inst/0/params/rdmVeloSelected",1)
    else:
       OSC.send_message("/rnbo/inst/0/params/rdmVeloSelected",0)
    if isLooping:
        OSC.send_message("/rnbo/inst/0/params/looper/recordLoop",1)
        looper_modes[0] = 1
    else:
        OSC.send_message("/rnbo/inst/0/params/looper/recordLoop",0)
        looper_modes[0] = 0
    if fillMode:
        OSC.send_message("/rnbo/inst/0/params/fillMode",1)
    else:
        OSC.send_message("/rnbo/inst/0/params/fillMode",0)

def secondaryControls(drumButtonMode,screen_mode,fx_selection,looper_modes,flat_drums):
    global reverseAll, rdmSwellMode, reverseDrum, djmMode, reverseLoop, halfTime, doubleTime, rdmAllPans, rdmAllVelos

    if  switches[15].Value():
        OSC.send_message("/rnbo/inst/0/params/PausePlay",1)
    else:
        OSC.send_message("/rnbo/inst/0/params/PausePlay",0)
    if encButtons[1].risingEdge():
        value = fx_selection.value - 1
        if value < 0:
            fx_selection.value = 4
        else:
            fx_selection.value = value
        OSC.send_message("/rnbo/inst/0/params/fxMode",fx_selection.value)
        screen_mode.value = 2
    if switches[9].risingEdge():
        reverseLoop = not reverseLoop
    if switches[11].risingEdge():
        djmMode = not djmMode
        screen_mode.value = 0
    if switches[10].risingEdge():
        screen_mode.value = 0
        startText = "/rnbo/inst/0/params/toggleParams/toggle"
        for i in range(7):
            getRandomPattern(i,flat_drums)
            OSC.send_message("/rnbo/inst/0/params/drumMode",i)
            for num in range(8):
                text = startText + str(num)
                OSC.send_message(text,int(flat_drums[i*8+num]))
    if encButtons[7].risingEdge():
        doubleTime = not doubleTime
    if encButtons[6].risingEdge():
        rdmAllVelos = not rdmAllVelos
    if encButtons[0].risingEdge():
        OSC.send_message("/rnbo/inst/0/params/setWarp",1)
    if encButtons[5].risingEdge():
        rdmAllPans = not rdmAllPans
    if encButtons[4].risingEdge():
        OSC.send_message("/rnbo/inst/0/params/randSounds",1)
    if encButtons[3].risingEdge():
        OSC.send_message("/rnbo/inst/0/params/resetSelectedFx",1)
    if encButtons[2].risingEdge():
        OSC.send_message("/rnbo/inst/0/params/resetFX",1)

    if rdmAllVelos:
        OSC.send_message("/rnbo/inst/0/params/randVelosAll",1)
    else:
        OSC.send_message("/rnbo/inst/0/params/randVelosAll",0)
    if rdmAllPans:
        OSC.send_message("/rnbo/inst/0/params/randPanAll",1)
    else:
        OSC.send_message("/rnbo/inst/0/params/randPanAll",0)
    if doubleTime:
       OSC.send_message("/rnbo/inst/0/params/timeMult",2.0)
       halfTime = False
    elif not doubleTime:
       OSC.send_message("/rnbo/inst/0/params/timeMult",1.0)
    if reverseLoop:
        OSC.send_message("/rnbo/inst/0/params/looper/reverseLoop",1)
    else:
        OSC.send_message("/rnbo/inst/0/params/looper/reverseLoop",0)

    if djmMode:
        OSC.send_message("/rnbo/inst/0/params/djmMode",1)
    else:
        OSC.send_message("/rnbo/inst/0/params/djmMode",0)


def analog_controls():
    maxVal = 62000
    minVal = 1000
    OSC.send_message("/rnbo/inst/0/params/drum1Vol",myMap(adcValues2[0].value,minVal,maxVal))
    OSC.send_message("/rnbo/inst/0/params/drum2Vol",myMap(adcValues2[1].value,minVal,maxVal))
    OSC.send_message("/rnbo/inst/0/params/drum3Vol",myMap(adcValues2[2].value,minVal,maxVal))
    OSC.send_message("/rnbo/inst/0/params/drum4Vol",myMap(adcValues2[3].value,minVal,maxVal))
    OSC.send_message("/rnbo/inst/0/params/drum5Vol",myMap(adcValues2[4].value,minVal,maxVal))
    OSC.send_message("/rnbo/inst/0/params/drum6Vol",myMap(adcValues2[5].value,minVal,maxVal))
    OSC.send_message("/rnbo/inst/0/params/drum7Vol",myMap(adcValues2[6].value,minVal,maxVal))
    OSC.send_message("/rnbo/inst/0/params/loopVol",myMap(adcValues2[7].value,minVal,maxVal))
    OSC.send_message("/rnbo/inst/0/params/inVol",myMap(adcValues[0].value,minVal,maxVal))
    OSC.send_message("/rnbo/inst/0/params/EQ_Low",myMap(adcValues[1].value,minVal,maxVal))
    OSC.send_message("/rnbo/inst/0/params/EQ_Mid",myMap(adcValues[2].value,minVal,maxVal))
    OSC.send_message("/rnbo/inst/0/params/EQ_High",myMap(adcValues[3].value,minVal,maxVal))
    OSC.send_message("/rnbo/inst/0/params/gain",myMap(adcValues[4].value,100,maxVal))

def encCallback(screen_mode,enc_index):
    prev_value = None
    def callback(enc):
        global multifunction
        nonlocal prev_value
        current_value = enc.value
        if prev_value is not None:
            if current_value > prev_value:
                OSC.send_message("/rnbo/inst/0/params/encoder_"+enc_index,1)
            elif current_value < prev_value:
                OSC.send_message("/rnbo/inst/0/params/encoder_"+enc_index,-1)
            else:
                OSC.send_message("/rnbo/inst/0/params/encoder_"+enc_index,0)
        if enc_index == "1" or enc_index == "2" or (multifunction and enc_index == "0"):
            screen_mode.value = 1
        elif enc_index == "3" or enc_index == "4":
            screen_mode.value = 2
        prev_value = current_value
    return callback

def handle_index(path,args,flat_drums):
    minus2 = args-1
    if minus2 < 0:
        minus2 = 7
    index = int(minus2)
    leds2[index].setValue(True)
    for i in range(8):
        if not i == index:
            leds2[i].setValue(False)
    drum_leds(flat_drums,index)
def drum_leds(flat_drums,step):
    for i in range(7):
        leds2[i+8].setValue(flat_drums[i*8+step])
        leds2[i+8].setValue(False)
def loop_led(looper_modes):
    if looper_modes[0]:
        leds2[15].blink(0.5)
    else:
        if looper_modes[1]:
            leds2[15].setValue(True)
        else:
            leds2[15].setValue(False)
def assign_array(path,*args,value_array,looper_modes,terminator):
    for i in range(len(args)):
        value_array[i] = args[i]
    loop_led(looper_modes)
    if terminator.value != 0:
        raise KeyboardInterrupt

dispatcher.map("/rnbo/inst/0/messages/out/step",lambda path, args: handle_index(path, args,flat_drums))
dispatcher.map("/rnbo/inst/0/messages/out/valueArray",lambda path, *args: assign_array(path, *args, value_array=value_array,looper_modes=looper_modes,terminator=terminator))
OSC.send_message( "/rnbo/listeners/add", "127.0.0.1:4321")

def server_stuff(flat_drums,fx_selection,value_array,looper_modes,terminator):
    try:
        server = BlockingOSCUDPServer(("127.0.0.1", 4321), dispatcher)
        server.serve_forever()
    except KeyboardInterrupt:
        reset_leds()
        print("exiting cleanly...server")
        sys.exit()
    except Exception as e:
        reset_drums(flat_drums)
def button_stuff(drum_button_mode,screen_mode,fx_selection,looper_modes,flat_drums,terminator):
    try:
        maxsteps = 100
        enc0 = RotaryEncoder(4,14,max_steps=maxsteps,wrap=True)
        enc1 = RotaryEncoder(6,12,max_steps=maxsteps,wrap=True)
        enc2 = RotaryEncoder(16,26,max_steps=maxsteps,wrap=True)
        enc3 = RotaryEncoder(22,27,max_steps=maxsteps,wrap=True)
        enc4 = RotaryEncoder(13,5,max_steps=maxsteps,wrap=True)
        enc0.when_rotated = encCallback(screen_mode,"0")
        enc1.when_rotated = encCallback(screen_mode,"1") 
        enc2.when_rotated = encCallback(screen_mode,"2") 
        enc3.when_rotated = encCallback(screen_mode,"3") 
        enc4.when_rotated = encCallback(screen_mode,"4")
        while True:
            drumLogic(drum_button_mode,screen_mode,fx_selection,looper_modes,flat_drums,terminator)
            if terminator.value != 0:
                raise KeyboardInterrupt
    except KeyboardInterrupt:
        reset_leds()
        reset_drums(flat_drums)
        print("exiting cleanly...buttons")
        sys.exit()
    except Exception as e:
        reset_drums(flat_drums)
def screen_stuff(fx_selection,flat_drums,drum_button_mode,value_array,screen_mode,terminator):
    try:
        display = Display(12)
        while True:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    return
            analog_controls()
            display.draw(flat_drums,drum_button_mode,value_array,screen_mode)
            if terminator.value != 0:
                raise KeyboardInterrupt
    except KeyboardInterrupt:
        #reset_leds()
        #reset_drums(flat_drums)
        print("exiting cleanly...screen")
        pygame.quit()
        sys.exit()
    except Exception as e:
        reset_drums(flat_drums)
if __name__ == "__main__":
    value_array = multiprocessing.Array('f',70)
    fx_selection = multiprocessing.Value('i',0)
    screen_mode = multiprocessing.Value('i',0)
    drum_button_mode = multiprocessing.Value('i',0)
    looper_modes = multiprocessing.Array('i',2) # (is_looping, looper_full)
    flat_drums = multiprocessing.Array('i',56)
    terminator = multiprocessing.Value('i',0) # 0: nothing, 1: quit script, 2: restart script
    buttons_process = multiprocessing.Process(target=button_stuff,args=(drum_button_mode,screen_mode,fx_selection,looper_modes,flat_drums,terminator))
    server_process = multiprocessing.Process(target=server_stuff,args=(flat_drums,fx_selection,value_array,looper_modes,terminator))
    screen_process = multiprocessing.Process(target=screen_stuff,args=(fx_selection,flat_drums,drum_button_mode,value_array,screen_mode,terminator))
    server_process.start()
    screen_process.start()
    buttons_process.start()
    #pause()
    server_process.join()
    screen_process.join()
    buttons_process.join()
    if screen_process.is_alive():
        screen_stuff.terminate()
        print('lol')
    if terminator.value == 2:
        restart_script()
#End Loop
