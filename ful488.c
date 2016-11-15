/*
 * Ful488
 * (c) 2015-2016 Fredrik Ahlberg <fredrik@z80.se>
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <usbdrv.h>
#include "ful488.h"
#include "gpib.h"

usbMsgLen_t usbFunctionSetup(uint8_t data[8])
{
	usbRequest_t *rq = (void *) data;
	static uint8_t dataBuffer[254], len;
	usbMsgPtr = dataBuffer;

	switch (rq->bRequest) {
	case FUL488_ATN1:
		dataBuffer[0] = gpib_command(rq->wValue.word);
		return 1;
	
	case FUL488_TALK:
		dataBuffer[0] = gpib_write((uint8_t *) rq->wValue.bytes,
			(rq->wIndex.word >> 8) & 0x7, 0, (rq->wIndex.word >> 8) & 0x80);
		return 1;
	
	case FUL488_LISTEN:
		len = 0;
		while ((len < 254) && (len < rq->wLength.word)) {
			if (gpib_read(&dataBuffer[len])) {
				break;
			}
			len++;
		}
		return len;

	case FUL488_REN:
		gpib_ren(rq->wValue.word);
		return 0;

	case FUL488_SRQ:
		dataBuffer[0] = gpib_srq();
		return 1;

	case FUL488_IFC:
		gpib_ifc();
		return 0;

	case FUL488_STATUS:
		dataBuffer[0] = gpib_status();
		return 1;
	}

	return 0;
}

int main(void)
{
	wdt_reset();
	wdt_enable(WDTO_120MS);

	usbInit();
	usbDeviceDisconnect();

	uint8_t t;
	for (t=255; t; t--) {
		_delay_ms(1);
		wdt_reset();
	}


	usbDeviceConnect();
	sei();

	gpib_init();

	for (;;) {
		usbPoll();
		wdt_reset();
	}
}

