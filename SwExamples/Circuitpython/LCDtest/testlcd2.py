"""
Example of CircuitPython/Raspberry Pi Pico
to display on 1.14" 135x240 (RGB) IPS screen
with ST7789 driver via SPI interface.

Connection between Pico and
the IPS screen, with ST7789 SPI interface.
3V3  - BLK (backlight, always on)
GP11 - CS
GP12 - DC
GP13 - RES
GP15 - SDA
GP14 - SCL
3V3  - VCC
GND  - GND
"""

import os
import board
import time
import terminalio
import displayio
import busio
from adafruit_display_text import label
import adafruit_st7789
import digitalio

print("==============================")
print(os.uname())
print("Hello Raspberry Pi Pico/CircuitPython ST7789 SPI IPS Display")
print(adafruit_st7789.__name__ + " version: " + adafruit_st7789.__version__)
print()

led = digitalio.DigitalInOut(board.GP20)
led.direction = digitalio.Direction.OUTPUT
led.value = True

# Release any resources currently in use for the displays
displayio.release_displays()

tft_cs = board.GP17
tft_dc = board.GP16
tft_res = board.GP21
spi_mosi = board.GP19
spi_clk = board.GP18

"""
classbusio.SPI(clock: microcontroller.Pin,
                MOSI: Optional[microcontroller.Pin] = None,
                MISO: Optional[microcontroller.Pin] = None)
"""
spi = busio.SPI(spi_clk, MOSI=spi_mosi)

display_bus = displayio.FourWire(
    spi, command=tft_dc, chip_select=tft_cs, reset=tft_res
)

display = adafruit_st7789.ST7789(display_bus,
                    width=240, height=240,
                    rowstart=80, colstart=0)
display.rotation = 270

group = displayio.Group(max_size=10)
display.show(group)

bitmap = displayio.Bitmap(240, 240, 240)

palette = displayio.Palette(240)
for p in range(240):
    palette[p] = (0x010000*p) + (0x0100*p) + p

for y in range(240):
    for x in range(240):
        bitmap[x,y] = y
        
tileGrid = displayio.TileGrid(bitmap, pixel_shader=palette, x=0, y=0)
group.append(tileGrid)

time.sleep(3.0)

while True:
    for p in range(240):
        palette[p] = p
    time.sleep(3.0)

    for p in range(240):
        palette[p] = 0x0100 * p
    time.sleep(3.0)

    for p in range(240):
        palette[p] = 0x010000 * p
    time.sleep(3.0)