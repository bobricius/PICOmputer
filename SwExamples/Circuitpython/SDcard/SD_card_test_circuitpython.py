import os
import board
import busio as io
import digitalio
import storage
import adafruit_sdcard
import microcontroller
from time import sleep
# pouzi ktorykolvek pin avsak nie rezervovany SPI
SD_CS = board.GP13
 
# pripojenie ku karte a import suboroveho systemu
spi = io.SPI(board.GP10, board.GP11, board.GP12)
cs = digitalio.DigitalInOut(SD_CS)
sdcard = adafruit_sdcard.SDCard(spi, cs)
vfs = storage.VfsFat(sdcard)
storage.mount(vfs, "/sd")


with open("/sd/test_sd_karty.txt", "w") as writefile:
    print("First Line", file=writefile)
    writefile.close()

with open("/sd/test_sd_karty.txt", "a") as appendfile:
    print("Second Line", file=appendfile)
    appendfile.close

with open("/sd/test_sd_karty.txt", "r") as inputfile:
    for line in inputfile:
        print(line)
    inputfile.close

while True:
    with open("/sd/teplota.txt", "a") as appendfile:        
        print(microcontroller.cpu.temperature, file=appendfile)
    appendfile.close
    sleep(5)
