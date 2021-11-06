# SPDX-FileCopyrightText: 2021 ladyada for Adafruit Industries
# SPDX-License-Identifier: MIT

"""
This test will initialize the display using displayio and draw a solid green
background, a smaller purple rectangle, and some yellow text.
"""
import time
import board
import busio
import gc
import terminalio
import displayio
import microcontroller

from digitalio import DigitalInOut

from adafruit_display_text import label
from adafruit_st7789 import ST7789

# Release any resources currently in use for the displays
displayio.release_displays()

tft_cs = board.GP17
tft_dc = board.GP16
tft_reset = board.GP21
spi_mosi = board.GP19
spi_clk = board.GP18
spi = busio.SPI(spi_clk, spi_mosi)
backlight = board.GP20

display_bus = displayio.FourWire(spi, command=tft_dc, chip_select=tft_cs, reset=tft_reset)

display = ST7789(
    display_bus, rotation=270, width=320, height=240, backlight_pin=backlight
)

# Make the display context
# splash = displayio.Group()
# display.show(splash)

print ("Free memory:")
print (gc.mem_free())
print ("Starting code.py...")

