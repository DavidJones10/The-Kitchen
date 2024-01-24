import time

import board
import busio
import digitalio

#Import MCP23017:

from adafruit_mcp230xx.mcp23017 import MCP23017




import adafruit_mcp3xxx.mcp3008 as MCP
from adafruit_mcp3xxx.analog_in import AnalogIn
spi = busio.SPI(clock=board.SCK, MISO=board.MISO, MOSI=board.MOSI)
cs = digitalio.DigitalInOut(board.D5)
cs2 = digitalio.DigitalInOut(board.D6)
adc = MCP.MCP3008(spi, cs)
adc2 = MCP.MCP3008(spi,cs2)

#channel 1 is the rotary pot. channel 4 uses the second adc.
channel1 = AnalogIn(adc, MCP.P6)
channel2 = AnalogIn(adc, MCP.P1)
channel3 = AnalogIn(adc, MCP.P2)
channel4 = AnalogIn(adc2, MCP.P0)

channels = [channel1,channel2,channel3,channel4]




# Initialize the I2C bus:
i2c = busio.I2C(board.SCL, board.SDA)

mcp = MCP23017(i2c)

#Assign pins
pin8 = mcp.get_pin(8)
pin9 = mcp.get_pin(9)
pin10 = mcp.get_pin(10)
pin11 = mcp.get_pin(11)
pin12 = mcp.get_pin(12)
pin13 = mcp.get_pin(13)
pin14 = mcp.get_pin(14)


#set pins to output
pin8.switch_to_output(value=False)
pin9.switch_to_output(value=False)
pin10.switch_to_output(value=False)
pin11.switch_to_output(value=False)
pin12.switch_to_output(value=False)
pin13.switch_to_output(value=False)
pin14.switch_to_output(value=False)

pins = [pin8,pin9,pin10,pin11,pin12,pin13,pin14]


while True:
    for i in range(0,len(channels)):
        potVal = channels[i].value
        #print("Pot {0} Value: {1}".format(i+1,potVal))
    potVal = channels[0].value
    for i in range(0,len(pins)):
        selection = [i*9000, (i+1)*9000]
        if selection[0] < potVal < selection[1]:
            pins[i].value = True
        else:
            pins[i].value = False
        #print("Pin On: {0}".format(pins[i].value))
    #time.sleep(1)