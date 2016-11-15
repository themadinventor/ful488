TARGET  =fw_ful488
MCU     =atmega88p
FCPU	=20000000UL
OBJS    =ful488.o gpib.o v-usb/usbdrv/usbdrv.o v-usb/usbdrv/usbdrvasm.o v-usb/usbdrv/oddebug.o
PROG	=usbasp

CC      =avr-gcc
OBJCOPY =avr-objcopy
SIZE    =avr-size

CFLAGS  =-g -Os -mmcu=$(MCU) -DF_CPU=$(FCPU) -I. -Wall -Iv-usb/usbdrv/
ASFLAGS  =-g -Os -mmcu=$(MCU) -DF_CPU=$(FCPU) -I. -Wall -Iv-usb/usbdrv/

all: $(TARGET).hex

flash: $(TARGET).hex
	avrdude -c $(PROG) -p$(MCU) -U flash:w:$^

reset:
	avrdude -c $(PROG) -p$(MCU)

fuse:
	avrdude -c $(PROG) -p$(MCU) -U lfuse:w:0xee:m -U hfuse:w:0xdf:m -U efuse:w:0x01:m

size: $(OBJS)
	$(SIZE) $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET).o

veryclean: clean
	rm -f $(TARGET).hex

$(TARGET).hex: $(TARGET).o
	$(OBJCOPY) -j .text -j .data -O ihex $(TARGET).o $@

$(TARGET).o: $(OBJS)
	$(CC) -g -mmcu=$(MCU) -Wall -o $@ $^
	$(SIZE) $@
