#!/usr/bin/env python3
#
# Ful488
# (c) 2015 Fredrik Ahlberg <fredrik@z80.se>
#

import usb.core
import usb.util

class Ful488(object):

    FUL488_ATN1 = 1	    # Write an ATN=1 bus command
    FUL488_TALK = 2	    # Talk ATN=0
    FUL488_LISTEN = 3	# Listen
    FUL488_REN  = 4		# Control REN
    FUL488_SRQ  = 5		# Read SRQ
    FUL488_IFC  = 6		# Pulse IFC

    GPIB_GTL	= 0x01	# Go To Local
    GPIB_LLO	= 0x11	# Local Lock Out
    GPIB_DCL	= 0x14	# Device Clear
    GPIB_SPE    = 0x18  # Serial Poll Enable
    GPIB_SPD    = 0x19  # Serial Poll Disable
    GPIB_PAD	= 0x1f  # Address mask for MLA, TMA
    GPIB_MLA	= 0x20	# My Listen Address
    GPIB_MTA	= 0x40	# My Talk Address
    GPIB_UNT	= 0x5f	# Untalk
    GPIB_UNL	= 0x3f	# Unlisten
    GPIB_MSA	= 0x60	# My Secondary Address

    def __init__(self):
        self.dev = usb.core.find(idVendor=0x16c0, idProduct=0x05dc)

        if self.dev is None:
            raise ValueError('Device not found')

        self.dev.set_configuration()

    def ren(self, enable = True):
        self.ev.ctrl_transfer(0x40, Ful488.FUL488_REN, int(enable), 0, None)

    def ifc(self):
        self.dev.ctrl_transfer(0x40, Ful488.FUL488_IFC, 0, 0, None)

    def _atn1(self, command):
        self.dev.ctrl_transfer(0x40, Ful488.FUL488_ATN1, command, 0, None)

    def srq(self):
        return self.dev.ctrl_transfer(0xC0, Ful488.FUL488_SRQ, 0, 0, 1)[0]

    def init_bus(self):
        self.ren(True)
        self.ifc()
        self._atn1(Ful448.GPIB_DCL)

    def talk(self, data, addr = None, eoi = True):
        if addr:
            self._atn1(Ful488.GPIB_MTA)
            self._atn1(Ful488.GPIB_MLA | addr)

        while len(data):
            d = data[:3]
            to_talk = len(d) | (0x80 if eoi and len(data) == len(d) else 0)
            d += '\0\0'
            self.dev.ctrl_transfer(0x40, Ful488.FUL488_TALK, ord(d[0])|(ord(d[1])<<8),
                    (to_talk << 8) | ord(d[2]))
            data = data[3:]

        if addr:
            self._atn1(Ful488.GPIB_UNT)
            self._atn1(Ful488.GPIB_UNL)

    def listen(self, addr = None):
        if addr:
            self._atn1(Ful488.GPIB_MLA)
            self._atn1(Ful488.GPIB_MTA | addr)

        output = []
        while True:
            d = self.dev.ctrl_transfer(0xC0, Ful488.FUL488_LISTEN, 0, 0, 254)
            output += [b for b in d]
            if len(d) < 254:
                break

        if addr:
            self._atn1(Ful488.GPIB_UNT)
            self._atn1(Ful488.GPIB_UNL)

        return bytes(output)

    def command(self, cmd, addr):
        self.talk(cmd, addr)
        if cmd[-1] == '?':
            return self.listen(addr)

    def spoll(self):
        self._atn1(Ful488.GPIB_SPE)
        self._atn1(Ful488.GPIB_MLA)
        self._atn1(Ful488.GPIB_MTA | 1)

        d = self.dev.ctrl_transfer(0xC0, Ful488.FUL488_LISTEN, 0, 0, 1)[0]

        self._atn1(Ful488.GPIB_UNT)
        self._atn1(Ful488.GPIB_UNL)

        self._atn1(Ful488.GPIB_SPD)

        print(bin(d))

if __name__ == '__main__':
    scope = Ful488()
    print(scope.srq())
    scope.command('*CLS',1)
    scope.command('*SRE 48', 1)
    scope.command('*ESE 1', 1)
    print(scope.srq())
    scope.spoll()
    print(scope.command('RUMPAxxx', 1))
    scope.spoll()
    #print('STB',scope.command('*STB?', 1))
    print(scope.srq())
    scope.spoll()
    print('DESE',scope.command('DESE?', 1))
    scope.spoll()
    print(scope.srq())
    print('ESE',scope.command('*ESE?', 1))
    print('ESR',scope.command('*ESR?', 1))
    print(scope.srq())
    print('SRE',scope.command('*SRE?', 1))
    print(scope.srq())
    scope.spoll()
