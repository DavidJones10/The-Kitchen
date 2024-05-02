from extra_rnbo import mySwitch, myLed
import board
import busio
import digitalio
from adafruit_mcp230xx.mcp23017 import MCP23017
from adafruit_mcp3xxx.mcp3008 import MCP3008
from adafruit_mcp3xxx.analog_in import AnalogIn
import random
from gpiozero import RotaryEncoder
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
ledValues = [False,False,False,False,False,False,False,False,
             False,False,False,False,False,False,False,False]
patLeds = [False,False,False,False,False,False,False,False,False]
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

def encCallback(enc_index):
    prev_value = None
    def callback(enc):
        global multifunction
        nonlocal prev_value
        current_value = enc.value
        if prev_value is not None:
            if current_value > prev_value:
                print("encoder: "+enc_index+" increased")
            elif current_value < prev_value:
                print("encoder: "+enc_index+" decreased")
        prev_value = current_value
    return callback

if __name__ == "__main__":
    maxsteps = 100
    enc0 = RotaryEncoder(4,14,max_steps=maxsteps,wrap=True)
    enc1 = RotaryEncoder(22,27,max_steps=maxsteps,wrap=True)
    enc2 = RotaryEncoder(13,5,max_steps=maxsteps,wrap=True)
    enc3 = RotaryEncoder(16,26,max_steps=maxsteps,wrap=True)
    enc4 = RotaryEncoder(12,6,max_steps=maxsteps,wrap=True)
    enc0.when_rotated = encCallback("0")
    enc1.when_rotated = encCallback("1") 
    enc2.when_rotated = encCallback("2") 
    enc3.when_rotated = encCallback("3") 
    enc4.when_rotated = encCallback("4")

    while True:
        for i in range(len(switches)):
            if switches[i].risingEdge():
                print("switch ",i," pressed")
                ledValues[i] = not ledValues[i]
            leds2[i].setValue(ledValues[i])
        
        for j in range(8):
            if encButtons[j].risingEdge():
                print("encoder ",j," pressed")
                patLeds[j] = not patLeds[j]
                leds[j].setValue(patLeds[j])
        leds[5].blink(0.5)
        leds[6].blink(1.0)
        leds[7].blink(2.0)
        leds2[12].blink(1.0)
        
        text = "adcValues:"
        for i in range(len(adcValues)):
            text += str(i) + ": " + str(int(adcValues[i].value))
        print(text)
        
        
        
        '''
        text = "adcValues2:"
        for i in range(len(adcValues2)):
            text += str(i) + ": " + str(int(adcValues2[i].value))
        print(text)
        '''
