import board
import digitalio

unitName ="ARMACHAT"
spread = 10
power = 23
bandwidth = 41700
codingRate = 8

myName ="Bobricius"
myID = 1
myGroup = 0
password = b'Sixteen byte key'

bright = 1
sleep = 0
font = 2
theme = 1

volume = 0

maxLines = 6
maxChars = 26


cols = [digitalio.DigitalInOut(x) for x in (board.GP1, board.GP2, board.GP3, board.GP4, board.GP5, board.GP14)]
rows = [digitalio.DigitalInOut(x) for x in (board.GP6, board.GP9, board.GP15, board.GP8, board.GP7, board.GP22)]
keys1 =   ((' ', '.', 'm', 'n', 'b',"dn"),
         ("ent", 'l', 'k', 'j', 'h',"lt"),
         ('p', 'o', 'i', 'u', 'y',"up"),
         ("bsp", 'z', 'x', 'c', 'v',"rt"),
         ('a', 's', 'd', 'f', 'g',"tab"),
         ('q', 'w', 'e', 'r', 't',"alt"))  

keys2 =   (('_', ',', '>', '<','""','{'),
         ('~', '-', '*', '&', '+','['),
         ('0', '9', '8', '7', '6','}'),
         ('=', '(', ')', '?', '/',']'),
         ('!', '@', '#', '$', '%','\\'),
         ('1', '2', '3', '4', '5',"alt"))

keys3 =   ((':', ';', 'M', 'N', 'B',"dn"),
         ("ent", 'L', 'K', 'J', 'H',"lt"),
         ('P', 'O', 'I', 'U', 'Y',"up"),
         ("bsp", 'Z', 'X', 'C', 'V',"rt"),
         ('A', 'S', 'D', 'F', 'G',"tab"),
         ('Q', 'W', 'E', 'R', 'T',"alt")) 