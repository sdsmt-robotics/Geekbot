#!/usr/bin/env python

import geekbot

geek = geekbot.Robot("/dev/ttyUSB0", 57600)

# !!!!!!! Don't touch anything above here !!!!!!!!

# V V V V V Work down here! V V V V V

geek.drive_forward(50)
time.sleep(2)
geek.halt()

