/*
 * Ful488
 * (c) 2015-2016 Fredrik Ahlberg <fredrik@z80.se>
 */

#ifndef FUL488_H
#define FUL488_H

enum {
	FUL488_ATN1 = 1,	/* Write an ATN=1 bus command */
	FUL488_TALK = 2,	/* Talk ATN=0 */
	FUL488_LISTEN = 3,	/* Listen */
	FUL488_REN = 4,		/* Control REN */
	FUL488_SRQ = 5,		/* Read SRQ */
	FUL488_IFC = 6,		/* Pulse IFC */
	FUL488_STATUS = 7,	/* Read status */
};

#endif
