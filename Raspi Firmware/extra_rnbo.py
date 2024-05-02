from digitalio import Direction, Pull
import time
import random

class mySwitch():
    def __init__(self, device, pin):
        self.pin = device.get_pin(pin)
        self.last_button_state = False
        self.last_button_press_time = 0.0
        self.debounce_time = .2
        self.pin.switch_to_input(Pull.UP)

    def Value(self,inversed=True):
        if not inversed:
            return self.pin.value
        else:
            return not self.pin.value

    def risingEdge(self):
        current_button_state = self.Value()
        if current_button_state and not self.last_button_state:
            self.last_button_state = current_button_state
            return True
        self.last_button_state = current_button_state
        return False

    def debounce(self):
        current_time = time.time()
        if self.Value():
            if current_time - self.last_button_press_time > self.debounce_time:
                self.last_button_press_time = current_time
                return True
        return False

    def timeHeld(self):
        current_time = time.time()
        if self.Value():
            return int(current_time - self.last_button_press_time)
        else:
            self.last_button_press_time = current_time
        return 0

class myLed():
    def __init__(self, device, pin):
        self.pin = device.get_pin(pin)
        self.pin.switch_to_output(False)
        self.last_toggle_time = 0

    def setValue(self, value):
        self.pin.value = value
        
    def blink(self, rate, active=True):
        current_time = time.monotonic()
        if active:
            if current_time - self.last_toggle_time >= rate:
                self.setValue(not self.pin.value)
                self.last_toggle_time = current_time

def myMap(input_value, input_range_low, input_range_high):
    mapped_value = (input_value - input_range_low) / (input_range_high - input_range_low)
    return max(0.0, min(mapped_value, 1.0))

def scale(input_value, input_range_low, input_range_high, output_range_low, output_range_high, is_int=False):
    if input_value < input_range_low:
        return output_range_low
    elif input_value > input_range_high:
        return output_range_high

    mapped_value = (input_value - input_range_low) / (input_range_high - input_range_low)
    scaled_value = mapped_value * (output_range_high - output_range_low) + output_range_low
    if is_int:
        return int(scaled_value)
    return scaled_value

kick_arrays = [[1, 0, 0, 1, 0, 0, 1, 1], [1, 0, 0, 1, 1, 0, 1, 1], [1, 0, 0, 1, 0, 0, 1, 0],
 [1, 0, 0, 1, 0, 0, 0, 0], [1, 0, 0, 1, 0, 0, 0, 1], [1, 0, 0, 0, 0, 0, 0, 1], [1, 0, 0, 0, 0, 0, 1, 0],
 [1, 0, 0, 0, 0, 1, 0, 0], [1, 0, 0, 0, 0, 1, 0, 0], [1, 0, 1, 0, 1, 0, 1, 0], [1, 0, 1, 0, 0, 1, 0, 1],
 [1, 0, 0, 1, 0, 1, 0, 1], [0, 1, 0, 1, 0, 1, 1, 1], [0, 1, 1, 1, 0, 1, 0, 1], [0, 1, 0, 1, 0, 1, 0, 1],
 [1, 0, 1, 0, 1, 1, 0, 1], [1, 0, 1, 0, 1, 0, 1, 0], [1, 0, 1, 1, 0, 1, 0, 1], [1, 0, 1, 0, 1, 1, 0, 1],
 [1, 0, 1, 1, 1, 0, 1, 0], [1, 0, 0, 1, 1, 0, 1, 0], [1, 1, 0, 0, 1, 0, 1, 0], [1, 0, 1, 0, 0, 1, 1, 0],
 [1, 0, 0, 1, 1, 0, 1, 0], [1, 0, 1, 1, 1, 1, 0, 1], [1, 0, 1, 0, 1, 1, 1, 0], [1, 1, 1, 1, 1, 1, 1, 0],
 [1, 1, 1, 1, 1, 1, 1, 1], [1, 0, 0, 0, 1, 0, 0, 0], [1, 0, 1, 0, 1, 0, 0, 0]]

snare_arrays = [[0, 0, 1, 0, 0, 0, 1, 0],[0, 0, 1, 0, 0, 0, 1, 0],[0, 0, 1, 0, 0, 0, 1, 0],
[0, 0, 1, 0, 1, 1, 0, 0],[0, 1, 1, 0, 0, 1, 0, 0],[0, 1, 0, 0, 0, 1, 0, 0],[0, 1, 0, 1, 0, 0, 1, 0],
[0, 1, 0, 1, 0, 0, 1, 0],[1, 1, 0, 1, 0, 0, 1, 0],[1, 0, 0, 1, 0, 0, 1, 1],[1, 0, 1, 1, 0, 1, 0, 0],
[1, 0, 0, 1, 0, 1, 0, 0],[1, 0, 0, 0, 0, 1, 0, 0],[1, 0, 1, 0, 0, 1, 0, 0],[0, 0, 1, 0, 0, 1, 0, 1],
[0, 0, 1, 0, 0, 1, 1, 0],[0, 0, 1, 0, 0, 1, 1, 0],[0, 0, 1, 0, 0, 0, 1, 0]]

hh_arrays = [[1, 0, 1, 0, 1, 0, 1, 1],[1, 0, 0, 0, 1, 0, 1, 1],[1, 1, 0, 0, 1, 0, 1, 1],
[1, 1, 0, 1, 0, 0, 1, 1],[1, 0, 1, 1, 0, 0, 1, 1],[0, 0, 1, 1, 0, 1, 0, 1],[0, 0, 1, 0, 0, 1, 0, 1],
[0, 1, 0, 1, 0, 1, 1, 1],[0, 1, 0, 1, 1, 1, 0, 1],[0, 1, 1, 1, 0, 1, 0, 1],[1, 1, 0, 1, 0, 1, 0, 1],
[1, 0, 0, 1, 1, 1, 0, 1],[1, 1, 0, 0, 1, 1, 0, 1],[1, 0, 0, 1, 1, 0, 0, 1],[1, 1, 0, 1, 1, 0, 0, 1],
[1, 1, 0, 1, 1, 0, 1, 1],[1, 1, 0, 1, 1, 1, 0, 1],[1, 0, 1, 1, 1, 1, 1, 1],[1, 0, 1, 0, 1, 0, 1, 0],
[1, 1, 1, 1, 1, 1, 1, 1],[0, 1, 0, 1, 0, 1, 0, 1]]

other_arrays = [[1, 0, 1, 0, 1, 0, 1, 1],[1, 0, 0, 0, 1, 0, 1, 1],
[1, 1, 0, 0, 1, 0, 1, 1],[1, 1, 0, 1, 0, 0, 1, 1],[1, 0, 1, 1, 0, 0, 1, 1],[0, 0, 1, 1, 0, 1, 0, 1],
[0, 0, 1, 0, 0, 1, 0, 1],[0, 1, 0, 1, 0, 1, 1, 1],[0, 1, 0, 1, 1, 1, 0, 1],[0, 1, 1, 1, 0, 1, 0, 1],
[1, 1, 0, 1, 0, 1, 0, 1],[1, 0, 0, 1, 1, 1, 0, 1],[1, 1, 0, 0, 1, 1, 0, 1],[1, 0, 0, 1, 1, 0, 0, 1],
[1, 1, 0, 1, 1, 0, 0, 1],[1, 1, 0, 1, 1, 0, 1, 1],[1, 1, 0, 1, 1, 1, 0, 1],[1, 0, 1, 1, 1, 1, 1, 1],
[1, 0, 1, 0, 1, 0, 1, 0],[1, 1, 1, 1, 1, 1, 1, 1],[0, 1, 0, 1, 0, 1, 0, 1],[0, 1, 0, 1, 0, 1, 0, 1],
[0, 1, 0, 1, 0, 0, 1, 0],[0, 1, 0, 1, 0, 0, 1, 0],[0, 1, 0, 1, 0, 0, 1, 0],[0, 1, 0, 1, 0, 0, 1, 0],
[0, 1, 0, 1, 0, 0, 1, 0],[0, 1, 0, 1, 0, 0, 1, 0],[1, 1, 0, 1, 0, 0, 1, 0],[1, 0, 0, 1, 0, 0, 1, 1],
[1, 0, 1, 1, 0, 1, 0, 0],[1, 0, 0, 1, 0, 1, 0, 0],[1, 0, 0, 0, 0, 1, 0, 0],[1, 0, 1, 0, 0, 1, 0, 0],
[0, 0, 1, 0, 0, 1, 0, 1],[0, 0, 1, 0, 0, 1, 1, 0],[0, 0, 1, 0, 0, 1, 1, 0],[0, 0, 1, 0, 0, 0, 1, 0],
[0, 0, 1, 0, 0, 0, 1, 0],[0, 0, 1, 0, 0, 0, 1, 0],[0, 0, 1, 0, 0, 0, 1, 0],[0, 0, 1, 0, 0, 0, 1, 0],
[0, 0, 1, 0, 0, 0, 1, 0],[1, 0, 1, 0, 1, 0, 1, 1],[1, 0, 0, 0, 1, 0, 1, 1]]

def getRandomPattern(mode, drumArr):
    idx = mode*8
    if mode == 0:
        drumArr[idx:idx+8] = random.choice(kick_arrays)
    elif mode == 1:
        drumArr[idx:idx+8] = random.choice(snare_arrays)
    elif mode == 2:
        drumArr[idx:idx+8] = random.choice(hh_arrays)
    elif mode == 3:
        drumArr[idx:idx+8] = random.choice(other_arrays)
    elif mode == 4:
        drumArr[idx:idx+8] = random.choice(hh_arrays)
    elif mode == 5:
        drumArr[idx:idx+8] = random.choice(snare_arrays)
    elif mode == 6:
        drumArr[idx:idx+8] = random.choice(kick_arrays)
    else:
        return

