import time
import board
import busio
import terminalio
import displayio
import gc
import aesio
from binascii import hexlify
import microcontroller
from adafruit_simple_text_display import SimpleTextDisplay
import adafruit_imageload
import adafruit_matrixkeypad
from adafruit_bitmap_font import bitmap_font
from pwmio import PWMOut

from adafruit_display_text import label
from adafruit_st7789 import ST7789
from config import config
import digitalio
from picomputer import picomputer

import ulora


#----------------------FUNCTIONS---------------------------
#int menuUp(int xmin,int xmax, int value )
#{
#    value++;
#    if (value > xmax) {
#      value = xmin;
#    }
#    if (value < xmin) {
#      value = xmax;
#    }
#    return (value);
#}
def valueUp (min, max, value):
	value = value + 1
	if value > max:
		value=min
	if value < min:
		value=max
	return value


def setup():
	menu = 0
	screen[0].text = "SETUP:"
	screen[1].text = "Use Left/Right"
	screen[2].text = "to switch page"
	screen[3].text = "[ESC] to exit"
	screen[4].text = ""
	screen[5].text = ""
	screen[6].text = ""
	screen[7].text = ""
	screen[8].text = ""
	picomputer.ring()
	screen.show()
	while True:
		keys = keypad.pressed_keys
		if keys:
			picomputer.beep()
			if keys[0]=="lt":
				if menu>0 :menu=menu-1
			if keys[0]=="rt":
				if menu<3 :menu=menu+1
			if keys[0]=="tab":
				picomputer.beep()
				return 1
			if menu==0:
				if keys[0]=="s":
					config.spread=valueUp(7,12,config.spread)
				screen[0].text = "{:.d} Radio:".format(menu)
				screen[1].text = "[F] Frequency: 915MHz"
				screen[2].text = "[S] Spread {:.d}".format(config.spread)
				screen[3].text = "[P] Power {:.d}".format(config.power)
				screen[4].text = "[S] Bandwidth {:.d}".format(config.bandwidth)
				screen[5].text = "[C] Coding rate {:.d}".format(config.codingRate)
				screen[6].text = "[X] Preset"
				screen[7].text = ""
				screen[8].text = "Ready ..."
				screen.show()
			elif menu==1:
				if keys[0]=="n":
					config.myName = editor(text=config.myName)
				screen[0].text = "{:.d} Identity:".format(menu)
				screen[1].text = "[N] Name: {} ".format(config.myName)
				screen[2].text = "[I] ID: {}".format(config.myID)
				screen[3].text = "[G] Group {}".format(config.myGroup)
				screen[4].text = "[E] Encryption {}".format(config.password)
				screen[5].text = ""
				screen[6].text = ""
				screen[7].text = ""
				screen[8].text = "Ready ..."
				screen.show()
			elif menu==2:
				screen[0].text = "{:.d} Display:".format(menu)
				screen[1].text = "[B] Bright {}".format(config.bright)
				screen[2].text = "[I] Sleep {}".format(config.sleep)
				screen[3].text = "[F] Font {}".format(config.font)
				screen[4].text = "[T] Theme {}".format(config.theme)
				screen[5].text = ""
				screen[6].text = ""
				screen[7].text = ""
				screen[8].text = "Ready ..."
				screen.show()
			elif menu==3:
				screen[0].text = "{:.d} Sound:".format(menu)
				screen[1].text = "[V] Volume {}".format(config.volume)
				screen[2].text = "[S] Silent"
				screen[3].text = "[T] Tone"
				screen[4].text = "[M] Melody"
				screen[5].text = ""
				screen[6].text = ""
				screen[7].text = ""
				screen[8].text = "Ready ..."
				screen.show()

	
def editor(text):
	cursor = 0
	layout = 0
	editLine = 0
	editText = text
	layoutName="abc"
	EditorScreen.show()
	line = ["0", "1", "2","3", "4", "5", "6"]
	line[0]=text
	line[1]=""
	line[2]=""
	line[3]=""
	line[4]=""
	line[5]=""
	line[6]=""
	EditorScreen[1].text = line[0]
	EditorScreen[2].text = line[1]
	EditorScreen[3].text = line[2]    
	EditorScreen[4].text = line[3]
	EditorScreen[5].text = line[4]
	EditorScreen[6].text = line[5]
	EditorScreen[7].text = line[6]
	while True:
		EditorScreen[0].text = "["+layoutName+"] "+str(editLine) +":"+str(cursor)+"/"+str(len(text))

		if layout == 0:
			keypad = adafruit_matrixkeypad.Matrix_Keypad(config.rows, config.cols, config.keys1)
			layoutName="abc"
		elif layout == 1:
			keypad = adafruit_matrixkeypad.Matrix_Keypad(config.rows, config.cols, config.keys2)
			layoutName="123"
		elif layout ==2:
			keypad = adafruit_matrixkeypad.Matrix_Keypad(config.rows, config.cols, config.keys3)
			layoutName="ABC"
		
		keys = keypad.pressed_keys
		
		if keys:
			if keys[0]=="alt":
				layout=layout+1
				picomputer.ring()
				if layout==3:
					layout=0
				keys[0]=""
			if keys[0]=='X':
				keys[0]=""
			if keys[0]=="bsp":
				if cursor>0 :
					editText=(editText[0:cursor-1])+(editText[cursor:])
					cursor=cursor-1
				keys[0]=""
			if keys[0]=="lt":
				if cursor>0 :cursor=cursor-1
				keys[0]=""
			if keys[0]=="rt":
				if cursor<len(editText) :cursor=cursor+1
				keys[0]=""
			if keys[0]=="up":
				line[editLine]=editText
				EditorScreen[editLine+1].text = editText
				if editLine>0 :editLine=editLine-1
			
				editText=line[editLine]
				cursor=0
				keys[0]=""
			if keys[0]=="dn":
				line[editLine]=editText
				EditorScreen[editLine+1].text = editText
				if editLine<config.maxLines :editLine=editLine+1

				editText=line[editLine]
				cursor=0
				keys[0]=""
			if keys[0]=="ent":
				picomputer.beep()
				return editText
			if keys[0]!="":
				if len(editText)<config.maxChars:
					editText=(editText[0:cursor])+keys[0]+(editText[cursor:])
					cursor=cursor+1
					layout=0
					while keypad.pressed_keys:
						pass
			line[editLine] = (editText[0:cursor])+"_"+(editText[cursor:])
			EditorScreen[editLine+1].text = line[editLine]
			EditorScreen.show()



#----------------------FUNCTIONS---------------------------
#configure picomputer devices (display, LED, Speaker)
#picomputer.init()

# Define the onboard LED
LED = digitalio.DigitalInOut(board.GP25)
LED.direction = digitalio.Direction.OUTPUT

displayio.release_displays()

tft_cs = board.GP21
tft_dc = board.GP16
tft_reset = board.GP17
spi_mosi = board.GP19
spi_clk = board.GP18
spi = busio.SPI(spi_clk, spi_mosi)
backlight = board.GP20

display_bus = displayio.FourWire(spi, command=tft_dc, chip_select=tft_cs, reset=tft_reset)

display = ST7789(
    display_bus, rotation=270, width=320, height=240, backlight_pin=backlight
)

# Make the display context
splash = displayio.Group()
display.show(splash)

text =  "Free RAM:"+str(gc.mem_free())+" Loading ..."
text_area = label.Label(terminalio.FONT, text=text, scale=2, background_tight=False, background_color=255)
text_area.x = 0
text_area.y = 100

display.show(text_area)


# font
#font_file = "fonts/neep-iso8859-1-12x24.bdf"
#font_file = "fonts/gohufont-14.bdf"
#font_file = "fonts/Gomme10x20n.bdf"
#font = bitmap_font.load_font(font_file)
font = terminalio.FONT


# Define radio parameters.
#RADIO_FREQ_MHZ = 869.45  # Frequency of the radio in Mhz. Must match your
RADIO_FREQ_MHZ = 868.0 #869.45  # Frequency of the radio in Mhz. Must match your
# module! Can be a value like 915.0, 433.0, etc.

# Define pins connected to the chip, use these if wiring up the breakout according to the guide:
#CS = digitalio.DigitalInOut(board.D5)
#RESET = digitalio.DigitalInOut(board.D6)
# Or uncomment and instead use these if using a Feather M0 RFM9x board and the appropriate
# CircuitPython build:
CS = digitalio.DigitalInOut(board.GP13)
#RESET = digitalio.DigitalInOut(board.GP5)


# Initialize SPI bus.
spi = busio.SPI(board.GP10, MOSI=board.GP11, MISO=board.GP12)

#a = 1/0
# Initialze RFM radio
#rfm9x = adafruit_rfm9x.RFM9x(spi, CS, RESET, RADIO_FREQ_MHZ)

print('starting Lora')
#rfm9x = ulora.LoRa()#modem_config=ulora.ModemConfig.Bw31_25Cr48Sf512) #RFM95_SPIBUS, RFM95_INT, SERVER_ADDRESS, RFM95_CS, reset_pin=RFM95_RST, freq=RF95_FREQ, tx_power=RF95_POW, acks=True)
rfm9x = ulora.LoRa(spi, CS, modem_config=ulora.ModemConfig.Bw125Cr45Sf2048,tx_power=23) #, interrupt=28
# Note that the radio is configured in LoRa mode so you can't control sync
# word, encryption, frequency deviation, or other settings!

# You can however adjust the transmit power (in dB).  The default is 13 dB but
# high power radios like the RFM95 can go up to 23 dB:

#rfm9x.tx_power = 23
#rfm9x.spreading_factor = 9
#rfm9x.signal_bandwidth = 41700#62500# 31250#250000
#rfm9x.coding_rate = 8
print ("Free memory:")
print (gc.mem_free())
EditorScreen = SimpleTextDisplay(display=display, title="Armachat EDITOR", title_scale=1,text_scale=2,
                                colors=(SimpleTextDisplay.GREEN, SimpleTextDisplay.WHITE,SimpleTextDisplay.WHITE,
                                        SimpleTextDisplay.WHITE,SimpleTextDisplay.WHITE, SimpleTextDisplay.WHITE,
                                         SimpleTextDisplay.WHITE,SimpleTextDisplay.WHITE,SimpleTextDisplay.WHITE))

screen = SimpleTextDisplay(display=display,font=font, title="Armachat main menu", title_scale=1,text_scale=2,
                                colors=(SimpleTextDisplay.GREEN, SimpleTextDisplay.WHITE,SimpleTextDisplay.WHITE,
                                        SimpleTextDisplay.WHITE,SimpleTextDisplay.WHITE, SimpleTextDisplay.WHITE,
                                         SimpleTextDisplay.WHITE,SimpleTextDisplay.WHITE,SimpleTextDisplay.RED))
print ("Screen ready,Free memory:")
print (gc.mem_free())
while True:
	screen[0].text = "Menu:"
	screen[1].text = "[N] New message"
	screen[2].text = "[M] Memory"
	screen[3].text = "[C] Contacts"
	screen[4].text = "[S] Setup"
	screen[5].text = "[P] Ping / link"
	screen[6].text = "[X] Chat"
	screen[7].text = "[T] Terminal"
	screen[8].text = "Ready ..."
	screen.show()
	picomputer.beep()
	keypad = adafruit_matrixkeypad.Matrix_Keypad(config.rows, config.cols, config.keys1)
	while True:
		keys = keypad.pressed_keys
		#picomputer.beep()
		if keys:
			break
		
	if keys[0]=='n':
		picomputer.ring()
		text=editor (text="")
	if keys[0]=='e':
		picomputer.ring()
		text=editor (text="")
		key = b'Sixteen byte key'
		inp = b'CircuitPython!!!' # Note: 16-bytes long
		outp = bytearray(len(inp))
		cipher = aesio.AES(key, aesio.MODE_CBC)
		cipher.encrypt_into(inp, outp)
		print(key)
		print(text)
		print(outp)
		cipher.decrypt_into(outp, inp)
		print(inp)

	if keys[0]=='s':
		picomputer.ring()
		setup()	
	if keys[0]=='t':
		screen.show_terminal()
		picomputer.ring()
		keys = keypad.pressed_keys
		while not keys:
			keys = keypad.pressed_keys













