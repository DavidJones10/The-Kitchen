import pygame
from extra_rnbo import myMap
import math

BLACK = (0, 0, 0)
DARK_GRAY = (50,50,50)
WHITE = (255, 255, 255)
RED = (255, 0, 0)
HIGHLIGHT = (200, 200, 200)
BACKGROUND = (130, 130, 140)
STEPLIGHT = (200,200,255)
NICE_BLUE = (0, 25, 255)
SCREEN_WIDTH = 320
SCREEN_HEIGHT = 240
SQUARE_SIZE = 20
CIRCLE_RADIUS = 7
GRID_WIDTH = 8
GRID_HEIGHT = 7
PADDING = 10
KNOB_RADIUS = 30

class Display():
    def __init__(self,dummy):
        pygame.init()
        infoObject = pygame.display.Info()
        self.width = infoObject.current_w
        self.height = infoObject.current_h
        self.screen = pygame.display.set_mode((self.width,self.height),pygame.FULLSCREEN)
        pygame.display.set_caption("The Kitchen V1")
        pygame.mouse.set_visible(False)
        self.pages = Pages(0)
        self.bpm = Text("BPM: 120")
        self.loop = Toggle(10,10,True,"Loop Rec")
        self.fill = Toggle(70,10,True,"Fill Mode")
        self.djm = Toggle(127,10,True,"DJM Mode")
        self.fx = Text("FX: Delay")
        self.time_mult = Text("x1.0")
        self.rdm_all_pans = Toggle(10,25,True,"Rdm All Pans")
        self.rdm_all_velos = Toggle(190,23,True,"Rdm All Velos")
    def draw(self,drum_arrays,drum_mode,value_array,screen_mode):
        self.screen.fill(BACKGROUND)
        bpm = value_array[1]
        self.pages.page = screen_mode.value
        self.pages.draw(self.screen,drum_arrays,value_array,drum_mode)
        self.bpm.set_text("BPM: " + str(bpm))
        self.bpm.draw(self.screen,263,5)
        self.loop.state = value_array[35]
        self.loop.draw(self.screen)
        self.fill.state = value_array[2]
        self.fill.draw(self.screen)
        self.djm.state = value_array[3]
        self.djm.draw(self.screen)
        self.fx.set_text("FX: " + fx_mode_text(int(value_array[36])))
        self.fx.draw(self.screen,187,5)
        self.time_mult.set_text("x"+str(round(value_array[69],1)))
        self.time_mult.draw(self.screen, 270, 17)
        self.rdm_all_pans.state = value_array[68]
        self.rdm_all_velos.state = value_array[67]
        self.rdm_all_pans.draw(self.screen)
        self.rdm_all_velos.draw(self.screen)
        pygame.display.flip()

        pygame.time.Clock().tick(60)

class Toggle:
    def __init__(self, x, y,is_circle=False,label=None):
        self.x = x
        self.y = y
        self.state = False
        self.is_circle = is_circle
        self.label = Text(label)

    def draw(self, surface):
        color = RED if self.state else WHITE
        if not self.is_circle:
            pygame.draw.rect(surface, color, (self.x, self.y, SQUARE_SIZE, SQUARE_SIZE))
            pygame.draw.rect(surface, BLACK, (self.x, self.y, SQUARE_SIZE, SQUARE_SIZE), 2)  # Outline
        else:
            pygame.draw.circle(surface, color, (self.x, self.y), CIRCLE_RADIUS)
            pygame.draw.circle(surface, BLACK, (self.x, self.y), CIRCLE_RADIUS, 2)  # Outline
        if self.label:
            self.label.draw(surface,self.x+ CIRCLE_RADIUS+1,self.y-5)
class DrumRow:
    def __init__(self, x, y,circles=False):
        self.x = x
        self.y = y
        self.active = False
        if circles:
            self.toggles = [Toggle(x + CIRCLE_RADIUS + (2*CIRCLE_RADIUS + PADDING) * i, y,circles) for i in range(GRID_WIDTH)]
        else:
            self.toggles = [Toggle(x + (SQUARE_SIZE + PADDING) * i, y,circles) for i in range(GRID_WIDTH)]
    def update(self, triggers):
        for toggle, trigger in zip(self.toggles, triggers):
            toggle.state = trigger

    def draw(self, surface):
        if self.active:
            pygame.draw.rect(surface,HIGHLIGHT, (self.x-5, self.y-5, self.x+(SQUARE_SIZE + PADDING) * 7 + 15,SQUARE_SIZE+10))
        for toggle in self.toggles:
            toggle.draw(surface)

class DrumGrid:
    def __init__(self, x, y, circles=False):
        self.x = x
        self.y = y
        if circles:
            self.rows = [DrumRow(x,y + CIRCLE_RADIUS+(2*CIRCLE_RADIUS + PADDING) * i, circles) for i in range(GRID_HEIGHT)]
        else:
            self.rows = [DrumRow(x,y +(SQUARE_SIZE + PADDING) * i, circles) for i in range(GRID_HEIGHT)]
        self.labels = [Text("Text {}".format(i)) for i in range(8)]
        self.labels[0].set_text("Sample 1")
        self.labels[1].set_text("Sample 2")
        self.labels[2].set_text("Sample 3")
        self.labels[3].set_text("Sample 4")
        self.labels[4].set_text("Synth Kick")
        self.labels[5].set_text("Synth Snare")
        self.labels[6].set_text("Synth Cymbal")
        self.labels[7].set_text("Pattern")
        self.labels[7].set_color(RED)
        self.labels[7].set_size(25)
    def update(self,arrays,drum_mode):
        for i,row in enumerate(self.rows):
            if i == drum_mode:
                row.active = True
            else:
                row.active = False 
            row.update(arrays[i*8:i*8+8])
    def drawTexts(self,surface,value_array):
        for i in range(7):
            self.labels[i].draw(surface,250+5,43+(i*(SQUARE_SIZE+PADDING)))
        self.labels[7].draw(surface,100,17)
    def draw(self,surface,value_array):
        for row in self.rows:
            row.draw(surface) 
        self.drawTexts(surface,value_array)
class Text:
    def __init__(self, text, font_size=10, font_name='arial', color=(255, 255, 255)):
        self.text = text
        self.font_size = font_size
        self.font_name = pygame.font.match_font(font_name)
        self.color = color
        self.font = pygame.font.SysFont(font_name, font_size)
        self.surface = self.font.render(text, True, color)
        self.rect = self.surface.get_rect()

    def set_text(self, text):
        self.text = text
        self.surface = self.font.render(text, True, self.color)

    def set_color(self, color):
        self.color = color
        self.surface = self.font.render(self.text, True, color)

    def set_size(self, font_size):
        self.font_size = font_size
        self.font = pygame.font.SysFont(self.font_name, font_size)
        self.surface = self.font.render(self.text, True, self.color)

    def set_bounds(self, width, height):
        self.rect.width = width
        self.rect.height = height

    def draw(self, screen, x, y):
        self.rect.topleft = (x, y)
        screen.blit(self.surface, self.rect)

def fx_mode_text(mode=0):
    if mode == 0:
        return "Distortion"
    if mode == 1:
        return "Delay"
    if mode == 2:
        return "Reverb"
    if mode == 3:
        return "Compressor"
    if mode == 4:
        return "Gate"

class Pages:
    def __init__(self,start_page=0):
        self.page = start_page
        self.grid = DrumGrid(15,38)
        self.sound = SoundPage("Sample 1")
        self.fx = fxPage(0)
    def draw(self, surface,drum_arrays, value_array,drum_mode):
        if self.page == 0: #Pattern page
            self.grid.update(drum_arrays,drum_mode.value)
            self.grid.draw(surface,value_array)
        elif self.page == 1: #Sound Page
            self.sound.draw(surface,value_array,drum_mode.value)
        elif self.page == 2:
            self.fx.draw(surface,value_array)
class SoundPage:
    def __init__(self,drum_name,text_size=23):
        self.name = drum_name
        self.sound_label = Text(self.name,30,color=NICE_BLUE)
        self.atk = RotarySlider(70, 180,"Attack: ","ms")
        self.dec = RotarySlider(210, 180,"Decay: ","ms")
        self.pan = RotarySlider(210, 90,"Pan: ")
        self.pitch_b = RotarySlider(225, 90,"Pitch Beg: ","kHz")
        self.pitch_e = RotarySlider(210, 190,"Pitch End: ","kHz") 
        self.warp = Text("Warp Ratio: ",text_size)
        self.warp_val = Text("1.0",text_size)
        self.pan_mode = Toggle(10,47,True,"Rdm Pans")
        self.velo_mode = Toggle(255,47,True,"Rdm Velos")
    def draw(self,surface, value_array,drum_mode):
        self.new_name(drum_mode)
        self.sound_label.set_text(self.name)
        i = 0
        text_buf = 0
        if drum_mode < 4:
            i = 4 + drum_mode*4
            self.pan.x = 210
            self.atk.x = 70
            self.dec.x = 70
            self.dec.y = 180
            self.atk.y = 90
            self.warp_val.set_text("    "+str(round(value_array[i+2],4)))
            self.warp_val.draw(surface, 160,180)
            self.warp.draw(surface, 150, 155)
            self.pan.set_value(round(value_array[i+3],2),-1,1)
        elif drum_mode > 3 and drum_mode < 7:
            i = 20 + (drum_mode-4)*5
            self.pan.x = 135
            self.atk.x = 45
            self.dec.x = 70
            self.dec.y = 190
            self.atk.y = self.pan.y
            self.pitch_b.set_value(round(value_array[i+2]*.001,2),.3,2.5) #Todo scale for each synth separately
            self.pitch_e.set_value(round(value_array[i+3]*.001,2),.3,2.5)
            self.pitch_b.draw(surface)
            self.pitch_e.draw(surface)
            self.pan.set_value(round(value_array[i+4],2),-1,1)
            text_buf = 15
        if drum_mode != 7:
            self.atk.set_value(round(value_array[i],2),0,500)
            self.dec.set_value(round(value_array[i+1],2),10,2000)
            self.atk.draw(surface)
            self.dec.draw(surface)
        self.sound_label.draw(surface,80, 25)
        if drum_mode == 7:
            self.warp_val.set_text("    "+str(round(value_array[45],4)))
            self.warp_val.draw(surface, 160,90)
            self.warp.draw(surface, 150,65)
            self.pan.x = 70
            self.pan.set_value(round(value_array[44],2),-1,1)
        self.pan.draw(surface,text_buf)
        ez_idx = drum_mode
        if ez_idx > 6:
            ez_idx = 6
        self.pan_mode.state = value_array[ez_idx+60]
        self.velo_mode.state = value_array[ez_idx+53]
        self.pan_mode.draw(surface)
        self.velo_mode.draw(surface)
    def new_name(self, drum_mode):
        if drum_mode == 0:
            self.name = "Sample 1"
        elif drum_mode == 1:
            self.name = "Sample 2"
        elif drum_mode == 2:
            self.name = "Sample 3"
        elif drum_mode == 3:
            self.name = "Sample 4"
        elif drum_mode == 4:
            self.name = "Synth Kick"
        elif drum_mode == 5:
            self.name = "Synth Snare"
        elif drum_mode == 6:
            self.name = "Synth Cymbal"
        elif drum_mode == 7:
            self.name = "Looper"
class fxPage: 
    def __init__(self,current_effect,label="Delay"):
        self.effect = current_effect
        self.label = Text(label,30,color=NICE_BLUE)
        self.param1 = RotarySlider(70,90,"")
        self.param2 = RotarySlider(210,90,"")
        self.wet = RotarySlider(70,180,"")
        self.param3 = RotarySlider(210,180,"")
        self.encLabel1 = Text("Enc 1")
        self.encLabel2 = Text("Enc 2")
        self.encLabel1mLine1 = Text("Enc 1")
        self.encLabel1mLine2 = Text("Multi")
        self.encLabel2mLine1 = Text("Enc 2")
        self.encLabel2mLine2 = Text("Multi")
    def draw(self,surface,value_array):
        fx = value_array[36]
        self.label.set_text(fx_mode_text(fx))
        self.label.draw(surface,80,25)
        self.encLabel1.draw(surface,12,70)
        self.encLabel2.draw(surface,12,160)
        self.param2.unit = ""
        if fx == 1:
            self.param1.label="Time: "
            self.param1.unit= "sec"
            self.param2.label="Feedback: "
            self.wet.label="Wet: "
            self.param1.set_value(round(value_array[37],4),0,2000)
            self.param1.draw(surface)
            self.param2.set_value(round(value_array[38],2),0,1)
            self.param2.draw(surface) 
            self.wet.set_value(round(value_array[39],2),0,1)
            self.wet.draw(surface) 
            self.encLabel1mLine1.draw(surface,152,70)
            self.encLabel1mLine2.draw(surface,153,80)
        elif fx ==0:
            self.param1.label="Amount: "
            self.param1.unit= ""
            self.wet.label="Wet: "
            self.wet.unit=""
            self.param1.set_value(round(value_array[40],2),1,35)
            self.param1.draw(surface)
            self.wet.set_value(round(value_array[41],2),0,1)
            self.wet.draw(surface) 
        elif fx == 2:
            self.param1.label="Size: "
            self.param1.unit= ""
            self.wet.label="Wet: "
            self.wet.unit=""
            self.param1.set_value(round(value_array[42],2),0,100)
            self.param1.draw(surface)
            self.wet.set_value(round(value_array[43],2),0,1)
            self.wet.draw(surface) 
        elif fx == 3:
            self.param1.label="Attack: "
            self.param1.unit= "ms"
            self.param2.label="Ratio: "
            self.wet.label="Threshold: "
            self.wet.unit="dB"
            self.param1.set_value(round(value_array[49],2),0,500)
            self.param1.draw(surface)
            self.param2.set_value(round(value_array[50],2),1,20)
            self.param2.draw(surface) 
            self.wet.set_value(round(value_array[51],2),-70,6)
            self.wet.draw(surface) 
            self.encLabel1mLine1.draw(surface,152,70)
            self.encLabel1mLine2.draw(surface,153,80)
            '''
            #USE THIS IF YOU ADD ANOTHER PARAMETER
            self.param3.label="Param 3: "
            self.param3.unit="Units"
            self.param3.set_value(round(value_array[69],2),0,1)
            self.param3.draw(surface)
            self.encLabel2mLine1.draw(surface,152,160)
            self.encLabel2mLine2.draw(surface,153,170)
            '''
        elif fx == 4:
            self.param1.label="Focutivity: "
            self.param1.unit= ""
            self.param2.label="Release: "
            self.param2.unit="ms"
            self.wet.label="Mode: "
            self.param1.set_value(round(value_array[46],2),0,1)
            self.param1.draw(surface)
            self.param2.set_value(round(value_array[47],2),0,500)
            self.param2.draw(surface) 
            self.wet.set_value(round(value_array[48],2),0,3)
            self.wet.draw(surface) 
            self.encLabel1mLine1.draw(surface,152,70)
            self.encLabel1mLine2.draw(surface,153,80)
            '''
            #USE THIS IF YOU ADD ANOTHER PARAMETER
            self.param3.label="Param 3: "
            self.param3.unit="Units"
            self.param3.set_value(round(value_array[69],2),0,1)
            self.param3.draw(surface)
            self.encLabel2mLine1.draw(surface,152,160)
            self.encLabel2mLine2.draw(surface,153,170)
            '''
class RotarySlider:
    def __init__(self, x, y, label=None, unit=""):
        self.x = x
        self.y = y
        self.value = 0  
        self.label = label
        self.unit = unit
        self.scaled = 0
    def draw(self, surface,text_buff=0):
        pygame.draw.circle(surface, WHITE, (self.x, self.y), KNOB_RADIUS)
        knob_angle = math.radians(240 -self.scaled * 300)  # Adjust angle based on value
        knob_x = int(self.x + (KNOB_RADIUS) * math.cos(knob_angle))
        knob_y = int(self.y - (KNOB_RADIUS) * math.sin(knob_angle))
        pygame.draw.line(surface, BLACK, (self.x,self.y),(knob_x,knob_y),4)
       # Draw label if provided
        if self.label:
            txt = self.label+str(self.value)+self.unit
            font = pygame.font.SysFont(None, 20)
            text = font.render(txt, True, WHITE)
            surface.blit(text, (self.x-KNOB_RADIUS-10+text_buff, self.y + KNOB_RADIUS +5))

    def set_value(self, value, min_, max_):
        # Set value of the slider (clamped between 0 and 100)
        self.value = value
        self.scaled = myMap(value,min_,max_)
